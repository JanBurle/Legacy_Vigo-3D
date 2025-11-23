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
* $Id: main.cpp 192 2007-01-13 03:54:25Z ian_b $
\*****************************************************************************/

/** \file
 */

  #include <vigoapp>
  #include <vigosim>

/*
  The physics is wrong, needs revision. Watch the Moon crash and Earth fly away.
*/

//-----------------------------------------------------------------------------
// parameters

  namespace 
  {
     double const FRAME_RATE      = 30.0;   // 30 frames a second
     uint   const NUM_FRAME_STEPS = 100;    // 10 steps per frame
     double const SPEEDUP         = 1e5;    // make simulation time much faster
     double const DELTA_T         = SPEEDUP/(FRAME_RATE*NUM_FRAME_STEPS); //[s]

    // planetarium
     double const G = 6.67e-11; // [Nm2/kg2]

     double const EARTH_MASS     = 5.97e24; //kg
     double const EARTH_RADIUS   = 6378e3;  //m

     double const MOON_MASS      = 7.35e22; //kg
     double const MOON_RADIUS    = 1738e3;  //m
     double const MOON_DISTANCE  = 384e6;
     double const MOON_PERIOD    = 27.32 * 24 * 3600; //s

    // a satelite on an orbit
     double const SAT_MASS       = 1e4;     //kg
     double const SAT_RADIUS     = 666e3;   //m
     double const SAT_DISTANCE   = 80e6;    //m
     double const SAT_SPEED      = 60000.0;  //s

     double const MAGNIFY        = 10.0;    // make bodies bigger
     
     double const SPACE_RADIUS   = MOON_DISTANCE*1.2;
  }

//-----------------------------------------------------------------------------
// celestial body description

  using namespace vigo;
  using namespace vigo::math;
  using shape::Color;
  
  typedef algebra3d::Pointd  Point;
  typedef algebra3d::Vectord Vector;
  typedef space::Scened      Scene;
  
  struct sBodyData
  {
    double mass, radius;
    Point  position;
    Vector velocity;
    Color  color;
    bool   lightSource;
    pcstr  textureFile; uint texWidth, texHeight;
  }
  body_data[] =
  {
    /* Earth */ { EARTH_MASS, EARTH_RADIUS, 
                  Point::_0, Vector::_0, Color(0,0,1), false,
                  "earth.bmp", 256, 256},
    /* Moon  */ { MOON_MASS,  MOON_RADIUS,
                  Point(0,0,MOON_DISTANCE), Vector(MOON_DISTANCE*(double)pi2/MOON_PERIOD,0,0), Color(.7,.7,.7), false,
                  "moon.bmp", 256, 128},
    /* satel */ { SAT_MASS,   SAT_RADIUS,   Point(0,0,SAT_DISTANCE),  Vector(0,SAT_SPEED,0),  Color(.0,.7,.7), false,
                  NULL, 0, 0}
  };
  
  uint const NUM_BODIES = sizeof(body_data)/sizeof(body_data[0]);

//-----------------------------------------------------------------------------
// a body 

  typedef space::Leaf<double,
            parts::Parts<
        parts::PV<double>, // position and velocity
        parts::RR,         // rotation
        parts::T0<double>, // no tracking
        parts::SS,         // shape
        parts::A1          // appearance
      >
    > body_t;

  class Body: public body_t
  {
    SUPER(body_t)
  public:
    // all bodies remembered here
    typedef std::list<Body*> bodies_t;
  private:
    static bodies_t m_bodies;

  public:
    static bodies_t& All()   { return m_bodies; }

  // the body mass
  private:
    double m_mass;
  public:
    GETTER(Mass,double,m_mass)

  public:
    Body(Scene& s, sBodyData const& bd)
      : super(s,true), m_mass(bd.mass)
    {
      // remember yourself as a body
      m_bodies.push_back(this);

      // make yourself visible in the scene
      SetShape(*new shape::Sphere(bd.radius*MAGNIFY));

      shape::Appearance *app = new shape::Appearance(*new shape::ShapeColorAlpha(bd.color));
      if(bd.lightSource)
        app->SetNoLight(true);
      else
        app->SetLight(true);
      SetAppearance(*app);

      // possibly with a texture
      if(bd.textureFile)
        app->SetTexture(new shape::Texture(bd.textureFile,bd.texWidth,bd.texHeight));

      // initial position and velocity
      SetPos(bd.position);
      SetVel(bd.velocity);
    }

    ~Body()
    {
      // forget yourself
      bodies_t::iterator it = std::find(m_bodies.begin(),m_bodies.end(),this);
      m_bodies.erase(it);
    }
  };

  Body::bodies_t Body::m_bodies;

//-----------------------------------------------------------------------------

  class GravitySimulation: public Simulation
  {
    SUPER(Simulation)
    
  public:
    GravitySimulation()
    {
      SetFrameRate(FRAME_RATE);
      SetFrameSteps(NUM_FRAME_STEPS);
    }

   ~GravitySimulation()
    {
    }
  };

//----------------------------------------------------------------------------
// a scene with multiple bodies

  class BodySystem: public Scene
  {
    SUPER(Scene)

  public:
    BodySystem(): super()
    {
      SetDeltaT(DELTA_T);

      /*>>>m_lights.push_back(
        light(shape::ColorAlpha(254/15,254/15,166/15,255/15),
              shape::ColorAlpha(254,254,166,255),Point(0.0,0.0001,0.0),false)
      );*/

      Create();
    }

    double  CameraFar() const { return 2*SPACE_RADIUS;   }
    Point   CameraEye() const { return Point(SPACE_RADIUS,0,0);}
    double  CameraFov() const { return 55;}
    
  private:
    void Create()
    {
      // add bodies
      for(uint i=0; i<NUM_BODIES; ++i)
      {
        Root().Add(*new Body(*this,body_data[i]));
      }
    }

  private:
    // update velocities
    virtual bool Act()
    {
      Body::bodies_t bodies = Body::All();
      Body::bodies_t::iterator bodiesBegin = bodies.begin(),
                               bodiesEnd   = bodies.end();

      // enumerate all pairs
      for(Body::bodies_t::iterator ai = bodiesBegin; ai!=bodiesEnd; ai++)
      {
        for(Body::bodies_t::iterator bi = ai; bi++, bi!=bodiesEnd;)
        {
          // mutual acceleration by a pair of bodies
          Body &a = **ai, &b = **bi;

          // distance and direction
          Vector v = b.Pos() - a.Pos();
          double distance   = v.Length();
          v = v.Unit();

          // the schwartz
          double F = G * (a.Mass() * b.Mass()) / (distance*distance);

          // acceleration
          Vector aAccel =   v*F / a.Mass();
          Vector bAccel = -(v*F / b.Mass());

          // accumulate 
          a.SetVel(a.Vel() + aAccel*DELTA_T);
          b.SetVel(b.Vel() + bAccel*DELTA_T);
        }
      }

      super::Act();
      return true;
    }
  };


//-----------------------------------------------------------------------------

  int main(int argc, pstr argv[])
  {
    try
    {
      Application app("gravity",argc,argv);      
      
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

      Window wnd("Gravity");
      Canvas cvs(wnd);
      GravitySimulation sim;
      BodySystem scene;
      sim.SetScene(scene);

      Camera cam;
      cam.Attach(cvs);
      cam.AttachTo(sim);
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
