// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

  #include "./math.hpp"
  #include <math.h>

  #include <stdlib.h>
  #include <time.h>

NAMESPACE_VIGO(math) //========================================================

  Const pi2  (MATH_PI/2),
        pi   (MATH_PI),
       _2pi  (MATH_PI*2),
       _4pi  (MATH_PI*4);

  Const c    (MATH_C);

//----------------------------------------------------------------------------
// The 'Mersenne Twister' random number generator
// Copyright M. Matsumoto & T. Nishimura
// http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
// Since 2002, MT is a free software
// This code is adapted from mt19937ar.c

  static uint32 const MATRIX_A   = 0x9908b0dfUL; // constant vector a
  static uint32 const UPPER_MASK = 0x80000000UL; // most significant w-r bits
  static uint32 const LOWER_MASK = 0x7fffffffUL; // least significant r bits

  Rand::Rand(): m_mt(), m_mti(0)
  {
    Seed((uint32)(time(NULL)^clock()));
  }


  Rand::Rand(uint32 seed): m_mt(), m_mti(0)
  {
    Seed(seed);
  }


  Rand::Rand(uint32 initKey[], uint keyLength): m_mt(), m_mti(0)
  {
    Seed(initKey,keyLength);
  }


  Rand::~Rand()
  {
  }


  void Rand::Seed(uint32 seed)
  {
    m_mt[0]= seed;
    for(m_mti=1; m_mti<N; ++m_mti)
    {
      m_mt[m_mti] = 1812433253UL * (m_mt[m_mti-1] ^ (m_mt[m_mti-1] >> 30))
                  + m_mti;
    }
  }


  void Rand::Seed(uint32 initKey[], uint keyLength)
  {
    Seed(19650218UL);

    int i=1, j=0;
    for(int k = (N>keyLength ? (int)N : (int)keyLength); k; --k)
    {
      m_mt[i] = (m_mt[i] ^ ((m_mt[i-1] ^ (m_mt[i-1] >> 30)) * 1664525UL))
                + initKey[j] + j;
      ++i; ++j;
      if(i>=N) { m_mt[0] = m_mt[N-1]; i=1; }
      if(j>=(int)keyLength) j=0;
    }

    for(int k=N-1; k; --k)
    {
      m_mt[i] = (m_mt[i] ^ ((m_mt[i-1] ^ (m_mt[i-1] >> 30)) * 1566083941UL))
                - i;
      ++i;
      if(i>=N) { m_mt[0] = m_mt[N-1]; i=1; }
    }

    m_mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */
  }


  uint32 Rand::UInt32()
  {
    uint32 y;
    static uint32 mag01[2]={0x0UL, MATRIX_A};

    if(m_mti >= N)
    { // generate N words at one time
      int kk;
      for(kk=0;kk<N-M;kk++)
      {
        y = (m_mt[kk]&UPPER_MASK)|(m_mt[kk+1]&LOWER_MASK);
        m_mt[kk] = m_mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
      }
      for(;kk<N-1;kk++)
      {
        y = (m_mt[kk]&UPPER_MASK)|(m_mt[kk+1]&LOWER_MASK);
        m_mt[kk] = m_mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
      }
      y = (m_mt[N-1]&UPPER_MASK)|(m_mt[0]&LOWER_MASK);
      m_mt[N-1] = m_mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

      m_mti = 0;
    }

    y = m_mt[m_mti++];

    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
  }


  uint32 Rand::UInt32(uint32 min, uint32 max)
  {
    long range = max-min;
    if(range<=0)
      return min;

    return min + UInt32()%range;
  }


  double Rand::Double()
  {
    uint32 a=UInt32()>>5,
           b=UInt32()>>6;
    return (a*67108864.0+b) * (1.0/9007199254740992.0);
  }


  double Rand::Double(double min,double max)
  {
    double range = max-min;
    if(range<=0)
      return min;

    return min + Double()*range;
  }

  double Rand::DoubleGauss()
  {
		// from http://www.taygeta.com/random/gaussian.html
		// Algorithm by Dr. Everett (Skip) Carter, Jr.

		double x1, x2, w, y1, y2;

		do {
			x1 = 2.0 * Double() - 1.0;
			x2 = 2.0 * Double() - 1.0;
			w = x1 * x1 + x2 * x2;
		} while ( w >= 1.0 );

		w = sqrt( (-2.0 * log( w ) ) / w );
		y1 = x1 * w;
		y2 = x2 * w;

		return y1;		// y2 is forgotted - might want to remember it for more efficiency
  }

  Rand rand;

NAMESPACE_END() // eof
