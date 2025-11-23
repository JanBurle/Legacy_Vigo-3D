// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief Resources
 *
 */

#ifndef VIGO_VIGO_RESOURCE_HPP_INCLUDED
#define VIGO_VIGO_RESOURCE_HPP_INCLUDED

  #include "../common/types.hpp"
  #include <string>

NAMESPACE_VIGO(res) //=========================================================
/** \defgroup grp_resource_hpp Resources
@{*/

  extern std::string const name;
  extern uint        const version;
  extern std::string const version_string;
  extern std::string const name_version;
  extern std::string const url;
  extern std::string const copyright;
  extern std::string const statusCopyright;
  extern std::string const licence;
  extern std::string const about;

  extern std::string const GPLnotice;

  std::string const& VigoText();

/**@} (defgroup)*/
NAMESPACE_END() //=============================================================

#endif

// eof
