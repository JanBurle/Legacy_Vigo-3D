// Adapted from Trolltech hellogl example

#ifndef QT_GUI_SLOTS_HPP_INCLUDED
#define QT_GUI_SLOTS_HPP_INCLUDED

#include <vigoapp>
#include "sim.hpp"
#include <QtCore/QObject>

//-----------------------------------------------------------------------------

  /// Holder of the slots
  class Slots : public QObject
  {
    Q_OBJECT

  public:
    Slots(vigo::Canvas& cvs,vigo::Camera& cam,Simulation& sim)
      : m_cvs(cvs), m_cam(cam), m_sim(sim)
    {
    }

  private:
    // example only: hold references to various Vigo objects
    vigo::Canvas& m_cvs;
    vigo::Camera& m_cam;
    Simulation&   m_sim;

  public slots:
    void setX(int x);
    void setY(int y);
    void setZ(int z);
  };


#endif

//** eof **********************************************************************
