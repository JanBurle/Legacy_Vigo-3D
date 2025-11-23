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
* $Id: tscene.cpp 229 2007-05-21 17:47:20Z ian_b $
\*****************************************************************************/

/** \file
 */

  #include "tscene.hpp"

//-----------------------------------------------------------------------------

  TonnetzScene::TonnetzScene()
    : m_space(new TSpace()), m_agents()
  {
    Create();
  }
  
  uint const N2 = 3; // how many in each direction around the centre
  uint const N  = 2*N2 + 1;
  
  void TonnetzScene::Create()
  {/**
    for(uint i=0; i<N; ++i)
    for(uint j=0; j<N; ++j)
    {
      TAgent &a = *new TAgent(*this);
      a.SetCurve(*m_plane);
      a.Set(double(i),double(j));
      Root().Add(a);
      m_agents.push_back(&a);
    }
    
    SetFreq(440.0,false);
  **/}


  void TonnetzScene::SetFreq(float /*freq*/,bool /*just*/)
  {
//      TAgent &a = *m_agents[i];
//      a.SetFreq(just ? Tone::JtFfth(freq,i)
//                     : Tone::EqSemi(freq,n));
//      a.SetLabel(vigo::sformat("%u",n));
  }
  
  
  void TonnetzScene::DisplayRoot(vigo::Camera const& cam,
                                 vigo::space::sceneGL::eDisplay ed)
  {
//    // line mesh
//    glColor3d(.3,.3,.3);
//    glBegin(GL_LINES);
//    uint const NUM_LINES = 1000;
//    uint const n         = m_moebius->N();
//    for(uint i=0; i<NUM_LINES; ++i)
//    {
//      Pointd
//        p1 = m_moebius->CurvePoint01(double(i)/NUM_LINES),
//        p2 = m_moebius->CurvePoint01(double(i+NUM_LINES/n)/NUM_LINES);
//      glVertex3d(p1.x,p1.y,p1.z);
//      glVertex3d(p2.x,p2.y,p2.z);
//    }
//    glEnd();
//
//    // boundary
//    glColor3d(0,1,1);
//    glBegin(GL_LINE_LOOP);
//    uint const NUM_STEPS = 100;
//    for(uint i=0; i<NUM_STEPS; ++i)
//    {
//      Pointd
//        p = m_moebius->CurvePoint01(double(i)/NUM_STEPS);
//      glVertex3d(p.x,p.y,p.z);
//    }
//    glEnd();
//
//    // lines between agents
//    glColor3d(1,1,0);
//    glBegin(GL_LINES);
//    for(uint i=0; i<NUM_AGENTS; ++i)
//    {
//      Pointf
//        p1 = m_agents[i]->Pos(),
//        p2 = m_agents[(i+7)%NUM_AGENTS]->Pos();
//      glVertex3f(p1.x,p1.y,p1.z);
//      glVertex3f(p2.x,p2.y,p2.z);
//    }
//    glEnd();
//
    super::DisplayRoot(cam,ed);
  }


//** eof **********************************************************************
