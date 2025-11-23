// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

  #include "./scene.hpp"
  #include "../common/ogl.hpp"

NAMESPACE_VIGO(space) //=======================================================

  void SceneBase::UpdateFrame() //>>> should perhaps only flag the scene
  {
    if(m_simulation) m_simulation->UpdateGL(0xFFFFFFFF);
  }

//-----------------------------------------------------------------------------

  void sceneGL::GLPush() { glPushMatrix(); }
  void sceneGL::GLPop()  { glPopMatrix();  }

  void sceneGL::GLTranslate(algebra3d::Pointd const& pos)
  {
    glTranslated(pos.x,pos.y,pos.z);
  }

  void sceneGL::GLTranslate(algebra3d::Pointf const& pos)
  {
    glTranslatef(pos.x,pos.y,pos.z);
  }

  void sceneGL::GLTranslate(algebra3d::Pointi const& pos)
  {
    glTranslatef(pos.x,pos.y,pos.z);
  }

  void sceneGL::GLRotate(algebra3d::Rotation const& rot)
  {
    glRotated(algebra3d::rad2deg(rot.th),rot.v.x,rot.v.y,rot.v.z);
  }

  void sceneGL::GLEnableLighting()  { glEnable (GL_LIGHTING); }
  void sceneGL::GLDisableLighting() { glDisable(GL_LIGHTING); }

  void sceneGL::GLColor(shape::ColorAlpha const &c)
  {
    glColor4d(c.r,c.g,c.b,c.a);
  }

  void sceneGL::GLColor(double r, double g, double b, double a)
  {
    glColor4d(r,g,b,a);
  }

NAMESPACE_END() // eof
