// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

  #include "./resource.hpp"

NAMESPACE_VIGO(res) //=========================================================

  std::string const source_host    = "vigo3d.net";

  #define MAJOR 1
  #define MINOR 1

  std::string const name           = "Vigo::3D";
  uint        const version        = MAJOR*100 + MINOR;
  std::string const version_string = sformat("%u.%uGPL",MAJOR,MINOR);
  std::string const name_version   = name + " ver." + version_string;
  std::string const url            = "http://vigo3d.net";
  std::string const copyright      = "Copyright (c) 2002-2016 Jan Burle";
  std::string const statusCopyright= sformat("%s v%s",
                                       name.c_str(),version_string.c_str());
  std::string const licence        = "GNU General Public License (GPL)";
  std::string const about          = sformat("| %s\n| %s\n| %s\n| http://%s",
                                             name_version.c_str(),
                                             copyright.c_str(),
                                             licence.c_str(),source_host.c_str());

  std::string const GPLnotice      = name_version
                                   + " http://"+source_host+"\n"
                                   + copyright +
    "\nVigo is free software, covered by the GNU General Public Licence.\n"
    "There is absolutely no warranty for Vigo.\n\n";

  std::string const& VigoText()
  {
	const time_t period = 12, urlTime = 3;

	time_t t = (time(NULL) % period);
	return (t < urlTime) ? url : statusCopyright;
  }

NAMESPACE_END() // eof
