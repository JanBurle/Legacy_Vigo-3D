// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

  #include "./nodeparts.hpp"
  #include "../common/ogl.hpp"

NAMESPACE_VIGO(parts) //=======================================================

  space::tracked_grpid GetNewGrpId()
  {
	class GrpIdHolder {};
	pvoid p = new GrpIdHolder; // to get a unique memory address
	return reinterpret_cast<space::tracked_grpid>(p);
	// intentionally leaks (one word of) memory
  }

NAMESPACE_END() // eof
