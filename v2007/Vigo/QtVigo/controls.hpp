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
* $Id: controls.hpp 193 2007-01-13 03:55:19Z ian_b $
\*****************************************************************************/

/** \file
 */

#ifndef VIGO_QTVIGO_CONTROLS_HPP_INCLUDED
#define VIGO_QTVIGO_CONTROLS_HPP_INCLUDED

  #include "../vigo/controls.hpp"
  #include <QtGui/QPushButton>
  #include <QtGui/QCheckBox>
  #include <QtGui/QSlider>
  #include <QtGui/QSpinBox>

//-----------------------------------------------------------------------------

  class QtVigoPushButton_: public QPushButton,
                           protected vigo::internal::pimpl_iface<vigo::control::PushButton>
  {
    SUPER(QPushButton)
    NO_GEN(QtVigoPushButton_)
    Q_OBJECT

  public:
    QtVigoPushButton_(vigo::control::PushButton& iface, pcstr title, bool checkable);
   ~QtVigoPushButton_();

    QtVigoPushButton_* Widget() { return this; }

  private slots:
    void Clicked(bool);
  };


//-----------------------------------------------------------------------------

  class QtVigoCheckBox_: public QCheckBox,
                         protected vigo::internal::pimpl_iface<vigo::control::CheckBox>
  {
    SUPER(QCheckBox)
    NO_GEN(QtVigoCheckBox_)
    Q_OBJECT

  public:
    QtVigoCheckBox_(vigo::control::CheckBox& iface, pcstr label);
   ~QtVigoCheckBox_();

    QtVigoCheckBox_* Widget() { return this; }

  private slots:
    void StateChanged(int);
  };


//-----------------------------------------------------------------------------

  class QtVigoSlider_: public QSlider,
                       protected vigo::internal::pimpl_iface<vigo::control::Slider>
  {
    SUPER(QSlider)
    NO_GEN(QtVigoSlider_)
    Q_OBJECT

  public:
    QtVigoSlider_(vigo::control::Slider& iface, pcstr label);
   ~QtVigoSlider_();

    void Set(int min,int max);
    void SetTracking(bool);
    void SetValue(int);
    
    QtVigoSlider_* Widget() { return this; }

  private slots:
    void ValueChanged(int);
  };


//-----------------------------------------------------------------------------

  class QtVigoSpinBox_: public QSpinBox,
                        protected vigo::internal::pimpl_iface<vigo::control::SpinBox>
  {
    SUPER(QSpinBox)
    NO_GEN(QtVigoSpinBox_)
    Q_OBJECT

  public:
    QtVigoSpinBox_(vigo::control::SpinBox& iface, pcstr label);
   ~QtVigoSpinBox_();

    void Set(int min,int max);
    void SetValue(int);
    
    QtVigoSpinBox_* Widget() { return this; }

  private slots:
    void ValueChanged(int);
  };


//-----------------------------------------------------------------------------

#endif

//** eof **********************************************************************
