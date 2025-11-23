// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief Space nodes
 *
 */

#ifndef VIGO_SPACE_NODES_HPP_INCLUDED
#define VIGO_SPACE_NODES_HPP_INCLUDED

  #include "./nodeparts.hpp"
  #include "./shape.hpp"
  #include "./subspace.hpp"
  #include "./message.hpp"

  #include <list>

NAMESPACE_VIGO(space) //=======================================================
/** \defgroup grp_nodes_hpp Nodes
@{*/

  #define NOT_ALLOWED__(name)   THROW_EXC2(EXC_RUNTIME,#name " not allowed")

  /// Graphics support.
  namespace sceneGL //>>>rename?
  {
    void GLPush();
    void GLPop();

    void GLTranslate(algebra3d::Pointd const& pos);
    void GLTranslate(algebra3d::Pointf const& pos);
    void GLTranslate(algebra3d::Pointi const& pos);
    void GLRotate(algebra3d::Rotation const& rot);

    /// display phases
    enum eDisplay { displayOPAQUE       = 0x01,
                    displayTRANSPARENT  = 0x02,
                    displayALL          = displayOPAQUE | displayTRANSPARENT };

    void GLEnableLighting();
    void GLDisableLighting();

    void GLColor(double,double,double,double);
    void GLColor(shape::ColorAlpha const&);
  }

//-----------------------------------------------------------------------------

  template<class Coord> class Scene;

  /// The base class of all nodes. Nodes must be heap-based.
  class ANode: public util::refcnt_heap
  {
    SUPER(util::refcnt_heap)

  public:
    /// (Mouse) selection callback.
    /// Return true if the scene have changed as a result.
    enum {selMOVEOUT=-2,selUNCLICK=-1,selMOVE=0,selCLICK=1, selDBLCLICK=2};
    virtual bool Select(int /*kind*/, uint /*mod*/) { return false; }

    /// Send a synchronous message. The addressee must exist! With a possible reply within Message.
    virtual void Send(ANode const& /*addressee*/, Message&)
      { NOT_ALLOWED__(Send) /*reimplemented later*/}
    /// Send a synchronous message. The reply is discarded. Allows multiple use of (const) Message.
    virtual void Send(ANode const& /*addressee*/, Message const&)
      { NOT_ALLOWED__(Send) /*reimplemented later*/}
    /// Send an asynchronous message. Fails if the addressee is not registered for messages. No reply.
    virtual void SendAsync(ANode const& /*addressee*/, Message const&,double /*deliveryTime*/=0.)
      { NOT_ALLOWED__(SendAsync) /*reimplemented later*/}
    /// Send a broadcast to those registered for messages. No reply. Costly.
    virtual void SendBroadcast(Message const&,double /*deliveryTime*/=0.)
      { NOT_ALLOWED__(SendBroadcast) /*reimplemented later*/}

    /// Receiving a message. Return true if the scene has changed as a result,
    /// modify the message in reply.
    virtual bool Receive(ANode const* /*sender*/,Message&)
      { return false; }

    /// Receive a broadcast (async) message. Return true if the scene has changed.
    virtual bool ReceiveBroadcast(ANode const* /*sender*/, Message const&)
      { return false; }
  };

//-----------------------------------------------------------------------------

  /// The base class of all agents. Agents can be registered for act/message callbacks..
  template<class Coord>
  class AgentBase: public ANode
  {
    SUPER(ANode)
    AgentBase<Coord> operator=(AgentBase<Coord> const&); // NO_ASSIGN

  public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    #define EXC_IDS
    DECLARE_EXC
    #undef EXC_IDS
#endif

    /** Create as a part of a scene.
     * @param reg Register as an agent for callbacks.
     */
    enum {regACT = 0x01, regMSG = 0x02};
    AgentBase(Scene<Coord>& scene, byte reg)
      : super(), m_scene(scene)
    {
      if(reg) Register(reg);
    }

    AgentBase(AgentBase const& r)
      : super(r), m_scene(r.m_scene)
    {
      Register(r.Registered());
    }

  protected:
    /// Should be protected.
   ~AgentBase()
    {
      Unregister();
    }

  public:
    /// Register as an agent.
    void Register(byte reg)
    {
      m_scene.Register(*this,reg);
    }

    /// Unregister as an agent.
    void Unregister(byte reg = 0xFF)
    {
      m_scene.Unregister(*this,reg);
    }

    bool IsRegisteredAct() const
    {
      return m_scene.IsRegisteredAct(*this);
    }

    bool IsRegisteredMsg() const
    {
      return m_scene.IsRegisteredMsg(*this);
    }

    byte Registered() const
    {
      return m_scene.Registered(*this);
    }

  public:
    /// What phase is the current one: 0 or more
    uint ActPhase() const
    {
      return m_scene.ActPhase();
    }

    GETTER(GetScene,    Scene<Coord>&, m_scene)
    GETTER(Root, AbstractNode<Coord>&, m_scene.Root())

    GETTER(SimTime,double,m_scene.SimTime())

    void UpdateFrame() { m_scene.UpdateFrame(); } // causes redraw

  protected:
    friend class Scene<Coord>;
    Scene<Coord> &m_scene;

  public: // reimplemented messaging
    void Send(ANode& address, Message& msg)
      { m_scene.Send(address,this,msg);      }
    void Send(ANode& address, Message const& msg)
      { Message &m = msg.Copy(); m_scene.Send(address,this,m); delete &m; }
    void SendAsync(ANode& address, Message const& msg,double deliveryTime=0.)
      { m_scene.SendAsync(address,this,msg,deliveryTime); }
    void SendBroadcast(Message const& msg,double deliveryTime=0.)
      { m_scene.SendBroadcast(this,msg,deliveryTime);     }

  public: //  misc
    GETTER(IsSelected, bool, this==m_scene.SelectedObject())

  protected:
    /** An elementary action.
     * @return True if has acted (moved, etc.) -- causes frame update.
     */
    virtual bool Act()  { return false; }
  };


//----------------------------------------------------------------------------

  /// The base class of scene nodes.
  template<typename Coord>
  class AbstractNode: public AgentBase<Coord>
  {
    SUPER(AgentBase<Coord>)
    AbstractNode<Coord> operator=(AbstractNode<Coord> const&); // NO_ASSIGN

    friend class subspace::TopSpace<Coord>;
    friend class subspace::SubspaceNode<Coord>;

  public:
    AbstractNode(Scene<Coord>& scene, bool reg)
      : super(scene,reg)
    {
    }

    AbstractNode(AbstractNode const& n)
      : super(n)
    {
    }

  protected:
   ~AbstractNode()
    {
    }

  public:
    /// Add a subnode. Optionally adjust the subnode's position and rotation.
    virtual void Add         (AbstractNode&,bool /*adjust*/=false) = 0;
    /// Remove a subnode. Optionally adjust the subnode's position and rotation.
    virtual void Remove      (AbstractNode&,bool /*adjust*/=false) = 0;

    /// Add self as a subnode. Optionally adjust the position and rotation.
    virtual void AddTo       (AbstractNode&,bool /*adjust*/=false) = 0;
    /// Remove self as a subnode. Optionally adjust the position and rotation.
    virtual void Remove      (bool /*adjust*/=false)               = 0;

    /// Set the pointer to a parent.
    virtual void SetParentPtr(AbstractNode*) = 0;
    /// Is this node a (grand-...)child of a node?
    virtual bool IsAncestorOf(AbstractNode&) = 0;

    /** Render this node using a camera.
     * @param dist This node's distance from the camera, passed for OpenGL optimization.
     * @param cam  The camera.
     */
    virtual void Display(Camera const& cam, algebra3d::Vectord const& dist,
                         sceneGL::eDisplay) = 0;

  private:
    friend class subspace::SubspaceLeaf<Coord>;
    // Subspace support
    virtual void SetNext(AbstractNode*)        = 0;
    virtual void SetPrev(AbstractNode*)        = 0;
    virtual void SetSubspace(subspace::SubspaceLeaf<Coord>*) = 0;

    virtual AbstractNode* GetNext()        = 0;
    virtual AbstractNode* GetPrev()        = 0;
    virtual subspace::SubspaceLeaf<Coord>* GetSubspace() = 0;

  public:
    virtual bool HasPP() const = 0;
    virtual bool HasPV() const = 0;
    virtual bool HasPA() const = 0;
    virtual bool HasPS() const = 0;
    virtual bool HasRR() const = 0;
    virtual bool HasRS() const = 0;
    virtual bool HasRA() const = 0;

    /// This node's position, relative to the parent (if there is any).
    virtual algebra3d::Point<Coord> Pos()       const   = 0;
    /// This node's position, absolute coordinates.
    virtual algebra3d::Point<Coord> AbsPos()    const   = 0;
    /// Set this node's position, parent-relative.
    virtual void SetPos(algebra3d::Point<Coord> const&) = 0;

    /// This node's rotation.
    virtual algebra3d::Rotation Rot()       const  = 0;
    /// This node's rotation, absolute coordinates.
    virtual algebra3d::Rotation AbsRot()    const   = 0;
    /// Set this node's rotation.
    virtual void SetRot(algebra3d::Rotation const&) = 0;

    /// This node's group id, for tracking.
    AGETTER(GrpId,    space::tracked_grpid)
    ASETTER(SetGrpId, space::tracked_grpid)

  public:
    virtual void SelectObject(algebra3d::Lined const& eyeRay,
                              AbstractNode*& picked,double& mag) = 0;
  protected:
    virtual shape::Shape*      GetShape()      const { return NULL; }
    virtual shape::Appearance* GetAppearance() const { return NULL; }
    virtual pcstr              GetLabel()      const { return NULL; }
  };


//----------------------------------------------------------------------------

  template<class Coord,class parts_Parts> class Node;

  /// A node with no children
  template<typename Coord,class parts_Parts>
  class Leaf: public AbstractNode<Coord>, public parts_Parts
  {
    SUPER(AbstractNode<Coord>)

  public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    #define EXC_IDS
    DECLARE_EXC
    #undef EXC_IDS
#endif

    Leaf(Scene<Coord>& scene, bool reg=false);

    Leaf(Leaf const& l)
      : super(l), parts_Parts(l)
    {
      SetTracked(true);
    }

   ~Leaf()
    {
      SetTracked(false);
    }

    /** Switch tracking on or off. Tracked nodes can be detected by Nbr().
        Tracking is on initially. */
    void SetTracked(bool on)
    {
      if(on)
      {
        if(!IsTracked())
          parts_Parts::EnterSubspace(this->m_scene.GetTopSubspace(),*this);
      }
      else
      {
        if(IsTracked())
          parts_Parts::LeaveSubspace(*this);
      }
    }

    /// Is this node being tracked?
    bool IsTracked() const
    {
      return parts_Parts::IsInSubspace();
    }

    /// Neighbours.
    void Nbr(double radius, tracked_collection<Coord>& result,
             uint maxNum=0/*all*/, bool sort=false)
    {
      parts_Parts::Nbr(*this,NULL,radius,result,maxNum,sort);
    }

    /// Neighbours.
    void Nbr(tracked_grpid grpid, double radius,
             tracked_collection<Coord>& result, uint maxNum=0/*all*/,
             bool sort=false)
    {
      parts_Parts::Nbr(*this,&grpid,radius,result,maxNum,sort);
    }

    GETTER(GrpId, space::tracked_grpid, parts_Parts::GrpId(reinterpret_cast<space::tracked_grpid>(&typeid(*this))))
    void SetGrpId(space::tracked_grpid id)
    {
      parts_Parts::SetGrpId(id);
    }

    bool HasPP() const { return parts_Parts::HasPP(); }
    bool HasPV() const { return parts_Parts::HasPV(); }
    bool HasPA() const { return parts_Parts::HasPA(); }
    bool HasPS() const { return parts_Parts::HasPS(); }
    bool HasRR() const { return parts_Parts::HasRR(); }
    bool HasRS() const { return parts_Parts::HasRS(); }
    bool HasRA() const { return parts_Parts::HasRA(); }

    algebra3d::Point<Coord> Pos()     const { return parts_Parts::Pos();  }
    algebra3d::Point<Coord> AbsPos()  const { NOT_ALLOWED__(AbsPos);
                                              return Pos(); }
    void SetPos(algebra3d::Point<Coord> const& pos)
    {
       parts_Parts::SetPos(pos);
       parts_Parts::UpdateSubspace(*this);
    }

    algebra3d::Rotation Rot()       const { return parts_Parts::Rot();  }
    algebra3d::Rotation AbsRot()    const { NOT_ALLOWED__(AbsRot);
	                                          return Rot(); }
    void SetRot(algebra3d::Rotation const& rot)
    {
      parts_Parts::SetRot(rot);
    }

    /// Calculate this would-be new position.
    algebra3d::Point<Coord> NewPos(double deltaT)
    {
      return parts_Parts::Pos() + parts_Parts::Vel() * deltaT;
    }

    /// Calculate the would-be new rotation.
    algebra3d::Rotation NewRot(double deltaT)
    {
      return parts_Parts::Rot() + parts_Parts::Spin()*deltaT;
    }

    /// A node that has velocity or spin, and optionally acceleration, can move on its own.
    bool Move()
    {
      bool moved = false;

      double const deltaT = this->GetScene().DeltaT();

      if(parts_Parts::HasPV())
      {
        SetPos(NewPos(deltaT)); parts_Parts::AdjVel(deltaT); moved = true;
      }

      if(parts_Parts::HasRS())
      {
        SetRot(NewRot(deltaT)); parts_Parts::AdjSpin(deltaT); moved = true;
      }

      return moved;
    }

    void Add(AbstractNode<Coord>&,bool /*adjust*/=false)
      { NOT_ALLOWED__(Add)     }
    void Remove(AbstractNode<Coord>&,bool /*adjust*/=false)
      { NOT_ALLOWED__(Remove)  }
    void MoveBy(algebra3d::Vector<Coord> const& d)
      { SetPos(Pos()+d);       }

    void AddTo(AbstractNode<Coord>& parent,bool adjust=false)
      { parent.Add(*this,adjust);     }
    void Remove(bool /*adjust*/=false)
      { NOT_ALLOWED__(Remove)  }

    void SetParentPtr(AbstractNode<Coord>*) { /*ignore*/    }
    bool IsAncestorOf(AbstractNode<Coord>&) { NOT_ALLOWED__(IsAncestorOf)
                                              return false; }
    void Display(Camera const& cam,algebra3d::Vectord const& dist,sceneGL::eDisplay);

  protected: // hook
    virtual void DrawSelf(Camera const& cam,algebra3d::Pointd const& pos)
    {
      shape::Shape *sh = this->GetShape();
      if(sh) sh->Draw(cam,pos);
    }

  public: // hook
    virtual void SelectObject(algebra3d::Lined const& eyeRay,
                      AbstractNode<Coord>*& picked,double& mag)
    {
      shape::Shape *sh = this->GetShape();
      // only shaped ones
      if(sh)
      {
        // translate to the origin, test hit, remember the closest object
        algebra3d::Pointd pos(Pos());
        if(sh->IsHit(eyeRay-algebra3d::Vectord(pos)))
        {
          double m = (pos-eyeRay.a).Magnitude();
          if((!picked) || (m<mag))
          {
            picked=this; mag = m;
          }
        }
      }
    }

  protected:
    shape::Shape*      GetShape()      const { return parts_Parts::ShapePart();      }
    shape::Appearance* GetAppearance() const { return parts_Parts::AppearancePart(); }
    pcstr              GetLabel()      const { return parts_Parts::LabelPart();      }

  protected:
    virtual void DisplayChildren(Camera const&,algebra3d::Vectord const&,
                                 sceneGL::eDisplay)  { }
    virtual bool Act()
    {
      return Move();
    }

  protected:
    // subspaces
    void SetNext(AbstractNode<Coord>* n) { parts_Parts::SetNext(n);     }
    void SetPrev(AbstractNode<Coord>* n) { parts_Parts::SetPrev(n);     }
    void SetSubspace(subspace::SubspaceLeaf<Coord>* s) { parts_Parts::SetSubspace(s); }

    AbstractNode<Coord>* GetNext()       { return parts_Parts::GetNext(); }
    AbstractNode<Coord>* GetPrev()       { return parts_Parts::GetPrev(); }
    subspace::SubspaceLeaf<Coord>* GetSubspace()   { return parts_Parts::GetSubspace(); }
  };


//-----------------------------------------------------------------------------

  /// A leaf node that is aware of its parent.
  template<typename Coord,class parts_Parts>
  class ParentedLeaf: public Leaf<Coord,parts_Parts>
  {
    typedef Leaf<Coord,parts_Parts> super;
    ParentedLeaf<Coord,parts_Parts> operator=(ParentedLeaf<Coord,parts_Parts> const&); // NO_ASSIGN

  public:
    ParentedLeaf(Scene<Coord>& scene, bool reg=false)
      : super(scene,reg), m_parent(NULL)
    {
    }

    ParentedLeaf(ParentedLeaf const& l)
      : super(l), m_parent(NULL)
    {
    }

    virtual ~ParentedLeaf() {}

    algebra3d::Point<Coord> AbsPos() const
    {
      return m_parent ? m_parent->AbsPos() + this->Pos() : this->Pos();
    }

    algebra3d::Rotation AbsRot() const
    {
      return m_parent ? m_parent->AbsRot() + this->Rot() : this->Rot();
    }

    void AddTo(AbstractNode<Coord>& parent,bool adjustPosRot=false)
    {
      Remove(adjustPosRot);
      parent.Add(*this,adjustPosRot);
    }

    void Remove(bool adjustPosRot=false)
    {
      if(m_parent) m_parent->Remove(*this,adjustPosRot);
    }

    void SetParentPtr(AbstractNode<Coord>*p)
    {
      m_parent = p;
    }

    bool IsAncestorOf(AbstractNode<Coord>&n)
    {
      return m_parent && (m_parent==&n || m_parent->IsAncestorOf(n));
    }

  protected:
    AbstractNode<Coord> *m_parent;
  };


//-----------------------------------------------------------------------------

  /// A general scene node.
  template<typename Coord,class parts_Parts>
  class Node: public ParentedLeaf<Coord,parts_Parts>
  {
    typedef ParentedLeaf<Coord,parts_Parts> super;
    Node& operator=(Node const&); //NO_ASSIGN

  public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    #define EXC_IDS WRONG_SCENE,NOT_CHILD
    DECLARE_EXC
    #undef EXC_IDS
#endif

  public:
    Node(Scene<Coord>& scene, bool reg=false)
      : super(scene,reg), m_children()
    {
    }

    Node(Node const& n)
      : super(n), m_children()
    {
    }

    virtual ~Node() {}

    void Add   (AbstractNode<Coord>&,bool adjustPosRot=false);
    void Remove(AbstractNode<Coord>&,bool adjustPosRot=false);

    void Remove(bool adjustPosRot=false) { super::Remove(adjustPosRot); }

  protected:
    void DisplayChildren(Camera const&,algebra3d::Vectord const&,
                         sceneGL::eDisplay);
  public:
    void SelectObject(algebra3d::Lined const& eyeRay,
                      AbstractNode<Coord>*& picked,double& mag)
    {
      super::SelectObject(eyeRay,picked,mag);
      for(node_iterator c = m_children.begin(); c != m_children.end(); ++c)
        (*c)->SelectObject(eyeRay,picked,mag);
    }

  public:
    typedef typename std::list<util::refptr<AbstractNode<Coord> > > node_list;
    typedef typename node_list::iterator node_iterator;
    typedef typename node_list::const_iterator node_const_iterator;
    GETTER(Nodes,node_list const&,m_children)

  protected:
    bool FindChild(AbstractNode<Coord>& child,node_iterator* it=NULL);
    node_list m_children;
  };


  template<class Coord,class parts_Parts>
  void Node<Coord,parts_Parts>::Add(AbstractNode<Coord>& child,bool adjustPosRot)
  {
    m_children.push_back(util::refptr<AbstractNode<Coord> >(&child));
    child.SetParentPtr(this); //what about multiple parents?
    if(adjustPosRot)
    {
      //>>>what about multiple parents?
      if(child.HasPP()) child.SetPos(child.Pos() - this->Pos());
      if(child.HasRR()) child.SetRot(child.Rot() - this->Rot());
    }
  }


  template<class Coord,class parts_Parts>
  void Node<Coord,parts_Parts>::Remove(AbstractNode<Coord>& child,bool adjustPosRot)
  {
    RUNTIME_CHECK_EXC3(&(this->GetScene()) == &child.GetScene(),
                       WRONG_SCENE,"scene mismatch")
    node_iterator it;
    RUNTIME_CHECK_EXC3(FindChild(child,&it), NOT_CHILD,"not a child");
    child.SetParentPtr(NULL);
    if(adjustPosRot)
    {
      //>>>what about multiple parents?
      if(child.HasPP()) child.SetPos(child.Pos() + this->Pos());
      if(child.HasRR()) child.SetRot(child.Rot() + this->Rot());
    }
    m_children.erase(it); // must follow child operations -- the child may disappear!
  }


  template<class Coord,class parts_Parts>
  void Node<Coord,parts_Parts>::DisplayChildren(Camera const& cam,
      algebra3d::Vectord const& dist,sceneGL::eDisplay ed)
  {
    for(node_iterator c = m_children.begin(); c != m_children.end(); ++c)
      (*c)->Display(cam,dist,ed);
  }


  template<class Coord,class parts_Parts>
  bool Node<Coord,parts_Parts>::FindChild(AbstractNode<Coord>& child,node_iterator* it)
  {
    node_iterator i;
    for(i = m_children.begin(); i != m_children.end(); ++i)
    {
      if(*i == &child)
      {
        if(it) *it = i;
        return true;
      }
    }

    return false;
  }

  #undef NOT_ALLOWED__

/**@} (defgroup)*/
NAMESPACE_END() //=============================================================

#endif

// eof
