// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief Geometry
 *
 *  Geometry
 */

#ifndef VIGO_SPACE_GEOM_HPP_INCLUDED
#define VIGO_SPACE_GEOM_HPP_INCLUDED

  #include "../common/refcnt.hpp"

  #include "./algebra.hpp"

NAMESPACE_VIGO(geom) //========================================================
/** \defgroup grp_geom_hpp Geometry
@{*/

  /// A 3D parametric curve.
  /// The user must either overload \c ParFun, or supply \c par_fun.
  class Curve3: public util::refcnt_auto
  {
    SUPER(util::refcnt_auto)

  private:
    double m_minPar, m_maxPar;  /// min <= u < max
    double m_scale;

  public:
    typedef algebra3d::Pointd (*par_fun)(double par);
  private:
    par_fun m_f;

  public:
    /** Constructs a 3D parametric curve.
     * @param minPar  The minimum value of the parameter.
     * @param maxPar  The maximum value of the parameter.
     * @param scale   A scaling factor.
     * @param f       A parametric function.
     */
    Curve3(double minPar,double maxPar,double scale=1.0,par_fun f=NULL)
      : super(),
        m_minPar(minPar), m_maxPar(maxPar), m_scale(scale), m_f(f)
    {
    }

    GETSET(MinPar,SetMinPar,double,m_minPar)
    GETSET(MaxPar,SetMaxPar,double,m_maxPar)
    void SetPar(double minPar,double maxPar) { m_minPar=minPar; m_maxPar=maxPar; }
    GETSET(Scale, SetScale, double,m_scale)

    SETTER(SetParFun,par_fun,m_f)

  public:
    /// Map (0,1) to (minpar,maxpar).
    double u01(double u) const;
  private:
    /// Map (minpar,maxpar) to a curve point.
    algebra3d::Pointd CurvePoint_(double u) const;

  protected:
    /// Either overload \c ParFun, or supply \c par_fun.
    virtual algebra3d::Pointd ParFun(double /*u*/) const
    {
      return algebra3d::Pointd::_0;
    }

  public:
    /// u=(minpar..maxpar)
    algebra3d::Pointd  CurvePoint  (double u) const;
    /// u=(0..1)
    algebra3d::Pointd  CurvePoint01(double u) const
      { return CurvePoint(u01(u));   }

    /// u=(minpar..maxpar)
    algebra3d::Vectord Tangent  (double u) const;
    /// u=(0..1)
    algebra3d::Vectord Tangent01(double u) const
      { return Tangent(u01(u)); }

    /// u=(minpar..maxpar)
    algebra3d::Vectord Across   (double u) const;
    /// u=(0..1)
    algebra3d::Vectord Across01 (double u) const
      { return Across(u01(u)); }
  };

//----------------------------------------------------------------------------

  /// A parametric circle.
  class Circle: public Curve3
  {
    SUPER(Curve3)

  private:
    /// radius
    double m_r;

  protected:
    virtual algebra3d::Pointd ParFun(double theta) const;

  public:
    Circle(double radius);
    void SetRadius(double radius);
    GETTER(Radius,double,m_r)
  };

  /// Ellipse
  class Ellipse: public Curve3
  {
    SUPER(Curve3)

  private:
    /// semiaxes
    double m_a, m_b;
    bool   m_swapped;

    /// eccentricity etc
    double m_e, m_e2, m_1_e2;

  protected:
    virtual algebra3d::Pointd ParFun(double theta) const;

  public:
    Ellipse(double a,double b);
    void Set(double a, double b);

    GETTER(A,double,m_swapped ? m_b : m_a)
    GETTER(B,double,m_swapped ? m_a : m_b)
    GETTER(E,double,m_e)

    double Radius(algebra3d::rad theta) const;
  };

  /// Orbital ellipse
  class OrbitalEllipse: public Curve3
  {
    SUPER(Curve3)

  private:
    /// semiaxes
    double m_a, m_b;
    bool   m_swapped;

    /// eccentricity etc
    double m_e, m_1_e2, m_c;

  protected:
    virtual algebra3d::Pointd ParFun(double theta) const;

  public:
    OrbitalEllipse(double a,double b);
    void Set(double a, double b);

    GETTER(A,double,m_swapped ? m_b : m_a)
    GETTER(B,double,m_swapped ? m_a : m_b)
    GETTER(E,double,m_e)

    double Radius(algebra3d::rad theta) const;
  };

//----------------------------------------------------------------------------

  /// Moebius.
  class MoebiusBoundary: public Curve3
  {
    SUPER(Curve3)

  private:
    /// radius
    double m_r;
    /// height
    double m_h;
    /// Number of rounds (2,3,4..)
    uint   m_n;

  protected:
    virtual algebra3d::Pointd ParFun(double theta) const;

  public:
    MoebiusBoundary(double radius,double height,uint n=2);
    void Set(double radius,double height) { m_r=radius; m_h=height; }
    GETTER(Radius,double,m_r)
    GETTER(Height,double,m_h)
    GETTER(N,     uint,  m_n)
    void SetN(uint);
  };

//----------------------------------------------------------------------------

  /// A parametric 3D surface.
  class Surface3: public util::refcnt_auto
  {
    SUPER(util::refcnt_auto)

  public:
    Surface3()
    {
    }

    virtual double u01(double u) const = 0;
    virtual double v01(double v) const = 0;

    virtual algebra3d::Pointd SurfacePoint(double u, double v) const = 0;

    algebra3d::Pointd SurfacePoint01(double u, double v) const
    {
      return SurfacePoint(u01(u),v01(v));
    }

    virtual algebra3d::Vectord Normal(double u, double v) const = 0;

    algebra3d::Vectord Normal01(double u, double v) const
    {
      return Normal(u01(u),v01(v));
    }
  };

//----------------------------------------------------------------------------

  class Tube: public Surface3
  {
    SUPER(Surface3)

  protected:
    util::refptr<Curve3> m_majorCurve,
                         m_minorCurve;

  public:
    Tube(Curve3* major, Curve3* minor);

    GETTER(MajorCurve, Curve3 const&, *m_majorCurve)
    GETTER(MinorCurve, Curve3 const&, *m_minorCurve)

    virtual double u01(double u) const
    {
      return m_majorCurve->u01(u);
    }

    virtual double v01(double v) const
    {
      return m_minorCurve->u01(v);
    }

    virtual algebra3d::Pointd SurfacePoint(double u, double v) const;

    algebra3d::Vectord Normal(double u, double v) const //>>> to cpp
    {
      return m_majorCurve->Tangent(u) ^ m_minorCurve->Tangent(v);
    }
  };

//----------------------------------------------------------------------------

  class Torus: public Tube
  {
    SUPER(Tube)

  public:
    Torus(double r1, double r2);
  };

/**@} (defgroup)*/
NAMESPACE_END() //=============================================================

#endif

// eof
