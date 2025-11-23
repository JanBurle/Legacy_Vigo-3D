// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
*/

  #include "./ogl.hpp"

  NAMESPACE_VIGO(ogl)

//-----------------------------------------------------------------------------

#ifdef _DEBUG
  void CheckGl(pcstr file, int line, pcstr msg) throw(exc)
#else
  void CheckGl(pcstr msg) throw(exc)
#endif
  {
    static uint const NUM_ERRORS = 6;
    static pcstr error_txt[NUM_ERRORS] =
    {
      "GL_INVALID_ENUM",
      "GL_INVALID_VALUE",
      "GL_INVALID_OPERATION",
      "GL_STACK_OVERFLOW",
      "GL_STACK_UNDERFLOW",
      "GL_OUT_OF_MEMORY"
    };

    GLenum r = glGetError();
    if(GL_NO_ERROR == r)
        return;

    std::string txt = (r-GL_INVALID_ENUM < NUM_ERRORS)
                      ? error_txt[r - GL_INVALID_ENUM]
                      : "";

#ifdef _DEBUG
    std::string smsg = sformat("GL error %u (%s) at %s, line %i",
                                r, txt.c_str(), file, line);
#else
    std::string smsg = sformat("GL error %u (%s)",
                                r, txt.c_str());
#endif
    if(!empty(msg))
        (smsg += "\n") += msg;

    THROW_EXC2(EXC_RUNTIME, smsg)
  }

NAMESPACE_END() // eof
