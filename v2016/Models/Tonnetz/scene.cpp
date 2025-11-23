// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

  #include "scene.hpp"
  #include "tagent.hpp"

//-----------------------------------------------------------------------------

  Scene::Scene()
  {
    SetNumActPhases(1);
  }


  std::string Scene::StatusText() const
  {
    TAgent *ta = dynamic_cast<TAgent*>(SelectedObject());
    if(!ta) return "";
    return vigo::sformat("f=%.2f",ta->Freq());
  }


// eof
