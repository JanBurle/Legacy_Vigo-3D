// Adapted from Trolltech hellogl example

#include "qtwindow.hpp"

#include <QtVigo/window.hpp>
#include <QtVigo/canvas.hpp>
#include "slots.hpp"

#include <QtGui/QWidget>
#include <QtGui/QSlider>
#include <QtGui/QHBoxLayout>

//-----------------------------------------------------------------------------

  void QtGuiWindow::CreateLayout()
  {
    QWidget *centralWidget = new QWidget; // this is the contents of the window

    // carefully!
    reinterpret_cast<QtVigoWindow_*>(this->Widget())->setCentralWidget(centralWidget);
    assert(m_canvas);
    QWidget *cvsWidget = static_cast<QWidget*>(m_canvas->Widget());
    assert(cvsWidget);

    // now build the layout
    QHBoxLayout *mainLayout = new QHBoxLayout;
    centralWidget->setLayout(mainLayout);

    xSlider = createSlider();
    ySlider = createSlider();
    zSlider = createSlider();

    mainLayout->addWidget(cvsWidget);
    mainLayout->addWidget(xSlider);
    mainLayout->addWidget(ySlider);
    mainLayout->addWidget(zSlider);

    xSlider->setValue(15 * 16);
    ySlider->setValue(345 * 16);
    zSlider->setValue(0 * 16);
  }

  void QtGuiWindow::Connect(vigo::Canvas& cvs,vigo::Camera& cam,Simulation& sim)
  {
    Slots *s = new Slots(cvs,cam,sim);
    QObject::connect(xSlider, SIGNAL(valueChanged(int)), s, SLOT(setX(int)));
    // etc... connect signals from interface elements
  }

  QSlider *QtGuiWindow::createSlider()
  {
      QSlider *slider = new QSlider(Qt::Vertical);
      slider->setRange(0, 360 * 16);
      slider->setSingleStep(16);
      slider->setPageStep(15 * 16);
      slider->setTickInterval(15 * 16);
      slider->setTickPosition(QSlider::TicksRight);
      return slider;
  }

// eof
