// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

#ifndef BOIDS_OBJECT_HPP_INCLUDED
#define BOIDS_OBJECT_HPP_INCLUDED

  #include <vigosim>
  #include "scene.hpp"

//-----------------------------------------------------------------------------

  typedef vigo::space::Node<float,
          vigo::parts::Parts<
          vigo::parts::PP<float>,
          vigo::parts::RR,
          vigo::parts::T0<float>,
          vigo::parts::SS,
          vigo::parts::A1
        >
      > ObjectNode;

//-----------------------------------------------------------------------------

  class PolygonObject: public ObjectNode
  {
    SUPER(ObjectNode)

  private:
    Vector m_normal;
    float  m_d;

  public:
    PolygonObject(Scene&);
   ~PolygonObject();

    static PolygonObject& FromString(Scene&,std::string const&);

    GETTER(GetScene, Scene&, (Scene&)super::GetScene())
    GETSET(Normal, SetNormal, Vector, m_normal)
    GETSET(D,      SetD,      float,   m_d)

    // R0-ray origin, Rd-ray direction, Ri-returned intersection point
    bool IntersectByRay(Point const& R0,Vector const& Rd,Point& Ri) const;
  };


//-----------------------------------------------------------------------------

#endif

// eof
