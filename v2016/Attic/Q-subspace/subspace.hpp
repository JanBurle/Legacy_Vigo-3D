// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *
 *  The Space is uniformly divided at the top level to a large number of boxes.
 *  Only the top-level boxes that contain agents exist physically, and are kept
 *  in an associative array (a splay tree or a map, etc.). If a number of agents
 *  in a single box (SubspaceLeaf) reaches a treshold, the terminal (leaf) box
 *  is replaced by a nonterminal (node, SubspaceNode) box that is subdivided
 *  into N^3 (8, 27, ...) subboxes, and so on, up to a maximum tree depth.
 *  The maximum depth limit is necessary to deal with a very large, albeit
 *  exceptional, temporary concentration of agents in one  place.
 *
 *  There is a discrete subdivision grid of a specified step. That determines
 *  The size of a smallest possible box. The top-level box size, in steps,
 *  is then determined by the subdivision order and maximum tree depth.
 *
 *  The space is subdivided in order to control the computational cost of the
 *  neighbourhood (Nbr) function.
 */

#ifndef Q_SUBSPACE_HPP_INCLUDED
#define Q_SUBSPACE_HPP_INCLUDED

  #include <vigocmn>
  #include <vigosim>
  using namespace vigo;

//-----------------------------------------------------------------------------

  /// the coordinate type, typically double or float
  typedef double coord;

  /// a point in space
  typedef algebra3d::Point<coord>  Point;
  /// a space vector
  typedef algebra3d::Vector<coord> Vector;

//-----------------------------------------------------------------------------

  class Space;
  class SubspaceLeaf;

  /// The Agent class. Subclass Agent to create your own specialized agents.
  class Agent
  {
    friend class SubspaceLeaf;

  public:
    #define EXC_IDS IN_SPACE, NOT_IN_SPACE
    DECLARE_EXC_ROOT
    #undef EXC_IDS

    Agent();                            ///< created at the position <0,0,0>
    Agent(Point const& pos);            ///< at the specified position
    Agent(Point const& pos,Space&);     ///< at the specified pos, immediately enter the space
    virtual ~Agent();

    void EnterSpace(Space&);            ///< add the agent to a space
    void LeaveSpace();                  ///< remove the agent from its space
    GETTER(IsInSpace, bool, NULL!=m_subspace) ///< is the agent currently in a space?

    GETTER(Pos, Point, m_pos)           ///< The agent's position.
    void SetPos(Point const&);          ///< Change the agent's position.

    typedef std::vector<Agent*> collection;
    /// collect neighbours within the radius, adds to coll (do empty before use!)
    void Nbr(coord radius,collection& coll);

  private:
    SubspaceLeaf *m_subspace;           ///< NULL, or this agent's subspace
    Agent        *m_prev, *m_next;      ///< the list of agents in the same subspace
    Point         m_pos;                ///< this agent's position
  };

//-----------------------------------------------------------------------------

  class ASubspace;

  class SpaceIdx
  {
  public:
    typedef util::int3d idx_t;             ///< 3D index on the space grid
    SpaceIdx(idx_t idx): m_idx(idx) {}
    virtual ~SpaceIdx() {}
    GETTER(Idx,idx_t const&,m_idx)

  protected:
    idx_t m_idx;
  };

  /// The abstract space or subspace.
  class ASpace: public SpaceIdx
  {
    SUPER(SpaceIdx)

    friend class Space;
    friend class ASubspace;
    friend class SubspaceNode;
    friend class SubspaceLeaf;

  public:
    //>>> some of these constant could be run-time parameters
    static uint const SUB_ORDER      = 2;  ///< subdivision order (2 or 3 is recommended)

  public:
    ASpace(idx_t const&);
    virtual ~ASpace() = 0;

  protected:
    /// Add an agent to the (sub)space,
    virtual void Add(Agent&)                                              = 0;
    /// Collect the agent's neighbours, in this subspace, or above (climbs up).
    virtual void Nbr(Agent const&,coord radius,Agent::collection&)  const = 0;
    /// Collect the agent's neighbours, in this subspace only (descends down).
    virtual void Nbr_(Agent const&,coord radius,Agent::collection&) const = 0;

    /// Remove an empty (i.e., with no agents) subspace.
    virtual void Remove(ASubspace&) = 0;
  };


//-----------------------------------------------------------------------------

  class SubspaceNode;

  /** An abstract cubic space partition, a leaf or a node; indexed by its leftmost
    * grid coordinates */
  class ASubspace: public ASpace
  {
    NO_GEN(ASubspace)
    SUPER(ASpace)

  public:
    /** @param size   the size in grid steps
      * @param parent the parent subspace or space
      * @param space  always the top-level space
      * @param level  the nesting level, starting with 0
      */
    ASubspace(ASpace::idx_t const&,uint size,
              ASpace& parent,Space& space,uint level);
   ~ASubspace();

  public:
    GETTER(Level, uint, m_level)

    /// Does the grid index fall within this subspace?
    bool Contains(idx_t const&) const;
    /// Does the vision sphere intersect this subspace?
    bool Intersects(Point const&,coord radius) const;

    void Nbr(Agent const&,coord radius,Agent::collection&) const;

  protected:
    /// Splits a subspace, if needed and possible.
    static ASubspace* QSplit(ASubspace*);

  protected:
    uint    m_size;
    ASpace& m_parent;
    Space&  m_space;
    uint    m_level;
  };


//-----------------------------------------------------------------------------

  /** The intermediate space partition */
  class SubspaceNode: public ASubspace
  {
    NO_GEN(SubspaceNode)
    SUPER(ASubspace)

    friend class ASubspace;
    friend class SubspaceLeaf;

  public:
    SubspaceNode(idx_t const&,uint size,
                 ASpace& parent,Space& space,uint level);
   ~SubspaceNode();

  private:
    void Add(Agent&);
    void Remove(ASubspace&);

    void Nbr_(Agent const&,coord radius,Agent::collection&) const;

  private:
    enum {NUM_SSPACES = SUB_ORDER*SUB_ORDER*SUB_ORDER};
    ASubspace* m_subspaces[SUB_ORDER][SUB_ORDER][SUB_ORDER];
    ushort     m_numSubspaces;
  };


//-----------------------------------------------------------------------------

  /** The terminal space partition */
  class SubspaceLeaf: public ASubspace
  {
    NO_GEN(SubspaceLeaf)
    SUPER(ASubspace)

    friend class Space;
    friend class Agent;
    friend class ASubspace;
    friend class SubspaceNode;

  public:
    SubspaceLeaf(idx_t const&,uint size,
                 ASpace& parent,Space& space,uint level);
   ~SubspaceLeaf();

    GETTER(NumAgents, uint, m_numAgents)

  private:
    void Add(Agent&);
    void Remove(Agent&);  ///< may trigger collapse
    void Remove_(Agent&); ///< does not trigger space collapse

    void Remove(ASubspace&) { NOT_HERE() } ///< Leafs do not have subspaces

    /// Move the agent to the new position, pass it to another subspace, if needed.
    void Move(Agent&,Point const& newPos);

    void Nbr_(Agent const&,coord radius,Agent::collection&) const;

  private:
    Agent *m_agents;
    uint   m_numAgents;
  };


//-----------------------------------------------------------------------------

  /// The top-level space, not limited in span.
  class Space: public ASpace
  {
    NO_GEN(Space)
    SUPER(ASpace)

    friend class ASubspace;
    friend class SubspaceNode;
    friend class SubspaceLeaf;

  public:
    /** @param gridStep The grid granularity, limits the subdivision. */
    Space(coord gridStep,uint maxLevel,uint maxLeafAgents);
   ~Space();

    void Add(Agent&);

  private:
    void Nbr(Agent const&,coord radius,Agent::collection&) const;
    void Nbr_(Agent const&,coord radius,Agent::collection&) const;

    void Remove(ASubspace&);

    idx_t FromPos(util::tuple3<coord> const& pos)  const;
    idx_t FromPos0(util::tuple3<coord> const& pos) const; // level0 adjusted
    Point FromIdx(idx_t const&) const;

  private:
    coord const m_gridStep;
    uint  const m_maxLevel;      ///< The maximum subdivision level (starts with 0).
    uint  const m_maxLeafAgents; ///< The maximum number of agents in a leaf subspace.
    // an associative container of "level 0" subspaces; also try std::map
    class assoc_ss: public util::splaytree<ASubspace*,SpaceIdx*>
    {
      int Compare(SpaceIdx* const& a1, SpaceIdx* const& a2) const;
    };

    assoc_ss m_l0subspaces;
    uint     m_l0size; // in grid steps
  };


//-----------------------------------------------------------------------------

#endif

// eof
