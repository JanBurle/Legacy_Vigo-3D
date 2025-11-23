/*****************************************************************************\
*    Copyright (c) 2004--2007 by Ian G Burleigh                               *
*    https://github.com/phi53/Vigo-3D                                         *
*                                                                             *
*    This program is free software; you can redistribute it and/or modify     *
*    it under the terms of the GNU General Public License as published by     *
*    the Free Software Foundation; either version 2 of the License, or        *
*    (at your option) any later version.                                      *
*                                                                             *
*    This program is distributed in the hope that it will be useful,          *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
*    GNU General Public License for more details.                             *
*                                                                             *
*    You should have received a copy of the GNU General Public License        *
*    along with this program; if not, write to the                            *
*    Free Software Foundation, Inc.,                                          *
*    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                *
*-----------------------------------------------------------------------------*
* $Id: main.cpp 226 2007-05-21 14:13:25Z ian_b $
\*****************************************************************************/

/** \file
 */

  #include <vigoapp>
  #include <vigosim>

//-----------------------------------------------------------------------------

  using namespace vigo;
  using namespace vigo::math;

  typedef algebra3d::Pointd  Point;
  typedef algebra3d::Vectord Vector;
  typedef space::Scened      Scene;

//-----------------------------------------------------------------------------

  double const SCENE_RADIUS   = 8.0;
  double const NEIGHBOUR_SIZE = 0.01;
  double const MAX_STEP_SIZE  = 0.08;
  uint   const NUM_NEIGHBOURS = 120;  // of each colour
  double const VISION_RADIUS  = .2;
  uint   const NUM_SHADES     =  8;

  shape::Appearance* GreenScale()
  {
    static shape::Appearance app[NUM_SHADES];
    static bool appSet = false;

    if(!appSet)
    {
      for(uint i=0; i<NUM_SHADES; ++i)
      {
        double green = .25 + (.75/NUM_SHADES) * i;
        app[i].SetColorAlpha(new shape::ShapeColorAlpha(shape::Color(0.,green,0.)));
      }

      appSet = true;
    }

    return app;
  }

  shape::Appearance* YellowScale()
  {
    static shape::Appearance app[NUM_SHADES];
    static bool appSet = false;

    if(!appSet)
    {
      for(uint i=0; i<NUM_SHADES; ++i)
      {
        double redGreen = .25 + (.75/NUM_SHADES) * i;
        app[i].SetColorAlpha(new shape::ShapeColorAlpha(shape::Color(redGreen,redGreen,0.)));
      }

      appSet = true;
    }

    return app;
  }

//-----------------------------------------------------------------------------
// a neighbour

  typedef space::Node<double,
            parts::Parts<
          parts::PP<double>,
          parts::R0,
          parts::TC<double>,
          parts::SS,
          parts::A1
        >
      > SimpleAgent;

  class Neighbour: public SimpleAgent
  {
    SUPER(SimpleAgent)

  private:
    enum {MAX_SHOWN_NBR = NUM_SHADES};
    // remembered neighbours
    space::tracked_collection<double> m_nearMe;

  public:
    Neighbour(Scene& s)
      : super(s,true), m_nearMe()
    {
    }

   ~Neighbour()
    {
    }

    bool Act()
    {
      switch(ActPhase())
      {
      case 0: // move
        MoveBy(Vector(Point::Random(Point::_0,MAX_STEP_SIZE)));
        break;

      case 1: // think
        // only look for neighbours of your kind
        Nbr(GrpId(),VISION_RADIUS,m_nearMe,MAX_SHOWN_NBR,true);

        // determine shade
        uint   ns = m_nearMe.size();
        double dn = ((double)ns+1/*myself*/)/MAX_SHOWN_NBR;
        dn *= NUM_SHADES;
        uint nn = min((uint)dn,NUM_SHADES-1);
        // the more neighbours, the darker
        SetAppearance(Scale()[NUM_SHADES-1-nn]);
        break;
      }

      return true;
    }

    void DrawSelf(Camera const& cam,algebra3d::Pointd const& pos)
    {
      // lines to neighbours
      for(uint i=0; i<m_nearMe.size(); ++i)
      {
        Point p = m_nearMe[i]->Pos() - Pos();

        //ib>>> add "GLPushColor/GLPopColor"
        GLdouble c[4]; glGetDoublev(GL_CURRENT_COLOR,c); // push
        glColor3d(.8,.8,.8);

        glBegin(GL_LINES);
        glVertex3d(0,0,0);
        glVertex3d(p.x,p.y,p.z);
        glEnd();

        glColor3d(c[0],c[1],c[2]); // pop
      }
    super::DrawSelf(cam,pos);
    }

  protected:
    virtual shape::Appearance* Scale() = 0;
  };

  class YellowNeighbour: public Neighbour
  {
    SUPER(Neighbour)

  public:
    YellowNeighbour(Scene& s)
      : super(s)
    {
    }

  protected:
    virtual shape::Appearance* Scale()
    {
      return YellowScale();
    }
  };


  class GreenNeighbour: public Neighbour
  {
    SUPER(Neighbour)

  public:
    GreenNeighbour(Scene& s)
      : super(s)
    {
    }

  protected:
    virtual shape::Appearance* Scale()
    {
      return GreenScale();
    }
  };


//-----------------------------------------------------------------------------

  class NeighbourSimulation: public Simulation
  {
    SUPER(Simulation)

  public:
    NeighbourSimulation()
    {
    }
  };


//-----------------------------------------------------------------------------
// a scene with multiple bodies

  class NeighbourScene: public Scene
  {
    SUPER(Scene)

  public:
    NeighbourScene()
    {
      SetNumActPhases(2);
      Create();
    }

    double CameraFar()    const { return 4*SCENE_RADIUS;   }
    Point  CameraEye()    const { return Point(0,0,SCENE_RADIUS);}

    // this is to show the global point of view
    void Display(Camera& cam)
    {
      super::Display(cam);
      root_node::node_list const& nodes = Root().Nodes();
      for(root_node::node_const_iterator n = nodes.begin(); n != nodes.end(); ++n)
      {
        // do something with the node
        Neighbour *nbr = dynamic_cast<Neighbour*>(n->ptr());
        if(nbr)
        {
          //WATCH(nbr->Pos())
          //glBegin(GL_LINES); ... glEnd();
        }
      }
    }

  private:
    void Create()
    {
      shape::Shape *sh = new shape::Sphere(NEIGHBOUR_SIZE);
      for(uint i=0; i<NUM_NEIGHBOURS; ++i)
      {
        Neighbour *n = new YellowNeighbour(*this);
        n->SetShape(*sh);
        Root().Add(*n);

        n = new GreenNeighbour(*this);
        n->SetShape(*sh);
        Root().Add(*n);
      }
    }
  };


//-----------------------------------------------------------------------------

  int main(int argc, pstr argv[])
  {
    try
    {
      Application app("neighbours",argc,argv);

      try
      {
        app.ProcessArgs();
      }
      catch(exc& e)
      {
        std::cerr << e.What()   << std::endl;
        std::cout << app.Help() << std::endl;
        return -1;
      }

      Window wnd("Neigbours");
      Canvas cvs(wnd);
      NeighbourSimulation sim;
      NeighbourScene      scene;
      sim.SetScene(scene);

      Camera cam;
      cam.AddLight(Camera::light(vigo::shape::ColorAlpha(254/15,254/15,166/15,255),
                                 vigo::shape::ColorAlpha(254,254,166,255),
                                 Point(10.0,0.0001,0.0),false));
      cam.AttachTo(sim);
      cam.Attach(cvs);
      cam.Reset();

      return app.Exec();
    }
    catch(exc& e)
    {
      std::cerr << e.What() << std::endl;
      return -1;
    }
  }


//** eof **********************************************************************
