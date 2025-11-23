// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief Basic "C" types and typedefs.
 *
 *  This file defines basic "C" types and typedefs. It should be the first file
 *  included in any unit. Using typedefs instead of built-in types makes
 *  code more readable and portable. Vigo uses these typedefs consistently,
 *  and you are encouraged to do the same in your code.
 */

#ifndef VIGO_COMMON_TYPES_H_INCLUDED
#define VIGO_COMMON_TYPES_H_INCLUDED

/*---------------------------------------------------------------------------*/

#ifndef __cplusplus
  #define inline __inline         /* make 'inline' keyword available in C */
#endif

/*---------------------------------------------------------------------------*/
/* Basic types                                                               */

#ifdef __cplusplus
  extern "C" {
#endif

/** \defgroup grp_types_c Portable primitive types and functions (plain C)
@{*/
  typedef void*          pvoid;   /**< void pointer */
  typedef void const*    pcvoid;  /**< void const pointer */

  #ifndef NULL
    #define NULL         (0)      /**< null pointer */
  #endif

  typedef char*          pstr;    /**< pointer to a C-type string */
  typedef char const*    pcstr;   /**< pointer to a constant C-type string */

  typedef unsigned char  byte;    /**< unsigned char = byte */
  typedef byte*          pbyte;   /**< pointer to a byte */
  typedef byte const*    pcbyte;  /**< pointer to a constant byte */

  typedef unsigned short ushort;  /**< unsigned short integer */
  typedef unsigned int   uint;    /**< unsigned integer */
  typedef unsigned long  ulong;   /**< unsigned long */

  typedef signed   char  int8;    /**< 8-bit integer */
  typedef unsigned char  uint8;   /**< 8-bit unsigned integer */

  typedef short          int16;   /**< 16-bit integer */
  typedef ushort         uint16;  /**< 16-bit unsigned integer */

  typedef int            int32;   /**< 32-bit integer */
  typedef uint           uint32;  /**< 32-bit unsigned integer */

#ifdef _MSC_VER
  typedef __int64          int64; /**< 64-bit integer  */
  typedef unsigned __int64 uint64;/**< 64-bit unsigned integer */
#else
  typedef long long          int64; /**< 64-bit integer  */
  typedef unsigned long long uint64;/**< 64-bit unsigned integer  */
#endif

/** \name Endianness
@{*/
  bool IsBigEndian();             /**< detects byte order on this system */

  uint64 htonll(uint64 x);        /**< 64bit host-to-network conversion (\c htonl and \c hton are parts of the standard library) */
  uint64 ntohll(uint64 x);        /**< 64bit network-to-host conversion (\c ntohl and \c ntoh are parts of the standard library) */
/**@}*/

/* off M$oft pesky warnings */
#ifdef _MSC_VER
  #pragma warning(disable: 4290)  // C++ Exception Specification ignored
  #pragma warning(disable: 4355)  // 'this' used in base member initializer list
  #pragma warning(disable: 4786)  // identifier was truncated (templates)
  #pragma warning(disable: 4503)  // decorated name length exceeded
  #pragma warning(disable: 4097)  // typedef used as synonym for class-name
#endif

/**@} (defgroup)*/
#ifdef __cplusplus
  } /* extern "C" */
#endif

/*---------------------------------------------------------------------------*/

#endif

// eof
