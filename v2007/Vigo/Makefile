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
# $Id: Makefile 191 2006-12-19 14:41:13Z ian_b $
###############################################################################

.PHONY: all vigo check dox clean help

all vigo check:
	$(MAKE) -f vigo.mak $@

dox:
	doxygen
	
clean:
	$(MAKE) -f vigo.mak $@ OMIT_DEPS=1

help:
	echo "usage: make [all|vigo|check|dox|clean]"

# eof
########################################################################
