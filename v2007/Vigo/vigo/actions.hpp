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
* $Id: actions.hpp 45 2006-06-21 10:20:33Z ian_b $
\*****************************************************************************/

/** \file
 *  \brief Vigo actions and controls
 */

#ifndef VIGO_VIGO_ACTIONS_HPP_INCLUDED
#define VIGO_VIGO_ACTIONS_HPP_INCLUDED

  #include "../common/types.hpp"

NAMESPACE_VIGO_() //===========================================================
/** \defgroup grp_actions_hpp Actions
@{*/

  class AbstractAction
  {
  public:
    AbstractAction()          {}
    virtual ~AbstractAction() {}

    /// Execute the action.
    virtual pvoid operator()() = 0;
  };


  class CallbackAction: public AbstractAction
  {
    SUPER(AbstractAction)
  public:
    typedef pvoid param;
    typedef pvoid (*pfun)(param);

  public:
    CallbackAction(pfun pf, param par): super(), m_pfun(pf), m_param(par) {}
    virtual pvoid operator()()
    {
      return m_pfun ? m_pfun(m_param) : NULL;
    }

  protected:
    pfun m_pfun; param m_param;
  };


  template<typename T>
  class DataAction: public CallbackAction
  {
    SUPER(CallbackAction)
  public:
    typedef T *pdata;

  public:
    DataAction(pfun pf, param par, pdata pd): super(pf,par), m_pdata(pd) {}
    GETTER(PData,   T*,   m_pdata)

    /// Modify the data and call back.
    virtual pvoid operator()(T data)
    {
      if(m_pdata) *m_pdata = data;
      return super::operator()();
    }
    
  protected:
    pdata m_pdata;
  };


  typedef DataAction<bool> ToggleAction;

/**@} (defgroup)*/
NAMESPACE_END_() //============================================================

#endif

//** eof **********************************************************************
