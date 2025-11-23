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
* $Id: main.cpp 46 2006-06-21 10:33:26Z ian_b $
\*****************************************************************************/

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

//** eof **********************************************************************
