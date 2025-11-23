// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief Vigo camera
 */

#ifndef VIGO_VIGO_CAMERA_HPP_INCLUDED
#define VIGO_VIGO_CAMERA_HPP_INCLUDED

  #include "../common/patterns.hpp"
  #include "./menu.hpp"
  #include "../space/algebra.hpp"
  #include "../space/color.hpp"
  #include <vector>

NAMESPACE_VIGO(control)
  class Dock;
NAMESPACE_END()

NAMESPACE_VIGO_() //===========================================================
/** \defgroup grp_camera_hpp Camera
@{*/

  class Canvas; class Simulation;

  class Camera
  {
    NO_GEN(Camera)
    friend class Window_; friend class Canvas;

  public:
    Camera();
    virtual ~Camera();

    /// \overload
    Camera(Canvas&,Simulation&);

  public:
    bool Attach(Canvas&);
    bool Detach(Canvas&);
    bool AttachTo(Simulation*);
    bool AttachTo(Simulation& sim)  { return AttachTo(&sim); }

    virtual void Reset();

    virtual bool Rotate   (int dx,int dy,int dz);
    virtual bool Translate(int dx,int dy);
    virtual bool Zoom     (int d);
    virtual bool Dolly    (int d);

    GETSET(Near,    SetNear,    double,                 m_near)
    GETSET(Far,     SetFar,     double,                 m_far)
    GETSET(Eye,     SetEye,     algebra3d::Pointd  const&, m_eye)
    GETSET(Center,  SetCenter,  algebra3d::Pointd  const&, m_center)
    GETSET(Up,      SetUp,      algebra3d::Vectord const&, m_up)
    GETSET(Fov,     SetFov,     double,                 m_fov)
    GETSET(ZoomF,   SetZoomF,   double,                 m_zoom)

    GETSET(ClearColor, SetClearColor, shape::Color const&, m_clearColor)

    bool Select(algebra3d::Lined const& ray, int kind, uint mod);
    void UpdateGL(uint planes);

  // painting services
  public:
    void PaintText(algebra3d::Pointd const&,pcstr);

  protected:
    virtual void PaintGL(Canvas&);
    virtual void PaintGridGL();
    virtual void PaintDone(Canvas& cvs);

    void ResetMembers();

  private:
    /// The canvas being painted, or NULL
    Canvas *m_activeCanvas;
  public:
    GETTER(ActiveCanvas, Canvas*, m_activeCanvas)

  private:
    void WindowsPolish();

  protected:
    double             m_far, m_near;
    algebra3d::Pointd  m_eye, m_center;
    algebra3d::Vectord m_up;
    double             m_fov /*deg*/;
    double             m_zoom;

    shape::Color       m_clearColor;

public:
    struct light
    {
      light(): ambient(), diffuse(), position(), directional(false)
      {
      }

      /** Light
        * @param amb Ambient
      */
      light(shape::ColorAlpha const& amb, shape::ColorAlpha const& diff,
            algebra3d::Pointd const& pos, bool dir)
        : ambient(amb), diffuse(diff), position(pos), directional(dir)
      {
      }

      shape::ColorAlpha ambient, diffuse;
      algebra3d::Pointd position;
      bool              directional;
    };

    typedef std::vector<light> lights;

  protected:
    lights m_lights;

  public:
    GETTER(Lights, lights const&, m_lights)

    void ClearLights();
    void AddLight(light const&);

  private:
    internal::has_many<Camera,Canvas> m_canvasses;

  public:
    GETTER(GetMenu,       Menu&,      *m_menu)
    GETTER(GetSimulation, Simulation*, m_sim)
    Menu* GetSimulationMenu() const;
    control::Dock* GetSimulationDock() const;

  private:
    Simulation *m_sim;
    static pvoid cb_update(pvoid);
    static pvoid cb_grab  (pvoid);
    static pvoid cb_movie (pvoid);
    static pvoid cb_reset (pvoid);

    static uint g_cameraIndex;
           uint m_cameraIndex;
	   uint m_grabIndex;
    void Grab();
    void Movie();
    void CapturePng(Canvas&);

  protected:
    Menu           *m_menu;
    bool            m_shapes; MenuAction m_actShapes;
    bool            m_labels; MenuAction m_actLabels;
    bool            m_grid;   MenuAction m_actGrid;
    bool            m_wire;   MenuAction m_actWire;
    bool            m_blend;  MenuAction m_actBlend;
    bool            m_smooth; MenuAction m_actSmooth;
                              MenuAction m_actGrab;
    bool            m_grabin; MenuAction m_actMovie;
                              MenuAction m_actReset;
  public:
    //>>> Setting camera switches is not synchronized with checkmarks in the menu
    GETSET(IsShapes, SetShapes, bool, m_shapes)
    GETSET(IsLabels, SetLabels, bool, m_labels)
    GETSET(IsWire,   SetWire,   bool, m_wire)
    GETSET(IsGrid,   SetGrid,   bool, m_grid)
    GETSET(IsBlend,  SetBlend,  bool, m_blend)
    GETSET(IsSmooth, SetSmooth, bool, m_smooth)

    GETTER(IsGrabin, bool, m_grabin)
  };

/**@} (defgroup)*/
NAMESPACE_END_() //============================================================

#endif

// eof
