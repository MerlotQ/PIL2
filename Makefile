################################################################################
#
#  Pilot Intelligence Library
#    http://www.pilotintelligence.com/
#
#  ----------------------------------------------------------------------------
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program. If not, see <http://www.gnu.org/licenses/>.
#
################################################################################


################################################################################
# PIL top level makefile
################################################################################

export MAKE             = make -j4
export MAKE_OS          = linux # win32 mac

export TOPDIR           = $(shell pwd)

#Where to put object files *.o
export BUILD_PATH       = $(TOPDIR)/build

#Where to put final applications
export BIN_PATH         = $(TOPDIR)/bin

#Where to put libraries
export LIBS_PATH        = $(TOPDIR)/libs
export LIB_MAKE_TYPE    = shared # static #


.PHONY: mkdirs all apps libs
all: mkdirs libs apps

libs: mkdirs
	@echo "Compiling librarys of PIL"
	$(MAKE) -C src

apps: mkdirs libs
	@echo "Compiling apps of PIL"
	$(MAKE) -C apps


.PHONY:
mkdirs:
	@if [ -d ./libs ];  then echo "dir 'libs' created"; else mkdir -p ./libs; fi
	@if [ -d ./build ];  then echo "dir 'build' created"; else mkdir -p ./build; fi

	
clean :
	rm -r $(BUILD_PATH)/* 


