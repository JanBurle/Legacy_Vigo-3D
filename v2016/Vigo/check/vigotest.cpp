// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

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

// eof
