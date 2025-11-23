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
* $Id: window.hpp 197 2007-01-15 19:32:55Z ian_b $
\*****************************************************************************/

/** \file
 */

#ifndef VIGO_QTVIGO_WINDOW_HPP_INCLUDED
#define VIGO_QTVIGO_WINDOW_HPP_INCLUDED

  #include "../common/pimpl.hpp"
  #include <QtGui/QMainWindow>
  #include <QtGui/QProgressDialog>
  #include <QtGui/QMenu>

//-----------------------------------------------------------------------------

  namespace vigo { class Window; class Canvas; class Menu;
    namespace control { class Dock; } }

  class QtVigoWindow_ : public QMainWindow,
                        protected vigo::internal::pimpl_iface<vigo::Window>
  {
    SUPER(QMainWindow)
    NO_GEN(QtVigoWindow_)
    Q_OBJECT

  public:
    QtVigoWindow_(vigo::Window& iface, pcstr title,bool show);
   ~QtVigoWindow_();

    QSize sizeHint() const
    {
      return QSize(400,400);
    }

  private:
    void closeEvent(QCloseEvent*);

  public slots:
    void about();
    void checkUpdate();
    void fullScreen(bool);
    void showDock(bool);

  private slots:
    void cancelUpdate();

  private:
    QProgressDialog       *m_progress;

  public:
    void AddMenu   (vigo::Menu*);
    void RemoveMenu(vigo::Menu*);
    void AddMenu   (vigo::Menu& menu)   { AddMenu   (&menu); }
    void RemoveMenu(vigo::Menu& menu)   { RemoveMenu(&menu); }

    void Polish();

    void Attach(vigo::Canvas&,bool setWidget);

    void Show(bool);

    void FullScreen(bool);
    bool IsFullScreen()   const;

    void ShowDock(bool);
    bool IsDockShown()    const;

    void Move  (int x, int y);
    void Resize(int w, int h);

    int X()     const;  int Y()      const;
    int Width() const;  int Height() const;

    QtVigoWindow_* Widget()  { return this; }

  private:
    vigo::Menu  *m_cameraMenu;
    vigo::Menu  *m_simulationMenu;
    QMenu       *m_helpMenu;
    vigo::control::Dock  *m_dock;
    bool         m_showDock;
  };

//-----------------------------------------------------------------------------

#endif

//** eof **********************************************************************
