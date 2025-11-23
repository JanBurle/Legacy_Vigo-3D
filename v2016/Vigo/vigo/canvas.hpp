// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief Vigo canvas
 */

#ifndef VIGO_VIGO_CANVAS_HPP_INCLUDED
#define VIGO_VIGO_CANVAS_HPP_INCLUDED

  #include "../common/pimpl.hpp"
  #include "../common/tuple.hpp"
  #include "../space/color.hpp"
  #include "../common/ogl.hpp"

class QtVigoCanvas_;

NAMESPACE_VIGO(control)
  class Dock;
NAMESPACE_END()

NAMESPACE_VIGO_() //===========================================================
/** \defgroup grp_canvas_hpp Canvas
@{*/

  class Window; class Camera; class Simulation; class Menu;

  /// Provides OpenGL drawing space.
  class Canvas: protected internal::pimpl<QtVigoCanvas_>
  {
    NO_GEN(Canvas)

  public:
    /// Creates a canvas, attached to a window.
    Canvas(Window&,bool setWidget=true/*hook for 7-QtGui*/);

   ~Canvas();

    bool    AttachTo(Camera*);
    /// \overload
    bool    AttachTo(Camera& cam) { return AttachTo(&cam); }

    /// The current camera, or \c NULL.
    Camera* GetCamera();
    /// The current camera menu, or \c NULL.
    Menu*   GetCameraMenu();

    /// The current simulation, or \c NULL.
    Simulation* GetSimulation();
    /// The current simulation menu, or \c NULL.
    Menu*   GetSimulationMenu();
    /// The current simulation control dock, or \c NULL.
    control::Dock* GetSimulationDock();

    /// \name Drawing functions (callbacks).
    //@{
    /// Initializes the canvas.
    virtual void InitializeGL();
    /// Clears the canvas to the current camera clear colour.
    virtual void ClearGL();
    /// Transforms the view based on the current camera position and settings.
    virtual void TransformGL();
    /// Paints a picture as seen by the current camera.
    virtual void PaintGL();
    /// Sets the OpenGL viewport.
    virtual void ResizeGL(int w, int h);
    /// Draws the current simulation status text.
    virtual void DrawStatusGL();
    //@}

    /// Refreshes the drawing.
    void UpdateGL(uint planes);

    /// \name Mouse (or other ponting device) operations
    //@{
  public:
    enum {modCTRL=0x01, modSHIFT=0x02, modALT=0x04, modMETA=0x08};
    void Select(int x, int y, int kind, uint mod);
    virtual bool CameraOp(int x, int y, uint mod);
    bool CameraRotate   (int dx,int dy,int dz);
    bool CameraTranslate(int dx,int dy);
    bool CameraZoom     (int d);
    bool CameraDolly    (int d);
  private:
    util::int2d m_mouseLastPos; bool m_isCameraOp;
    //@}

  public:
    /// \name Screen capture
    //@{
    bool CaptureToPng(pcstr pngFile) const;
    //@}

    /// \name 3D text
    //@{
    void PaintText(double x, double y, double z, pcstr text);
    //@}

    /// \name Overlay
    //@{
    void SetColor(shape::ColorAlpha const& c);
    void PaintRect(int x1,int y1,int x2,int y2);
    void PaintBox(int x1,int y1,int x2,int y2);
    uint CharWidth();
    uint CharHeight();
    void SetTextPos(uint x, uint y);
    void PaintText(char);
    void PaintText(pcstr);
    void PaintText(std::string const& s) { PaintText(s.c_str()); }
    //@}
    //
    void WindowPolish();

    pvoid Widget();

  protected:
    int m_w, m_h;

  private:
    Window *m_wnd;
    Camera *m_cam;
    // for unproject
    double m_modelMatrix[16], m_projMatrix[16];
    // glu
    GLUquadricObj* m_quadricObj;
  public:
    GLUquadricObj* QuadricObj();
  };

/**@} (defgroup)*/
NAMESPACE_END_() //============================================================

#endif

// eof
