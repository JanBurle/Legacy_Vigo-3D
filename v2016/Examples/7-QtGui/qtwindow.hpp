// Adapted from Trolltech hellogl example

#ifndef QT_GUI_WINDOW_HPP_INCLUDED
#define QT_GUI_WINDOW_HPP_INCLUDED

#include <vigoapp>
#include "sim.hpp"

class QSlider;

//----------------------------------------------------------------------------

  class QtGuiWindow : public vigo::Window
  {
    SUPER(vigo::Window)

  public:
    QtGuiWindow(pcstr title): super(title) {}
    void CreateLayout();

    // connect signals and slots
    void Connect(vigo::Canvas&,vigo::Camera&,Simulation&);

  private:
    QSlider  *createSlider();

    QSlider  *xSlider;
    QSlider  *ySlider;
    QSlider  *zSlider;
  };

#endif

// eof
