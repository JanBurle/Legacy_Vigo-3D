/*****************************************************************************\
*    Copyright (c) 2006 by Ian G Burleigh           burleigh@cpsc.ucalgary.ca *
*    https://github.com/phi53/Vigo-3D                                         *
*                                                                             *
*    This program is free software; you can redistribute it and/or modify     *
*    it under the terms of the GNU General Public License as published by     *
*    the Free Software Foundation; either version 2 of the License, or        *
*    (at your option) any later version.                                      *
*                                                                             *
*    This program is distributed in the hope that it will be useful,          *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
*    GNU General Public License for more details.                             *
*                                                                             *
*    You should have received a copy of the GNU General Public License        *
*    along with this program; if not, write to the                            *
*    Free Software Foundation, Inc.,                                          *
*    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                *
*-----------------------------------------------------------------------------*
* $Id: tscene.hpp 47 2006-06-21 10:34:49Z ian_b $
\*****************************************************************************/

/** \file
 */

#ifndef TONNETZ_TSCENE_HPP_INCLUDED
#define TONNETZ_TSCENE_HPP_INCLUDED

  #include "scene.hpp"
  #include "tagent.hpp"

//----------------------------------------------------------------------------

  class TSpace: public vigo::geom::Curve3 // hack
  {
    SUPER(vigo::geom::Curve3)

  public:
    TSpace(): super(0,0) {}

    Pointd ParFun(double u) const
    {
      double x, y;
      y = floor(u/1000.); x = u-y;
      return Pointd(x,y,0.);
    }
  };

//-----------------------------------------------------------------------------
    
  class TonnetzScene: public Scene
  {
    SUPER(Scene)

  public:
    TonnetzScene();

  private:
    void Create();

  public:
    void SetFreq(float freq,bool just);

  protected:
    void DisplayRoot(vigo::Camera const&,vigo::space::sceneGL::eDisplay);

  private:
    vigo::util::refptr<TSpace>  m_space;
    std::vector<TAgent*>	m_agents;
  };


//-----------------------------------------------------------------------------

#endif

//** eof **********************************************************************
