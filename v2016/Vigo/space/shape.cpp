// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

  #include "./shape.hpp"
  #include "../common/ogl.hpp"
  #include "../vigo/canvas.hpp"
  #include <math.h>

//>>> hack
  namespace vigo{ namespace space { namespace sceneGL
  {
    void GLPush();
    void GLPop();
  }}}

  NAMESPACE_VIGO(shape)

//----------------------------------------------------------------------------

  void Line::Set(algebra3d::Pointd const& a, algebra3d::Pointd const& b)
  {
    m_a = a;
    m_b = b;
  }

  void Line::Draw(Camera const&,algebra3d::Vectord const&) const
  {
    glBegin(GL_LINES);
    glVertex3d(m_a.x,m_a.y,m_a.z);
    glVertex3d(m_b.x,m_b.y,m_b.z);
    glEnd();
  #ifdef WIN32 //>>> !!
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  #endif
  }

//----------------------------------------------------------------------------

  Polygon::Polygon()
    : Shape(), m_vertices()
  {
  }

  Polygon::Polygon(vertices const& v)
    : Shape(), m_vertices(v)
  {
  }

  void Polygon::Set(vertices const& v)
  {
    m_vertices = v;
  }

  void Polygon::Push(algebra3d::Pointd const& vertex)
  {
    m_vertices.push_back(vertex);
  }

  static void PolygonBegin(Camera const& cam)
  {
    if(cam.IsWire()) //>>> should also consider flagWIRE/drawWIRE
      glBegin(GL_LINE_LOOP);
    else
      glBegin(GL_POLYGON);
  }

  void Polygon::Draw(Camera const& cam,algebra3d::Vectord const&) const
  {
    PolygonBegin(cam);

    for(vertices::const_iterator i=m_vertices.begin(); i<m_vertices.end(); ++i)
    {
      glVertex3d(i->x,i->y,i->z);
    }

    glEnd();
  }

  bool Polygon::IsHit(algebra3d::Lined const& ray) const
  {
    //>>> TODO! real polygon intersection
    double dist = ray.Magnitude(algebra3d::Pointd::_0);
    return dist<=1.0;
  }

//----------------------------------------------------------------------------

  void Cube::Draw(Camera const& cam,algebra3d::Vectord const&) const
  {//>>>review with freeglut
    double const &s(m_size);

    PolygonBegin(cam);
    glVertex3d(0, 0, 0);
    glVertex3d(0, s, 0);
    glVertex3d(s, s, 0);
    glVertex3d(s, 0, 0);
    glEnd();

    PolygonBegin(cam);
    glVertex3d(0, 0, s);
    glVertex3d(s, 0, s);
    glVertex3d(s, s, s);
    glVertex3d(0, s, s);
    glEnd();

    PolygonBegin(cam);
    glVertex3d(0, 0, 0);
    glVertex3d(0, 0, s);
    glVertex3d(0, s, s);
    glVertex3d(0, s, 0);
    glEnd();

    PolygonBegin(cam);
    glVertex3d(s, 0, 0);
    glVertex3d(s, s, 0);
    glVertex3d(s, s, s);
    glVertex3d(s, 0, s);
    glEnd();

    PolygonBegin(cam);
    glVertex3d(0, 0, 0);
    glVertex3d(s, 0, 0);
    glVertex3d(s, 0, s);
    glVertex3d(0, 0, s);
    glEnd();

    PolygonBegin(cam);
    glVertex3d(0, s, 0);
    glVertex3d(0, s, s);
    glVertex3d(s, s, s);
    glVertex3d(s, s, 0);
    glEnd();
  }

//----------------------------------------------------------------------------

  void Box::Draw(Camera const& cam,algebra3d::Vectord const&) const
  {//>>>review with freeglut
    double const &x(m_size.x);
    double const &y(m_size.y);
    double const &z(m_size.z);

    double const tmax = 0.5;

    double const x0 = -m_size.x/2.0, x1 = x+x0, xt = x0/tmax;
    double const y0 = -m_size.y/2.0, y1 = y+y0, yt = y0/tmax;
    double const z0 = -m_size.z/2.0, z1 = z+z0, zt = z0/tmax;

    PolygonBegin(cam);
    glTexCoord2d(0.0,0.0);  glVertex3d(x0, y0, z0);
    glTexCoord2d(0.0,yt);   glVertex3d(x0, y1, z0);
    glTexCoord2d(xt, yt);   glVertex3d(x1, y1, z0);
    glTexCoord2d(xt, 0.0);  glVertex3d(x1, y0, z0);
    glEnd();

    PolygonBegin(cam);
    glTexCoord2d(0.0,0.0);  glVertex3d(x0, y0, z1);
    glTexCoord2d(xt, 0.0);  glVertex3d(x1, y0, z1);
    glTexCoord2d(xt, yt);   glVertex3d(x1, y1, z1);
    glTexCoord2d(0.0,yt);   glVertex3d(x0, y1, z1);
    glEnd();

    PolygonBegin(cam);
    glTexCoord2d(0.0,0.0);  glVertex3d(x0, y0, z0);
    glTexCoord2d(zt, 0.0);  glVertex3d(x0, y0, z1);
    glTexCoord2d(zt, yt);   glVertex3d(x0, y1, z1);
    glTexCoord2d(0.0,yt);   glVertex3d(x0, y1, z0);
    glEnd();

    PolygonBegin(cam);
    glTexCoord2d(0.0,0.0);  glVertex3d(x1, y0, z0);
    glTexCoord2d(0.0,yt);   glVertex3d(x1, y1, z0);
    glTexCoord2d(zt, yt);   glVertex3d(x1, y1, z1);
    glTexCoord2d(zt, 0.0);  glVertex3d(x1, y0, z1);
    glEnd();

    PolygonBegin(cam);
    glTexCoord2d(0.0,0.0);  glVertex3d(x0, y0, z0);
    glTexCoord2d(xt, 0.0);  glVertex3d(x1, y0, z0);
    glTexCoord2d(xt, zt);   glVertex3d(x1, y0, z1);
    glTexCoord2d(0.0,zt);   glVertex3d(x0, y0, z1);
    glEnd();

    PolygonBegin(cam);
    glTexCoord2d(0.0,0.0);  glVertex3d(x0, y1, z0);
    glTexCoord2d(0.0,zt);   glVertex3d(x0, y1, z1);
    glTexCoord2d(xt, zt);   glVertex3d(x1, y1, z1);
    glTexCoord2d(xt, 0.0);  glVertex3d(x1, y1, z0);
    glEnd();
  }

//----------------------------------------------------------------------------

  algebra3d::Vector<double> Text::SCALE(0.0005,0.0005,0.0005);

  void Text::Draw(Camera const& cam,algebra3d::Vectord const&) const
  {
    glPushMatrix();
    glScaled(m_scale.x,m_scale.y,m_scale.z);

    // cam: asks (Qt)canvas to do it //>>> fix constness
    const_cast<Camera*>(&cam)->PaintText(algebra3d::Pointd::_0,m_text.c_str());

    glPopMatrix();
  }

//----------------------------------------------------------------------------

  /*static uint ScreenSize(double size,Camera const& cam,algebra3d::Pointd const& pos)
  {//>>>WRONG!!
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT,viewport);

    //>>>was(out) double distance = max(cam.Near(),(cam.Eye()-pos).Length());
    double distance = (cam.Eye()-pos).Project(cam.Eye()-cam.Center());
    uint   viewportHeight = viewport[3]-viewport[1];

    double sSize = size*viewportHeight*tan(algebra3d::deg2rad(cam.Fov()/2)) / (cam.ZoomF()*distance); //>>> verifY!
    return (uint)sSize; //>>> MAXINT
  }*/

//----------------------------------------------------------------------------

  void Disk::Draw(Camera const& cam,algebra3d::Vectord const& /*pos*/) const
  {//>>>make as Sphere
    GLUquadricObj* q = cam.ActiveCanvas()->QuadricObj();

    uint nSlices = 8;

    if(cam.IsWire()) //>>> should also consider flagWIRE/drawWIRE
    {
      gluQuadricDrawStyle(q,GLU_LINE);
      gluQuadricNormals(q,GLU_NONE);
    }
    else
    {
      gluQuadricDrawStyle(q,GLU_FILL);
      gluQuadricNormals(q,GLU_SMOOTH);
    }

    gluQuadricTexture(q,true);//>>> if there is a texture?
    gluQuadricNormals(q,GLU_SMOOTH);

    gluDisk(q,m_holeRadius,m_radius,nSlices,/*loops*/nSlices);
    return;
  }

  bool Disk::IsHit(algebra3d::Lined const& ray) const
  {
    double dist = ray.Magnitude(algebra3d::Pointd::_0);
    return (dist<=m_radius && dist>=m_holeRadius); //>>>TODO orientation
  }

//----------------------------------------------------------------------------

  void Sphere::Draw(Camera const& cam,algebra3d::Vectord const& /*pos*/) const
  {//>>>REVIEW, IS WRONG!!
    //uint screenSize = ScreenSize(2*m_radius,cam,pos);

    //static struct {uint size; uint nSlices;} ns[] =
    //{ {200,96}, {120,48}, {80,36}, {40,24}, {20,12}};

    //static uint NUM_NS = sizeof(ns)/sizeof(ns[0]);
    GLUquadricObj* q = cam.ActiveCanvas()->QuadricObj();

    uint nSlices = 12;

    /*for(uint i=0; i<NUM_NS; ++i)
    {
      if(ns[i].size<=screenSize)
      {
        nSlices = ns[i].nSlices; break;
      }
    }

    if(0.0>=m_radius || nSlices<4)
    {
      glBegin(GL_POINTS);
      glVertex3f(0,0,0);
      glEnd();
      return;
    }*/

    if(cam.IsWire()) //>>> should also consider flagWIRE/drawWIRE
    {
      gluQuadricDrawStyle(q,GLU_LINE);
      gluQuadricNormals(q,GLU_NONE);
    }
    else
    {
      gluQuadricDrawStyle(q,GLU_FILL);
      gluQuadricNormals(q,GLU_SMOOTH);
    }

    gluQuadricTexture(q,true);//>>> if there is a texture?
    gluQuadricNormals(q,GLU_SMOOTH); //>>>out?
    gluSphere(q,m_radius,nSlices,nSlices);
    return;
  }

  bool Sphere::IsHit(algebra3d::Lined const& ray) const
  {
    double dist = ray.Magnitude(algebra3d::Pointd::_0);
    return dist<=m_radius;
  }

//----------------------------------------------------------------------------

  void Cylinder::Draw(Camera const& cam,algebra3d::Vectord const& /*pos*/) const
  {//>>>review and do together with sphere SLICING IS WRONG!!
    //uint screenSize = ScreenSize(2*m_radius,cam,pos);

    //static struct {uint size; uint nSlices;} ns[] =
    //{ {200,96}, {120,48}, {80,36}, {40,24}, {20,12}};

    //static uint NUM_NS = sizeof(ns)/sizeof(ns[0]);
    GLUquadricObj* q = cam.ActiveCanvas()->QuadricObj();

    uint nSlices = 12;

    /*for(uint i=0; i<NUM_NS; ++i)
    {
      if(ns[i].size<=screenSize)
      {
        nSlices = ns[i].nSlices; break;
      }
    }*/

    uint nStacks = (uint)(nSlices * (m_height / m_radius));
    uint nRings = nSlices;; //>>>

    if(cam.IsWire()) //>>> should also consider flagWIRE/drawWIRE
    {
      gluQuadricDrawStyle(q,GLU_LINE);
      gluQuadricNormals(q,GLU_NONE);
    }
    else
    {
      gluQuadricDrawStyle(q,GLU_FILL);
      gluQuadricNormals(q,GLU_SMOOTH);
    }

    //>>> add texturing
    gluCylinder(q,m_radius,m_radius,m_height,nSlices,nStacks);

    if(m_bottoms)
    {
      gluDisk(q,0,m_radius,nSlices,nRings);
      glTranslated(0.0,0.0,m_height);
      gluDisk(q,0,m_radius,nSlices,nRings);
    }

  /*>>
    int slices, stacks, rings;
    if(m_radius<1.0) //>>> better
    {
      slices = 10;
      stacks = rings = 2;
    }
    else
    {
      slices = 20;
      stacks = rings = 4;
    }

    if(flags && flags->IsWire())
    {
      gluQuadricDrawStyle(q,GLU_LINE);
      gluQuadricNormals(q,GLU_NONE);
    }
    else
    {
      gluQuadricDrawStyle(q,GLU_FILL);
      gluQuadricNormals(q,GLU_SMOOTH);
    }

    gluCylinder(q,m_radius,m_radius,m_height,slices,stacks);
    if(m_bottoms)
    {
      gluDisk(q,0,m_radius,slices,rings);
      glTranslated(0.0,0.0,m_height);
      gluDisk(q,0,m_radius,slices,rings);
    }
  */}

NAMESPACE_END() // eof
