// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

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

// eof
