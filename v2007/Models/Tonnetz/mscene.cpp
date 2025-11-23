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
* $Id: mscene.cpp 47 2006-06-21 10:34:49Z ian_b $
\*****************************************************************************/

/** \file
 */

  #include "mscene.hpp"

//-----------------------------------------------------------------------------

  MoebiusScene::MoebiusScene()
    : m_moebius(new vigo::geom::MoebiusBoundary(10,1,2)),
      m_agents()
  {
    Create();
  }
  
  
  void MoebiusScene::SetRHN(double r, double h, uint n)
  {
    m_moebius->Set(r,h);
    m_moebius->SetN(n);
  }

  
  void MoebiusScene::Play(int what)
  {
    //TAgent *ta = dynamic_cast<TAgent*>(SelectedObject());
    double t = SimTime() + 1.;
    switch(what)
    {
    case 0:
      for(uint i=0; i<3*12; ++i)
      {
        TAgent &ag = *m_agents[(i*7)%12];
        SendAsync(ag,NULL,vigo::space::Message(1),t+(i*.6));
        SendAsync(ag,NULL,vigo::space::Message(0),t+(i*.6)+.59);
      }
      break;
    case 1:
      for(uint i=0; i<3*12; ++i)
      {
        TAgent &ag = *m_agents[((23-i)*7)%12];
        SendAsync(ag,NULL,vigo::space::Message(1),t+(i*.6));
        SendAsync(ag,NULL,vigo::space::Message(0),t+(i*.6)+.59);
      }
      break;
    case 2:
      for(uint i=0; i<3*12; ++i)
      {
        TAgent &ag = *m_agents[i%12];
        SendAsync(ag,NULL,vigo::space::Message(1),t+(i*.6));
        SendAsync(ag,NULL,vigo::space::Message(0),t+(i*.6)+.59);
      }
      break;
    case 3:
      for(uint i=0; i<3*12; ++i)
      {
        TAgent &ag = *m_agents[(23-i)%12];
        SendAsync(ag,NULL,vigo::space::Message(1),t+(i*.6));
        SendAsync(ag,NULL,vigo::space::Message(0),t+(i*.6)+.59);
      }
      break;
    }
  }

  uint const NUM_AGENTS = 12;

  void MoebiusScene::Create()
  {
    for(uint i=0; i<NUM_AGENTS; ++i)
    {
      TAgent &a = *new TAgent(*this);
      a.SetCurve(*m_moebius);
      a.SetU(double(i)/NUM_AGENTS);
      Root().Add(a);
      m_agents.push_back(&a);
    }
    
    SetFreq(440.0,false);
  }
  
  
  void MoebiusScene::SetFreq(float freq,bool just)
  {
    for(uint i=0; i<NUM_AGENTS; ++i)
    {
      TAgent &a = *m_agents[i];
      
      uint n = (i*7) % NUM_AGENTS;
      a.SetFreq(just ? Tone::JtFfth(freq,i)
                     : Tone::EqSemi(freq,n));
      a.SetLabel(vigo::sformat("%u",n));
    }
  }
  
  
  void MoebiusScene::DisplayRoot(vigo::Camera const& cam,
                                 vigo::space::sceneGL::eDisplay ed)
  {
    // line mesh
    glColor3d(.3,.3,.3);
    glBegin(GL_LINES);
    uint const NUM_LINES = 1000;
    uint const n         = m_moebius->N();
    for(uint i=0; i<NUM_LINES; ++i)
    {
      Pointd
        p1 = m_moebius->CurvePoint01(double(i)/NUM_LINES),
        p2 = m_moebius->CurvePoint01(double(i+NUM_LINES/n)/NUM_LINES);
      glVertex3d(p1.x,p1.y,p1.z);
      glVertex3d(p2.x,p2.y,p2.z);
    }
    glEnd();

    // boundary
    glColor3d(0,1,1);
    glBegin(GL_LINE_LOOP);
    uint const NUM_STEPS = 100;
    for(uint i=0; i<NUM_STEPS; ++i)
    {
      Pointd
        p = m_moebius->CurvePoint01(double(i)/NUM_STEPS);
      glVertex3d(p.x,p.y,p.z);
    }
    glEnd();

    // lines between agents
    glColor3d(1,1,0);
    glBegin(GL_LINES);
    for(uint i=0; i<NUM_AGENTS; ++i)
    {
      Pointf
        p1 = m_agents[i]->Pos(),
        p2 = m_agents[(i+7)%NUM_AGENTS]->Pos();
      glVertex3f(p1.x,p1.y,p1.z);
      glVertex3f(p2.x,p2.y,p2.z);
    }
    glEnd();

    super::DisplayRoot(cam,ed);
  }

//** eof **********************************************************************
