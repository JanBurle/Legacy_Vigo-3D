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
* $Id: shape.hpp 193 2007-01-13 03:55:19Z ian_b $
\*****************************************************************************/

/** \file
 *  \brief Shape classes.
 *
 */

#ifndef VIGO_SPACE_SHAPE_HPP_INCLUDED
#define VIGO_SPACE_SHAPE_HPP_INCLUDED

  #include "../common/refcnt.hpp"
  #include "./algebra.hpp"
  #include "./color.hpp"
  #include "../vigo/camera.hpp"
  #include "./appearance.hpp"

  #include <vector>
  #include <string>
  #include <stdarg.h>

  NAMESPACE_VIGO(scene)
    class Scene;
  NAMESPACE_END()

NAMESPACE_VIGO(shape) //=======================================================
/** \defgroup grp_shape_hpp Shapes
@{*/

  class Shape: public util::refcnt_auto
  {
    SUPER(util::refcnt_auto)

  protected:
    Shape(): super()
    {
    }

  public:
    virtual void Draw(Camera const&,algebra3d::Vectord const&/*tracking
                      the screen distance*/) const = 0;                  
    /// Selection.
    virtual bool IsHit(algebra3d::Lined const& /*ray*/) const
      { return false; }
  };

//----------------------------------------------------------------------------

  class Line: public Shape
  {
    SUPER(Shape)

  private:
    algebra3d::Pointd m_a, m_b;

  public:
    Line(algebra3d::Pointd const& a, algebra3d::Pointd const& b)
      : Shape(), m_a(a), m_b(b)
    {
    }

    Line(algebra3d::Pointf const& a, algebra3d::Pointf const& b)
      : Shape(), m_a(a), m_b(b)
    {
    }

    GETTER(A,algebra3d::Pointd,m_a)
    GETTER(B,algebra3d::Pointd,m_b)
    void Set(algebra3d::Pointd const& a, algebra3d::Pointd const& b);
    void Set(algebra3d::Pointf const& a, algebra3d::Pointf const& b)
    {
      Set(algebra3d::Pointf(a), algebra3d::Pointf(b));
    }

    //>>> TODO bool IsHit(algebra3d::Lined const& /*ray*/) const;
    
  protected:
    void Draw(Camera const&,algebra3d::Vectord const&) const;
  };

//----------------------------------------------------------------------------

  class Polygon: public Shape
  {
    SUPER(Shape)

  public:
    typedef std::vector<algebra3d::Pointd> vertices;

    Polygon();
    Polygon(vertices const&);

    void Set(vertices const&);

    void Push(algebra3d::Pointd const& vertex);
    void Push(algebra3d::Pointf const& vertex) { Push(algebra3d::Pointd(vertex)); }

    GETTER(N,uint,m_vertices.size())

    algebra3d::Pointd GetVertex(uint n) const
    {
      return m_vertices[n];
    }

    bool IsHit(algebra3d::Lined const& /*ray*/) const;
    
  protected:
    void Draw(Camera const&,algebra3d::Vectord const&) const;

  private:
    vertices m_vertices;
  };

//----------------------------------------------------------------------------

  class Cube: public Shape //>>> TODO subclass Box?
  {
    SUPER(Shape)

  private:
    double m_size;

  public:
    Cube(double size): Shape(), m_size(size)
    {
    }

    GETSET(Size,SetSize,double,m_size)

    //>>> TODO bool IsHit(algebra3d::Lined const& /*ray*/) const;
    
  protected:
    void Draw(Camera const&,algebra3d::Vectord const&) const;
  };

//----------------------------------------------------------------------------

  class Box: public Shape
  {
    SUPER(Shape)

  private:
    algebra3d::Vectord m_size;

  public:
    Box(algebra3d::Vectord size): Shape(), m_size(size)
    {
    }

    Box(double x, double y, double z): Shape(), m_size(x,y,z)
    {
    }
		
    GETSET(Size,SetSize,algebra3d::Vectord,m_size)
    
    //>>> TODO bool IsHit(algebra3d::Lined const& /*ray*/) const;
    
  protected:
    void Draw(Camera const&,algebra3d::Vectord const&) const;
  };

//----------------------------------------------------------------------------

  class Text: public Shape
  {
    SUPER(Shape)

  private:
    std::string        m_text;
    algebra3d::Vectord m_scale;

    static algebra3d::Vectord SCALE;

  public:
    Text(pcstr text): Shape(), m_text(text), m_scale(SCALE)
    {
    }

    GETTER(GetText,std::string const&,m_text)
    void SetText(pcstr text)
    {
      m_text = text ? text : "";
    }

  protected:
    virtual void Draw(Camera const&,algebra3d::Vectord const&) const;
  };

//----------------------------------------------------------------------------
// Disk

  class Disk: public Shape
  {
    SUPER(Shape)

  private:
    double m_radius, m_holeRadius;

  public:
    Disk(double radius, double holeRadius=0.0)
      : super(), m_radius(radius), m_holeRadius(holeRadius)
    {
    }

    GETSET(Radius,    SetRadius,     double, m_radius)
    GETSET(HoleRadius,SetHoleRadius, double, m_holeRadius)

    bool IsHit(algebra3d::Lined const& ray) const;

  protected:
    void Draw(Camera const&,algebra3d::Vectord const&) const;
  };

//----------------------------------------------------------------------------
// Sphere

  class Sphere: public Shape
  {
    SUPER(Shape)

  private:
    double m_radius;

  public:
    Sphere(double radius): super(), m_radius(radius)
    {
    }

    GETSET(Radius,SetRadius,double,m_radius)

    bool IsHit(algebra3d::Lined const& ray) const;

  protected:
    void Draw(Camera const&,algebra3d::Vectord const&) const;
  };

//----------------------------------------------------------------------------
// Cylinders

  class Cylinder: public Shape
  {
    SUPER(Shape)

  private:
    double m_radius;
    double m_height;
    bool   m_bottoms;

  public:
    Cylinder(double radius,double height,bool bottoms=true)
    : super(), m_radius(radius), m_height(height), m_bottoms(bottoms)
    {
    }

    GETSET(Radius, SetRadius, double,m_radius)
    GETSET(Height, SetHeight, double,m_height)
    GETSET(Bottoms,SetBottoms,bool,  m_bottoms)

    //>>> TODO bool IsHit(algebra3d::Lined const& /*ray*/) const;
    
  protected:
    void Draw(Camera const&,algebra3d::Vectord const&) const;
  };

/**@} (defgroup)*/
NAMESPACE_END() //=============================================================

#endif

//** eof **********************************************************************
