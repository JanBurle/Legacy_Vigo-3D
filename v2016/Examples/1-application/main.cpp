// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

  // include Vigo application classes
  #include <vigoapp>
  // include Vigo simulation classes
  #include <vigosim>

  // miscelaneous includes
  #include <iostream>

  typedef vigo::algebra3d::Pointd  Point;

//-----------------------------------------------------------------------------

  class Simulation: public vigo::Simulation
  {
    SUPER(vigo::Simulation)

  public:
    Simulation(): super()
    {
    }

    virtual void Advance()
    {
      super::Advance();

      // demonstrates how to exit the application
      if(SimTime()>12.0)  // run for twelve seconds, then exit
        vigo::Application::Get().Exit(123);
    }
  };

  class Scene: public vigo::space::Scened
  {
    SUPER(vigo::space::Scened)

    double CameraFar()  const { return 100;   }
    Point  CameraEye()  const { return Point::_0;}
  };

//-----------------------------------------------------------------------------

  int main(int argc, pstr argv[])
  {
    try // wrap everything in a try-catch clause
    {
      // one application object first; must exist all the time
      vigo::Application app("",argc,argv);

      // add a user-specified command-line switch
      bool sayHello = false;
      app.AddCmdlineSwitch("--hello", sayHello, "Say hello when started");

      try // process command-line arguments
      {
        app.ProcessArgs();
      }
      catch(vigo::exc& e)
      {
        std::cerr << e.What()   << std::endl;
        std::cout << app.Help() << std::endl;
        return -1;
      }

      if(sayHello)
        std::cout << "Hello, there!" << std::endl;

      // then one window
      vigo::Window wnd("My Window");
      // one canvas for the window
      vigo::Canvas cvs(wnd);

      // you may have many cameras
      vigo::Camera cam1;

      // a camera projects on a canvas, possibly on several canvases
      cam1.Attach(cvs);

      // a simulation and a scene
      Simulation sim;
      Scene scene;
      sim.SetScene(scene);

      // train the camera on the simulation
      sim.Attach(cam1);

      // start the simulation
      sim.Start();

      // run the application and return the return code
      int returnCode = app.Exec();

      std::cout << "Exiting with code " << returnCode << std::endl;
      // everything went ok, return the exit code
      return returnCode;
    }
    catch(vigo::exc& e)
    {
      // an exception at this point signals something wrong
      // a good way is to print the message to stderr
      std::cerr << e.What() << std::endl;

      // since the program failed, return <>0
      return -1;
    }
  }

// eof
