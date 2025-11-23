// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief Vigo actions and controls
 */

#ifndef VIGO_VIGO_CONTROLS_HPP_INCLUDED
#define VIGO_VIGO_CONTROLS_HPP_INCLUDED

  #include "./actions.hpp"
  #include "../common/pimpl.hpp"

  class QtVigoPushButton_;
  class QtVigoCheckBox_;
  class QtVigoSlider_;
  class QtVigoSpinBox_;

NAMESPACE_VIGO(control) //=====================================================
/** \defgroup grp_controls_hpp Controls
@{*/

  class Control
  {
  protected:
    Control()           {}

  public:
    virtual ~Control()  {}

    virtual pvoid Widget() = 0;

    //virtual void Show(bool) {}
    //virtual void Refresh()  {}
  };


//-----------------------------------------------------------------------------

  class PushButton: public Control, protected internal::pimpl<QtVigoPushButton_>
  {
    NO_GEN(PushButton)

  public:
    PushButton(pcstr title,ToggleAction&);
   ~PushButton();

    void Clicked(bool value) { m_action(value); }

    pvoid Widget();

  private:
    ToggleAction& m_action;
  };


//-----------------------------------------------------------------------------

  class CheckBox: public Control, protected internal::pimpl<QtVigoCheckBox_>
  {
    NO_GEN(CheckBox)

  public:
    CheckBox(pcstr label,ToggleAction&);
   ~CheckBox();

    void Clicked(bool value) { m_action(value); }

    pvoid Widget();

  private:
    ToggleAction& m_action;
  };


//-----------------------------------------------------------------------------

  class Slider: public Control, protected internal::pimpl<QtVigoSlider_>
  {
    NO_GEN(Slider)

  public:
    Slider(pcstr label,DataAction<int>&,int min,int max,int initValue,bool tracking);
   ~Slider();

    void Set(int min,int max);
    void SetTracking(bool);
    void SetValue(int);

    void Changed(int value) { m_action(value); }

    pvoid Widget();

  private:
    DataAction<int>& m_action;
  };


//-----------------------------------------------------------------------------

  class SpinBox: public Control, protected internal::pimpl<QtVigoSpinBox_>
  {
    NO_GEN(SpinBox)

  public:
    SpinBox(pcstr label,DataAction<int>&,int min,int max,int initValue);
   ~SpinBox();

    void Set(int min,int max);
    void SetValue(int);

    void Changed(int value) { m_action(value); }

    pvoid Widget();

  private:
    DataAction<int>& m_action;
  };


/**@} (defgroup)*/
NAMESPACE_END() //=============================================================

#endif

// eof
