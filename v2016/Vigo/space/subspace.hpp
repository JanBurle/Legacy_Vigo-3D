// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 * \brief Subdivision of space
 *
 * Subdivision of the space. Needed for efficient neighbour lookup. (Experimental)
 */

#ifndef VIGO_SPACE_SUBSPACE_HPP_INCLUDED
#define VIGO_SPACE_SUBSPACE_HPP_INCLUDED

  #include "../common/types.hpp"
  #include "../common/array.hpp"
  #include "../common/tuple.hpp"
  #include "../common/splaytree.hpp"
  #include "./algebra.hpp"
  #include <vector>
  #include <typeinfo>

#ifdef __SUNOS__
#include <strings.h>
#endif

NAMESPACE_VIGO(parts)
  template<typename Coord> struct T1__;
NAMESPACE_END()

/** \defgroup grp_division_hpp Space subdivision
@{*/
NAMESPACE_VIGO(space) //=======================================================

  /// A scene graph node
  template<typename Coord> class AbstractNode;

  /// A collection of pointers to tracked nodes
  template<typename Coord>
  struct tracked_collection: std::vector<AbstractNode<Coord>*>
  {
  };

  /// The tracked group id
  typedef uint32 tracked_grpid;

NAMESPACE_END()
//-----------------------------------------------------------------------------
NAMESPACE_VIGO(subspace)

  template<typename Coord>
  class ASubspace;

  template<typename Coord>
  class SubspaceNode;

  template<typename Coord>
  class SubspaceLeaf;

  template<typename Coord>
  class TopSpace;

  //>>> some of these constants could be run-time parameters
  static uint const SUB_ORDER = 2;  ///< subdivision order (2 or 3 is recommended)

  /// The index of a subspace compartment
  class SpaceIdx
  {
  public:
    typedef util::int3d idx_t;	///< 3D index on the space grid
    SpaceIdx(idx_t idx): m_idx(idx)	{}
    virtual ~SpaceIdx()			{}
    GETTER(Idx,idx_t const&,m_idx)

  protected:
    idx_t m_idx;
  };

  /// The abstract space or subspace.
  template<typename Coord>
  class ASpace: public SpaceIdx
  {
    SUPER(SpaceIdx)

    friend class ASubspace<Coord>;
    friend class TopSpace<Coord>;
    friend class SubspaceNode<Coord>;
    friend class SubspaceLeaf<Coord>;

  public:
    ASpace(SpaceIdx::idx_t const& idx): super(idx)	{}
    virtual ~ASpace()					{}

  protected:
    /// Add a node to this (sub)space,
    virtual void Add(space::AbstractNode<Coord>&) = 0;
    /// Collect the node's neighbours, in this subspace, or above (climbs up).
    virtual void Nbr(space::AbstractNode<Coord> const&,space::tracked_grpid const*,
                     double radius,space::tracked_collection<Coord>&)
      const = 0;
    /// Collect the node's neighbours, in this subspace only (descends down).
    virtual void Nbr_(space::AbstractNode<Coord> const&,space::tracked_grpid const*,
                      double radius,space::tracked_collection<Coord>&)
      const = 0;

    /// Remove an empty (i.e., with no nodes) subspace.
    virtual void Remove(ASubspace<Coord>&) = 0;
  };


//-----------------------------------------------------------------------------

  /** An abstract cubic space partition, a leaf or a node; indexed by its leftmost
    * grid coordinates */
  template<typename Coord>
  class ASubspace: public ASpace<Coord>
  {
    NO_GEN(ASubspace<Coord>)
    SUPER(ASpace<Coord>)

    friend class TopSpace<Coord>;

  public:
    /** @param idx    this partition's index
      * @param size   its size in grid steps
      * @param parent the parent subspace or space
      * @param space  always the top-level space
      * @param level  the nesting level, starting with 0
      */
    ASubspace(SpaceIdx::idx_t const& idx, uint size,
              ASpace<Coord>& parent,TopSpace<Coord>& space,uint level)
      : super(idx),
        m_size(size), m_parent(parent), m_space(space), m_level(level)
    {
    }

   ~ASubspace()
    {
    }

  public:
    GETTER(Level, uint, m_level)

    /// Does the grid index fall within this subspace?
    bool Contains(SpaceIdx::idx_t const& idx) const
    {
      SpaceIdx::idx_t const &myIdx = SpaceIdx::Idx();
      return
        idx.x >= myIdx.x && idx.x < myIdx.x+(int)m_size &&
        idx.y >= myIdx.y && idx.y < myIdx.y+(int)m_size &&
        idx.z >= myIdx.z && idx.z < myIdx.z+(int)m_size;
    }

    /// Does the vision sphere intersect this subspace?
    bool Intersects(algebra3d::Point<Coord> const& pos,double radius) const
    {
      algebra3d::Point<Coord> minCoord = m_space.FromIdx(SpaceIdx::Idx()),
                              maxCoord = m_space.FromIdx(SpaceIdx::Idx()+m_size);
      return
        pos.x+radius >= minCoord.x && pos.x-radius < maxCoord.x &&
        pos.y+radius >= minCoord.y && pos.y-radius < maxCoord.y &&
        pos.z+radius >= minCoord.z && pos.z-radius < maxCoord.z;
    }

    void Nbr(space::AbstractNode<Coord> const& node,space::tracked_grpid const* grpid,
             double radius,space::tracked_collection<Coord>& coll) const
    {
      algebra3d::Point<Coord> minCoord = m_space.FromIdx(SpaceIdx::Idx()),
                              maxCoord = m_space.FromIdx(SpaceIdx::Idx()+m_size);
      algebra3d::Point<Coord> pos = node.Pos();

      bool isInside =
        pos.x-radius >= minCoord.x &&
        pos.x+radius <  maxCoord.x &&
        pos.y-radius >= minCoord.y &&
        pos.y+radius <  maxCoord.y &&
        pos.z-radius >= minCoord.z &&
        pos.z+radius <  maxCoord.z;

      if(isInside)
        Nbr_(node,grpid,radius,coll);
      else
        m_parent.Nbr(node,grpid,radius,coll);
    }

  protected:
    /// Splits a subspace, if needed and possible.
    static ASubspace* QSplit(ASubspace* ss)
    {
      assert(ss);
      SubspaceLeaf<Coord> *sl = dynamic_cast<SubspaceLeaf<Coord>*>(ss);
      if(!sl || sl->NumNodes()<=sl->m_space.m_maxLeafNodes || sl->Level()>=sl->m_space.m_maxLevel)
      {
        // already split, or splitting not triggered
        return ss;
      }

      // ss is a leaf, replace it with a node
      SubspaceNode<Coord> *node
        = new SubspaceNode<Coord>(sl->Idx(),sl->m_size,sl->m_parent,sl->m_space,sl->m_level);
      // move nodes over
      space::AbstractNode<Coord> *a;
      while(NULL!=(a=sl->m_nodes))
      {
        sl->Remove_(*a);
        node->Add(*a);
      }

      delete sl;
      return node;
    }


  protected:
    uint   	     m_size;
    ASpace<Coord>&   m_parent;
    TopSpace<Coord>& m_space;
    uint             m_level;
  };


//-----------------------------------------------------------------------------

  /** The intermediate space partition */
  template<typename Coord>
  class SubspaceNode: public ASubspace<Coord>
  {
    NO_GEN(SubspaceNode<Coord>)
    SUPER(ASubspace<Coord>)

    friend class ASubspace<Coord>;
    friend class SubspaceLeaf<Coord>;

  public:
    SubspaceNode(SpaceIdx::idx_t const& idx,uint size,
                 ASpace<Coord>& parent,TopSpace<Coord>& space,uint level)
      : super(idx,size,parent,space,level),
        m_subspaces(), m_numSubspaces(0)
    {
      bzero(&m_subspaces,sizeof m_subspaces);
    }

   ~SubspaceNode()
    {
      ASubspace<Coord>** ps = (ASubspace<Coord>**)m_subspaces;
      for(uint i=0; i<NUM_SSPACES; ++i, ++ps)
      {
        delete *ps;
      }
    }

  private:
    void Add(space::AbstractNode<Coord>& node)
    {
      assert(NULL==node.GetSubspace());

      // the grid size of this node's subspaces
      uint const subSize = this->m_size/SUB_ORDER;
      // the node's grid coordinates
      SpaceIdx::idx_t nodeIdx = this->m_space.FromPos(node.Pos());
      // relative index of this node's subspace
      SpaceIdx::idx_t ssIdx = (nodeIdx-SpaceIdx::Idx()) / subSize;
      // take the subspace
      assert(ssIdx.x>=0 && ssIdx.y>=0 && ssIdx.z>=0);
      assert(ssIdx.x<(int)SUB_ORDER && ssIdx.y<(int)SUB_ORDER && ssIdx.z<(int)SUB_ORDER);
      ASubspace<Coord>* &ss = m_subspaces[ssIdx.x][ssIdx.y][ssIdx.z];
      // create the subspace, if there is not one
      if(!ss)
      {
        // the new subspace's grid index
        SpaceIdx::idx_t gridIdx = SpaceIdx::Idx()+(ssIdx*subSize);
        // create
        ss = new SubspaceLeaf<Coord>(gridIdx,subSize,*this,this->m_space,this->m_level+1);
        ++m_numSubspaces;
      }

      // we have the subspace
      ss->Add(node);
      ss = QSplit(ss);
    }

    void Remove(ASubspace<Coord>& rs)
    {
      ASubspace<Coord>** ps = (ASubspace<Coord>**)m_subspaces;
      for(uint i=0; i<NUM_SSPACES; ++i, ++ps)
      {
        ASubspace<Coord>* &ss = *ps;
        if(ss==&rs)
        {
          delete ss; ss = NULL;
          if(0==--m_numSubspaces) // no more subspaces here, the node is not needed
            this->m_parent.Remove(*this);
          return;
        }
      }

      NOT_HERE() // remss should have been there
    }

    void Nbr_(space::AbstractNode<Coord> const& node,space::tracked_grpid const* grpid,
              double radius,space::tracked_collection<Coord>& coll) const
    {
      algebra3d::Point<Coord> pos = node.Pos();

      ASubspace<Coord>** ps = (ASubspace<Coord>**)m_subspaces;
      for(uint i=0; i<NUM_SSPACES; ++i, ++ps)
      {
        ASubspace<Coord>* &ss = *ps;
        if(ss && ss->Intersects(pos,radius))
          ss->Nbr_(node,grpid,radius,coll);
      }
    }

  private:
    enum {NUM_SSPACES = SUB_ORDER*SUB_ORDER*SUB_ORDER};
    ASubspace<Coord>* m_subspaces[SUB_ORDER][SUB_ORDER][SUB_ORDER];
    ushort     m_numSubspaces;
  };


//-----------------------------------------------------------------------------

  /** The terminal space partition */
  template<typename Coord>
  class SubspaceLeaf: public ASubspace<Coord>
  {
    NO_GEN(SubspaceLeaf<Coord>)
    SUPER(ASubspace<Coord>)

    friend class TopSpace<Coord>;
    friend class ASubspace<Coord>;
    friend class SubspaceNode<Coord>;
    friend class parts::T1__<Coord>;

  public:
    SubspaceLeaf(SpaceIdx::idx_t const& idx,uint size,
                 ASpace<Coord>& parent,TopSpace<Coord>& space,uint level)
    : super(idx,size,parent,space,level),
      m_nodes(NULL), m_numNodes(0)
    {
    }

   ~SubspaceLeaf()
    {
      while(m_nodes)
        Remove_(*m_nodes);
    }

    GETTER(NumNodes, uint, m_numNodes)

  private:
    void Add(space::AbstractNode<Coord>& node)
    {
      assert(NULL==node.GetSubspace());

      if(!m_nodes)
      {
        assert(0==m_numNodes);
        m_nodes = &node;
      }
      else
      {
        node.SetNext(m_nodes);
        m_nodes->SetPrev(&node);
        m_nodes = &node;
      }

      node.SetSubspace(this);
      ++m_numNodes;
    }

    void Remove(space::AbstractNode<Coord>& node) ///< may trigger collapse
    {
      assert(this==node.GetSubspace());

      Remove_(node);
      if(0==m_numNodes)
        this->m_parent.Remove(*this);
    }

    void Remove_(space::AbstractNode<Coord>& node) ///< does not trigger space collapse
    {
      assert(this==node.GetSubspace());

      space::AbstractNode<Coord> *next = node.GetNext(),
                                 *prev = node.GetPrev();

      if(m_nodes == &node)  // is in the front
        m_nodes = next;
      else
        prev->SetNext(next);

      if(NULL!=next)
        next->SetPrev(prev);

      node.SetPrev(NULL); node.SetNext(NULL);
      node.SetSubspace(NULL);

      --m_numNodes;
    }

    void Remove(ASubspace<Coord>&) { NOT_HERE() } ///< Leafs do not have subspaces

    /// Move the node to the new position, pass it to another subspace, if needed.
    void Move(space::AbstractNode<Coord>& node)
    {
      SpaceIdx::idx_t newIdx = this->m_space.FromPos(node.Pos());

      if(!this->Contains(newIdx))
      {
        Remove_(node);
        this->m_space.Add(node);
        if(0==m_numNodes)
          this->m_parent.Remove(*this);
      }
    }

    void Nbr_(space::AbstractNode<Coord> const& node,space::tracked_grpid const* grpid,
              double radius,space::tracked_collection<Coord>& coll) const
    {
      assert(0<radius);

      algebra3d::Point<Coord> pos = node.Pos();
      double mag = radius*radius;
      for(space::AbstractNode<Coord>*a = m_nodes; a; a=a->GetNext())
      {
        if(a!=&node /*not self*/ &&
	   (!grpid || a->GrpId() == *grpid ) && //>>> temp, split subspaces
	   (a->Pos()-pos).Magnitude()<=mag/*within radius*/)
          coll.push_back(a);
      }
    }

  private:
    space::AbstractNode<Coord> *m_nodes;
    uint   m_numNodes;
  };


//-----------------------------------------------------------------------------

  /// The top-level space, not limited in span.
  template<typename Coord>
  class TopSpace: public ASpace<Coord>
  {
    NO_GEN(TopSpace<Coord>)
    SUPER(ASpace<Coord>)

    friend class ASubspace<Coord>;
    friend class SubspaceNode<Coord>;
    friend class SubspaceLeaf<Coord>;

  public:
    /** @param gridStep The grid granularity, limits the subdivision.
      * @param maxLevel The maximum nesting depth.
      * @param maxLeafNodes The maximum number of nodes in a subspace, before it splits.
      */
    TopSpace(Coord gridStep,uint maxLevel,uint maxLeafNodes)
      : super(SpaceIdx::idx_t(0,0,0)), m_gridStep(gridStep),
        m_maxLevel(maxLevel), m_maxLeafNodes(max(maxLeafNodes,1u)),
        m_l0subspaces(), m_l0size(0)
    {
      m_l0size = math::powu(SUB_ORDER,m_maxLevel);
    }

   ~TopSpace()
    {
      while(!m_l0subspaces.empty())
      {
        ASubspace<Coord> *ss = *m_l0subspaces.current();
        VALIDATE_EXC(m_l0subspaces.remove(ss))
        delete ss;
      }
    }

    void Add(space::AbstractNode<Coord>& node)
    {
      assert(NULL==node.GetSubspace());

      // the level0 subspace index
      SpaceIdx::idx_t l0idx = FromPos0(node.Pos());
      // get the subspace
      ASubspace<Coord>* const *pss;
      { // to avoid "taking address of a temporary >>>
        SpaceIdx s0idx(l0idx);
        pss = m_l0subspaces.find(&s0idx);
      }
      ASubspace<Coord> *ss;
      if(pss)
      {
        ss = *pss;
      }
      else
      {
        ss = new SubspaceLeaf<Coord>(l0idx,m_l0size,*this,*this,0);
        VALIDATE_EXC(m_l0subspaces.insert(ss))
      }

      ss->Add(node);
      // see if needs splitting
      ASubspace<Coord> *spl = ASubspace<Coord>::QSplit(ss);
      if(spl!=ss)
      {
        VALIDATE_EXC(m_l0subspaces.remove(ss))
        VALIDATE_EXC(m_l0subspaces.insert(spl))
      }
    }

  private:
    void Nbr(space::AbstractNode<Coord> const& node,space::tracked_grpid const* grpid,
             double radius,space::tracked_collection<Coord>& coll) const
    {
      Nbr_(node,grpid,radius,coll); // simply pass down
    }

    void Nbr_(space::AbstractNode<Coord> const& node,space::tracked_grpid const* grpid,
              double radius,space::tracked_collection<Coord>& coll) const
    {
      assert(0<radius);

      util::tuple3<Coord> pos = node.Pos();

      SpaceIdx::idx_t minIdx = FromPos0(algebra3d::Point<Coord>(pos-radius)),
                      maxIdx = FromPos0(algebra3d::Point<Coord>(pos+radius)) + m_l0size;

      for(int x = minIdx.x; x < maxIdx.x; x+=m_l0size)
      for(int y = minIdx.y; y < maxIdx.y; y+=m_l0size)
      for(int z = minIdx.z; z < maxIdx.z; z+=m_l0size)
      {
        SpaceIdx::idx_t idx(x,y,z);
        ASubspace<Coord>* const *pss;
	{ //>>> taking address of temporary
	  SpaceIdx sidx(idx); pss = m_l0subspaces.find(&sidx);
	}
        if(pss)
          (*pss)->Nbr_(node,grpid,radius,coll);
      }
    }

    void Remove(ASubspace<Coord>& ss)
    {
      VALIDATE_EXC(m_l0subspaces.remove(&ss))
      delete &ss;
    }

    SpaceIdx::idx_t FromPos(util::tuple3<Coord> const& pos)  const
    {
      util::tuple3<Coord> p = pos/m_gridStep;
      return SpaceIdx::idx_t((int)floor(p.x),(int)floor(p.y),(int)floor(p.z));
    }

    SpaceIdx::idx_t FromPos0(util::tuple3<Coord> const& pos) const // level0 adjusted
    {
      util::tuple3<Coord> p = pos/(m_l0size*m_gridStep);
      return SpaceIdx::idx_t((int)floor(p.x),(int)floor(p.y),(int)floor(p.z)) * m_l0size;
    }

    algebra3d::Point<Coord> FromIdx(SpaceIdx::idx_t const& idx) const
    {
      util::tuple3<Coord> v(idx.x,idx.y,idx.z); // converts to coord
      return algebra3d::Point<Coord>(v * m_gridStep);
    }

  private:
    Coord const m_gridStep;
    uint  const m_maxLevel;      ///< The maximum subdivision level (starts with 0).
    uint  const m_maxLeafNodes; ///< The maximum number of nodes in a leaf subspace.
    // an associative container of "level 0" subspaces; also try std::map
    class assoc_ss: public util::splaytree<ASubspace<Coord>*,SpaceIdx*>
    {
      int Compare(SpaceIdx* const& a1, SpaceIdx* const& a2) const
      {
        assert(a1 && a2);
        SpaceIdx::idx_t const &i1 = a1->Idx(), &i2 = a2->Idx();
        if(i1.x < i2.x) return -1;
        if(i1.x > i2.x) return +1;
        if(i1.y < i2.y) return -1;
        if(i1.y > i2.y) return +1;
        if(i1.z < i2.z) return -1;
        if(i1.z > i2.z) return +1;
        return 0;
      }
    };

    assoc_ss m_l0subspaces;	// level 0
    uint     m_l0size; 		// in grid steps
  };


NAMESPACE_END() //=============================================================
/**@} (defgroup)*/

#endif

// eof
