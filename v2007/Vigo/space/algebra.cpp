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
* $Id:algebra.cpp 30 2006-04-06 19:13:28Z ian_b $
\*****************************************************************************/

  #include "./algebra.hpp"
  #include <iostream>
 
  NAMESPACE_VIGO(algebra3d)
  
//----------------------------------------------------------------------------

  static void NormUnit(Quaternion& q)
  {
    double scale = sqrt(q.w*q.w + q.x*q.x + q.y*q.y +q.z*q.z);
    q.w /= scale;
    q.x /= scale;
    q.y /= scale;
    q.z /= scale;
  }

  Quaternion::Quaternion()
    : super(0,0,0,1)  // identity
  {
  }

  Quaternion::Quaternion(Quaternion const& r)
    : super(r)
  {
  }

  Quaternion::Quaternion(double w, double x, double y, double z)
    : super(x,y,z,w) //>>> swap
  {
    NormUnit(*this);
  }

  Quaternion::Quaternion(Rotation const& r)
  {
    *this = r;
  }

  Quaternion::Quaternion(rad pitchX, rad yawY, rad rollZ)
  {
    //>>> for some reason is wrong, so correct
    double Z=pitchX, X=yawY, Y=rollZ;
    double sx  = sin(X / 2),
           cx  = cos(X / 2),
           sy  = sin(Y / 2),
           cy  = cos(Y / 2),
           sz  = sin(Z / 2),
           cz  = cos(Z / 2),
           cxy = cx * cy,
           sxy = sx * sy;

    w = cz * cxy        + sz * sxy;
    x = sz * cxy        - cz * sxy;
    y = cz * sx * cy + sz * cx * sy;
    z = cz * cx * sy - sz * sx * cy;
  }

  Quaternion& Quaternion::operator=(Quaternion const& r)
  {
    w = r.w;
    x = r.x;
    y = r.y;
    z = r.z;
    return *this;
  }

  Quaternion& Quaternion::operator=(Rotation const& r)
  {
    double s = sin(r.th / 2),
           c = cos(r.th / 2);

    w = c;
    x = r.v.x * s;
    y = r.v.y * s;
    z = r.v.z * s;

    return *this;
  }

  Quaternion Quaternion::operator - () const
  {
    return Quaternion(w,-x,-y,-z);
  }

  Quaternion Quaternion::operator * (double d) const
  {
    return Quaternion(*this) *= d;
  }

  Quaternion& Quaternion::operator *=(double f)
  {
    double theta = f * acos(w) * 2.0;
    w = cos(theta / 2.0);
    NormUnit(*this);
    return *this;
  }

  Quaternion Quaternion::operator * (Quaternion const& r) const
  {
    return Quaternion(w*r.w - x*r.x - y*r.y - z*r.z,
                      w*r.x + x*r.w + y*r.z - z*r.y,
                      w*r.y - x*r.z + y*r.w + z*r.x,
                      w*r.z + x*r.y - y*r.x + z*r.w);
  }

  Quaternion& Quaternion::operator*=(Quaternion const& r)
  {
    return (*this = (*this * r));
  }

  Quaternion const Quaternion::_1(1.0, 0.0, 0.0, 0.0);

//----------------------------------------------------------------------------

  Rotation::Rotation()
    : th(0.0), v(Vectord::Z)
  {
  }

  Rotation::Rotation(Rotation const& r)
    : th(r.th), v(r.v)
  {
  }

  Rotation::Rotation(Vectord const& av)
  {
    *this = av;
  }

  Rotation::Rotation(rad ath,Vectord const& av)
    : th(ath), v(av.Unit())
  {
  }

  Rotation::Rotation(Quaternion const& q)
  {
    *this = q;
  }

  Rotation::Rotation(rad pitchX, rad yawY, rad rollZ)
  {
    *this = Quaternion(pitchX,yawY,rollZ);
  }

  Rotation& Rotation::operator=(Rotation const& r)
  {
    th = r.th;
    v  = r.v;
    return *this;
  }

  Rotation& Rotation::operator=(Vectord const& av)
  {
    if(av.Magnitude()<EPS)
    {
      th = 0;
       v = Vectord::Z;
    }
    else
    {
      th = av.Angle(Vectord::Z);
      Vectord norm = Vectord::Z ^ av;

      if(norm.Magnitude()<EPS)
        v = Vectord::X.Normal(av); // use arbitrary vector != Z
      else
        v  = norm.Unit();
    }
    return *this;
  }

  Rotation& Rotation::operator=(Quaternion const& q)
  {
    double scale = sqrt(q.x*q.x + q.y*q.y + q.z*q.z);
    if(scale<EPS) // no rotation
      *this = Rotation(0.0,Vectord::Z);
    else
      *this = Rotation(2 * acos(q.w),
                       Vectord(q.x/scale,q.y/scale,q.z/scale));

    return *this;
  }

  Rotation Rotation::operator + (Rotation const& r) const
  {
    if(0.0==th)
      return r;

    if(0.0==r.th)
      return *this;

    return Quaternion(*this) * Quaternion(r);
  }

  Rotation& Rotation::operator +=(Rotation const& r)
  {
    *this = (*this + r);
    return *this;
  }

  Rotation  Rotation::operator - () const
  {
    return Rotation(-th,v);
  }

  Rotation Rotation::operator - (Rotation const& r) const
  {
    return *this + (-r);
  }

  Rotation& Rotation::operator -=(Rotation const& r)
  {
    return *this += (-r);
  }

  Rotation Rotation::operator * (double r) const
  {
    return Rotation(th*r,v);
  }

  Rotation& Rotation::operator *=(double r)
  {
    th *= r;
    return *this;
  }

  Rotation Rotation::operator / (double r) const
  {
    return Rotation(th/r,v);
  }

  Rotation& Rotation::operator /=(double r)
  {
    th /= r;
    return *this;
  }

  std::ostream& operator<<(std::ostream& os, Rotation const& r)
  {
    return os << r.th << ',' << r.v.x << ',' << r.v.y << ',' << r.v.z;
  }

  Rotation const Rotation::_0;

NAMESPACE_END() //** eof ******************************************************
