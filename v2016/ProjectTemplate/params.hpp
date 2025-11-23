// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

#ifndef VIGOPROJECT_PARAMS_HPP_INCLUDED
#define VIGOPROJECT_PARAMS_HPP_INCLUDED

  #include <vigobase>

  namespace params {

//----------------------------------------------------------------------------

  struct APP // application params
  {
	//>>> declare params here
    static uint param; // dummy
  };

//----------------------------------------------------------------------------

  void Load(pcstr fname);
  void Save(pcstr fname);

  void Print(std::ostream& os);

//----------------------------------------------------------------------------

  } // namespace

#endif

// eof

