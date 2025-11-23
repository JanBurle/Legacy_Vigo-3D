###############################################################################
# This file is a part of Vigo::3D, a multi-agent simulation and visualization #
# system.                                                                     #
#                                                                             #
# Copyright (c) 2002--2006 by Ian G. Burleigh       burleigh@cpsc.ucalgary.ca #
# burleigh@cpsc.ucalgary.ca                         http://ard-galen.org/vigo #
#                                                                             #
# This program is free software; you can redistribute it and/or modify it     #
# under the terms of the GNU General Public License as published by the Free  #
# Software Foundation; either version 2 of the License, or (at your option)   #
# any later version.                                                          #
#                                                                             #
# This program is distributed in the hope that it will be useful, but WITHOUT #
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       #
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for   #
# more details.                                                               #
#                                                                             #
# You should have received a copy of the GNU General Public License along     #
# with this program; if not, write to the                                     #
#   Free Software Foundation, Inc.,                                           #
#   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                 #
#-----------------------------------------------------------------------------#
# $Id: Makefile 219 2007-05-20 22:43:41Z ian_b $
###############################################################################

.PHONY: all clean

# note that 5-odevigo is not included in EXAMPLES (needs ode)
# to build 5-odevigo, cd to 5-odevigo and make ...

EXAMPLES = 1-application 2-gravity 3-neighbours 4-dock 6-boids 7-QtGui

all clean:
	for d in $(EXAMPLES); do echo "**** " $$d " ****"; (cd $$d && $(MAKE) $@); done

# eof
########################################################################
