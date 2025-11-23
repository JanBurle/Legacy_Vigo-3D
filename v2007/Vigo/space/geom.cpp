/*****************************************************************************\
* This file is a part of Vigo::3D, a multi-agent simulation and visualization *
* system.                                                                     *
*                                                                             *
* Copyright (c) 2002--2006 by Ian G. Burleigh       burleigh@cpsc.ucalgary.ca *
* burleigh@cpsc.ucalgary.ca                         http://ard-galen.org/vigo *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for   *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program; if not, write to the                                     *
*   Free Software Foundation, Inc.,                                           *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                 *
*-----------------------------------------------------------------------------*
* $Id: geom.cpp 30 2006-04-06 19:13:28Z ian_b $
\*****************************************************************************/

  #include "./geom.hpp"
  #include <math.h>

  NAMESPACE_VIGO(geom)

  using algebra3d::Pointd;
  using algebra3d::Vectord;
  using algebra3d::rad;

//----------------------------------------------------------------------------

  double Curve3::u01(double u) const
  {
    return m_minPar + u*(m_maxPar-m_minPar);
  }

  Pointd Curve3::CurvePoint_(double u) const
  {
    return m_f ? (*m_f)(u) : ParFun(u);
  }

  Pointd Curve3::CurvePoint(double u) const
  {
    return m_scale * Vectord(CurvePoint_(u));
  }

  Vectord Curve3::Tangent(double u) const
  {
    double const EPS = (m_maxPar-m_minPar)/1000;//>>> arbitrary, revise
    return (CurvePoint_(u+EPS)-CurvePoint_(u)).Unit();
    //>>> whatis Unit() of _0, I guess should be _0
  }

  Vectord Curve3::Across(double u) const //>>>hack
  {
    double const EPS = (m_maxPar-m_minPar)/2;
    return (CurvePoint_(u+EPS)-CurvePoint_(u)).Unit();
  }

//----------------------------------------------------------------------------

  Pointd Circle::ParFun(double theta) const
  {
    return Pointd(m_r*cos(theta), m_r*sin(theta), 0.0);
  }

  Circle::Circle(double r)
    : super(0.0,math::_2pi),
      m_r(r)
  {
  }

//----------------------------------------------------------------------------

  Pointd Ellipse::ParFun(double theta) const
  {
    double rt = Radius(theta);
    Pointd ret(rt*cos(theta), rt*sin(theta), 0.0);
    if(m_swapped)
      swap(ret.x,ret.y);
    return ret;
  }

  Ellipse::Ellipse(double a,double b)
    : super(0.0,math::_2pi),
      m_a(), m_b(), m_swapped(false)
      /* m_e(), m_e2(), m_1_e2() */
  {
    Set(a,b);
  }

  void Ellipse::Set(double a, double b)
  {
    m_swapped = (a<b);
    if(m_swapped)
      swap(a,b);

    m_a = a;
    m_b = b;

    if(m_a < algebra3d::EPS)
      m_e = 0.0;
    else
      m_e    = sqrt(1-sqr(b)/sqr(a));

    m_e2   = m_e*m_e;
    m_1_e2 = 1 - m_e2;
  }

  double Ellipse::Radius(rad theta) const
  {
    return m_a * sqrt(m_1_e2 / (1 - m_e2*sqr(cos(theta))));
  }

//----------------------------------------------------------------------------

  Pointd OrbitalEllipse::ParFun(double theta) const
  {
    double rt = Radius(theta);
    Pointd ret(/*m_c +*/ rt*cos(theta), rt*sin(theta), 0.0);
    if(m_swapped)
      swap(ret.x,ret.y);
    return ret;
  }

  OrbitalEllipse::OrbitalEllipse(double a,double b)
    : super(0.0,math::_2pi),
      m_a(), m_b(), m_swapped(false)
      /* m_e(), m_1_e2(), m_c() */
  {
    Set(a,b);
  }

  void OrbitalEllipse::Set(double a, double b)
  {
    m_swapped = (a<b);
    if(m_swapped)
      swap(a,b);

    m_a = a;
    m_b = b;

    if(m_a < algebra3d::EPS)
      m_e = 0.0;
    else
      m_e = sqrt(1-sqr(b)/sqr(a));

    m_1_e2 = 1 - sqr(m_e);
  }

  double OrbitalEllipse::Radius(rad theta) const
  {
    return m_a * m_1_e2 / (1 + m_e*cos(theta));
  }

//----------------------------------------------------------------------------

  Pointd MoebiusBoundary::ParFun(double theta) const
  {
    double r = m_r + m_h*cos(theta/m_n);
    return Pointd(r*cos(theta), r*sin(theta), m_h*sin(theta/m_n));
  }

  MoebiusBoundary::MoebiusBoundary(double r, double h, uint n)
    : super(0.0,2*(double)math::_2pi),
      m_r(r), m_h(h), m_n(2)
  {
    SetN(n);
  }

  void MoebiusBoundary::SetN(uint n)
  {
    RUNTIME_CHECK_EXC2(n>=1, "n must be at least 1")
    m_n = n;
    SetPar(0.0,m_n*(double)math::_2pi);
  }

//----------------------------------------------------------------------------

  Tube::Tube(Curve3* major,Curve3* minor)
    : m_majorCurve(major ? major : new Circle(1.0)),
      m_minorCurve(minor ? minor : new Circle(0.1))
  {
  }

  Pointd Tube::SurfacePoint(double u, double v) const
  {
    Vectord t  = m_majorCurve->Tangent(u);
    Vectord n1 = t.Normal(m_majorCurve->Across(u));
    if(n1==Vectord::_0) // t and 'across' were parallel
    {
      n1 = t.Normal(Vectord::Z);
      if(n1==Vectord::_0) // t and Z were parallel
        n1 = t.Normal(Vectord::Y);
    }

    Vectord n2 = t.Normal(n1);
    Pointd   p = m_minorCurve->CurvePoint(v);

    return m_majorCurve->CurvePoint(u) + (n1*p.x + n2*p.y + t*p.z); // typically p.z=0
  }

//----------------------------------------------------------------------------

  Torus::Torus(double r1, double r2)
    : Tube(new Circle(r1),new Circle(r2))
  {
  }

NAMESPACE_END() //** eof ******************************************************
