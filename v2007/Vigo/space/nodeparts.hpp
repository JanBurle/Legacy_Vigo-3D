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
* $Id: nodeparts.hpp 226 2007-05-21 14:13:25Z ian_b $
\*****************************************************************************/

/** \file
 *  \brief Parts of nodes.
 *
 * User-specific node classes are to be constructed from "node parts".
 * That way their memory footprint is minimal -- oonlyy what is needed.
 * Other attributes have default (not stored) values and cannot be set.
 */

#ifndef VIGO_SPACE_NODEPARTS_HPP_INCLUDED
#define VIGO_SPACE_NODEPARTS_HPP_INCLUDED

  #include "../common/types.hpp"
  #include "./algebra.hpp"
  #include "./geom.hpp"
  #include "./subspace.hpp"
  #include "./shape.hpp"

/// Parts used to assemble a space node
NAMESPACE_VIGO(parts) //=======================================================
/** \defgroup grp_nodeparts_hpp Node parts
@{*/

  #define NOT_ALLOWED__(name)   THROW_EXC2(EXC_RUNTIME,#name " not allowed")
  #define SET___(name,type)     void name(type)   { NOT_ALLOWED__(name) }
  #define GET___(name,type,val) type name() const { NOT_ALLOWED__(name) return val; }

//-----------------------------------------------------------------------------
// position

  struct P_base
  {
  };

  /// Does not have a specific position; assumed to be at the origin, with
  /// zero velocity and acclereation.
  template<typename Coord>
  struct P0: P_base
  {
    P0()          {}
    P0(P0 const&) {}

    GETTER(Pos,   algebra3d::Point<Coord>, algebra3d::Point<Coord> ::_0)
    SET___(SetPos,algebra3d::Point<Coord>  const&)
    GETTER(Vel,   algebra3d::Vector<Coord>,algebra3d::Vector<Coord>::_0)
    SET___(SetVel,algebra3d::Vector<Coord> const&)
    GETTER(Acc,   algebra3d::Vector<Coord>,algebra3d::Vector<Coord>::_0)
    SET___(SetAcc,algebra3d::Vector<Coord> const&)

  protected:
    void AdjVel(double) {} ///< Adjust the velocity (no effect).

  public:
    bool HasPP() const { return false; }	///< Has a position?
    bool HasPV() const { return false; }	///< Has a velocity?
    bool HasPA() const { return false; }	///< Has an acceleration?
    bool HasPS() const { return false; }	///< Has a parametric position? 
  };

  /// Has a position.
  template<typename Coord>
  struct PP: P_base
  {
    PP()                            {}
    PP(PP const& p): m_pos(p.m_pos) {}

    GETTER(Pos,   algebra3d::Point<Coord>, m_pos)
    SETTER(SetPos,algebra3d::Point<Coord>  const&,m_pos)
    GETTER(Vel,   algebra3d::Vector<Coord>,algebra3d::Vector<Coord>::_0)
    SET___(SetVel,algebra3d::Vector<Coord> const&)
    GETTER(Acc,   algebra3d::Vector<Coord>,algebra3d::Vector<Coord>::_0)
    SET___(SetAcc,algebra3d::Vector<Coord> const&)

  protected:
    void AdjVel(double) {}

  public:
    bool HasPP() const { return true;  }
    bool HasPV() const { return false; }
    bool HasPA() const { return false; }
    bool HasPS() const { return false; }

  private:
    algebra3d::Point<Coord> m_pos;
  };

  /// Has a position and a velocity.
  template<typename Coord>
  struct PV: P_base
  {
    PV()                                            {}
    PV(PV const& p): m_pos(p.m_pos), m_vel(p.m_vel) {}

    GETTER(Pos,   algebra3d::Point<Coord>,        m_pos)
    SETTER(SetPos,algebra3d::Point<Coord> const&, m_pos)
    GETTER(Vel,   algebra3d::Vector<Coord>,       m_vel)
    SETTER(SetVel,algebra3d::Vector<Coord> const&,m_vel)
    GETTER(Acc,   algebra3d::Vector<Coord>,algebra3d::Vector<Coord>::_0)
    SET___(SetAcc,algebra3d::Vector<Coord> const&)

  protected:
    void AdjVel(double) {}

  public:
    bool HasPP() const { return true;  }
    bool HasPV() const { return true;  }
    bool HasPA() const { return false; }
    bool HasPS() const { return false; }

  private:
    algebra3d::Point<Coord>  m_pos;
    algebra3d::Vector<Coord> m_vel;
  };

  /// Has a position, velocity and acceleration.
  template<typename Coord>
  struct PA: P_base
  {
    PA()                                                            {}
    PA(PA const& p): m_pos(p.m_pos), m_vel(p.m_vel), m_acc(p.m_acc) {}

    GETTER(Pos,   algebra3d::Point<Coord>,        m_pos)
    SETTER(SetPos,algebra3d::Point<Coord> const&, m_pos)
    GETTER(Vel,   algebra3d::Vector<Coord>,       m_vel)
    SETTER(SetVel,algebra3d::Vector<Coord> const&,m_vel)
    GETTER(Acc,   algebra3d::Vector<Coord>,       m_acc)
    SETTER(SetAcc,algebra3d::Vector<Coord> const&,m_acc)

  protected:
    void AdjVel(double deltaT) { m_vel += m_acc*deltaT; }

  public:
    bool HasPP() const { return true;  }
    bool HasPV() const { return true;  }
    bool HasPA() const { return true;  }
    bool HasPS() const { return false; }

  private:
    algebra3d::Point<Coord>  m_pos;
    algebra3d::Vector<Coord> m_vel;
    algebra3d::Vector<Coord> m_acc;
  };

  /// Has a position on a parametric 3d curve, specified as (u).
  template<typename Coord>
  struct P2: P_base
  {
    P2(): m_u(0) {}
    P2(P2 const& p): m_u(p.m_u), m_curve(p.m_curve) {}

    GETSET(U, SetU, double, m_u)

    void SetCurve(geom::Curve3& curve) { m_curve = &curve;  }
    void RemoveCurve()                 { m_curve.release(); }

    geom::Curve3 *GetCurve() const
    {
      return m_curve.isNull() ? NULL : &(*m_curve);
    }

    algebra3d::Pointd Posd() const
    {
      if(!m_curve.isNull())
        return m_curve->CurvePoint01(m_u);
      else
        return algebra3d::Pointd::_0;
    }

    algebra3d::Point<Coord> Pos() const
    {
      return algebra3d::Point<Coord>(Posd());
    }

    SET___(SetPos,algebra3d::Point<Coord>  const&)
    GETTER(Vel,   algebra3d::Vector<Coord>,algebra3d::Vector<Coord>::_0)
    SET___(SetVel,algebra3d::Vector<Coord> const&)
    GETTER(Acc,   algebra3d::Vector<Coord>,algebra3d::Vector<Coord>::_0)
    SET___(SetAcc,algebra3d::Vector<Coord> const&)

  protected:
    void AdjVel(double) {}

  public:
    bool HasPP() const { return false; }
    bool HasPV() const { return false; }
    bool HasPA() const { return false; }
    bool HasPS() const { return true;  }

  private:
    double m_u; // same precision as in Curve3/Surface3
    util::refptr<geom::Curve3> m_curve;
  };

  /// Has a position on a parametric 3d surface, specified as a pair (u,v).
  template<typename Coord>
  struct P3: P_base
  {
    P3(): m_u(0), m_v(0) {}
    P3(P3 const& p): m_u(p.m_u), m_v(p.m_v), m_surface(p.m_surface) {}

    GETSET(U, SetU, double, m_u)
    GETSET(V, SetV, double, m_v)
    void Set(double u, double v) { m_u = u; m_v = v; }

    void SetSurface(geom::Surface3& surface) { m_surface = &surface; }
    void RemoveSurface()                     { m_surface.release();  }

    geom::Surface3 *GetSurface() const
    {
      return m_surface.isNull() ? NULL : &(*m_surface);
    }

    algebra3d::Point<Coord> Posd() const
    {
      if(!m_surface.isNull())
        return m_surface->SurfacePoint01(m_u,m_v);
      else
        return algebra3d::Pointd::_0;
    }

    algebra3d::Point<Coord> Pos() const
    {
      return algebra3d::Point<Coord>(Posd());
    }

    algebra3d::Rotation S3Rot() const
    {
      if(!m_surface.isNull())
        return algebra3d::Rotation(m_surface->Normal01(m_u,m_v));
      else
        return algebra3d::Rotation::_0;
    }

    SET___(SetPos,algebra3d::Point<Coord>  const&)
    GETTER(Vel,   algebra3d::Vector<Coord>,algebra3d::Vector<Coord>::_0)
    SET___(SetVel,algebra3d::Vector<Coord> const&)
    GETTER(Acc,   algebra3d::Vector<Coord>,algebra3d::Vector<Coord>::_0)
    SET___(SetAcc,algebra3d::Vector<Coord> const&)

  protected:
    void AdjVel(double) {}

  public:
    bool HasPP() const { return false; }
    bool HasPV() const { return false; }
    bool HasPA() const { return false; }
    bool HasPS() const { return true;  }

  private:
    double m_u, m_v;
    util::refptr<geom::Surface3> m_surface;
  };

//-----------------------------------------------------------------------------
// rotation

  struct R_base
  {
  };


  /// No rotation.
  struct R0: R_base
  {
    R0()          {}
    R0(R0 const&) {}

    GETTER(Rot,    algebra3d::Rotation,       algebra3d::Rotation::_0)
    SET___(SetRot, algebra3d::Rotation const&)
    GETTER(Spin,   algebra3d::Rotation,       algebra3d::Rotation::_0)
    SET___(SetSpin,algebra3d::Rotation const&)
    GETTER(Spna,   algebra3d::Rotation,       algebra3d::Rotation::_0)
    SET___(SetSpna,algebra3d::Rotation const&)

    void AdjSpin(double) {}
    
    bool HasRR() const { return false; }	///< Has a rotation.
    bool HasRS() const { return false; }	///< Has a spin.
    bool HasRA() const { return false; }	///< Has a spin acceleration.
  };

  /// Specified rotation.
  struct RR: R_base
  {
    RR()                            {}
    RR(RR const& r): m_rot(r.m_rot) {}

    GETTER(Rot,    algebra3d::Rotation,m_rot)
    SETTER(SetRot, algebra3d::Rotation const&,m_rot)
    GETTER(Spin,   algebra3d::Rotation,algebra3d::Rotation::_0)
    SET___(SetSpin,algebra3d::Rotation const&)
    GETTER(Spna,   algebra3d::Rotation,algebra3d::Rotation::_0)
    SET___(SetSpna,algebra3d::Rotation const&)

    void AdjSpin(double) {}
    
    bool HasRR() const { return true;  }
    bool HasRS() const { return false; }
    bool HasRA() const { return false; }

  private:
    algebra3d::Rotation m_rot;
  };

  /// A specified rotation and a spin.
  struct RS: R_base
  {
    RS()                                              {}
    RS(RS const& r): m_rot(r.m_rot), m_spin(r.m_spin) {}

  public:
    GETTER(Rot,    algebra3d::Rotation,       m_rot)
    SETTER(SetRot, algebra3d::Rotation const&,m_rot)
    GETTER(Spin,   algebra3d::Rotation,       m_spin)
    SETTER(SetSpin,algebra3d::Rotation const&,m_spin)
    GETTER(Spna,   algebra3d::Rotation,       algebra3d::Rotation::_0)
    SET___(SetSpna,algebra3d::Rotation const&)

    void AdjSpin(double) {}
    
    bool HasRR() const { return true;  }
    bool HasRS() const { return true;  }
    bool HasRA() const { return false; }

  private:
    algebra3d::Rotation m_rot;
    algebra3d::Rotation m_spin;
  };

  /// A specified rotation, spin and spin acceleration.
  struct RA: R_base
  {
    RA()                                                                {}
    RA(RA const& r): m_rot(r.m_rot), m_spin(r.m_spin), m_spna(r.m_spna) {}

    GETTER(Rot,    algebra3d::Rotation,       m_rot)
    SETTER(SetRot, algebra3d::Rotation const&,m_rot)
    GETTER(Spin,   algebra3d::Rotation,       m_spin)
    SETTER(SetSpin,algebra3d::Rotation const&,m_spin)
    GETTER(Spna,   algebra3d::Rotation,       m_spna)
    SETTER(SetSpna,algebra3d::Rotation const&,m_spna)

    void AdjSpin(double deltaT) { m_spin += m_spna*deltaT; }
    
    bool HasRR() const { return true; }
    bool HasRS() const { return true; }
    bool HasRA() const { return true; }

  private:
    algebra3d::Rotation m_rot;
    algebra3d::Rotation m_spin;
    algebra3d::Rotation m_spna;
  };

//-----------------------------------------------------------------------------
// tracking

  //>>> not a very nice hack
  #define GrpIdByClass(Class) reinterpret_cast<space::tracked_grpid>(&typeid(Class))
  /// returns a unique, new group id
  space::tracked_grpid GetNewGrpId();

  /// Not tracked.
  template<typename Coord>
  struct T0
  {
    T0() {}
   ~T0() {}

    void Nbr(space::AbstractNode<Coord>&,space::tracked_grpid const*,double,
             space::tracked_collection<Coord>&,uint=0,bool=false)
    {
      NOT_ALLOWED__(Nbr)
    }

    void EnterSubspace(subspace::TopSpace<Coord>&,
                       space::AbstractNode<Coord>&)	{}
    void LeaveSubspace(space::AbstractNode<Coord>&)	{}
    void UpdateSubspace(space::AbstractNode<Coord>&)	{}
    bool IsInSubspace() const				{ return false; }

    space::tracked_grpid GrpId(space::tracked_grpid) const
    {
      return 0;	
    }
    SET___(SetGrpId, space::tracked_grpid)

  protected:
    // subspace support
    GET___(GetNext,    space::AbstractNode<Coord>*, NULL)
    SET___(SetNext,    space::AbstractNode<Coord>*)
    GET___(GetPrev,    space::AbstractNode<Coord>*, NULL)
    SET___(SetPrev,    space::AbstractNode<Coord>*)
    GET___(GetSubspace,subspace::SubspaceLeaf<Coord>*,  NULL)
    SET___(SetSubspace,subspace::SubspaceLeaf<Coord>*)
  };

//-----------------------------------------------------------------------------

  // Tracked (abstract - not to be used directly).
  template<typename Coord>
  struct T1__
  {
    T1__(): m_prev(NULL), m_next(NULL), m_subspace(NULL)  {}
   ~T1__()                                                {}

    T1__(T1__ const&): m_prev(NULL), m_next(NULL), m_subspace(NULL)  {}

    static algebra3d::Point<Coord> compare_nodePos;
    static int CompareNodeDistances(pcvoid first, pcvoid second)
    {
      Coord fm = ((*((space::AbstractNode<Coord>**) first))->Pos()-compare_nodePos).Magnitude();
      Coord sm = ((*((space::AbstractNode<Coord>**)second))->Pos()-compare_nodePos).Magnitude();
      if(fm<sm) return -1;
      if(fm>sm) return +1;
      return 0;
    }

    void Nbr(space::AbstractNode<Coord>& node, space::tracked_grpid const* grpid,
       double radius, space::tracked_collection<Coord>& result,
       uint maxNum=0/*all*/, bool sort=false)
    {
      if(!IsInSubspace() || (0>=radius))
      {
        result.clear(); return;
      }

      m_subspace->Nbr(node,grpid,radius,result);

      size_t resultSize = result.size(); 
      if(sort && 0<resultSize)
      {
        // std::sort is less efficient then qsort; std::sort SIGSEGVd on i686-redhat 
        // std::sort(result.begin(),result.end(),CompareDistances(node.Pos()));

        compare_nodePos = node.Pos();
        space::AbstractNode<Coord> **p = &(*result.begin());
        qsort(p, resultSize, sizeof(*p), CompareNodeDistances);
      }
      
      if(maxNum>0/*not all*/ && maxNum<result.size())
      {
        result.resize(maxNum); //>>> not optimal
      }
    }

    void EnterSubspace(subspace::TopSpace<Coord>& ts,space::AbstractNode<Coord>& node)
    {
      RUNTIME_CHECK_EXC2(!IsInSubspace(),"Node already in subspace")
      ts.Add(node);
    }
    
    void LeaveSubspace(space::AbstractNode<Coord>& node)
    {
      RUNTIME_CHECK_EXC2(IsInSubspace(),"Node is not in subspace")
      m_subspace->Remove(node);
    }
    
    void UpdateSubspace(space::AbstractNode<Coord>& node)
    {
      m_subspace->Move(node);
    }
    
    bool IsInSubspace() const { return NULL!=m_subspace; }

  protected:
    GETSET(GetNext,    SetNext,    space::AbstractNode<Coord>*, m_next)
    GETSET(GetPrev,    SetPrev,    space::AbstractNode<Coord>*, m_prev)
    GETSET(GetSubspace,SetSubspace,subspace::SubspaceLeaf<Coord>*,  m_subspace)

  private:
    T1__<Coord> operator=(T1__<Coord> const&); // NO_ASSIGN
    
    space::AbstractNode<Coord>     *m_prev, *m_next;
    subspace::SubspaceLeaf<Coord>  *m_subspace;

    struct CompareDistances
    {
      CompareDistances(algebra3d::Point<Coord> const& pos)
        : m_pos(pos) {}
  	          
      bool operator()(space::AbstractNode<Coord>* const& first,
                      space::AbstractNode<Coord>* const& second)
      {
        return (first->Pos()-m_pos).Magnitude() < (second->Pos()-m_pos).Magnitude();
      }
    private:
      algebra3d::Point<Coord> m_pos;
    };
  };

  template<typename Coord>
    algebra3d::Point<Coord> T1__<Coord>::compare_nodePos;

//-----------------------------------------------------------------------------

  // Tracked by class types.
  template<typename Coord>
  struct TC: T1__<Coord>
  {
  private:
    SUPER(T1__<Coord>)

  public:
    TC(): super() {}
   ~TC()          {}

    TC(TC const& t): super(t) {}

    space::tracked_grpid GrpId(space::tracked_grpid classGrpId) const
    {
      return classGrpId;	
    }
    SET___(SetGrpId, space::tracked_grpid)

  private:
    TC<Coord> operator=(TC<Coord> const&); // NO_ASSIGN
  };

//-----------------------------------------------------------------------------

  // Tracked individually.
  template<typename Coord>
  struct TI: T1__<Coord>
  {
  private:
    SUPER(T1__<Coord>)
    space::tracked_grpid m_grpid;

  public:
    TI(): super(), m_grpid(0) {}
   ~TI()                      {}

    TI(TI const& t): super(t) {}

    space::tracked_grpid GrpId(space::tracked_grpid) const
    {
      return m_grpid;	
    }
    SETTER(SetGrpId, space::tracked_grpid, m_grpid)

  private:
    TI<Coord> operator=(TI<Coord> const&); // NO_ASSIGN
  };

//----------------------------------------------------------------------------
// Shapes and labels.

  /// No shape or label.
  struct S0
  {
    S0()            {}
    S0(S0 const&)   {}

    void SetShape(shape::Shape&)    { NOT_ALLOWED__(SetShape)    }
    void RemoveShape()              { NOT_ALLOWED__(RemoveShape) }
    shape::Shape* ShapePart() const { return NULL;               }
    pcstr         LabelPart() const { return NULL;               }
  };

  /// May have a shape, no label.
  struct SS
  {
    SS(): m_shape()                     {}
    SS(SS const& s): m_shape(s.m_shape) {}

    void SetShape(shape::Shape& s)  	{ m_shape = &s;           }
    void RemoveShape()              	{ m_shape.release();      }
    void SetLabel(pcstr)            	{ NOT_ALLOWED__(SetLabel) }
    void SetLabel(std::string const&)	{ NOT_ALLOWED__(SetLabel) }

    shape::Shape* ShapePart() const 	{ return m_shape.ptr();   }
    pcstr         LabelPart() const 	{ return NULL;            }
    
  private:
    util::refptr<shape::Shape> m_shape;
  };

  /// May have a shape; has a label
  struct SL
  {
    SL(): m_shape()                     {}
    SL(SL const& s): m_shape(s.m_shape) {}

    void SetShape(shape::Shape& s)  	{ m_shape = &s;            }
    void RemoveShape()              	{ m_shape.release();       }
    void SetLabel(pcstr label)      	{ m_label = label?label:"";}
    void SetLabel(std::string const& l)	{ m_label = l;		   }

    shape::Shape* ShapePart() const { return m_shape.ptr();    }
    pcstr         LabelPart() const { return m_label.empty() ? NULL : m_label.c_str(); }
    
  private:
    util::refptr<shape::Shape> m_shape;
    std::string                m_label;
  };


//----------------------------------------------------------------------------
// Appearances.

  /// No appearance.
  struct A0
  {
    A0()          {}
    A0(A0 const&) {}

    void SetAppearance(shape::Appearance&)    { NOT_ALLOWED__(SetAppearance)    }
    void RemoveAppearance()                   { NOT_ALLOWED__(RemoveAppearance) }
    shape::Appearance* AppearancePart() const { return NULL; }
  };

  /// May have an appearance
  struct A1
  {
    A1(): m_appearance()                          {}
    A1(A1 const& a): m_appearance(a.m_appearance) {}

    void SetAppearance(shape::Appearance& a)  { m_appearance = &a;          }
    void RemoveAppearance()                   { m_appearance.release();     }
    shape::Appearance* AppearancePart() const { return m_appearance.ptr();  }

  private:
    util::refptr<shape::Appearance> m_appearance;
  };


//-----------------------------------------------------------------------------
  /// Putting parts together.
  template<class P, class R, class T, class S, class A>
  struct Parts: P, R, T, S, A
  {
    Parts(): P(), R(), T(), S(), A() {}
    Parts(Parts const& p): P(p), R(p), T(p), S(p), A(p) {}
  };

//-----------------------------------------------------------------------------

#undef SET___
#undef GET___
#undef NOT_ALLOWED__

/**@} (defgroup)*/
NAMESPACE_END() //=============================================================

#endif

//** eof **********************************************************************
