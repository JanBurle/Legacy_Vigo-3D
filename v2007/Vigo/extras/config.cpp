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
* $Id: config.cpp 193 2007-01-13 03:55:19Z ian_b $
\*****************************************************************************/

/** \file
 */

  #include "./config.hpp"

  NAMESPACE_VIGO(extras)

//----------------------------------------------------------------------------

  bool Config::lessIgnoreCase::operator()(std::string const& a, std::string const& b) const
  {
  #ifndef stricmp
    #define stricmp strcasecmp
  #endif
   return stricmp(a.c_str(),b.c_str()) < 0;
  }
 
  std::string Config::FindString(pcstr grp, pcstr tag) const throw(not_found)
  {
    grp_map::const_iterator gi = m_cfgData.find(grp);
    if(gi == m_cfgData.end())
      throw not_found();

    tag_map t = gi->second;
    tag_map::const_iterator ti = t.find(tag);
    if(ti == t.end())
      throw not_found();

    return ti->second;
  }

  Config::Config(): m_cfgData()
  {
  }

  void Config::Load(std::istream& is) throw(exc)
  {
    std::string s, group, lastGroup;

    grp_map::iterator itGroup = m_cfgData.end();

    for(;;)
    {
      std::getline(is,s);
      if(is.bad())
        THROW_EXC2(LOAD_FAILED,"Config::Load() failed")

      if(is.fail()) // end of file
        break;

      // strip comments
      std::string::size_type commPos = s.find('#');
      if(std::string::npos!=commPos)
        s = s.substr(0,commPos);

      // process one line, look for '='
      std::string tag, value;

      std::string::size_type eqPos = s.find('=');
      if(std::string::npos==eqPos)
      {
        // no '=', potential group tag
        tag = trim(s);
        if(!tag.empty() && '['==*tag.begin() && ']'==*(tag.rbegin()))
        {
          group = tag.substr(1,tag.size()-2);
        #ifndef __SUNOS__
          tag.clear();
        #else
          tag = "";
        #endif
        }
      }
      else
      {
        tag   = trim(s.substr(0,eqPos));
        value = trim(s.substr(eqPos+1));
      }

      if(!tag.empty())
      {
        if(m_cfgData.end()==itGroup || group!=lastGroup)
        {
          itGroup   = m_cfgData.insert(grp_map::value_type(group,tag_map()));
          lastGroup = group;
        }

        (*itGroup).second.insert(tag_map::value_type(tag,value));
      }
    }
  }

  void Config::Save(std::ostream& os) const throw(exc)
  {
    // go through groups
    for(grp_map::const_iterator gi = m_cfgData.begin(); gi!=m_cfgData.end(); ++gi)
    {
      // print the group [tag]
      os << '[' << gi->first << ']' << std::endl;

      // go through tags within the group
      tag_map t = gi->second;
      for(tag_map::const_iterator ti = t.begin(); ti!=t.end(); ++ti)
      {
        // print the tag=value pair
        os << ti->first << '=' << ti->second << std::endl;
      }
    }
  }

  void Config::Load(pcstr fileName) throw(exc)
  {
    if(empty(fileName))
      THROW_EXC2(EMPTY_FILENAME,"Config::Load() empty file name")

    std::ifstream ifs(fileName);
    if(!ifs.good())
      THROW_EXC2(CANT_OPEN_FILE,
        std::string("Config::Load() can't open file: ") + fileName)

    Load(ifs);
  }

  void Config::Save(pcstr fileName) const throw(exc)
  {
    if(empty(fileName))
      THROW_EXC2(EMPTY_FILENAME,"Config::Save() empty file name")

    std::ofstream ofs(fileName);
    Save(ofs);
  }

  bool Config::Exists(pcstr grp)
  {
    grp_map::iterator gi = m_cfgData.find(grp);
    return (gi != m_cfgData.end());
  }

  bool Config::Exists(pcstr grp, pcstr tag)
  {
    grp_map::iterator gi = m_cfgData.find(grp);
    if(gi == m_cfgData.end())
      return false;

    tag_map t = gi->second;
    tag_map::iterator ti = t.find(tag);

    return (ti != t.end());
  }

  std::string Config::GetString(pcstr grp, pcstr tag, std::string const& defValue) const
  {
    try
    {
      return FindString(grp,tag);
    }
    catch(not_found&)
    {
      return defValue;
    }
  }

  std::string Config::GetString(pcstr grp, pcstr tag, pcstr defValue) const
  {
    return GetString(grp,tag,std::string(defValue));
  }

  double Config::GetDouble(pcstr grp, pcstr tag, double defValue) const
  {
    try
    {
      std::string s = FindString(grp,tag);
      if(s.empty())
        return 0.0;
      else
        return(atof(s.c_str()));
    }
    catch(not_found&)
    {
      return defValue;
    }
  }

  int Config::GetInt(pcstr grp, pcstr tag, int defValue) const
  {
    try
    {
      std::string s = FindString(grp,tag);
      if(s.empty())
        return 0;
      else
        return(atoi(s.c_str()));
    }
    catch(not_found&)
    {
      return defValue;
    }
  }

  uint Config::GetUint(pcstr grp, pcstr tag, uint defValue) const
  {
    return (uint)GetInt(grp,tag,(int)defValue);
  }

  bool Config::GetBool(pcstr grp, pcstr tag, bool defValue) const
  {
    try
    {
      std::string s = FindString(grp,tag);
      if(s.empty())
        return false;
      else
      {
        char c = (char)tolower(s[0]);
        // supports "yes", "yeah", "ay", "ano", "true"
        return('y'==c || 'a'==c || 't'==c);
      }
    }
    catch(not_found&)
    {
      return defValue;
    }
  }

  void Config::Clear()
  {
    m_cfgData.clear();
  }

  void Config::Erase(pcstr grp)
  {
    grp_map::iterator gi = m_cfgData.find(grp);
    if(gi != m_cfgData.end())
      m_cfgData.erase(gi);
  }

  void Config::Erase(pcstr grp, pcstr tag)
  {
    grp_map::iterator gi = m_cfgData.find(grp);
    if(gi != m_cfgData.end())
    {
      tag_map t = gi->second;
      tag_map::iterator ti = t.find(tag);

      if(ti != t.end())
      {
        t.erase(ti);
      }
    }
  }

  void Config::SetString(pcstr grp, pcstr tag, std::string const& value)
  {
    grp_map::iterator itGrp = m_cfgData.find(grp);
    if(itGrp==m_cfgData.end())
      itGrp = m_cfgData.insert(grp_map::value_type(grp,tag_map()));

    tag_map &tm = (*itGrp).second;
    tag_map::iterator itTag = tm.find(tag);
    if(itTag==tm.end())
      itTag = tm.insert(tag_map::value_type(tag,""));

    (*itTag).second = value;
  }

  void Config::SetString(pcstr grp, pcstr tag, pcstr value)
  {
    SetString(grp,tag,std::string(value));
  }

  void Config::SetDouble(pcstr grp, pcstr tag, double value)
  {
    SetString(grp,tag,sformat("%g",value));
  }

  void Config::SetInt(pcstr grp, pcstr tag, int value)
  {
    SetString(grp,tag,sformat("%d",value));
  }

  void Config::SetUint(pcstr grp, pcstr tag, uint value)
  {
    SetInt(grp,tag,(int)value);
  }

  void Config::SetBool(pcstr grp, pcstr tag, bool value)
  {
    SetString(grp,tag,value ? "yes" : "no");
  }

  Config::cfg_items Config::GetGroup(pcstr grp)
  {
    Config::cfg_items items;

    grp_map::iterator itGrp = m_cfgData.find(grp);
    if(itGrp!=m_cfgData.end())
    {
      tag_map &tm = itGrp->second;
      for(tag_map::iterator it=tm.begin(), itEnd=tm.end(); it!=itEnd; ++it)
      {
        items.push_back(cfg_item(it->first,it->second));
      }
    }

    return items;
  }

  Config::config_dat::config_dat(pcstr g, pcstr t, std::string& v, pcstr d)
    : grp(g), tag(t), type('s'), var(), def()
  {
    var.s = &v;
    def.s = d;
  }

  Config::config_dat::config_dat(pcstr g, pcstr t, double& v, double d)
    : grp(g), tag(t), type('d'), var(), def()
  {
    var.d = &v;
    def.d = d;
  }

  Config::config_dat::config_dat(pcstr g, pcstr t, float& v, float f)
    : grp(g), tag(t), type('f'), var(), def()
  {
    var.f = &v;
    def.f = f;
  }

  Config::config_dat::config_dat(pcstr g, pcstr t, int& v, int d)
    : grp(g), tag(t), type('i'), var(), def()
  {
    var.i = &v;
    def.i = d;
  }

  Config::config_dat::config_dat(pcstr g, pcstr t, uint& v, uint d)
    : grp(g), tag(t), type('u'), var(), def()
  {
    var.u = &v;
    def.u = d;
  }

  Config::config_dat::config_dat(pcstr g, pcstr t, bool& v, bool d)
    : grp(g), tag(t), type('b'), var(), def()
  {
    var.b = &v;
    def.b = d;
  }

  Config::config_dat::config_dat()
    : grp(""), tag(""), type(' '), var(), def()
  {
    static int dat_junk_int = 0;
    var.i = &dat_junk_int;
  }

  Config::config_dat::config_dat(config_dat const& r)
    : grp(r.grp), tag(r.tag), type(r.type), var(r.var), def(r.def)
  {
  }

  void Config::Load(config_dat const list[])
  {
    for(config_dat const *l = list; l && !empty(l->tag); ++l)
    {
      switch(l->type)
      {
      case 's':
        SetString(l->grp,l->tag,*l->var.s);
        break;
      case 'd':
        SetDouble(l->grp,l->tag,*l->var.d);
        break;
      case 'f':
        SetFloat(l->grp,l->tag,*l->var.f);
        break;
      case 'i':
        SetInt(l->grp,l->tag,*l->var.i);
        break;
      case 'u':
        SetUint(l->grp,l->tag,*l->var.u);
        break;
      case 'b':
        SetBool(l->grp,l->tag,*l->var.b);
        break;
      }
    }
  }

  void Config::Save(config_dat const list[]) const
  {
    for(config_dat const *l = list; l && !empty(l->tag); ++l)
    {
      switch(l->type)
      {
      case 's':
        *l->var.s = GetString(l->grp,l->tag,std::string(l->def.s));
        break;
      case 'd':
        *l->var.d = GetDouble(l->grp,l->tag,l->def.d);
        break;
      case 'f':
        *l->var.f = GetFloat(l->grp,l->tag,l->def.f);
        break;
      case 'i':
        *l->var.i = GetInt(l->grp,l->tag,l->def.i);
        break;
      case 'u':
        *l->var.u = GetInt(l->grp,l->tag,l->def.u);
        break;
      case 'b':
        *l->var.b = GetBool(l->grp,l->tag,l->def.b);
        break;
      }
    }
  }

NAMESPACE_END() //** eof ******************************************************
