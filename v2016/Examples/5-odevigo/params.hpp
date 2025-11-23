// Copyright (c) 2006 by Marcin Pilat http://pilat.org

#ifndef ODEVIGO_PARAMS_HPP_INCLUDED
#define ODEVIGO_PARAMS_HPP_INCLUDED

  #include <vigobase>

  namespace params {

//----------------------------------------------------------------------------

  struct APP // application params
  {
    static uint numAgents;
    static bool randPos;   // randomize the agents' initial position?
  };

//----------------------------------------------------------------------------

  void Load(pcstr fname);
  void Save(pcstr fname);

  void Print(std::ostream& os);

//----------------------------------------------------------------------------

  } // namespace

#endif

// eof

