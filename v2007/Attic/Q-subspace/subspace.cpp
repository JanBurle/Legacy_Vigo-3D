/*****************************************************************************\
*    Copyright (c) 2006 by Ian G Burleigh           burleigh@cpsc.ucalgary.ca *
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
* $Id: subspace.cpp 182 2006-03-08 14:06:05Z ian_b $
\*****************************************************************************/

/** \file
 */

  #include "subspace.hpp"

//-----------------------------------------------------------------------------

  Agent::Agent()
    : m_subspace(NULL), m_prev(NULL), m_next(NULL),
      m_pos()
  {
    //!!TRACE("Agent()",this)
  }


  Agent::Agent(Point const& pos)
    : m_subspace(NULL), m_prev(NULL), m_next(NULL),
      m_pos(pos)
  {
    //!!TRACE2("Agent(pos)",this,pos)
  }


  Agent::Agent(Point const& pos,Space& space)
    : m_subspace(NULL), m_prev(NULL), m_next(NULL),
      m_pos(pos)
  {
    //!!TRACE2("Agent(pos,space)",this,pos)
    EnterSpace(space);
  }

  
  Agent::~Agent()
  {
    //!!TRACE2("~Agent() IsInSpace()",this,IsInSpace())
    if(IsInSpace())
      LeaveSpace();
  }


  void Agent::EnterSpace(Space& space)
  {
    //!!TRACE("Agent::EnterSpace()",this)
    RUNTIME_CHECK_EXC3(!IsInSpace(),IN_SPACE,"Agent already in space")
    space.Add(*this);
  }

  
  void Agent::LeaveSpace()
  {
    //!!TRACE("Agent::LeaveSpace()",this)
    RUNTIME_CHECK_EXC3(IsInSpace(),NOT_IN_SPACE,"Agent is not in space")
    m_subspace->Remove(*this);
  }

  
  void Agent::SetPos(Point const& newPos)
  {
    //!!TRACE4("Agent::SetPos(newPos) Pos() IsInSPace()",this,newPos,Pos(),IsInSpace())
    m_pos = newPos;
    if(IsInSpace())
      m_subspace->Move(*this,newPos);
  }

  
  void Agent::Nbr(coord radius,collection& coll)
  {
    RUNTIME_CHECK_EXC3(IsInSpace(),NOT_IN_SPACE,"Agent is not in space")
    if(0>=radius) return;
    //!!TRACE3("Agent::Nbr(radius) pos",this,radius,Pos())
    m_subspace->Nbr(*this,radius,coll);
  }


//-----------------------------------------------------------------------------
  
  ASpace::ASpace(idx_t const& idx)
    : super(idx)
  {
  }


  ASpace::~ASpace()
  {
  }


//-----------------------------------------------------------------------------

  ASubspace::ASubspace(idx_t const& idx, uint size,
                       ASpace& parent,Space& space,uint level)
    : super(idx),
      m_size(size), m_parent(parent), m_space(space), m_level(level)
  {
  }


  ASubspace::~ASubspace()
  {
  }


  bool ASubspace::Contains(idx_t const& idx) const
  {
    idx_t const &myIdx = Idx();
    return
      idx.x >= myIdx.x && idx.x < myIdx.x+(int)m_size &&
      idx.y >= myIdx.y && idx.y < myIdx.y+(int)m_size &&
      idx.z >= myIdx.z && idx.z < myIdx.z+(int)m_size;
  }

  
  bool ASubspace::Intersects(Point const& pos,coord radius) const
  {
    Point minCoord = m_space.FromIdx(Idx()),
          maxCoord = m_space.FromIdx(Idx()+m_size);
    return
      pos.x+radius >= minCoord.x && pos.x-radius < maxCoord.x &&
      pos.y+radius >= minCoord.y && pos.y-radius < maxCoord.y &&
      pos.z+radius >= minCoord.z && pos.z-radius < maxCoord.z;
  }

  
  void ASubspace::Nbr(Agent const& agent,coord radius,Agent::collection& coll) const
  {
    Point minCoord = m_space.FromIdx(Idx()),
          maxCoord = m_space.FromIdx(Idx()+m_size);
    Point pos = agent.Pos();

    bool isInside =
      pos.x-radius >= minCoord.x &&
      pos.x+radius <  maxCoord.x &&
      pos.y-radius >= minCoord.y &&
      pos.y+radius <  maxCoord.y &&
      pos.z-radius >= minCoord.z &&
      pos.z+radius <  maxCoord.z;

    //!!TRACE4("ASubspace::Nbr() Idx minC maxC inside",Idx(),minCoord,maxCoord,isInside)
    if(isInside)
      Nbr_(agent,radius,coll);
    else
      m_parent.Nbr(agent,radius,coll);
  }

  
  ASubspace* ASubspace::QSplit(ASubspace* ss)
  { 
    assert(ss);
    SubspaceLeaf *sl = dynamic_cast<SubspaceLeaf*>(ss);
    if(!sl || sl->NumAgents()<=sl->m_space.m_maxLeafAgents || sl->Level()>=sl->m_space.m_maxLevel)
    {
      // already split, or splitting not triggered
      return ss;
    }
    
    // ss is a leaf, replace it with a node
    SubspaceNode *node = new SubspaceNode(sl->Idx(),sl->m_size,sl->m_parent,sl->m_space,sl->m_level);
    //!!TRACE2("splitting",ss,node)
    // move agents over
    Agent *a;
    while(NULL!=(a=sl->m_agents))
    {
      sl->Remove_(*a);
      node->Add(*a);
    }
  
    delete sl;
    return node;
  }


//-----------------------------------------------------------------------------

  SubspaceNode::SubspaceNode(idx_t const& idx, uint size,
                             ASpace& parent,Space& space,uint level)
    : super(idx,size,parent,space,level),
      m_subspaces(), m_numSubspaces(0)
  {
    //!!TRACE5("SubspaceNode(idx,size,parent,level)",this,idx,size,&parent,level)
    bzero(&m_subspaces,sizeof m_subspaces);
  }


  SubspaceNode::~SubspaceNode()
  {
    //!!TRACE("~SubspaceNode()",this)
    ASubspace** ps = (ASubspace**)m_subspaces;
    for(uint i=0; i<NUM_SSPACES; ++i, ++ps)
    {
      delete *ps;
    }
  }


  void SubspaceNode::Add(Agent& agent)
  {
    //!!TRACE2("SubspaceNode::Add(agent)",this,&agent)
    assert(!agent.IsInSpace());

    // the grid size of this node's subspaces
    uint const subSize = m_size/SUB_ORDER;
    // the agent's grid coordinates
    idx_t agentIdx = m_space.FromPos(agent.Pos());
    // relative index of this agent's subspace
    idx_t ssIdx = (agentIdx-Idx()) / subSize;
    // take the subspace
    assert(ssIdx.x>=0 && ssIdx.y>=0 && ssIdx.z>=0);
    assert(ssIdx.x<(int)SUB_ORDER && ssIdx.y<(int)SUB_ORDER && ssIdx.z<(int)SUB_ORDER);
    ASubspace* &ss = m_subspaces[ssIdx.x][ssIdx.y][ssIdx.z];
    //!!TRACE3("adding: agentIdx,subSize,ssIdx",agentIdx,subSize,ssIdx)
    // create the subspace, if there is not one
    if(!ss)
    {
      // the new subspace's grid index
      idx_t gridIdx = Idx()+(ssIdx*subSize);
      // create
      ss = new SubspaceLeaf(gridIdx,subSize,*this,m_space,m_level+1);
      ++m_numSubspaces;
    }

    // we have the subspace
    ss->Add(agent);
    ss = QSplit(ss);
  }


  void SubspaceNode::Remove(ASubspace& rs)
  {
    //!!TRACE2("SubspaceNode::Remove(.)",this,&rs)
    ASubspace** ps = (ASubspace**)m_subspaces;
    for(uint i=0; i<NUM_SSPACES; ++i, ++ps)
    {
      ASubspace* &ss = *ps;
      if(ss==&rs)
      {
        delete ss; ss = NULL;
        if(0==--m_numSubspaces) // no more subspaces here, the node is not needed
          m_parent.Remove(*this);
        return;
      }
    }

    NOT_HERE() // remss should have been there
  }


  void SubspaceNode::Nbr_(Agent const& agent,coord radius,Agent::collection& coll) const
  {
    Point pos = agent.Pos();

    ASubspace** ps = (ASubspace**)m_subspaces;
    for(uint i=0; i<NUM_SSPACES; ++i, ++ps)
    {
      ASubspace* &ss = *ps;
      if(ss && ss->Intersects(pos,radius))
        ss->Nbr_(agent,radius,coll);
    }
  }


//-----------------------------------------------------------------------------

  SubspaceLeaf::SubspaceLeaf(idx_t const& idx, uint size,
                             ASpace& parent,Space& space,uint level)
    : super(idx,size,parent,space,level),
      m_agents(NULL), m_numAgents(0)
  {
    //!!TRACE5("SubspaceLeaf(idx,size,parent,level)",this,idx,size,&parent,level)
  }


  SubspaceLeaf::~SubspaceLeaf()
  {
    //!!TRACE("~SubspaceLeaf()",this)
    while(m_agents)
      Remove_(*m_agents);
  }


  void SubspaceLeaf::Add(Agent& agent)
  {
    //!!TRACE2("SubspaceLeaf::Add(agent)",this,&agent)
    assert(!agent.IsInSpace());

    if(!m_agents)
    {
      assert(0==m_numAgents);
      m_agents = &agent;
    }
    else
    {
      agent.m_next     = m_agents;
      m_agents->m_prev = &agent;
      m_agents         = &agent;
    }

    agent.m_subspace = this;
    ++m_numAgents;
  }

  
  void SubspaceLeaf::Remove(Agent& agent)
  {
    //!!TRACE2("SubspaceLeaf::Remove(agent)",this,&agent)
    assert(this==agent.m_subspace);

    Remove_(agent);
    if(0==m_numAgents)
      m_parent.Remove(*this);
  }


  void SubspaceLeaf::Remove_(Agent& agent)
  {
    //!!TRACE2("SubspaceLeaf::Remove_(agent)",this,&agent)
    assert(this==agent.m_subspace);
    
    Agent *next = agent.m_next,
          *prev = agent.m_prev;
                           
    if(m_agents == &agent)  // is in the front
      m_agents = next;
    else
      prev->m_next = next;

    if(NULL!=next)
      next->m_prev = prev;

    agent.m_prev = agent.m_next = NULL;
    agent.m_subspace  = NULL;

    --m_numAgents;
  }

  
  void SubspaceLeaf::Move(Agent& agent,Point const& newPos)
  {
    //!!TRACE3("SubspaceLeaf::Move(agent,newPos)",this,&agent,newPos)
    agent.m_pos  = newPos;
    idx_t newIdx = m_space.FromPos(newPos);
    
    if(!Contains(newIdx))
    {
      Remove_(agent);
      m_space.Add(agent);
      if(0==m_numAgents)
        m_parent.Remove(*this);
    }
  }

  
  void SubspaceLeaf::Nbr_(Agent const& agent,coord radius,Agent::collection& coll) const
  {
    assert(0<radius);

    Point pos = agent.Pos();
    coord mag = radius*radius;
    for(Agent *a = m_agents; a; a=a->m_next)
    {
      if(a!=&agent /*not self*/ && (a->Pos()-pos).Magnitude()<=mag/*within radius*/)
        coll.push_back(a);
    }
  }

  
//-----------------------------------------------------------------------------

  Space::Space(coord gridStep, uint maxLevel, uint maxLeafAgents)
    : super(idx_t(0,0,0)), m_gridStep(gridStep),
      m_maxLevel(maxLevel), m_maxLeafAgents(max(maxLeafAgents,1u)),
      m_l0subspaces(), m_l0size(0)
  {
    //!!TRACE3("Space(gridStep) l0size",this,gridStep,m_l0size)
    m_l0size = math::powu(SUB_ORDER,m_maxLevel);
  }


  Space::~Space()
  {
    //!!TRACE("~Space()",this)
    while(!m_l0subspaces.empty())
    {
      ASubspace *ss = *m_l0subspaces.current();
      VALIDATE_EXC(m_l0subspaces.remove(ss))
      delete ss;
    }
  }


  void Space::Add(Agent& agent)
  {
    //!!TRACE3("Space::Add(agent) pos",this,&agent,agent.Pos())
    assert(!agent.IsInSpace());

    // the level0 subspace index
    idx_t l0idx = FromPos0(agent.Pos());
    //!!TRACE("Space::Add: l0idx",l0idx)
    // get the subspace
    ASubspace* const *pss = m_l0subspaces.find(&SpaceIdx(l0idx));
    ASubspace *ss;
    if(pss)
    {
      ss = *pss;
    }
    else
    {
      ss = new SubspaceLeaf(l0idx,m_l0size,*this,*this,0);
      VALIDATE_EXC(m_l0subspaces.insert(ss))
    }

    ss->Add(agent);
    // see if needs splitting
    ASubspace *spl = ASubspace::QSplit(ss);
    if(spl!=ss)
    {
      VALIDATE_EXC(m_l0subspaces.remove(ss))
      VALIDATE_EXC(m_l0subspaces.insert(spl))
    }
  }

 
  void Space::Nbr(Agent const& agent,coord radius,Agent::collection& coll) const
  {
    Nbr_(agent,radius,coll); // simply pass down
  }

  
  void Space::Nbr_(Agent const& agent,coord radius,Agent::collection& coll) const
  {
    assert(0<radius);

    util::tuple3<coord> pos = agent.Pos();

    idx_t minIdx = FromPos0(Point(pos-radius)),
          maxIdx = FromPos0(Point(pos+radius)) + m_l0size;

    for(int x = minIdx.x; x < maxIdx.x; x+=m_l0size)
    for(int y = minIdx.y; y < maxIdx.y; y+=m_l0size)
    for(int z = minIdx.z; z < maxIdx.z; z+=m_l0size)
    {
      idx_t idx(x,y,z);
      ASubspace* const *pss = m_l0subspaces.find(&SpaceIdx(idx));
      if(pss)
        (*pss)->Nbr_(agent,radius,coll);
    }
  }


  void Space::Remove(ASubspace& ss)
  {
    VALIDATE_EXC(m_l0subspaces.remove(&ss))
    delete &ss;
  }


  ASpace::idx_t Space::FromPos(util::tuple3<coord> const& pos) const
  {
    util::tuple3<coord> p = pos/m_gridStep;
    //!!TRACE4("FromPos",pos,m_gridStep,p,idx_t((int)floor(p.x),(int)floor(p.y),(int)floor(p.z)))
    return idx_t((int)floor(p.x),(int)floor(p.y),(int)floor(p.z));
  }

  
  ASpace::idx_t Space::FromPos0(util::tuple3<coord> const& pos) const
  {
    util::tuple3<coord> p = pos/(m_l0size*m_gridStep);
    //!!TRACE4("FromPos0",pos,m_gridStep,p,idx_t((int)floor(p.x),(int)floor(p.y),(int)floor(p.z)) * m_l0size)
    return idx_t((int)floor(p.x),(int)floor(p.y),(int)floor(p.z)) * m_l0size;
  }

  
  Point Space::FromIdx(ASpace::idx_t const& idx) const
  {
    util::tuple3<coord> v(idx.x,idx.y,idx.z); // converts to coord
    //!!TRACE2("FromIdx",v,v*m_gridStep)
    return Point(v * m_gridStep);
  }

  
  int Space::assoc_ss::Compare(SpaceIdx* const& a1, SpaceIdx* const& a2) const
  {
    assert(a1 && a2);
    idx_t const &i1 = a1->Idx(), &i2 = a2->Idx();
    if(i1.x < i2.x) return -1;
    if(i1.x > i2.x) return +1;
    if(i1.y < i2.y) return -1;
    if(i1.y > i2.y) return +1;
    if(i1.z < i2.z) return -1;
    if(i1.z > i2.z) return +1;
    return 0;
  }
  
  
//** eof **********************************************************************
