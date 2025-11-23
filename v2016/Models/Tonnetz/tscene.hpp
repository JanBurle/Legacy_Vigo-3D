// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

#ifndef TONNETZ_TSCENE_HPP_INCLUDED
#define TONNETZ_TSCENE_HPP_INCLUDED

  #include "scene.hpp"
  #include "tagent.hpp"

//----------------------------------------------------------------------------

  class TSpace: public vigo::geom::Curve3 // hack
  {
    SUPER(vigo::geom::Curve3)

  public:
    TSpace(): super(0,0) {}

    Pointd ParFun(double u) const
    {
      double x, y;
      y = floor(u/1000.); x = u-y;
      return Pointd(x,y,0.);
    }
  };

//-----------------------------------------------------------------------------

  class TonnetzScene: public Scene
  {
    SUPER(Scene)

  public:
    TonnetzScene();

  private:
    void Create();

  public:
    void SetFreq(float freq,bool just);

  protected:
    void DisplayRoot(vigo::Camera const&,vigo::space::sceneGL::eDisplay);

  private:
    vigo::util::refptr<TSpace>  m_space;
    std::vector<TAgent*>	m_agents;
  };


//-----------------------------------------------------------------------------

#endif

// eof
