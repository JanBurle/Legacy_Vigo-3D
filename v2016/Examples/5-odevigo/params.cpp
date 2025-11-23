// Copyright (c) 2006 by Marcin Pilat http://pilat.org

  #include "params.hpp"
  #include <vigoext>
  #include <iomanip>

  namespace params {

//----------------------------------------------------------------------------

  // define params here
  uint  APP::numAgents;
  bool  APP::randPos;

//----------------------------------------------------------------------------

  #define DO_PARAMS                     \
    DO_PARAM(APP, numAgents, 86)        \
    DO_PARAM(APP, randPos,   true)      \
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

  void Load(pcstr fname)
  {
    vigo::extras::Config config;
    config.Load(fname);
    config.Save(cfg);
  }

  void Save(pcstr fname)
  {
    vigo::extras::Config config;
    config.Load(cfg);
    config.Save(fname);
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
