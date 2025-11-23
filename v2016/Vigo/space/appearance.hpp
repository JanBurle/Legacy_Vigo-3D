// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief Appearance classes.
 *
 */

#ifndef VIGO_SPACE_APPEARANCE_HPP_INCLUDED
#define VIGO_SPACE_APPEARANCE_HPP_INCLUDED

  #include "../common/refcnt.hpp"
  #include "./color.hpp"

NAMESPACE_VIGO(shape) //=======================================================
/** \defgroup grp_appearance_hpp Appearance
@{*/

  struct DrawFlags
  {
    uint flags;
    enum {flagSOLID   = 0x0001, //>>> not implemented
          flagWIRE    = 0x0002, //>>> not implemented
          inheritSW   = 0x0008, //>>> not implemented

          flagLIGHT   = 0x0010, //>>> not implemented?
          flagNOLIGHT = 0x0020,
          inheritLN   = 0x0080,

          inheritCLR  = 0x0100,
          inheritTEX  = 0x0200,

          drawSOLID   = 0x1000,   // mutable
          drawLIGHT   = 0x2000};  // mutable

    DrawFlags();
    DrawFlags(uint flags);

  public:
    bool IsFlag(uint f) const
    {
      return 0!=(flags&f);
    }

    bool IsSolid()   const { return IsFlag(flagSOLID);   }
    bool IsWire ()   const { return IsFlag(flagWIRE);    }
    bool InheritSW() const { return IsFlag(inheritSW);   }

    bool IsLight  () const { return IsFlag(flagLIGHT);   }
    bool IsNoLight() const { return IsFlag(flagNOLIGHT); }
    bool InheritLN() const { return IsFlag(inheritLN);   }

    bool InheritCLR() const { return IsFlag(inheritCLR); }
    bool InheritTEX() const { return IsFlag(inheritTEX); }

    void SetFlag(uint,bool);

    void SetSolid(bool solid)     { SetFlag(flagSOLID,solid);     }
    void SetWire (bool wire)      { SetFlag(flagWIRE, wire);      }

    void SetLight  (bool light)   { SetFlag(flagLIGHT,  light);   }
    void SetNoLight(bool noLight) { SetFlag(flagNOLIGHT,noLight); }

    void SetBits(uint,bool);
    /// sets drawXXXX flags
    void SetDraw(DrawFlags const* carryFrom) const;
  };

//-----------------------------------------------------------------------------

  class ShapeColorAlpha: public util::refcnt_auto, public ColorAlpha
  {
    SUPER(util::refcnt_auto)

  public:
    ShapeColorAlpha(): super(), ColorAlpha()
    {
    }

    ShapeColorAlpha(ColorAlpha const& ca): super(), ColorAlpha(ca)
    {
    }

    ShapeColorAlpha(double r,double g,double b,double a=1.0)
      : super(), ColorAlpha(r,g,b,a)
    {
    }

   ~ShapeColorAlpha()
    {
    }
  };

//-----------------------------------------------------------------------------

  class Texture: public util::refcnt_auto
  {
    SUPER(util::refcnt_auto)

  public:
    Texture(pcstr textureFile, uint width, uint height);
   ~Texture();

    void Activate()   const;
    void Deactivate() const;

  protected:
    uint m_texture;
  };

//----------------------------------------------------------------------------

  class Appearance: public util::refcnt_auto, public DrawFlags
  {
    SUPER(util::refcnt_auto)

  public:
    Appearance(uint flags=0x0000)
      : super(), DrawFlags(flags)
    {
    }

    Appearance(ShapeColorAlpha& ca,uint flags=0x0000)
      : super(), DrawFlags(flags)
    {
      SetColorAlpha(&ca);
    }

    /// \overload
    Appearance(double r, double g, double b, double a=1.0, uint flags=0x0000)
      : super(), DrawFlags(flags)
    {
      SetColorAlpha(new shape::ShapeColorAlpha(r,g,b,a));
    }

    Appearance(Texture& tx,uint flags=0x0000)
      : super(), DrawFlags(flags)
    {
      SetTexture(&tx);
    }

    GETTER(GetColorAlpha, ShapeColorAlpha*, m_colorAlpha.ptr())
    GETTER(GetTexture,    Texture*,         m_texture.ptr())

    SETTER(SetColorAlpha, ShapeColorAlpha*, m_colorAlpha)
    SETTER(SetTexture,    Texture*,         m_texture)

    GETTER(HasColorAlpha, bool, !m_colorAlpha.isNull())
    GETTER(HasTexture,    bool, !m_texture.isNull())

  private:
    util::refptr<ShapeColorAlpha> m_colorAlpha;
    util::refptr<Texture>         m_texture;
  };

/**@} (defgroup)*/
NAMESPACE_END() //=============================================================

#endif

// eof
