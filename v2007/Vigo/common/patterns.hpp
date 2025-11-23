/*****************************************************************************\
* This file is a part of Vigo::3D, a multi-agent simulation and visualization *
* system.                                                                     *
*                                                                             *
* Copyright (c) 2002--2006 by Ian G. Burleigh       burleigh@cpsc.ucalgary.ca *
* burleigh@cpsc.ucalgary.ca                         http://ard-galen.org/vigo *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for   *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program; if not, write to the                                     *
*   Free Software Foundation, Inc.,                                           *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                 *
*-----------------------------------------------------------------------------*
* $Id: patterns.hpp 147 2006-09-02 18:29:07Z ian_b $
\*****************************************************************************/

/** \file
 *  \brief Useful pattern classes.
 *
 */

#ifndef VIGO_COMMON_PATTERNS_HPP_INCLUDED
#define VIGO_COMMON_PATTERNS_HPP_INCLUDED
#ifndef DOXYGEN_SHOULD_SKIP_THIS

  #include "./types.hpp"
  #include <map>

NAMESPACE_VIGO(internal) //====================================================
// These classes are for internal use.

/*>>> not used:*/
//  template <class TMaster,class TBody> class master_of_many;
//
//  /// A backlink to a master of this body. The master may or may not exist.
//  template <class TBody,class TMaster>
//  class mastered_by
//  {
//  public:
//    typedef master_of_many<TMaster,TBody> master_b; // master of this body
//    typedef mastered_by   <TBody,TMaster> body_b;   // this body
//  public:
//    mastered_by()                : m_master(NULL)    { }
//    mastered_by(master_b* master): m_master(master)  { }
//    mastered_by(master_b& master): m_master(&master) { }
//    virtual ~mastered_by()                           { AttachTo(NULL); }
//
//    virtual bool AttachTo(master_b*);
//
//    master_b*       Master()        { return m_master; }
//    master_b const* Master() const  { return m_master; }
//
//  private:
//    master_b *m_master;
//  };
//
//
//  /// A master of 0..many bodies.
//  template <class TMaster,class TBody>
//  class master_of_many
//  {
//  public:
//    typedef master_of_many<TMaster,TBody> master_m; // this master
//    typedef mastered_by   <TBody,TMaster> body_m;   // body of this master
//  protected:
//    typedef std::map<body_m*,body_m*> container;
//    typedef typename container::iterator iterator;
//
//  public:
//    master_of_many(): m_mastered()  { }
//    virtual ~master_of_many()       { while(!m_mastered.empty())
//                                        Detach(*m_mastered.begin()->first); }
//
//    virtual bool Attach(body_m&);
//    virtual bool Detach(body_m&);
//
//    GETTER(Mastered, container&, m_mastered)
//
//    body_m* FirstBody()
//    {
//      if(m_mastered.empty()) return NULL;
//      return m_mastered.begin()->first;
//    }
//
//  protected:
//    container m_mastered;
//  };
//
//
//-----------------------------------------------------------------------------
//
//  template<class TBody,class TMaster>
//  bool mastered_by<TBody,TMaster>::AttachTo(master_b* m)
//  {
//    if(m != m_master)
//    {
//      if(m_master) { m_master->Detach(*/*>>>static_cast<body_t*>(*/this);
//                     m_master = NULL;   }
//      if(m)        { m_master = m;
//                     m_master->Attach(*/*static_cast<body_t*>*/(this)); }
//      return true; // change
//    }
//
//    return false;
//  }
//
//  template <class TMaster,class TBody>
//  bool master_of_many<TMaster,TBody>::Attach(body_m& b)
//  {
//    iterator it = m_mastered.find(&b);
//    if(it!=m_mastered.end()) return false;
//    m_mastered[&b] = &b; b.AttachTo(/*>>>static_cast<master_t*>*/(this));
//    return true;
//  }
//
//  template <class TMaster,class TBody>
//  bool master_of_many<TMaster,TBody>::Detach(body_m& b)
//  {
//    iterator it = m_mastered.find(&b);
//    if(it==m_mastered.end()) return false;
//    m_mastered.erase(it); b.AttachTo(/*>>>static_cast<master_t*>*/(NULL));
//    return true;
//  }

//-----------------------------------------------------------------------------
// master-bodies

  template <class This,class TBody>
  class has_many
  {
  public:
    typedef std::map<TBody*,TBody*> map;
    map& Map() { return m_map; }
      
  protected:
    typedef typename map::iterator iterator;

  public:
    has_many(This& t): m_this(t), m_map()  { }
   ~has_many()                             { DetachAll(); }
    bool Attach(TBody&);
    bool Detach(TBody&);
    
    void DetachAll();

    TBody* First()
    {
      if(m_map.empty()) return NULL;
      return m_map.begin()->first;
    }

    TBody const* First() const
    {
      return const_cast<has_many*>(this)->First();
    }

    void ForEach(void (*fn)(TBody&))
    {
      for(iterator it=m_map.begin(); it!=m_map.end(); ++it)
        fn(*(it->first));
    }

    void ForEach(void (*fn)(TBody&,pvoid),pvoid param)
    {
      for(iterator it=m_map.begin(); it!=m_map.end(); ++it)
        fn(*(it->first),param);
    }

    GETTER(Size,   uint,m_map.size())
    GETTER(IsEmpty,bool,m_map.empty())

  protected:
    This &m_this;
    map   m_map;
  };


  template <class This,class TBody>
  bool has_many<This,TBody>::Attach(TBody& b)
  {
    iterator it = m_map.find(&b);
    if(it!=m_map.end()) return false;
    m_map[&b] = &b;
    b.AttachTo(&m_this);
    return true;
  }

  template <class This,class TBody>
  bool has_many<This,TBody>::Detach(TBody& b)
  {
    iterator it = m_map.find(&b);
    if(it==m_map.end()) return false;
    m_map.erase(it); b.AttachTo(static_cast<This*>(NULL));
    return true;
  }

  template <class This,class TBody>
  void has_many<This,TBody>::DetachAll()
  {
    while(!m_map.empty())
      Detach(*m_map.begin()->first);
  }

NAMESPACE_END() //=============================================================

#endif
#endif

//** eof **********************************************************************
