// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief Algebra of a 3D space with Cartesian coordinates [x,y,z]
 *
 *  3d point, vector, rotation
 */

#ifndef VIGO_SPACE_ALGEBRA_HPP_INCLUDED
#define VIGO_SPACE_ALGEBRA_HPP_INCLUDED

  #include "../common/types.hpp"
  #include "../common/math.hpp"
  #include <math.h>
  #include "../common/tuple.hpp"

NAMESPACE_VIGO(algebra3d) //===================================================
/** \defgroup grp_algebra_hpp 3D algebra
@{*/

  /// angle units: degrees, radians
  typedef double deg, rad;

  /// Radians to degrees.
  double const RAD2DEG = 180.0/MATH_PI;
  inline deg rad2deg(rad angle)  { return angle * RAD2DEG; }

  /// Degrees to radians.
  double const DEG2RAD = MATH_PI/180.0;
  inline rad deg2rad(deg angle)  { return angle * DEG2RAD; }

  /// almost zero
  double const EPS = 0.000001; //>>> review

//----------------------------------------------------------------------------

  template<typename Coord> struct Vector;
  struct Rotation;

  /// 3D point (x, y, z).
  /** A templated 3-D Point class. \c Coord can be a floating point type
   *  (\c float or \c double) or an integral type (\c int, \c long) in the
   *  case of a discrete 3D space. However, not all algebra will work in
   *  discrete Coordinate system.
   */
  template<typename Coord>
  struct Point: public util::tuple3<Coord>
  {
    SUPER(util::tuple3<Coord>)

  public:
    /// A point at the origin.
    Point()
      : super(0, 0, 0)
    {
    }

    /// A point at a specific location.
    Point(Coord ax,  Coord ay, Coord az)
      : super(ax,ay,az)
    {
    }

    explicit Point<Coord>(super const& s)
      : super(s)
    {
    }

    explicit Point<Coord>(pcstr s)
      : super(s)
    {
    }

    explicit Point<Coord>(std::string const& s)
      : super(s)
    {
    }

    /// Conversion from another precision
    template<typename Coord2>
    explicit Point<Coord>(Point<Coord2> const& p)
      : super(p.x,p.y,p.z)
    {
    }

    /// A function that can be applied on \c Coord.
    typedef Coord (*fcc)(Coord);

    /// A point specified by an application of the \c fcc function.
    Point Apply(fcc f) const
    {
      return Point(f(this->x),f(this->y),f(this->z));
    }

    /// A point translated by a vector.
    Point operator + (Vector<Coord> const& r) const
    {
      return Point(*this) += r;
    }

    /// A point translated by a vector.
    Point& operator +=(Vector<Coord> const& r)
    {
      this->x += r.x;  this->y += r.y;  this->z += r.z;
      return *this;
    }

    /// A point translated by a vector in the opposite direction.
    Point operator - (Vector<Coord> const& r) const
    {
      return Point(*this) -= r;
    }

    /// A point translated by a vector in the opposite direction.
    Point& operator -=(Vector<Coord> const& r)
    {
      this->x -= r.x;  this->y -= r.y;  this->z -= r.z;
      return *this;
    }

    /// A vector defined by two points.
    Vector<Coord> operator -(Point const& r) const
    {
      return Vector<Coord>(this->x-r.x, this->y-r.y, this->z-r.z);
    }

    bool operator ==(Point<Coord> const& r) const
    {
      return (this->x==r.x) && (this->y==r.y) && (this->z==r.z);
    }

    bool operator !=(Point<Coord> const& r) const
    {
      return !(*this==r);
    }

  public:
    /// A point at a random location, each Coordinate (x,y,z) between min and max
    static Point Random(Coord min, Coord max,
                        math::Rand* rand = NULL)
    {
      if(!rand) rand = &math::rand;
      return Point(rand->Double(min,max),
                   rand->Double(min,max),
                   rand->Double(min,max));
    }

    /// A point within a spherical space (centre,radius).
    static Point Random(Point const& centre, double radius,
                        math::Rand* rand = NULL)
    {
      return Random(centre,0,radius,rand);
    }

    template<typename Float> // Float may be float or double
    static Float templated_sin(Float f)
    {
      return (Float)sin((double)f);
    }

    /// A point within a spherical layer (centre, min:max radius).
    static Point Random(Point const& centre,
                        double minRadius, double maxRadius,
                        math::Rand* rand = NULL)
    {  // A random vector constrained within a spherical layer
      if(minRadius < 0.0)        minRadius = 0.0;
      if(maxRadius <= minRadius) return centre;

      if(!rand) rand = &math::rand;
      Point         r = Random(0,math::_2pi,rand);
      Vector<Coord> p = Vector<Coord>(r.Apply(templated_sin)).Unit();
      return centre + p * rand->Double(minRadius,maxRadius);
    }

    /// A random unit vector
    static Point Random(math::Rand* rand = NULL)
    {
      return Vector<Coord>(Random(Point::_0,1.0,rand)).Unit();
    }

  public:
    /// A point within a box <min,max)
    bool IsBetween(Point const& min, Point const& max) const
    {
      return this->x>=min.x && this->y>=min.y && this->z>=min.z
         &&  this->x< max.x && this->y< max.y && this->z< max.z;
    }

  public:
    /// Origin.
    static Point const _0; // additive identity
  };

  /// insert into a stream
  template<typename Coord>
  std::ostream& operator<<(std::ostream& os, Point<Coord> const& p)
	{
    return os << "(" << p.x << ',' << p.y << ',' << p.z << ')';
  }


  template<typename Coord>
  Point<Coord> const Point<Coord>::_0(0,0,0);

  typedef Point<double> Pointd;
  typedef Point<float>  Pointf;
  typedef Point<int>    Pointi;

//----------------------------------------------------------------------------

  /// 3D vector (origin) -> (x,y,z).
  /** A templated 3-D vector class. \c Coord can be a floating point type
   *  (\c float or \c double) or an integral type (\c int, \c long) in the
   *  case of a discrete 3D space. However, not all algebra will work in
   *  discrete Coordinate system.
   */
  template<typename Coord>
  struct Vector: Point<Coord>
  {
    SUPER(Point<Coord>)

  public:
    /// A zero vector.
    Vector(): super()
    {
    }

    /// A specific vector.
    Vector(Coord x, Coord y, Coord z): super(x,y,z)
    {
    }

    /// Conversion.
    /*>>>ADD: explicit*/ Vector(Point<Coord> const& p)
      : super(p)
    {
    }

    explicit Vector(pcstr s)
      : super(s)
    {
    }

    explicit Vector(std::string const& s)
      : super(s)
    {
    }

    /// Conversion from another precision
    template<typename Coord2>
    explicit Vector(Vector<Coord2> const& v)
      : super(v.x,v.y,v.z)
    {
    }

    /// The magnitude (x^2+y^2+z^2) of the vector.
    Coord Magnitude() const
    {
      return this->x*this->x + this->y*this->y + this->z*this->z;
    }

    /// The length of the vector.
    double Length() const
    {
      return sqrt(Magnitude());
    }

    /// Manhattan length of the vector.
    double Manhattan() const
    {
      return this->x + this->y + this->z;
    }

    /// The vector scaled to a unit vector
    /** If the vector is too short, \c Unit() returns _0.
      */
    Vector Unit() const
    {
      return (*this)/Length();
    }

    /// A function that can be applied on \c Coord.
    typedef Coord (*fcc)(Coord);

    /// A vector specified by an application of the \c fcc function.
    Vector Apply(fcc f) const
    {
      return super::Apply(f);
    }

    /// Add vectors.
    Vector operator + (Vector const& r) const
    {
      return Vector(*this) += r;
    }

    /// Add vectors.
    Vector& operator +=(Vector const& r)
    {
      super::operator +=(r);
      return *this;
    }

    /// The opposite vector.
    Vector  operator - () const
    {
      return Vector(-this->x, -this->y, -this->z);
    }

    /// Subtract vectors.
    Vector  operator - (Vector const& r) const
    {
      return Vector(*this) -= r;
    }

    /// Subtract vectors.
    Vector& operator -=(Vector const& r)
    {
      super::operator -=(r);
      return *this;
    }

    /// Scale the vector.
    Vector  operator * (Coord d) const
    {
      return Vector(*this) *= d;
    }

    /// Scale the vector.
    Vector& operator *=(Coord d)
    {
      this->x *= d;  this->y *= d;  this->z *= d;
      return *this;
    }

    /// Scale the vector.
    Vector  operator / (double d) const
    {
      return Vector(*this) /= d;
    }

    /// Scale the vector.
    Vector& operator /=(double d)
    {
      if(abs(d)<EPS) *this = Z;
      else
      {
        this->x /= d;  this->y /= d;  this->z /= d;
      }

      return *this;
    }

    /// Dot product.
    Coord operator * (Vector const& r) const
    {
      return this->x*r.x + this->y*r.y + this->z*r.z;
    }

    /// The angle between two vectors.
    rad Angle(Vector const& r) const
    {
      double l2 = Length()*r.Length();
      if(l2<EPS) return 0;

      double d = ((*this)*r)/l2;

      // acount for small overruns for parallel vectors
      if(d > 1.0)  d =  1.0;
      else
      if(d < -1.0) d = -1.0;

      return acos(d);
    }

    /// Project the vector on another.
    Coord Project(Vector const& r) const
    {
      return (*this)*r.Unit();
    }

    /// Cross product.
    Vector  operator ^ (Vector const& r) const
    {
      return Vector(this->y*r.z - this->z*r.y,
                    this->z*r.x - this->x*r.z,
                    this->x*r.y - this->y*r.x);
    }

    /// Cross product.
    Vector& operator ^=(Vector const& r)
    {
      return *this = (*this)^r;
    }

    /// The normal of two vectors.
    Vector  Normal(Vector const& r) const
    {
      return (*this ^ r).Unit();
    }

    /// A rotated vector
    Vector Rotate(Rotation const&) const;

    bool operator ==(Vector<Coord> const& r) const
    {
      return super::operator==(r);
    }

    bool operator !=(Vector<Coord> const& r) const
    {
      return super::operator!=(r);
    }

  public:
    /// Unit vectors.
    static Vector const X, Y, Z;
    /// Origin.
    static Vector const _0; // additive identity
  };

  template<typename Coord>
  inline Vector<Coord> operator *(Coord d, Vector<Coord> const& v)
  {
    return v * d;
  }

  /// insert into a stream
  template<typename Coord>
  std::ostream& operator<<(std::ostream& os, Vector<Coord> const& p)
	{
    return os << "(" << p.x << ',' << p.y << ',' << p.z << ')';
  }

  template<typename Coord> Vector<Coord> const Vector<Coord>::X(1,0,0);
  template<typename Coord> Vector<Coord> const Vector<Coord>::Y(0,1,0);
  template<typename Coord> Vector<Coord> const Vector<Coord>::Z(0,0,1);

  template<typename Coord> Vector<Coord> const Vector<Coord>::_0(0,0,0);

  typedef Vector<double> Vectord;
  typedef Vector<float>  Vectorf;
  typedef Vector<int>    Vectori;

//----------------------------------------------------------------------------

  /// 3D line, specified by two points
  template<typename Coord>
  struct Line
  {
    Point<Coord> a, b;

  public:
    /// A null line
    Line()
      : a(), b()
    {
    }

    /// A specific line
    Line(Point<Coord> const& aa, Point<Coord> const& bb)
      : a(aa), b(bb)
    {
    }

    /// A line translated by a vector.
    Line operator + (Vector<Coord> const& r) const
    {
      return Line(*this) += r;
    }

    /// A line translated by a vector.
    Line& operator +=(Vector<Coord> const& r)
    {
      this->a += r; this->b += r;
      return *this;
    }

    /// A line translated by a vector in the opposite direction.
    Line operator - (Vector<Coord> const& r) const
    {
      return Line(*this) -= r;
    }

    /// A line translated by a vector in the opposite direction.
    Line& operator -=(Vector<Coord> const& r)
    {
      this->a -= r; this->b -= r;
      return *this;
    }

  public:
    /// The squared minimum distance from a point to this line.
    /// Coord should be a floating point type
    Coord Magnitude(Point<Coord> const& p) const
    {
      Vector<Coord> v = b-a;
      return (v^(a-p)).Magnitude() / v.Magnitude();
    }

  public:
    /// Origin.
    static Line const X, Y, Z; // along the axes
  };

  template<typename Coord>
  Line<Coord> const Line<Coord>::X(Point<Coord>::_0, Vector<Coord>::X);
  template<typename Coord>
  Line<Coord> const Line<Coord>::Y(Point<Coord>::_0, Vector<Coord>::Y);
  template<typename Coord>
  Line<Coord> const Line<Coord>::Z(Point<Coord>::_0, Vector<Coord>::Z);

  typedef Line<double> Lined;
  typedef Line<float>  Linef;
  typedef Line<int>    Linei;

//----------------------------------------------------------------------------
// Useful formulas



//----------------------------------------------------------------------------

  /// Quaternion
  struct Quaternion: public util::tuple4<double>
  {
    SUPER(util::tuple4<double>)

    /// Default constructor
    Quaternion();
    /// Copy constructor.
    Quaternion(Quaternion const& r);
    ///
    Quaternion(double w, double x, double y, double z);
    ///
    Quaternion(Rotation const&);
    ///
    Quaternion(rad pitchX, rad yawY, rad rollZ);

    /// Assignment.
    Quaternion& operator=(Quaternion const&);
    /// Assignment.
    Quaternion& operator=(Rotation const&);

    /// Conjugate (polarity)
    Quaternion  operator - () const;

    /// Interpolation
    Quaternion  operator * (double) const;
    Quaternion& operator *=(double);

    /// Non-commutative product.
    Quaternion  operator * (Quaternion const&) const;
    Quaternion& operator *=(Quaternion const&);

    /// Unit vectors.
    static Quaternion const _1; // multiplicative identity quaternion
  };

//----------------------------------------------------------------------------

  /// Rotation by an angle \c theta around a vector (angle-axis)
  struct Rotation
  {
    rad           th; // theta
    Vectord v;

    /// Zero angle, \c Z vector.
    Rotation();
    /// Copy constructor.
    Rotation(Rotation const& r);
    /// Rotation to a specific orientation.
    Rotation(Vectord const&);
    ///
    Rotation(rad,Vectord const&);
    ///
    Rotation(Quaternion const&);
    ///
    Rotation(rad pitchX, rad yawY, rad rollZ);

    /// Assignment.
    Rotation& operator=(Rotation const& r);
    /// Rotation to a specific orientation.
    Rotation& operator=(Vectord const&);
    ///
    Rotation& operator=(Quaternion const&);

    /// Add rotations.
    Rotation  operator + (Rotation const& r) const;
    /// Add rotations.
    Rotation& operator +=(Rotation const&);

    /// The opposite rotation.
    Rotation  operator - () const;

    /// Subtract rotations.
    Rotation  operator - (Rotation const&) const;
    /// Subtract rotations.
    Rotation& operator -=(Rotation const&);

    Rotation  operator * (double) const;
    Rotation& operator *=(double);

    Rotation  operator / (double) const;
    Rotation& operator /=(double);

    ///
    static Rotation const _0; // additive identity
  };

  /// insert into a stream
  std::ostream& operator<<(std::ostream& os, Rotation const& r);

//----------------------------------------------------------------------------

  template<typename Coord>
  Vector<Coord> Vector<Coord>::Rotate(Rotation const& r) const
  {
    Vector<Coord> vc(r.v.x,r.v.y,r.v.z);
    double dot = (*this)*vc;
    double mag = r.v.Magnitude(), lgt = sqrt(mag);
    double ct  = cos(r.th), st = sin(r.th);

    double u = r.v.x, v = r.v.y, w = r.v.z,
           ux = u*this->x, vy = v*this->y, wz = w*this->z,
           uy = u*this->y, vz = v*this->z, wx = w*this->x,
           vx = v*this->x, wy = w*this->y, uz = u*this->z;

    return Vector(
      u*dot + (this->x*(v*v + w*w) - u*(vy+wz))*ct + lgt*(-wy+vz)*st,
      v*dot + (this->y*(u*u + w*w) - v*(ux+wz))*ct + lgt*(+wx-uz)*st,
      w*dot + (this->z*(u*u + v*v) - w*(ux+vy))*ct + lgt*(-vx+uy)*st
    ) / mag;
  }

/**@} (defgroup)*/
NAMESPACE_END() //=============================================================

#endif

// eof
