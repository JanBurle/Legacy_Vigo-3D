// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief Basic "C++" types, typedefs, macros, etc.
 *
 *  This file defines basic "C++" types, typedefs and macros that make life in
 *  C++ easier. This file should be included in every C++ unit. Vigo uses them
 *  consistently, and you are encouraged to do the same.
 *
 *  Note that the most of the macros are "function-style" macros, used to
 *  generate boilerplate code. There is no following semicolon needed when
 *  such a macro is used!
 */

#ifndef VIGO_COMMON_TYPES_HPP_INCLUDED
#define VIGO_COMMON_TYPES_HPP_INCLUDED

  #include "types.h"
  #include <exception>
  #include <string>
  #include <cassert>

  #ifdef _DEBUG
  #include <iostream>
  #include <iomanip>
  #endif

//----------------------------------------------------------------------------
/** \defgroup grp_types_hpp Types (C++), macros, etc.
@{*/
/** \name Machine type limits
@{*/

  int8    const MIN_INT8   = 0x80;
  int8    const MAX_INT8   = 0x7F;

  uint8   const MIN_UINT8  = 0x00;
  uint8   const MAX_UINT8  = 0xFF;

  int16   const MIN_INT16  = 0x8000;
  int16   const MAX_INT16  = 0x7FFF;

  uint16  const MIN_UINT16 = 0x0000;
  uint16  const MAX_UINT16 = 0xFFFF;

  int32   const MIN_INT32  = 0x80000000;
  int32   const MAX_INT32  = 0x7FFFFFFF;

  uint32  const MIN_UINT32 = 0x00000000;
  uint32  const MAX_UINT32 = 0xFFFFFFFF;

  int64   const MIN_INT64  = 0x8000000000000000ll;
  int64   const MAX_INT64  = 0x7FFFFFFFFFFFFFFFll;

  uint64  const MIN_UINT64 = 0x0000000000000000ll;
  uint64  const MAX_UINT64 = 0xFFFFFFFFFFFFFFFFll;
/**@}*/

//----------------------------------------------------------------------------
/** \name Macros for namespace definition
@{*/

  #define NAMESPACE_VIGO_()   namespace vigo {
  #define NAMESPACE_END_()                   }

  #define NAMESPACE_VIGO(name) namespace vigo { namespace name {
  #define NAMESPACE_END()                     }                }
/**@}*/

NAMESPACE_VIGO_() //===========================================================

/** \name Macros that help to declare classes.
@{*/

 /** \def SUPER(superClass)
  * Declares the \c super synonym for a superclass.
  */
 /** \def COPY_CONS(Class)
  * Emits a copy constructor signature.
  */
 /** \def OP_ASSIGN(Class)
  * Emits an assignment operator signature.
  */
 /** \def OP_EQ(Class)
  * Emits an equality operator signature.
  */
 /** \def OP_NE(Class)
  * Emits an inequality operator signature.
  */
 /** \def NO_COPY_CONS(Class)
  * Declares a private copy constructor.
  * If no definition is provided, this disables the copy constructor entirely.
  * Note that a copy constructor could otherwise be generated automatically.
  */
 /** \def NO_ASSIGN(Class)
  * Declares a private assignment operator.
  * If no definition is provided, this disables the assignment entirely.
  * Note that an assignment operator could otherwise be generated automatically.
  */
 /** \def NO_GEN(Class)
  * Ensures that a copy constructor and an assignment operator will not
  * be automatically generated.
  */
 /** \def AGETTER(name,type)
  * A shorthand to define a pure virtual (abstract) getter method.
  * @param name The name of the generated getter method.
  * @param type the return type
  */
 /** \def VGETTER(name,type,expr)
  * A shorthand to define a virtual getter method.
  * @param name The name of the generated getter method.
  * @param type the return type
  * @param expr An expression which value is returned; typically a data member.
  */
 /** \def GETTER(name,type,expr)
  * A shorthand to define an inline getter method.
  * @param name The name of the generated getter method.
  * @param type the return type
  * @param expr An expression which value is returned; typically a data member.
  */
 /** \def ASETTER(name,type)
  *
  * A shorthand to define a pure virtual (abstract) setter method.
  * @param name The name of the generated setter method.
  * @param type The type of the input value.
  */
 /** \def VSETTER(name,type,var)
  *
  * A shorthand to define a virtual inline setter method.
  * @param name The name of the generated setter method.
  * @param type The type of the input value.
  * @param var  The variable to be set; typically a data member.
  */
 /** \def SETTER(name,type,var)
  *
  * A shorthand to define an inline setter method.
  * @param name The name of the generated setter method.
  * @param type The type of the input value.
  * @param var  The variable to be set; typically a data member.
  */
 /** \def GETSET(nameGet,nameSet,type,var)
   * A shorthand to define both getter and setter methods at once.
   * @param nameGet The getter name.
   * @param nameSet The setter name.
   * @param type    The type of both the input and return value.
   * @param var     The variable which value is to be retrieved or set,
   *                typically a data member.
   */
 /** \def COMPARABLE(type)
   * Defines comparison operators ( ==, !=, \<, etc.). The class (C)
   * must have a method <tt>int Compare(C const& r) const</tt> that returns
   * a negative value if <tt>*this</tt> is less than \c r, a positive value if
   * <tt>*this</tt> is greater than \c r, and 0 if <tt>*this</tt> and r are
   * identical.
   */

  #define SUPER(superClass)      typedef superClass super;

  #define COPY_CONS(Class)       Class(Class const& r)
  #define COPY_CONS_C(Class)     Class::Class(Class const& r)

  #define OP_ASSIGN(Class)       Class& operator= (Class const& r)
  #define OP_ASSIGN_C(Class)     Class& Class::operator= (Class const& r)
  #define OP_EQ(Class)           bool operator== (Class const& r) const
  #define OP_EQ_C(Class)         bool Class::operator== (Class const& r) const
  #define OP_NE(Class)           bool operator!= (Class const& r) const
  #define OP_NE_C(Class)         bool Class::operator!= (Class const& r) const

  #define NO_COPY_CONS(Class)   \
    private: COPY_CONS(Class);

  #define NO_ASSIGN(Class)      \
    private: OP_ASSIGN(Class);

  #define NO_GEN(Class)         \
    NO_COPY_CONS(Class)         \
    NO_ASSIGN(Class)

  #define AGETTER(name,type)   \
    virtual type name() const = 0;
  #define VGETTER(name,type,expr)   \
    virtual GETTER(name,type,expr)
  #define GETTER(name,type,expr)   \
    type name() const { return expr; }

  #define ASETTER(name,type)   \
    virtual void name(type v) =0;
  #define VSETTER(name,type,var)   \
    virtual void name(type v) { var = v; }
  #define SETTER(name,type,var)   \
    void name(type v) { var = v; }

  #define GETSET(nameGet,nameSet,type,var)   \
    GETTER(nameGet,type,var) \
    SETTER(nameSet,type,var)

  #define COMPARABLE(type)                     \
    bool operator==(type const& test) const    \
    {                                          \
        return (bool)(Compare(test) == 0);     \
    }                                          \
                                               \
    bool operator!=(type const& test) const    \
    {                                          \
        return (bool)(Compare(test) != 0);     \
    }                                          \
                                               \
    bool operator< (type const& test) const    \
    {                                          \
        return (bool)(Compare(test) < 0);      \
    }                                          \
                                               \
    bool operator<=(type const& test) const    \
    {                                          \
        return (bool)(Compare(test) <= 0);     \
    }                                          \
                                               \
    bool operator> (type const& test) const    \
    {                                          \
        return (bool)(Compare(test) > 0);      \
    }                                          \
                                               \
    bool operator>=(type const& test) const    \
    {                                          \
        return (bool)(Compare(test) >= 0);     \
    }
/**@}*/
NAMESPACE_END_()
/**@} (defgroup)*/
NAMESPACE_VIGO_()

//----------------------------------------------------------------------------
/** \defgroup grp_types_hpp_exc Exception handling
@{*/

  /// The base of all exceptions.
  /** The class hierarchy is mirrored in a hierarchy of exception classes.
    * That allows exceptions to be handled selectively based on their origin.
    * The class \c exc is the base class of all exceptions.
    * Macros \c DECLARE_EXC, \c DECLARE_EXC_ROOT and \c EXC_IDS allow easy
    * declaration of derived exception subclasses.
    */
  class exc: public std::exception
  {
    // SUPER(std::exception)
  protected:
    uint        m_id;       //< A numeric exception code.
    std::string m_msg;      //< A text message.

  public:
    virtual ~exc() throw() {}

    /// An overriden std::exception::what() method.
    /// @return A text message contained in the exception object.
    virtual pcstr what() const throw()
    {
      return m_msg.c_str();
    }

    /** Makes a copy of an exception object. Since exceptions may be caught
     *  using a base class reference, calling this method is a way of creating
     *  a copy of the original.
     */
    virtual exc& Copy() const
    {
      return *new exc(*this);
    }

    /// A set of numeric exception codes.
    enum
    {
      EXC_NONE   = 0,      ///< an unclassified exception
      EXC_SYSTEM,          ///< a system error (something went gravely wrong, panic!)
      EXC_ASSERT,          ///< an assertion failed
      EXC_NOT_IMPLEMENTED, ///< this feature has not (yet) been implemented
      EXC_NOT_HERE,        ///< this code location should not habe been reached
      EXC_RUNTIME,         ///< a runtime check failed
      EXC_USER   = 1000    ///< user exception codes start from here
    };

    /** The default exception constructor.
     *  @param id  a numeric exception code
     *  @param msg a text message
     */
    exc(uint id=EXC_NONE, pcstr msg=NULL)
      : m_id(id), m_msg(msg ? msg : "")
    {
    }

    /// \overload
    exc(uint id, std::string const& msg)
      : m_id(id), m_msg(msg)
    {
    }

    /// Gets the exception id code.
    GETTER(Id, uint, m_id)
    /// Gets the exception message.
    GETTER(Msg, std::string const&, m_msg)
    /// Returns a human readable exception description.
    std::string What() const;
  };

  /// Throws an exception.
  /// Do not use directly, use a \c THROW_EXCx macro instead.
  template<class E>
  void throw_exc(E const& e)
  {
  #ifdef _DEBUG
    {std::cout << "** throw_exc ** " << e.What() << " **" << std::endl;}
  #endif
    // do not throw an exception if another one is being handled
    if(!std::uncaught_exception())
      throw e;
  }

//----------------------------------------------------------------------------
/** \name Macros that throw exceptions.
@{*/

  /** Throws an exception.
   *  The type of the exceptiom depends on the current scope: the macro ought
   *  to be used within a method of a class which declaration included
   *  \c DECLARE_EXC or \c DECLARE_EXC_ROOT, or its subclass.
   */
  #define THROW_EXC2(id,msg)     \
    throw_exc(exc(exc::id,msg));

  /// Throws an unclassified (\c EXC_NONE) exception
  #define THROW_EXC(msg)         \
    THROW_EXC2(EXC_NONE,msg)
/**@}*/

//----------------------------------------------------------------------------
/** \name Macros that declare the exception class hierarchy.
@{*/

  /// A helper macro, do not use diirectly.
  #define DECLARE_EXC_SUPER(superExc)                   \
    class exc: public superExc                          \
    {                                                   \
    public:                                             \
      enum {EXC_USER = 1000,EXC_IDS};                   \
                                                        \
      exc(uint id=vigo::exc::EXC_NONE,pcstr msg=NULL)   \
        : superExc(id,msg)                              \
      {                                                 \
      }                                                 \
                                                        \
      exc(uint id,std::string const& msg)               \
        : superExc(id,msg)                              \
      {                                                 \
      }                                                 \
                                                        \
      /* a deep copy factory */                         \
      virtual vigo::exc& Copy() const                   \
      {                                                 \
        return *new exc(*this);                         \
      }                                                 \
    };

  /** \def DECLARE_EXC
    * declares a nested exception subclass. Define \c EXC_IDS as a list of
    * exception codes before using \c DECLARE_EXC. The superclass must have
    * declared or inherited its own exception class.
    */
  #define DECLARE_EXC       DECLARE_EXC_SUPER(super::exc)
  /** \def DECLARE_EXC_ROOT
    * Declares a root of a hierarchy of nested exception classes, as a subclass
    * of the top-level \c vigo::exc excetion class.
    * Define \c EXC_IDS as a list of exception ids
    * before using \c DECLARE_EXC.
    */
  #define DECLARE_EXC_ROOT  DECLARE_EXC_SUPER(vigo::exc)
/**@}*/
/**@} (defgroup)*/

//----------------------------------------------------------------------------
/** \defgroup grp_types_hpp_def Macros for defensive programming and debugging.
@{*/

/** \def ASSERT_EXC(expr)
  * Asserts \c expr!=0. In a debug build throws \c EXC_ASSERT if \c expr
  * evaluates to 0. In a release build the macro expands to a null statement.
  */

/** \def ASSERT_EXC2(expr,id,msg)
  * Same as \c ASSERT_EXC, except that you can specify the exception code
  * and message.
  */

/** \def VALIDATE_EXC(expr)
  * In a debug build acts the same as \c ASSERT_EXC.
  * In a release build \c expr is still evaluated, but its result is ignored
  * and no exception is thrown.
  */

/** \def TRACE(tag,expr)
  * This is a handy macro for formatted trace print.
  * In a debug build it prints a trace to stdout.
  * In a release build the macro expands to a null statement.
  * For the type of \c expr, there must exist a stream insertion operator.
  * The macro is often used as: <tt>TRACE("tag",sformat("...",...)</tt>.
  */

/** \def TRACE1(tag,expr1)
  * A one-expression variant of \c TRACE.
  */

/** \def TRACE2(tag,expr1,expr2)
  * A two-expression variant of \c TRACE.
  */

/** \def TRACE3(tag,expr1,expr2,expr3)
  * A three-expression variant of \c TRACE.
  */

/** \def TRACE4(tag,expr1,expr2,expr3,expr4)
  * A four-expression variant of \c TRACE.
  */

/** \def TRACE5(tag,expr1,expr2,expr3,expr4,expr5)
  * A four-expression variant of \c TRACE.
  */

/** \def WATCH(expr)
  * Expands to <tt>TRACE("expr",expr)</tt>. Useful for tracing (watching)
  * a variable or a simple expression.
  * In a release build the macro expands to a null statement;
  */

/** \def NOT_HERE()
  * Use this to mark a place that should not be reached, as in:
  * \code
  * switch(field) // x, y or z
  * {
  *   case x: ... break;
  *   case y: ... break;
  *   case z: ... break;
  *   default: NOT_HERE()
  * }
  * \endcode
  * The macro throws an exception in a debug build, and expands to a null
  * statement in a release build.
  */

/** \def NOT_HERE1(msg)
  * Same as \c NOT_HERE, allows to specify an exception message.
  */

#ifdef _DEBUG

  #define ASSERT_EXC(expr)  \
    {if(!(expr))            \
      throw_exc(exc(exc::EXC_ASSERT, \
        sformat("** ASSERT ** " __FILE__"(%u): " #expr, __LINE__)));}

  #define ASSERT_EXC2(expr,id,msg)  \
    {if(!(expr))            \
      THROW_EXC2(id,msg)}

  #define VALIDATE_EXC(expr) ASSERT_EXC(expr)

  #define TRACE(tag,expr) \
    {std::cout << "[" << tag << "] " << expr << std::endl;}
  #define TRACE2(tag,expr1,expr2) \
    {std::cout << "[" << tag << "] " << expr1 << "|" << expr2 << std::endl;}
  #define TRACE3(tag,expr1,expr2,expr3) \
    {std::cout << "[" << tag << "] " << expr1 << "|" << expr2 \
                                     << "|" << expr3 << std::endl;}
  #define TRACE4(tag,expr1,expr2,expr3,expr4) \
    {std::cout << "[" << tag << "] " << expr1 << "|" << expr2 << "|" << expr3\
                                     << "|" << expr4 << std::endl;}
  #define TRACE5(tag,expr1,expr2,expr3,expr4,expr5) \
    {std::cout << "[" << tag << "] " << expr1 << "|" << expr2 << "|" << expr3\
                                     << "|" << expr4 << "|" << expr5 << std::endl;}

  #define NOT_HERE() \
    {throw_exc(exc(exc::EXC_NOT_HERE,""));}

  #define NOT_HERE1(msg) \
    {throw_exc(exc(exc::EXC_NOT_HERE,msg));}

#else

  #define ASSERT_EXC(expr)                      {}
  #define ASSERT_EXC2(expr,id,msg)              {}
  #define VALIDATE_EXC(expr)                    {expr;}
  #define TRACE(tag,expr)                       {}
  #define TRACE2(tag,expr1,expr2)               {}
  #define TRACE3(tag,expr1,expr2,expr3)         {}
  #define TRACE4(tag,expr1,expr2,expr3,expr4)   {}
  #define TRACE5(tag,expr1,expr2,expr3,expr4,expr5) {}
  #define NOT_HERE()                            {}
  #define NOT_HERE1(msg)                        {}

#endif

  #define TRACE1(tag,expr)  TRACE(tag,expr)
  #define WATCH(expr)       TRACE(#expr,expr)

/** \def RUNTIME_CHECK_EXC(expr)
  * Asserts \c expr!=0. Throws an \c EXC_RUNTIME exception if \c expr
  * evaluates to 0.
  */

/** \def RUNTIME_CHECK_EXC2(expr,msg)
  * Same as \c EXC_RUNTIME, except that you can specify the exception message.
  */

/** \def RUNTIME_CHECK_EXC3(expr,id,msg)
  * Same as \c EXC_RUNTIME, except that you can specify the exception code
  * and message.
  */

  #define RUNTIME_CHECK_EXC(expr)  \
    {if(!(expr))            \
      throw_exc(exc(exc::EXC_RUNTIME));}

  #define RUNTIME_CHECK_EXC2(expr,msg)  \
    {if(!(expr))            \
      THROW_EXC2(EXC_RUNTIME,msg)}

  #define RUNTIME_CHECK_EXC3(expr,id,msg)  \
    {if(!(expr))            \
      THROW_EXC2(id,msg)}

/** \def NOT_IMPLEMENTED(msg)
  * Use this macro to mark the yet undeveloped code. If the place is reached,
  * the macro throws \c EXC_NOT_IMPLEMENTED.
  */

  #define NOT_IMPLEMENTED(msg) \
    {throw_exc(exc(exc::EXC_NOT_IMPLEMENTED,msg));}

/** \def SYSTEM_ERROR(msg)
  * Use this macro to indicate that something went gravely wrong.
  * It throws an \c EXC_SYSTEM exception.
  */

  #define SYSTEM_ERROR(msg) \
    {throw_exc(exc(exc::EXC_SYSTEM,msg));}
/**@}*/

//-----------------------------------------------------------------------------
/** \defgroup grp_types_hpp_var Various commonly used templated functions.
@{*/

  #undef min
  /// Templated \c min function.
  template<typename T>
  T min(T a, T b)
  {
    return a<b ? a : b;
  }

  #undef max
  /// Templated \c max function.
  template<typename T>
  T max(T a, T b)
  {
    return a>b ? a : b;
  }

  #undef abs
  /// Templated \c abs function.
  template<typename T>
  T abs(T a)
  {
    return a>=0 ? a : -a;
  }

  #undef sgn
  /// Templated \c sgn function.
  template<typename T>
  T sgn(T a)
  {
    return a>=0 ? +1 : -1;
  }

  #undef swap
  /// Templated \c swap function.
  template<typename T>
  void swap(T& a, T& b)
  {
    T tmp = a;
    a = b;
    b = tmp;
  }

  /// Templated \c odd function.
  template<typename T>
  bool odd(T a)
  {
    return 0!=(a&1);
  }

  /// Templated \c even function.
  template<typename T>
  bool even(T a)
  {
    return 0==(a&1);
  }

  /// Templated \c sqr function.
  template<typename T>
  T sqr(T t)
  {
    return t*t;
  }

/**@}*/

//-----------------------------------------------------------------------------
/** \defgroup grp_types_hpp_str String manipulation.
@{*/

  /// String formatting. Called exactly as like \c printf.
  /// @return The formatted string.
  std::string sformat(pcstr format,...);

  /// Trims the whitespace on the left of a string.
  /** @param  s          The source string.
    * @param  whiteSpace A string that specifies whitespace characters,
    *         if \c NULL is passed, the usual set (\\f\\n\\r\\t\\v\\x20)
    *         will be used as a default.
    * @return The trimmed string.
    */
  std::string trimleft (std::string const& s, pcstr whiteSpace=NULL);

  /// Trims the whitespace on the right.
  std::string trimright(std::string const& s, pcstr whiteSpace=NULL);

  /// Trims a whitespace on the left and right.
  std::string trim     (std::string const& s, pcstr whiteSpace=NULL);

  /// Tests if a C-string is empty, i.e. NULL or starting with a zero byte.
  inline bool empty(pcstr s)
  {
      return !s || !*s;
  }
/**@}*/

NAMESPACE_END_() //============================================================

#endif

// eof
