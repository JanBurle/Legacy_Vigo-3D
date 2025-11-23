// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

#ifndef VIGO_QTVIGO_WINDOW_HPP_INCLUDED
#define VIGO_QTVIGO_WINDOW_HPP_INCLUDED

  #include "../common/pimpl.hpp"
  #include <QtGui/QMainWindow>
  #include <QtGui/QProgressDialog>
  #include <QtGui/QMenu>

//-----------------------------------------------------------------------------

  namespace vigo { class Window; class Canvas; class Menu;
    namespace control { class Dock; } }

  class QtVigoWindow_ : public QMainWindow,
                        protected vigo::internal::pimpl_iface<vigo::Window>
  {
    SUPER(QMainWindow)
    NO_GEN(QtVigoWindow_)
    Q_OBJECT

  public:
    QtVigoWindow_(vigo::Window& iface, pcstr title,bool show);
   ~QtVigoWindow_();

    QSize sizeHint() const
    {
      return QSize(400,400);
    }

  private:
    void closeEvent(QCloseEvent*);

  public slots:
    void about();
    void checkUpdate();
    void fullScreen(bool);
    void showDock(bool);

  private slots:
    void cancelUpdate();

  private:
    QProgressDialog       *m_progress;

  public:
    void AddMenu   (vigo::Menu*);
    void RemoveMenu(vigo::Menu*);
    void AddMenu   (vigo::Menu& menu)   { AddMenu   (&menu); }
    void RemoveMenu(vigo::Menu& menu)   { RemoveMenu(&menu); }

    void Polish();

    void Attach(vigo::Canvas&,bool setWidget);

    void Show(bool);

    void FullScreen(bool);
    bool IsFullScreen()   const;

    void ShowDock(bool);
    bool IsDockShown()    const;

    void Move  (int x, int y);
    void Resize(int w, int h);

    int X()     const;  int Y()      const;
    int Width() const;  int Height() const;

    QtVigoWindow_* Widget()  { return this; }

  private:
    vigo::Menu  *m_cameraMenu;
    vigo::Menu  *m_simulationMenu;
    QMenu       *m_helpMenu;
    vigo::control::Dock  *m_dock;
    bool         m_showDock;
  };

//-----------------------------------------------------------------------------

#endif

// eof
