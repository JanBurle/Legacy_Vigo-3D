// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

#ifndef TONNETZ_MSCENE_HPP_INCLUDED
#define TONNETZ_MSCENE_HPP_INCLUDED

  #include "scene.hpp"
  #include "tagent.hpp"

//-----------------------------------------------------------------------------

  class MoebiusScene: public Scene
  {
    SUPER(Scene)

  public:
    MoebiusScene();

    void SetRHN(double r, double h, uint n); // Radius, Height, Number of loops
    void Play(int what);

  private:
    void Create();

  public:
    void SetFreq(float freq,bool just);

  protected:
    void DisplayRoot(vigo::Camera const&,vigo::space::sceneGL::eDisplay);

  private:
    vigo::util::refptr<vigo::geom::MoebiusBoundary> m_moebius;
    std::vector<TAgent*> m_agents;
  };

//-----------------------------------------------------------------------------

#endif

// eof
