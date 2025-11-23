// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

#ifndef VIGO_QTVIGO_DOCK_HPP_INCLUDED
#define VIGO_QTVIGO_DOCK_HPP_INCLUDED

  #include "../vigo/dock.hpp"
  #include "../vigo/controls.hpp"
  #include "../common/pimpl.hpp"
  #include <QtGui/QDockWidget>

//-----------------------------------------------------------------------------

  class QtVigoDock_: public QDockWidget,
                     protected vigo::internal::pimpl_iface<vigo::control::Dock>
  {
    SUPER(QDockWidget)
    NO_GEN(QtVigoDock_)
    Q_OBJECT

  public:
    QtVigoDock_(vigo::control::Dock& iface, pcstr title);
   ~QtVigoDock_();

    vigo::control::PushButton* AddPushButton(pcstr label,vigo::ToggleAction&);
    vigo::control::CheckBox*   AddCheckBox  (pcstr label,vigo::ToggleAction&);
    vigo::control::Slider*     AddSlider    (pcstr label,vigo::DataAction<int>&,
                                             int min=0,int max=100,int initValue=0,
					     bool tracking=false);
    vigo::control::SpinBox*    AddSpinBox   (pcstr label,vigo::DataAction<int>&,
                                             int min=0,int max=100,int initValue=0);
  public:
    void Show(bool);

    QtVigoDock_* Widget()       { return this; }

  private:
    void AddControl(vigo::control::Control*);
    std::list<vigo::control::Control*> m_controls;
  };


//-----------------------------------------------------------------------------

#endif

// eof
