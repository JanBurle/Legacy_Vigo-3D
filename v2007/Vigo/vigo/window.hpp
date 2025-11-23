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
* $Id: window.hpp 196 2007-01-14 20:34:29Z ian_b $
\*****************************************************************************/

/** \file
 *  \brief Vigo window
 */

#ifndef VIGO_VIGO_WINDOW_HPP_INCLUDED
#define VIGO_VIGO_WINDOW_HPP_INCLUDED

  #include "../common/pimpl.hpp"
  #include "../common/patterns.hpp"
  #include "./menu.hpp"

  class QtVigoWindow_;

NAMESPACE_VIGO_() //===========================================================
/** \defgroup grp_window_hpp Window
@{*/

  class Canvas;

  class Window: protected internal::pimpl<QtVigoWindow_>
  {
    NO_GEN(Window)

  public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    #define EXC_IDS TOO_MANY_CANVASSES
    DECLARE_EXC_ROOT
    #undef  EXC_IDS
#endif

    /// Create a top level window.
    Window(pcstr title,bool show=true);
   ~Window();

    /// \name Menu functions
    //@{
    void AddMenu   (Menu&);
    void RemoveMenu(Menu&);
    //@}

    /// \name Visibility functions
    //@{
    void Show(bool);
    void FullScreen(bool);
    void ShowDock(bool);
    //@}

    void Move  (int x, int y);
    void Resize(int w, int h);

    int X();     int Y();
    int Width(); int Height();

    /// Attach a canvas to this window.
    void Attach(Canvas&,bool setWidget=true/*hook for 7-QtGui*/);

    void Polish();
    pvoid Widget();

    GETTER(GetCanvas,Canvas*,m_canvas)

  protected:
    Canvas    *m_canvas;
    Menu       m_windowMenu;
    bool       m_fullscreen;  MenuAction m_actFullscreen;
    bool       m_showDock;    MenuAction m_actShowDock;

  private:
    static pvoid cb_fullscreen(pvoid);
    static pvoid cb_showdock  (pvoid);
  };

/**@} (defgroup)*/
NAMESPACE_END_() //============================================================

#endif

//** eof **********************************************************************
