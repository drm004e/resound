#    Resound
#    Copyright 2007 David Moore and James Mooney
#
#    This file is part of Resound.
#
#    Resound is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    Resound is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with Resound; if not, write to the Free Software
#    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

# resound
# includes
include(FindBoost)

include(UsePkgConfig)

PKGCONFIG(libxml++-2.6 LIBXML_INCLUDE_DIRS LIBXML_LIB_DIRS LIBXML_LINK_FLAGS LIBXML_CFLAGS)



include_directories (
${RESOUND_SOURCE_DIR}/
/usr/include/jack
${Boost_INCLUDE_DIRS}
)



link_directories(
${Boost_LIBRARY_DIRS}
${LIBXML_LIB_DIRS}
)

# use dbuggin flags
IF(UNIX)
	SET(CMAKE_CXX_FLAGS "-g ${LIBXML_CFLAGS}")
ELSEIF(APPLE)
	SET(CMAKE_CXX_FLAGS "-g")
ELSE(UNIX)
ENDIF(UNIX)

# build the excecutable
SET(sourcefiles
	core_objects.hpp
	core_objects.cpp
	dynamicobject.hpp
	dynamicobject.cpp
	engine.hpp
	engine.cpp
	jackengine.hpp
	jackengine.cpp
	main.hpp
	main.cpp
	oscmanager.hpp
	oscmanager.cpp
)
IF(UNIX)
	add_executable (resoundnv ${sourcefiles})
ELSEIF(APPLE)
	FILE(MAKE_DIRECTORY resoundnv.app)
	FILE(MAKE_DIRECTORY resoundnv.app/Contents)
	FILE(MAKE_DIRECTORY resoundnv.app/Contents/MacOS)
	FILE(MAKE_DIRECTORY resoundnv.app/Resources)
	add_executable (resoundnv MACOSX_BUNDLE ${sourcefiles})
ELSE(UNIX)
ENDIF(UNIX)
#link with libs
target_link_libraries (resoundnv jack boost_program_options lo xml++-2.6)
