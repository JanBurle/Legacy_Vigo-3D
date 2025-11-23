// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

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

// eof
