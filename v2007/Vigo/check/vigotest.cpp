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
* $Id:vigotest.cpp 30 2006-04-06 19:13:28Z ian_b $
\*****************************************************************************/

/** \file
 *  \brief CppUnit tests main.
 *
 */
 
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/TestResultCollector.h>
#include <fstream>
#include <iostream>

//-----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
  CppUnit::TestFactoryRegistry &registry = 
     CppUnit::TestFactoryRegistry::getRegistry();

  CppUnit::TextUi::TestRunner runner;

  if(argc>2)
  {
    std::cerr << "usage: " << argv[0] << " <outfile>" << std::endl;
    exit(1);
  }
  
  std::ostream &out = std::cout;  
 
  CppUnit::TestResultCollector& collector = runner.result();
  // outputter will be deleted by the runner
  CppUnit::TextOutputter* pOutputter = 
    new CppUnit::TextOutputter(&collector,out);
    
  runner.setOutputter(pOutputter);
  runner.addTest(registry.makeTest());
    
  bool wasSuccessful = runner.run("", false, true, false);
  return wasSuccessful ? 0 : 1;
}

//** eof **********************************************************************
