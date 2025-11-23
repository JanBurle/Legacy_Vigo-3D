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

  #include "object.hpp"
  #include "params.hpp"

//-----------------------------------------------------------------------------

  PolygonObject::PolygonObject(Scene& s)
    : super(s,true),
      m_normal(), m_d(0)
  {
  }

  PolygonObject::~PolygonObject()
  {
  }

//-----------------------------------------------------------------------------

  static bool match(std::string& s, pcstr head, bool eat)
  {
    assert(!vigo::empty(head));
    uint l = strlen(head);
    if(s.substr(0,l)==head)
    {
      if(eat) s = vigo::trimleft(s.substr(l));
      return true;
    }

    return false;
  }

  static bool triple(std::string& s, float& a, float& b, float& c, bool eat)
  {
    if(eat && !match(s,"(",true))
      return false;

    if(3!=sscanf(s.c_str(),"%f,%f,%f", &a, &b, &c))
      return false;

    uint pos = s.find(")");
    s = vigo::trimleft(s.substr(pos+1));

    return true;
  }

  PolygonObject& PolygonObject::FromString(Scene& scene,std::string const& objectData)
  {
    // Note: the following code is not exception-safe (leaks PolygonObjects and Polygons)
    // I am ok with it, since an exception should abort the simulation
    std::string s(objectData);
    PolygonObject &o = *new PolygonObject(scene);
    
    if(match(s,"poly",true))
    {
      vigo::shape::Polygon &p = *new vigo::shape::Polygon();
      o.SetShape(p);

      for(;;)
      {
        if(s.empty()) break;

        if(match(s,"c",true))      // color
        {
          float r, g, b;
          if(!triple(s,r,g,b,true))
            THROW_EXC(vigo::sformat("syntax error: [%s]",objectData.c_str()))

          o.SetAppearance(*new vigo::shape::Appearance(r,g,b));
        }
        else if(match(s,"(",false))  // point
        {
          float x, y, z;
          if(!triple(s,x,y,z,true))
            THROW_EXC(vigo::sformat("syntax error: [%s]",objectData.c_str()))

          p.Push(Point(x,y,z));

          if(3==p.N())  // calculate its normal vector
          {
            Pointd  P1 = p.GetVertex(0),
                    P2 = p.GetVertex(1),
                    P3 = p.GetVertex(2);
            Vectord V1 = P2 - P1;
            Vectord V2 = P3 - P2;
            Vectord  N = (V1^V2).Unit();
            o.SetNormal(Vector(N));
            double d = -(N*P1);
            o.SetD(d);
          }
        }
        else
        {
          THROW_EXC(vigo::sformat("syntax error: [%s]",objectData.c_str()))
        }
      }

      if(p.N()<3)
        THROW_EXC("incomplete polygon")
    }
    else
    {
      THROW_EXC(vigo::sformat("unknown object type: [%s]",objectData.c_str()))
    }

    return o;
  }

//-----------------------------------------------------------------------------

  typedef vigo::util::tuple2<float> uv;

  static uv Flatten(Point const& p, uint ind)
  {
    switch(ind)
    {
    case 0:
      return uv(p.y,p.z);
    case 1:
      return uv(p.x,p.z);
    default: //case 2:
      return uv(p.x,p.y);
    }
  }

  static uv Flatten(Pointd const& p, uint ind)
  {
    return Flatten(Point(p),ind);
  }

  bool PolygonObject::IntersectByRay(Point const& R0,Vector const& Rd,Point& Ri) const
  {
    float nr = m_normal*Rd;
    if(vigo::abs(nr)<vigo::algebra3d::EPS) return false; // parallel

    if(nr>0) return false; // away

    float t = -(m_normal*R0+m_d)/nr;
    if(t<0) return false; // behind

    if(t>1) return false; // too far

    // polygon
    vigo::shape::Polygon *p = dynamic_cast<vigo::shape::Polygon*>(GetShape());
    if(!p) return false;

    // intersection point
    Ri = R0 + t*Rd;

    // inside of the polygon? Jordan Curve theorem
    uint  maxInd; // 0,1,2 .. x,y,z
    float maxCoord;
    if(vigo::abs(m_normal.x) > vigo::abs(m_normal.y))
    {
      maxCoord = m_normal.x;
      maxInd   = 0;
    }
    else
    {
      maxCoord = m_normal.y;
      maxInd   = 1;
    }

    if(vigo::abs(m_normal.z) > vigo::abs(maxCoord))
      maxInd   = 2;

    uv ri = Flatten(Ri,maxInd);

    uint numCrossings = 0;
    for(uint i=0; i+1<p->N(); i++)
    {
      uv a = Flatten(p->GetVertex(i),  maxInd) - ri; // translated
      uv b = Flatten(p->GetVertex(i+1),maxInd) - ri;
      // test edge intersection in 0..+u direction
      if((vigo::sgn(a.v) * vigo::sgn(b.v))>=0)
        continue; // does not cross

      double ratio = a.v/(a.v-b.v);
      if(a.u + ratio*(b.u-a.u) > 0)
        numCrossings++;
    }

    return vigo::odd(numCrossings);
  }


//** eof **********************************************************************
