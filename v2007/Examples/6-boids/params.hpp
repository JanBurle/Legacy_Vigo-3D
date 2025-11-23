/*****************************************************************************\
*    Copyright (c) 2004--2007 by Ian G Burleigh                               *
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
* $Id: main.cpp 46 2006-06-21 10:33:26Z ian_b $
\*****************************************************************************/

#ifndef BOIDS_PARAMS_HPP_INCLUDED
#define BOIDS_PARAMS_HPP_INCLUDED

  #include <vigobase>
  #include <vigoext>

  namespace params {

//----------------------------------------------------------------------------

  struct APP // application params
  {
    static uint        numBoids;
    static std::string boidSource;
    static float       boidSpread;
  };

  struct CAMERA
  {
    static double      far;
    static std::string eye;
    static std::string center;
    static std::string up;
    static std::string lightAmbColor;
    static std::string lightDiffColor;
    static std::string lightPosition;
  };

  struct PHYSICS
  {
    static float Cd;      // drag coeff
    static float Cl;      // lift coeff
    static float rho2;    // fluid (air) density
    static float gee;     // g
  };

  struct FLOCKING
  {
    static bool  flock;   // flocking on/off
    static float Cs;      // separation
    static float Ca;      // alignment
    static float Cc;      // cohesion
    static float Cg;      // global goal
  };

  struct BOID
  {
    static std::string color;
    static float size;
    static float mass;
    static float rA;      // ratio: wing area/size^2
    static float rS;      // ration: front cross-section/size^2
    static float perc;    // perception distance (flockmates)
    static float vision;  // vision distance (obstacles)
    static float maxRoll; // maximum roll allowed
    static float rollD;   // roll delta between simulation steps
    static float maxPitch;// maximum pitch allowed
    static float pitchD;  // pitch delta between simulation steps
    static float thrust;
  };

//----------------------------------------------------------------------------

  extern vigo::extras::Config config;

  void Load(pcstr fname);
  void Print(std::ostream& os);

//----------------------------------------------------------------------------

  } // namespace

#endif

/*** eof ********************************************************************/
