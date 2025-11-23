// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief Colours.
 *
 */

#ifndef VIGO_SPACE_COLOR_HPP_INCLUDED
#define VIGO_SPACE_COLOR_HPP_INCLUDED

  #include "../common/types.hpp"
  #include "../common/tuple.hpp"

NAMESPACE_VIGO(shape) //=======================================================
/** \defgroup grp_color_hpp Colours
@{*/

  /// RGB colour.
  struct Color
  {
    double r, g, b;

    /// White colour.
    Color(): r(1.0), g(1.0), b(1.0)
    {
    }

    /// Specified, 0..1.
    Color(double ar,double ag,double ab)
      : r(ar), g(ag), b(ab)
    {
    }

    /// \overload
    Color(util::double3d const& d)
      : r(d.x), g(d.y), b(d.z)
    {
    }

    /// Specified, 0..255.
    Color(int ar,int ag,int ab)
      : r(ar/255.), g(ag/255.), b(ab/255.)
    {
    }

    /// \overload
    Color(util::int3d const& u)
      : r(u.x/255.), g(u.y/255.), b(u.z/255.)
    {
    }
  };


  /// Transparency
  struct Alpha
  {
    double a;

    /// Opaque.
    Alpha(): a(1.0)
    {
    }

    /// Specified. 0=fully transparent, 1=opaque.
    Alpha(double aa)
      : a(aa)
    {
    }
  };

  /// RGBA
  struct ColorAlpha: Color, Alpha
  {
    /// White, opaque.
    ColorAlpha()
      : Color(1.,1.,1.), Alpha(1.)
    {
    }

    /// Specified, 0..1
    ColorAlpha(double r,double g,double b,double a=1.0)
      : Color(r,g,b), Alpha(a)
    {
    }

    /// \overload
    ColorAlpha(util::double4d const& d)
      : Color(d.x,d.y,d.z), Alpha(d.w)
    {
    }

    /// Specified, 0..255
    ColorAlpha(int r,int g,int b,int a=255)
      : Color(r,g,b), Alpha(a/255.)
    {
    }

    /// \overload
    ColorAlpha(util::int4d const& u)
      : Color(u.x,u.y,u.z), Alpha(u.w/255.)
    {
    }

    ColorAlpha(Color const& c)
      : Color(c.r,c.g,c.b), Alpha(1.0)
    {
    }
  };

/**@} (defgroup)*/
NAMESPACE_END() //=============================================================

#endif

// eof
