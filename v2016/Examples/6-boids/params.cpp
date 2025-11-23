// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

  #include "params.hpp"
  #include <iomanip>

  namespace params {

//----------------------------------------------------------------------------

  uint        APP::numBoids;
  std::string APP::boidSource;
  float       APP::boidSpread;

  double      CAMERA::far;
  std::string CAMERA::eye;
  std::string CAMERA::center;
  std::string CAMERA::up;

  std::string CAMERA::lightAmbColor;
  std::string CAMERA::lightDiffColor;
  std::string CAMERA::lightPosition;

  float   PHYSICS::Cd;
  float   PHYSICS::Cl;
  float   PHYSICS::rho2;
  float   PHYSICS::gee;

  bool    FLOCKING::flock;
  float   FLOCKING::Cs;
  float   FLOCKING::Ca;
  float   FLOCKING::Cc;
  float   FLOCKING::Cg;

  std::string BOID::color;
  float   BOID::size;
  float   BOID::mass;
  float   BOID::rA;
  float   BOID::rS;
  float   BOID::perc;
  float   BOID::vision;
  float   BOID::maxRoll;
  float   BOID::rollD;
  float   BOID::maxPitch;
  float   BOID::pitchD;
  float   BOID::thrust;

//----------------------------------------------------------------------------

  #define DO_PARAMS                      \
    DO_PARAM(APP,  numBoids,      1)     \
    DO_PARAM(APP,  boidSource, "0,0,0")   \
    DO_PARAM(APP,  boidSpread,  10.0)     \
    DELIM_GROUP                           \
    DO_PARAM(CAMERA,    far,   100.0)     \
    DO_PARAM(CAMERA,    eye,   "")        \
    DO_PARAM(CAMERA,    center,"")        \
    DO_PARAM(CAMERA,    up,    "")        \
    DO_PARAM(CAMERA,    lightAmbColor, "")  \
    DO_PARAM(CAMERA,    lightDiffColor, "") \
    DO_PARAM(CAMERA,    lightPosition, "")  \
    DELIM_GROUP                           \
    DO_PARAM(PHYSICS,   Cd,      0.1)     \
    DO_PARAM(PHYSICS,   Cl,      0.02)    \
    DO_PARAM(PHYSICS,   rho2,    0.5)     \
    DO_PARAM(PHYSICS,   gee,     9.8)     \
    DELIM_GROUP                           \
    DO_PARAM(FLOCKING,  flock,   true)    \
    DO_PARAM(FLOCKING,  Cs,      1.0)     \
    DO_PARAM(FLOCKING,  Ca,      1.0)     \
    DO_PARAM(FLOCKING,  Cc,      1.0)     \
    DO_PARAM(FLOCKING,  Cg,      1.0)     \
    DELIM_GROUP                           \
    DO_PARAM(BOID,      color,   "1,1,0") \
    DO_PARAM(BOID,      size,    1.0)     \
    DO_PARAM(BOID,      mass,    1.0)     \
    DO_PARAM(BOID,      rA,      0.8)     \
    DO_PARAM(BOID,      rS,      0.2)     \
    DO_PARAM(BOID,      perc,   10.0)     \
    DO_PARAM(BOID,      vision, 10.0)     \
    DO_PARAM(BOID,      maxRoll, 0.4)     \
    DO_PARAM(BOID,      rollD,   0.05)    \
    DO_PARAM(BOID,      maxPitch,0.5)     \
    DO_PARAM(BOID,      pitchD,  0.02)    \
    DO_PARAM(BOID,      thrust,  1.0)     \
    DELIM_GROUP

//----------------------------------------------------------------------------

  #define DO_PARAM(group,param,val) vigo::extras::Config::config_dat(#group,#param,group::param,val),
  #define DELIM_GROUP
  vigo::extras::Config::config_dat cfg[] =
  {
    DO_PARAMS
    vigo::extras::Config::config_dat() // sentinel
  };
  #undef DO_PARAM
  #undef DELIM_GROUP

  vigo::extras::Config config;

  void Load(pcstr fname)
  {
    config.Load(fname);
    config.Save(cfg);
  }

  #define DO_PARAM(group,param,val) \
    os << std::setw(24) << #group << "::" << #param << " = " << group::param << std::endl;
  #define DELIM_GROUP         \
    os << std::endl;
  void Print(std::ostream& os)
  {
    DO_PARAMS
  };
  #undef DO_PARAM
  #undef DELIM_GROUP

//----------------------------------------------------------------------------

  } // namespace

// eof
