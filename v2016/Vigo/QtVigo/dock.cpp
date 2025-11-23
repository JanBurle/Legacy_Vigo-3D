// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

  #include "./dock.hpp"
#undef foreach //>>>
  #include "../vigo/window.hpp"
  #include "../QtVigo/window.hpp"

  #include <QtGui/QLayout>
  #include <QtGui/QLabel>

//-----------------------------------------------------------------------------

  QtVigoDock_::QtVigoDock_(vigo::control::Dock& iface, pcstr title)
    : super(title,NULL/*>>>QtVigoWindow_::Get().Widget()*/ /*parent*/),
      vigo::internal::pimpl_iface<vigo::control::Dock>(iface),
      m_controls()
  {
    setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
  }


  QtVigoDock_::~QtVigoDock_()
  {
    while(!m_controls.empty())
    {
      delete *m_controls.begin();
      m_controls.pop_front();
    }
  }


  vigo::control::PushButton* QtVigoDock_::AddPushButton(pcstr label,vigo::ToggleAction& action)
  {
    vigo::control::PushButton* p = new vigo::control::PushButton(label,action);
    AddControl(p);
    return p;
  }


  vigo::control::CheckBox* QtVigoDock_::AddCheckBox(pcstr label,vigo::ToggleAction& action)
  {
    vigo::control::CheckBox* p = new vigo::control::CheckBox(label,action);
    AddControl(p);
    return p;
  }


  vigo::control::Slider* QtVigoDock_::AddSlider(pcstr label,vigo::DataAction<int>& action,
                                                int min,int max,int initValue,
		    		                bool tracking)
  {
    vigo::control::Slider* p = new vigo::control::Slider(label,action,min,max,initValue,tracking);
    if(!vigo::empty(label)) layout()->addWidget(new QLabel(label));
    AddControl(p);
    return p;
  }


  vigo::control::SpinBox* QtVigoDock_::AddSpinBox(pcstr label,vigo::DataAction<int>& action,
                                                int min,int max,int initValue)
  {
    vigo::control::SpinBox* p = new vigo::control::SpinBox(label,action,min,max,initValue);
    if(!vigo::empty(label)) layout()->addWidget(new QLabel(label));
    AddControl(p);
    return p;
  }


  void QtVigoDock_::Show(bool b)
  {
    if(b) show(); else hide();
  }


  void QtVigoDock_::AddControl(vigo::control::Control* c)
  {
    layout()->addWidget((QWidget*)c->Widget());
    adjustSize();
    m_controls.push_back(c);
  }

// eof
