// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

  #include "./dock.hpp"
  #include "../QtVigo/dock.hpp"

NAMESPACE_VIGO(control) //*****************************************************

  Dock::Dock(pcstr title)
    : internal::pimpl<QtVigoDock_>(new QtVigoDock_(*this,title))
  {
  }


  Dock::~Dock()
  {
  }


#define PIMPL_FACE_CLASS Dock
#define PIMPL_IMPL_CLASS QtVigoDock_
#include "../common/pimpl.inc"

  PIMPL_IMPL(AddPushButton, PushButton*, (pcstr label,ToggleAction& action), (label,action),)
  PIMPL_IMPL(AddCheckBox,   CheckBox*,   (pcstr label,ToggleAction& action), (label,action),)
  PIMPL_IMPL(AddSlider,     Slider*,     (pcstr label,DataAction<int>& action,
                           int min,int max,int init,bool tr), (label,action,min,max,init,tr),)
  PIMPL_IMPL(AddSpinBox,    SpinBox*,    (pcstr label,DataAction<int>& action,
                           int min,int max,int init), (label,action,min,max,init),)

  PIMPL_IMPL(Show,   void, (bool b), (b),)
  PIMPL_IMPL(Widget, pvoid,(),(),)

NAMESPACE_END() // eof
