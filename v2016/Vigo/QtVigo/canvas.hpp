// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

#ifndef VIGO_QTVIGO_CANVAS_HPP_INCLUDED
#define VIGO_QTVIGO_CANVAS_HPP_INCLUDED

  #include "../vigo/canvas.hpp"
  #include "../common/pimpl.hpp"
  #include <QtCore/QObject>
  #include <QtOpenGL/QGLWidget>

//-----------------------------------------------------------------------------

  class QMouseEvent;

  class QtVigoCanvas_: public QGLWidget,
                       protected vigo::internal::pimpl_iface<vigo::Canvas>
  {
    SUPER(QGLWidget)
    NO_GEN(QtVigoCanvas_)
    Q_OBJECT

  public:
    QtVigoCanvas_(vigo::Canvas& iface);
   ~QtVigoCanvas_();

    QSize minimumSizeHint() const { return QSize(50,50);   }
    QSize sizeHint()        const { return QSize(400,400); }

    void UpdateGL(uint planes);
    bool CaptureToPng(pcstr fileName) const;

    void PaintText(double x, double y, double z, pcstr text);

    QtVigoCanvas_* Widget()       { return this;           }

  private:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

  // manipulate
  private:
    void mouseMoveEvent       (QMouseEvent*);
    void mousePressEvent      (QMouseEvent*);
    void mouseReleaseEvent    (QMouseEvent*);
    void mouseDoubleClickEvent(QMouseEvent*);

    void MouseEvent(QMouseEvent*,int kind);

  private:
    uint m_updatingPlanes;
  };

//-----------------------------------------------------------------------------

#endif

// eof
