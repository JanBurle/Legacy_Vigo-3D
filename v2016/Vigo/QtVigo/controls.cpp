// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

  #include "./controls.hpp"

//-----------------------------------------------------------------------------

  QtVigoPushButton_::QtVigoPushButton_(vigo::control::PushButton& iface, pcstr title, bool checkable)
    : super(title), vigo::internal::pimpl_iface<vigo::control::PushButton>(iface)
  {
    setCheckable(checkable);
    if(!connect(this,SIGNAL(clicked(bool)),this,SLOT(Clicked(bool))))
      vigo::throw_exc(vigo::exc(vigo::exc::EXC_RUNTIME,
                                "connect SLOT(Clicked(bool)) failed"));
  }


  QtVigoPushButton_::~QtVigoPushButton_()
  {
  }


  void QtVigoPushButton_::Clicked(bool b)
  {
    m_iface.Clicked(b);
  }


//-----------------------------------------------------------------------------

  QtVigoCheckBox_::QtVigoCheckBox_(vigo::control::CheckBox& iface, pcstr label)
    : super(label), vigo::internal::pimpl_iface<vigo::control::CheckBox>(iface)
  {
    if(!connect(this,SIGNAL(stateChanged(int)),this,SLOT(StateChanged(int))))
      vigo::throw_exc(vigo::exc(vigo::exc::EXC_RUNTIME,
                                "connect SLOT(StateChanged(int)) failed"));
  }


  QtVigoCheckBox_::~QtVigoCheckBox_()
  {
  }


  void QtVigoCheckBox_::StateChanged(int state)
  {
    m_iface.Clicked(state==Qt::Checked);
  }


//-----------------------------------------------------------------------------

  QtVigoSlider_::QtVigoSlider_(vigo::control::Slider& iface, pcstr /*label*/)
    : super(Qt::Horizontal/*label not used*/), vigo::internal::pimpl_iface<vigo::control::Slider>(iface)
  {
    if(!connect(this,SIGNAL(valueChanged(int)),this,SLOT(ValueChanged(int))))
      vigo::throw_exc(vigo::exc(vigo::exc::EXC_RUNTIME,
                                "connect SLOT(ValueChanged(int)) failed"));
  }


  QtVigoSlider_::~QtVigoSlider_()
  {
  }


  void QtVigoSlider_::Set(int min,int max)
  {
    setMinimum(min);
    setMaximum(max);
  }


  void QtVigoSlider_::SetTracking(bool tracking)
  {
    setTracking(tracking);
  }


  void QtVigoSlider_::SetValue(int value)
  {
    setValue(value);
  }


  void QtVigoSlider_::ValueChanged(int value)
  {
    m_iface.Changed(value);
  }

//-----------------------------------------------------------------------------

  QtVigoSpinBox_::QtVigoSpinBox_(vigo::control::SpinBox& iface, pcstr /*label*/)
    : super(), vigo::internal::pimpl_iface<vigo::control::SpinBox>(iface)
  {
    if(!connect(this,SIGNAL(valueChanged(int)),this,SLOT(ValueChanged(int))))
      vigo::throw_exc(vigo::exc(vigo::exc::EXC_RUNTIME,
                                "connect SLOT(ValueChanged(int)) failed"));
  }


  QtVigoSpinBox_::~QtVigoSpinBox_()
  {
  }


  void QtVigoSpinBox_::Set(int min,int max)
  {
    setMinimum(min);
    setMaximum(max);
  }


  void QtVigoSpinBox_::SetValue(int value)
  {
    setValue(value);
  }


  void QtVigoSpinBox_::ValueChanged(int value)
  {
    m_iface.Changed(value);
  }


// eof
