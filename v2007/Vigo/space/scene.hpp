/*****************************************************************************\
*    Copyright (c) 2004--2007 by Ian G Burleigh                               *
*    https://github.com/phi53/Vigo-3D                                         *
*                                                                             *
*    This program is free software; you can redistribute it and/or modify     *
*    it under the terms of the GNU General Public License as published by     *
*    the Free Software Foundation; either version 2 of the License, or        *
*    (at your option) any later version.                                      *
*                                                                             *
*    This program is distributed in the hope that it will be useful,          *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
*    GNU General Public License for more details.                             *
*                                                                             *
*    You should have received a copy of the GNU General Public License        *
*    along with this program; if not, write to the                            *
*    Free Software Foundation, Inc.,                                          *
*    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                *
*-----------------------------------------------------------------------------*
* $Id: scene.hpp 193 2007-01-13 03:55:19Z ian_b $
\*****************************************************************************/

/** \file
 *  \brief Objects in space.
 *
 */

#ifndef VIGO_SPACE_SCENE_HPP_INCLUDED
#define VIGO_SPACE_SCENE_HPP_INCLUDED

  #include "../common/types.hpp"
  #include "../common/refcnt.hpp"
  #include "../common/splaytree.hpp"
  #include "./algebra.hpp"
  #include "../vigo/simulation.hpp"
  #include "./nodeparts.hpp"
  #include "./nodes.hpp"
  #include "./message.hpp"
  #include <vector>
  #include <list>

NAMESPACE_VIGO_()
  class Camera;
NAMESPACE_END_()

NAMESPACE_VIGO(space) //=======================================================
/** \defgroup grp_scene_hpp Scene
@{*/

  class SceneBase
  {
    NO_GEN(SceneBase)

  public:
    SceneBase(): m_numActPhases(1), m_actPhase(0), m_simulation(NULL)
    {
    }
      
    virtual ~SceneBase()
    {
    }

  private:
    /// 'Act' control
    uint m_numActPhases;  // number of phases
    uint m_actPhase;      // the current (0..m_numActPhases-1)
    
  public:
    GETSET(NumActPhases,SetNumActPhases,uint,m_numActPhases)
    GETTER(ActPhase,uint,m_actPhase)
    SETTER(SetActPhase,uint,m_actPhase)

  public:
    virtual double             CameraFar()    const = 0;
    virtual double             CameraNear()   const = 0;
    virtual algebra3d::Pointd  CameraEye()    const = 0;
    virtual algebra3d::Pointd  CameraCenter() const = 0;
    virtual algebra3d::Vectord CameraUp()     const = 0;
    virtual double             CameraFov()    const = 0;

    virtual std::string        StatusText()   const { return ""; }
  public:
    virtual bool   Act()             = 0;
    virtual double DeltaT()    const = 0;
    virtual void   SetDeltaT(double) = 0;
    virtual void   AdvanceTime()     = 0;
    virtual double SimTime()   const = 0;
    virtual void   Display(Camera&)  = 0;
    virtual void   DisplayRoot(Camera const& cam,sceneGL::eDisplay ed) = 0;

    virtual bool SelectObject(algebra3d::Lined const& /*eyeRay*/,
                              int /*kind*/, uint /*mod*/)  { return false; }
    void UpdateFrame();

  private:
    friend class vigo::Simulation;
    vigo::Simulation *m_simulation;
  };

  template<class Coord> class Scene: public SceneBase
  {
    SUPER(SceneBase)
    //NO_GEN
    Scene(Scene<Coord> const&);
    Scene<Coord> operator=(Scene<Coord> const&);

  private:
    friend class AgentBase<Coord>;
    /// a list of registered agents
    std::list<AgentBase<Coord>*>  m_regAgents;
    /// a list of scheduled agents
    std::list<AgentBase<Coord>*>  m_schAgents;
    /// agents registered for async messaging
    class assoc_msgagents: public util::splaytree<ANode*,ANode*>
    {
      int Compare(ANode* const& a1,ANode* const& a2) const
      {
        if(a1<a2) return -1;
	if(a1>a2) return +1;
	return 0;
      }
    }
    m_msgAgents;

    bool m_useSch;
    bool m_acting;
    typename std::list<AgentBase<Coord>*>::iterator m_it;

    void CopyToActs()
    {
      if(m_acting && !m_useSch)
      {
        m_schAgents.assign(m_it,m_regAgents.end());
        m_useSch = true;
      }
    }

    /// messaging
    class assoc_envelopes: public util::splaytree<MessageEnvelope*,MessageDeliveryTime*>
    {
      int Compare(MessageDeliveryTime* const& t1,MessageDeliveryTime* const& t2) const
      {
        double dt1 = t1->deliveryTime;
        double dt2 = t2->deliveryTime;
        if(dt1 < dt2) return -1;
	if(dt1 > dt2) return +1;
	return 0;
      }
    }
    m_envelopes, m_broadcasts;
 
  public:
    void Send(ANode& addressee, ANode* sender, Message& msg)
    {
      if(addressee.Receive(sender,msg))
        m_messageSent = true;
    }

    void SendAsync(ANode& address, ANode* sender, Message const& msg, double deliveryTime=0.)
    {
      m_envelopes.insert(new MessageEnvelope(deliveryTime,msg,&address,sender));
    }

    void SendBroadcast(ANode* sender, Message const& msg, double deliveryTime=0.)
    {
      m_broadcasts.insert(new MessageEnvelope(deliveryTime,msg,NULL,sender));
    }

  public:
    /// scene graph root
    typedef Node<Coord,parts::Parts<parts::P0<Coord>,parts::R0,
                                    parts::T0<Coord>,parts::S0,parts::A1> >
      root_node;
  private:
    util::refptr<root_node> m_root;

  public:
    GETTER(Root, root_node&, *m_root)

  private:
    /// simulation time step
    double m_deltaT;
    /// current simulation time
    double m_simTime;

  public:
    GETSET(DeltaT,  SetDeltaT,  double, m_deltaT)
    GETSET(SimTime, SetSimTime, double, m_simTime)

    void AdvanceTime()  { m_simTime += m_deltaT; }
    
  private:
    /// drawing attributes
    std::vector<shape::Appearance      const*> m_appearanceStack;
    std::vector<shape::ShapeColorAlpha const*> m_colorStack;
    std::vector<shape::Texture         const*> m_textureStack;

  public:
    void PushAppearance(shape::Appearance const& app)
    {
      shape::Appearance const* carry =
        (m_appearanceStack.size()>0) ? *(m_appearanceStack.end()-1) : NULL;

      if(carry) app.SetDraw(carry);
      m_appearanceStack.push_back(&app);
    }

    void PopAppearance()
    {
      m_appearanceStack.pop_back();
    }
    
    void PushColor(shape::ShapeColorAlpha const& c)
    {
      m_colorStack.push_back(&c);
    }

    void PopColor()
    {
      m_colorStack.pop_back();
    }
    
    shape::ShapeColorAlpha const* TopColor() const
    {
      return (m_colorStack.size()>0) ? *(m_colorStack.end()-1) : NULL;
    }
        
    void PushTexture(shape::Texture const& tx)
    {
      m_textureStack.push_back(&tx);
    }

    void PopTexture()
    {
      m_textureStack.pop_back();
    }
    
    shape::Texture const* TopTexture() const
    {
      return (m_textureStack.size()>0) ? *(m_textureStack.end()-1) : NULL;
    }
        
  public:
  /* Important: m_root has to be destroyed before (declared after) m_schAgents */
    Scene()
      : super(),
        m_regAgents(), m_schAgents(), m_msgAgents(), m_useSch(false), m_acting(false), m_it(),
        m_envelopes(), m_broadcasts(),
        m_root(new root_node(*this)),
        m_deltaT(1.), m_simTime(0.),
        m_appearanceStack(), //>>> move to Camera?
        m_colorStack(), m_textureStack(),
        m_selectedObject(NULL), m_lastMod(0),
	m_topspace(10.,8,30) //>>> this temp, should be user's pars, even runtime adjust
    {
      shape::Appearance *defaultAppearance = new shape::Appearance();
      m_root->SetAppearance(*defaultAppearance);
      
      defaultAppearance->SetColorAlpha(
        new shape::ShapeColorAlpha(shape::ColorAlpha(1.0,1.0,1.0,1.0)));
      defaultAppearance->SetBits(
        shape::DrawFlags::flagSOLID|shape::DrawFlags::inheritSW|
        shape::DrawFlags::flagLIGHT|shape::DrawFlags::inheritLN|
        shape::DrawFlags::inheritCLR, true);
    }
    
    virtual ~Scene()
    {
      for(MessageEnvelope* const *me = m_envelopes.min(); me; me=m_envelopes.next())
      {
        delete *me;
      }

      for(MessageEnvelope* const *me = m_broadcasts.min(); me; me=m_broadcasts.next())
      {
        delete *me;
      }
    }

  public:
    void Display(Camera& cam)
    {
      if(cam.IsBlend())
      {
        DisplayRoot(cam,sceneGL::displayOPAQUE);
        DisplayRoot(cam,sceneGL::displayTRANSPARENT);
      }
      else
      {
        DisplayRoot(cam,sceneGL::displayALL);
      }
    }
    
    void DisplayRoot(Camera const& cam,sceneGL::eDisplay ed)
    {
      m_root->Display(*const_cast<Camera*>(&cam)/*>>>fix*/,algebra3d::Vectord::_0,ed);
    }
    
    bool SelectObject(algebra3d::Lined const& eyeRay, int kind, uint mod)
    {
      AbstractNode<Coord> *picked = NULL; double magnitude = 0.0;
      m_root->SelectObject(eyeRay,picked,magnitude);
      AbstractNode<Coord> *oldSel = m_selectedObject;
      
      if(m_selectedObject!=picked)
      {
        if(m_selectedObject)
        {
          // deselect
          m_selectedObject=NULL; oldSel->Select(-2,mod);
        }

        // select new
        m_selectedObject = picked;
        if(m_selectedObject) m_selectedObject->Select(kind,(m_lastMod=mod));
        return true;
      }
      else if(m_selectedObject && (0!=kind || mod!=m_lastMod))
      {
        // still the same object, but an event has occured
        m_selectedObject->Select(kind,(m_lastMod=mod));
        return true;
      }
      return false;
    }

    GETTER(SelectedObject,AbstractNode<Coord>*,m_selectedObject)
    
    virtual double             CameraFar()    const = 0;
    virtual double             CameraNear()   const { return CameraFar()/100.;         }
    virtual algebra3d::Pointd  CameraEye()    const { return algebra3d::Pointd(0,0,1); }
    virtual algebra3d::Pointd  CameraCenter() const { return algebra3d::Pointd::_0;    }
    virtual algebra3d::Vectord CameraUp()     const { return algebra3d::Vectord::Y;    }
    virtual double             CameraFov()    const { return 20;  }
  private:
    AbstractNode<Coord> *m_selectedObject; uint m_lastMod;

    typedef AgentBase<Coord> ABase;
    
  public:
    void Register(AgentBase<Coord>& a, byte reg)
    {
      if((reg & ABase::regACT) && !IsRegisteredAct(a))
      {
        CopyToActs();
        m_regAgents.push_back(&a);
      }
      if(reg & ABase::regMSG)
      {
        m_msgAgents.insert(&a); // false if duplicate key, that's ok
      }
    }
    
    void Unregister(AgentBase<Coord>& a, byte reg)
    {
      if(reg & ABase::regACT)
      {
        // could be registered
        typename std::list<AgentBase<Coord>*>::iterator i =
          std::find(m_regAgents.begin(),m_regAgents.end(), &a);

        if(i!=m_regAgents.end())
        {
          CopyToActs();
          m_regAgents.erase(i);
          // if registered, still could be scheduled >>>make a map?
          i = std::find(m_schAgents.begin(),m_schAgents.end(), &a);
          if(i!=m_schAgents.end())
            m_schAgents.erase(i);
        }
      }

      if(reg & ABase::regMSG)
      {
        m_msgAgents.remove(&a);
      }
    }
    
    bool IsRegisteredAct(AgentBase<Coord> const& a) const
    {
      //>>> sequential: perhaps map?
      typename std::list<AgentBase<Coord>*>::const_iterator i =
        std::find(m_regAgents.begin(),m_regAgents.end(), &a);
      return i!=m_regAgents.end();
    }

    bool IsRegisteredMsg(AgentBase<Coord> const& a) const
    {
      return NULL!=m_msgAgents.find(&a);
    }

    byte Registered(AgentBase<Coord> const& a) const
    {
      byte reg = 0;
      if(IsRegisteredAct()) reg |= ABase::regACT;
      if(IsRegisteredMsg()) reg |= ABase::regMSG;
      return reg;
    }

  private:
    bool m_messageSent;
    
  public:
    virtual bool Act()
    {
      bool isDirty = false;

      m_messageSent = false; //>>> reconsider returning bool to dirty the scene
      
      if(0==ActPhase())
      {
        // broadcasts
        while(!m_broadcasts.empty())
        {
          MessageEnvelope *e = *m_broadcasts.min();
	  if(e->deliveryTime > SimTime())
	    break;

  	  for(ANode* const *a = m_msgAgents.min(); a; a=m_msgAgents.next())
            if((*a)->ReceiveBroadcast(e->sender,e->msg))
              m_messageSent = true;

	  delete e;
          m_broadcasts.remove(e);
        }
      
        // messages
        while(!m_envelopes.empty())
        {
          MessageEnvelope *e = *m_envelopes.min();
	  if(e->deliveryTime > SimTime())
	    break;
          
	  ANode *a   = e->addressee;
	  Message &m = e->msg;
	  
          if(a && m_msgAgents.find(a) && a->Receive(e->sender,m))
            m_messageSent = true;

	  delete e;
          m_envelopes.remove(e);
        }
      }
      
      m_it = m_regAgents.begin();
      typename std::list<AgentBase<Coord>*>::iterator end = m_regAgents.end();

      m_useSch = false;
      m_acting = true;

      for(;;)
      {
        AgentBase<Coord> *it;

        if(m_useSch)
        {
          if(m_schAgents.empty())
            break;

          it = *m_schAgents.begin();
          m_schAgents.pop_front();
        }
        else
        {
          if(m_it==end)
            break;

          it = *(m_it++);
        }

        if(it->Act())
          isDirty = true;
      }

      m_acting = false;

      return isDirty || m_messageSent;
    }

  public:
    subspace::TopSpace<Coord>& GetTopSubspace() { return m_topspace; }

  protected:
    subspace::TopSpace<Coord> m_topspace;
  };

  typedef Scene<double> Scened;
  typedef Scene<float>  Scenef;

//-----------------------------------------------------------------------------
// those that need Scene

  template<class Coord,class parts_Parts>
  Leaf<Coord,parts_Parts>::Leaf(Scene<Coord>& scene, bool reg)
    : super(scene,reg), parts_Parts()
  {
    SetTracked(true);
  }

  template<class Coord,class parts_Parts>
  void Leaf<Coord,parts_Parts>::Display(Camera const& cam,
                             algebra3d::Vectord const& dist,
                             sceneGL::eDisplay ed)
  {   
//>>> glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH,&i)=32; >>>do these in the constructor
//>>> glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH,&i);
    sceneGL::GLPush();
    sceneGL::GLTranslate(Pos());
    sceneGL::GLRotate(Rot());
    
    bool colorPushed      = false,
         texturePushed    = false,
         textureActive    = false,
         appearancePushed = false,
         isTransparent    = false,
         colorSet         = false;   
    
    shape::Texture    *tex = NULL;
    shape::Appearance *app = this->GetAppearance();
    if(app)
    {
      if(app->HasColorAlpha())
      {
        shape::ShapeColorAlpha &clr = *app->GetColorAlpha();
        if(app->InheritCLR())
        {
          this->m_scene.PushColor(clr); colorPushed = true;
        }
        sceneGL::GLColor(clr); isTransparent = clr.a<0.999/*>>>?*/;
        colorSet = true;
      }
      
      if(app->HasTexture())
      {
        tex = app->GetTexture();
        if(app->InheritTEX())
        {
          this->m_scene.PushTexture(*tex); texturePushed = true;
        }
        tex->Activate(); textureActive = true;
      }
      else
      {
        shape::Texture const *tex = this->m_scene.TopTexture();
        if(tex)
        {
          tex->Activate(); textureActive = true;
        }
      }
      
      this->m_scene.PushAppearance(*app); appearancePushed = true;
    }

    if(!colorSet)
    {
      shape::ShapeColorAlpha const *clr = this->m_scene.TopColor(); assert(clr);
      sceneGL::GLColor(*clr); isTransparent = clr->a<0.999/*>>>?*/;
      colorSet = true;
    }
    
    // paint
    if(cam.IsShapes())
    {
      bool doDraw = true;
      switch(ed)
      {
      case sceneGL::displayTRANSPARENT:
        doDraw = isTransparent;
        break;

      case sceneGL::displayOPAQUE:
        doDraw = !isTransparent;
        break;

      default:
        doDraw = true;  // displayALL
      }

      if(doDraw)
      {
        bool switchLight = !(1/*>>>(to camera?) m_scene.SwLights() {was: world::IsLight()}
                          && app.IsFlag(shape::DrawFlags::drawLIGHT)*/);
        if(switchLight) sceneGL::GLDisableLighting();
        DrawSelf(cam,algebra3d::Pointd(Pos()) + dist);
        if(switchLight) sceneGL::GLEnableLighting();
      }
    }

    if(textureActive)
      tex->Deactivate();

    DisplayChildren(cam,algebra3d::Pointd(Pos())+dist,ed);

    if(cam.IsLabels() && (ed & sceneGL::displayOPAQUE))
    {
      pcstr label = this->GetLabel();
      if(label)
      {
        //>>> switch lights?
        //>>> what about the label colour?
        sceneGL::GLColor(1.,1.,1.,1.);
        algebra3d::Vectord v = cam.Center()-cam.Eye(); v/=16.; //>>> hack v
        const_cast<Camera*>(&cam)->/*>>>fix*/PaintText(algebra3d::Pointd::_0-v,label);
      }
    }

    if(colorPushed)
      this->m_scene.PopColor();
      
    if(texturePushed)
      this->m_scene.PopTexture();
      
    if(appearancePushed)
      this->m_scene.PopAppearance();
    
    sceneGL::GLPop();
  }

/**@} (defgroup)*/
NAMESPACE_END() //=============================================================

#endif

//** eof **********************************************************************
