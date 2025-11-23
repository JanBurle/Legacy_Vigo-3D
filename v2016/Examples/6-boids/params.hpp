// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

#ifndef BOIDS_PARAMS_HPP_INCLUDED
#define BOIDS_PARAMS_HPP_INCLUDED

  #include <vigobase>
  #include <vigoext>

  namespace params {

//----------------------------------------------------------------------------

  struct APP // application params
  {
    static uint        numBoids;
    static std::string boidSource;
    static float       boidSpread;
  };

  struct CAMERA
  {
    static double      far;
    static std::string eye;
    static std::string center;
    static std::string up;
    static std::string lightAmbColor;
    static std::string lightDiffColor;
    static std::string lightPosition;
  };

  struct PHYSICS
  {
    static float Cd;      // drag coeff
    static float Cl;      // lift coeff
    static float rho2;    // fluid (air) density
    static float gee;     // g
  };

  struct FLOCKING
  {
    static bool  flock;   // flocking on/off
    static float Cs;      // separation
    static float Ca;      // alignment
    static float Cc;      // cohesion
    static float Cg;      // global goal
  };

  struct BOID
  {
    static std::string color;
    static float size;
    static float mass;
    static float rA;      // ratio: wing area/size^2
    static float rS;      // ration: front cross-section/size^2
    static float perc;    // perception distance (flockmates)
    static float vision;  // vision distance (obstacles)
    static float maxRoll; // maximum roll allowed
    static float rollD;   // roll delta between simulation steps
    static float maxPitch;// maximum pitch allowed
    static float pitchD;  // pitch delta between simulation steps
    static float thrust;
  };

//----------------------------------------------------------------------------

  extern vigo::extras::Config config;

  void Load(pcstr fname);
  void Print(std::ostream& os);

//----------------------------------------------------------------------------

  } // namespace

#endif

// eof
