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
* $Id: config.hpp 193 2007-01-13 03:55:19Z ian_b $
\*****************************************************************************/

/** \file
 *  \brief Configuration
 *
 *  Configuration class. Loads/Saves data from/to a file or a stream,
 *  interfaces with memory variables in bulk;
 *  organizes entries as [group]tag=value.
 */

#ifndef VIGO_EXTRAS_CONFIG_HPP_INCLUDED
#define VIGO_EXTRAS_CONFIG_HPP_INCLUDED

  #include "../common/types.hpp"
  #include <fstream>
  #include <string>
  #include <vector>
  #include <map>

NAMESPACE_VIGO(extras) //======================================================
/** \defgroup grp_config_hpp Configuration files
@{*/

  /// Configuration data
  class Config
  {
    NO_GEN(Config)

  public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    #define EXC_IDS EMPTY_FILENAME,CANT_OPEN_FILE,LOAD_FAILED
    DECLARE_EXC_ROOT
    #undef EXC_IDS
#endif

  private:
    /// sort function
    struct lessIgnoreCase: std::binary_function<std::string,std::string,bool>
    {
      bool operator()(std::string const& a, std::string const& b) const;
    };

    /// mapping tag -> value
    typedef std::multimap<std::string const,std::string,lessIgnoreCase> tag_map;
    /// mapping group -> (tag->value)
    typedef std::multimap<std::string const,tag_map,lessIgnoreCase>     grp_map;
    /// data
    grp_map m_cfgData;

    class not_found ///< exception, used internally
    {
    };

    /// find a [group, tag] string
    std::string FindString(pcstr grp, pcstr tag) const throw(not_found);

  public:
    /// construct an empty object
    Config();

    /// load data from a stream
    void Load(std::istream&)        throw(exc);
    /// save data to a stream
    void Save(std::ostream&)  const throw(exc);

    /// load data from a file
    void Load(pcstr fileName)       throw(exc);
    /// save data to a file
    void Save(pcstr fileName) const throw(exc);

    /// query
    bool Exists(pcstr grp);
    /// query
    bool Exists(pcstr grp, pcstr tag);

    /// get a string value, return defValue if group:tag is not present
    std::string GetString(pcstr grp, pcstr tag, std::string const& defValue) const;
    /// \overload
    std::string GetString(pcstr grp, pcstr tag, pcstr defValue)              const;

    /// get a double value, return defValue if group:tag is not present
    double      GetDouble(pcstr grp, pcstr tag, double defValue)             const;
    /// get a float value, return defValue if group:tag is not present
    float       GetFloat (pcstr grp, pcstr tag, float  defValue)             const
      { return (float)GetDouble(grp,tag,(double)defValue); }
    /// get an integer value, return defValue if group:tag is not present
    int         GetInt   (pcstr grp, pcstr tag, int    defValue)             const;
    /// get an unsigned integer value, return defValue if group:tag is not present
    uint        GetUint  (pcstr grp, pcstr tag, uint   defValue)             const;
    /// get a boolean value, return defValue if group:tag is not present
    bool        GetBool  (pcstr grp, pcstr tag, bool   defValue)             const;

    /// remove all data
    void Clear();
    /// remove all data in a group
    void Erase(pcstr grp);
    /// remove a specific piece of data
    void Erase(pcstr grp, pcstr tag);

    /// set a string value
    void SetString(pcstr grp, pcstr tag, std::string const& value);
    /// \overload
    void SetString(pcstr grp, pcstr tag, pcstr value);

    /// set a double value
    void SetDouble(pcstr grp, pcstr tag, double value);
    /// set a float value
    void SetFloat (pcstr grp, pcstr tag, float  value)
      { SetDouble(grp,tag,(double)value); }
    /// set an integer value
    void SetInt   (pcstr grp, pcstr tag, int    value);
    /// set an unsigned integer value
    void SetUint  (pcstr grp, pcstr tag, uint   value);
    /// set a boolean value
    void SetBool  (pcstr grp, pcstr tag, bool   value);

  public:
    typedef std::pair<std::string,std::string> cfg_item;
    typedef std::vector<cfg_item>              cfg_items;

    /// read a group
    cfg_items GetGroup(pcstr grp);

  public:
    /// memory interface: mixed pointer to variables
    union config_var
    {
      std::string *s;
      double      *d;
      float       *f;
      int         *i;
      uint        *u;
      bool        *b;
    };

    /// memory interface: the default value
    union config_def
    {
      pcstr        s;
      double       d;
      float        f;
      int          i;
      uint         u;
      bool         b;
    };

    /// memory interface; one entry
    struct config_dat
    {
      config_dat(pcstr grp, pcstr tag, std::string& var, pcstr  def);
      config_dat(pcstr grp, pcstr tag, double&      var, double def);
      config_dat(pcstr grp, pcstr tag, float&       var, float  def);
      config_dat(pcstr grp, pcstr tag, int&         var, int    def);
      config_dat(pcstr grp, pcstr tag, uint&        var, uint   def);
      config_dat(pcstr grp, pcstr tag, bool&        var, bool   def);
      config_dat();

      config_dat(config_dat const&);

      pcstr       grp;
      pcstr       tag;
      char        type;
      config_var  var;
      config_def  def;
    };

    /// load data from memory locations (bulk set of variables)
    void Load(config_dat const[]);
    /// save data to memory locations
    void Save(config_dat const[]) const;
  };

/**@} (defgroup)*/
NAMESPACE_END() //=============================================================

#endif

//** eof **********************************************************************
