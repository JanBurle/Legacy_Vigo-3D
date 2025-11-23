// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

#ifndef VIGO_QTVIGO_APPLICATION_HPP_INCLUDED
#define VIGO_QTVIGO_APPLICATION_HPP_INCLUDED

  #include "../vigo/application.hpp"
  #include "../common/pimpl.hpp"
  #include <QtGui/QApplication>
  #include <vector>

//-----------------------------------------------------------------------------

  class QtVigoApplication_: protected vigo::internal::pimpl_iface<vigo::Application>
  {
    //SUPER(QApplication)
    NO_GEN(QtVigoApplication_)
    //Q_OBJECT

  public:
    // screen layout: default or as requested on the command line
    struct layout_t
    {
      int  x,y,w,h;
      bool fullscreen, showdock;

      layout_t()
        : x(0), y(0), w(0), h(0),
          fullscreen(false), showdock(false)
      {
      }

      bool IsSpecified() const
      {
        return x!=0 || y!=0 || w!=0 || h!=0;
      }
    };

  public:
    QtVigoApplication_(vigo::Application& iface,
                       pcstr name,int argc,pstr argv[]);
    virtual ~QtVigoApplication_();

    // global access to the application object
    static QtVigoApplication_& Get();

    int Exec();
    void Exit(int code);

    // the main window position and size, as specified on the command line
    GETTER(WindowLayout, layout_t const&,    m_windowLayout)
    // configuration file name
    GETTER(CfgFileName,  std::string const&, m_cfgFileName)

  public:
    static void InfoPopup(pcstr msg);
    static bool QuestionPopup(pcstr msg);
    static void ErrorPopup(pcstr msg);

  private:
    /** Extract the i-th argument's value as a string, starting with \c offset.
      *  Values may, but need not be separated, as in:
      * "-ffile" or "-f file".
      * Advance \c i
      */
    std::string ArgStr(int& i, uint offset=0);

    /// the i-th argument value as an integer
    int ArgInt(int& i, uint offset=0)
    {
      return atoi(ArgStr(i,offset).c_str());
    }

    /** Command-line switches description.
     *  Example: { "--showphysics",params::showPhysics,"display physics"}.
     *  Last row must have an empty tag.
     */
    struct sSwitch
    {
      sSwitch(pcstr tag_, bool* pValue_, pcstr helpText_)
        : tag(tag_ ? tag_ : ""), pValue(pValue_),
          helpText(helpText_ ? helpText_ : "")
      {
      }

      std::string tag;
      bool* pValue;
      std::string helpText;
    };

    /// Add a switch description for argument processing.
    void AddSwitch(sSwitch const& sw)
    {
      m_switches.push_back(sw);
    }

  public:
    void AddCmdlineSwitch(pcstr tag, bool& value, pcstr helpText);

    /**  Process all command-line arguments.
      *  Those that are switches may be followed by '+' or '-';
      *  '+' is the default.
      */
    void ProcessArgs();

    /// The complete help text.
    std::string Help();

  private:
    typedef std::vector<sSwitch> switches_t;

    int         m_argc;
    pstr       *m_argv;
    layout_t    m_windowLayout;
    switches_t  m_switches;
    std::string m_name;
    std::string m_cfgFileName;

    QApplication m_qApp; // dues to Qt's treatment of argc&
  };

//-----------------------------------------------------------------------------

#endif

// eof
