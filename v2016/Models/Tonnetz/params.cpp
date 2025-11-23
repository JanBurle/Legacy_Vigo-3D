// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

  #include "params.hpp"
  #include <vigoext>
  #include <iomanip>

  namespace params {

//----------------------------------------------------------------------------

  uint        APP::port;

  std::string SC::host;
  uint        SC::port;

  std::string Tones::fundName;
  double      Tones::fundHz;

  double      Tones::lowCutoff, Tones::highCutoff;

  uint        Tones::num5up;
  uint        Tones::num5dn;
  uint        Tones::num3up;
  uint        Tones::num3dn;

//----------------------------------------------------------------------------

  #define DO_PARAMS                      \
    DO_PARAM(APP,     port, 7002)        \
    DELIM_GROUP                          \
    DO_PARAM(SC,      host, "localhost") \
    DO_PARAM(SC,      port, 7000)        \
    DELIM_GROUP                          \
    DO_PARAM(Tones, fundName, "a")       \
    DO_PARAM(Tones, fundHz,   440.)      \
    DO_PARAM(Tones, lowCutoff,16.)       \
    DO_PARAM(Tones, highCutoff,18e3)     \
    DO_PARAM(Tones, num5up,   99)        \
    DO_PARAM(Tones, num5dn,   99)        \
    DO_PARAM(Tones, num3up,   99)        \
    DO_PARAM(Tones, num3dn,   99)        \
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
