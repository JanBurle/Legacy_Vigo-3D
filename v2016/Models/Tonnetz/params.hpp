// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

#ifndef TONNETZ_PARAMS_HPP_INCLUDED
#define TONNETZ_PARAMS_HPP_INCLUDED

  #include <vigobase>

  namespace params {

//----------------------------------------------------------------------------

  struct APP // this application
  {
    static uint        port;  // OSC UDP  port
  };

  struct SC  // SuperCollider
  {
    static std::string host;  // OSC UDP host
    static uint        port;  // OSC UDP port
  };

//----------------------------------------------------------------------------

  struct Tones
  {
    static std::string fundName;
    static double      fundHz;

    static double      lowCutoff, highCutoff;

    static uint        num5up;
    static uint        num5dn;
    static uint        num3up;
    static uint        num3dn;
  };

//----------------------------------------------------------------------------

  void Load(pcstr fname);
  void Save(pcstr fname);

  void Print(std::ostream& os);

//----------------------------------------------------------------------------

  } // namespace

#endif

// eof

