// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief Math constants and functions
 *
 */

#ifndef VIGO_COMMON_MATH_HPP_INCLUDED
#define VIGO_COMMON_MATH_HPP_INCLUDED

#include "./types.hpp"

NAMESPACE_VIGO(math) //========================================================
/** \defgroup grp_math_hpp Math constants and functions
@{*/

  #define MATH_PI  (3.14159265358979323846)   ///< Pi
  #define MATH_C   (2.99792458e8)             ///< speed of light


  /// A floating point constant. Provides conversion.
  class Const
  {
  public:
    Const(double value)
      : m_doubleValue(value), m_floatValue((float)value)
    {
    }

    /// Conversion operators
    operator float()  const  { return m_floatValue;  }
    operator double() const  { return m_doubleValue; }

  private:
    double m_doubleValue;
    float  m_floatValue;
  };

  /// Built-in constants.
  extern Const pi2,  ///< Pi/2
               pi,   ///< Pi
              _2pi,  ///< Pi*2
              _4pi;  ///< Pi*4

  extern Const c;    ///< the speed of light

//----------------------------------------------------------------------------

  /// Fixed point base^exp.
  template <typename uord/*unsigned ordinal*/>
  uord powu(uord base, uord exp)
  {
    if(0==exp) return 1;
    uord res = base;
    while(--exp>0)
      res *= base;
    return res;
  }

//----------------------------------------------------------------------------

  /// Random number generator (Mersenne Twister).
  class Rand
  {
    NO_GEN(Rand)

  private:
    enum { N = 624,  // the size of the state vector
           M = 397};
    uint32 m_mt[N];
    uint   m_mti;

  public:
    /// Construct a generator, with a random 32bit seed.
    Rand();
    /// Construct a generator, with a specified 32bit seed.
    Rand(uint32 seed);
    /// Construct a generator, specified >32bit seed.
    Rand(uint32 initKey[], uint keyLength);
   ~Rand();

    /// Seed the generator
    void Seed(uint32 seed);
    void Seed(uint32 initKey[], uint keyLength);

    /// Generate random integers between <0,2^32).
    uint32 UInt32();
    /// Generate random integers between <min,max).
    uint32 UInt32(uint32 min, uint32 max);

    /// Generate a random double between <0.0,1.0).
    double Double();
    /// Generate a random double between <min,max);
    double Double(double min,double max);

    /// Generate a random double between <0.0,1.0) using a gaussian distribution
    double DoubleGauss();

    /// Generate a random boolean value.
    bool   Bool()  { return vigo::odd(UInt32()); }
  };

  /// A built-in generator instance.
  extern Rand rand;

/**@} (defgroup)*/
NAMESPACE_END() //=============================================================

#endif

// eof
