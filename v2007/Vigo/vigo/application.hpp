/*****************************************************************************\
* This file is a part of Vigo::3D, a multi-agent simulation and visualization *
* system.                                                                     *
*                                                                             *
* Copyright (c) 2002--2006 by Ian G. Burleigh       burleigh@cpsc.ucalgary.ca *
* burleigh@cpsc.ucalgary.ca                         http://ard-galen.org/vigo *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for   *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program; if not, write to the                                     *
*   Free Software Foundation, Inc.,                                           *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                 *
*-----------------------------------------------------------------------------*
* $Id: application.hpp 45 2006-06-21 10:20:33Z ian_b $
\*****************************************************************************/

/** \file
 *  \brief Vigo application
 */

#ifndef VIGO_VIGO_APPLICATION_HPP_INCLUDED
#define VIGO_VIGO_APPLICATION_HPP_INCLUDED

  #include "../common/pimpl.hpp"

  class QtVigoApplication_;

NAMESPACE_VIGO_() //===========================================================
/** \defgroup grp_application_hpp Vigo application
@{*/

  class Application: protected internal::pimpl<QtVigoApplication_>
  {
    NO_GEN(Application)

  public:
    Application(pcstr name,int argc,pstr argv[]);
   ~Application();

    /// global access to the application object
    static Application& Get();

    /** Adds a command line switch (on/off) that the application understands.
     *  Example: AddCmdlineSwitch("--showphysics",&params::showPhysics,"display physics"}.
     *  @param tag      A command line tag.
     *  @param value    A boolean variable that will be set accordingly.
     *  @param helpText A short help text.
     */
    void AddCmdlineSwitch(pcstr tag, bool& value, pcstr helpText);
    
    /// Process command line arguments/
    void ProcessArgs();
    /// The main application loop.
    int  Exec();          
    /// Exit the program.
    /// @param code The error code (0=success).
    void Exit(int code);

    /// An application usage help text.
    std::string Help();

  public:
    /// The generated name of a configuration file. The application must handle the file itself.
    std::string const& CfgFileName() const;

  public:
    /// \name Message boxes.
    //@{
    /// Information message dialog.
    static void InfoPopup(pcstr);
    /// \overload
    static void InfoPopup(std::string const& msg)
    {
      InfoPopup(msg.c_str());
    }

    /// Question dialog.
    /// @return true==yes
    static bool QuestionPopup(pcstr);
    /// \overload
    static bool QuestionPopup(std::string const& msg)
    {
      return QuestionPopup(msg.c_str());
    }

    /// Error message dialog.
    static void ErrorPopup(pcstr);
    /// \overload
    static void ErrorPopup(std::string const& msg)
    {
      ErrorPopup(msg.c_str());
    }
    //@}
  };

/**@} (defgroup)*/
NAMESPACE_END_() //============================================================

#endif

//** eof **********************************************************************
