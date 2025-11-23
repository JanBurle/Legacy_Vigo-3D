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
* $Id: main.cpp 182 2006-03-08 14:06:05Z ian_b $
\*****************************************************************************/

/** \file
 *  
 *  This is a test of my method of adaptive subdivision of a space with agents.
 *  The purpose of the subdivision is to lower the complexity of neighbour
 *  lookup. The finalized solution will be incorporated into Vigo::3D.
 */

  #include "subspace.hpp"
  #include <iostream>
  #include <vector>
  #include <time.h>

//-----------------------------------------------------------------------------

  void test(coord gridStep,uint maxLevel,uint maxAgents,uint numSteps,
            uint numAgents,coord spread,coord step,coord vision)
  {
    std::cout << gridStep << "\t" << maxLevel << "\t" << maxAgents << "\t" << numSteps << "\t" << numAgents << "\t" << spread  << "\t" << step << "\t" << vision << "\t";

    Space space(gridStep,maxLevel,maxAgents);

    std::vector<Agent*> agents;

    //!!std::cout << "***** " << numAgents << " agents ***** " << std::endl;
    for(uint i=0; i<numAgents; ++i)
    {
      Agent *a = new Agent(Point::Random(Point::_0,spread),space);
      agents.push_back(a);
    }
  
    time_t tt = time(NULL);

    // random walk
    for(uint s=0; s<numSteps; ++s)
    {
      //!!std::cout << "***** step ***** " << s << std::endl;
      for(uint i=0, iMax=agents.size(); i<iMax; ++i)
      {
        //!!std::cout << "*** move *** " << i;
        Agent &a = *agents[i];
        a.SetPos(Point::Random(a.Pos(),step));
        //!!std::cout << " to " << a.Pos() << std::endl;
      }
      
      uint totalNbr = 0;
      //!!std::cout << "*** avg. nbr  *** = " << std::flush;
      for(uint i=0, iMax=agents.size(); i<iMax; ++i)
      {
        //!!std::cout << "*** nbr  *** " << i << " = ";
        Agent &a = *agents[i];
        Agent::collection nbrColl;
        a.Nbr(vision,nbrColl);
        //!!std::cout << nbrColl.size() << std::endl;
        totalNbr += nbrColl.size();
      }
      //!!std::cout << (double)totalNbr/(double)numAgents << std::endl;
    }

    std::cout << time(NULL)-tt << std::endl;
  }


  void test()
  {
    for(coord gridStep  = 10;  gridStep>0.001;   gridStep/=10)
    for(uint  maxLevel  = 7;   maxLevel>1;       maxLevel-=1)
    for(uint  maxAgents = 1;   maxAgents<2000;   maxAgents*=10)
    for(uint  numAgents = 10;  numAgents<2000000;numAgents*=10)
    for(coord spread    = 1.;  spread<200.;      spread*=10)
    for(coord step      = .1;  step<20.;         step*=10)
    for(coord vision    = .1;  vision<20.;       vision*=10)
    {
      test(gridStep,maxLevel,maxAgents,1000/*numSteps*/,numAgents,spread,step,vision);
    }
  }   

  int main(int /*argc*/, pstr /*argv*/[])
  {
    try
    {
      test();
    }
    catch(exc const& e)
    {
      std::cerr << e.What() << std::endl;
      return -1;
    }

    return 0;
  }

//** eof **********************************************************************
