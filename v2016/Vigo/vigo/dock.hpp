// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief Vigo window
 */

#ifndef VIGO_VIGO_DOCK_HPP_INCLUDED
#define VIGO_VIGO_DOCK_HPP_INCLUDED

  #include "./controls.hpp"
  #include "../common/pimpl.hpp"

  class QtVigoDock_;

NAMESPACE_VIGO(control) //=====================================================
/** \defgroup grp_dock_hpp Dock
@{*/

  class Dock: protected internal::pimpl<QtVigoDock_>
  {
    NO_GEN(Dock)

  public:
    Dock(pcstr title);
   ~Dock();

    PushButton* AddPushButton(pcstr label,ToggleAction&);
    CheckBox*   AddCheckBox  (pcstr label,ToggleAction&);
    Slider*     AddSlider    (pcstr label,DataAction<int>&,int min,int max,int init,bool tracking);
    SpinBox*    AddSpinBox   (pcstr label,DataAction<int>&,int min,int max,int init);

    void Show(bool);

    pvoid Widget();
  };

/**@} (defgroup)*/
NAMESPACE_END() //=============================================================

#endif

// eof
