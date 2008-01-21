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

# libresound_client


include(FindBoost)

# includes
IF(UNIX)
	INCLUDE_DIRECTORIES(
		${RESOUND_SOURCE_DIR}/
		${Boost_INCLUDE_DIRS}
	)
ELSEIF(APPLE)
	INCLUDE_DIRECTORIES(
		${RESOUND_SOURCE_DIR}/
		${Boost_INCLUDE_DIRS}
	)
ELSE(UNIX)
ENDIF(UNIX)

LINK_DIRECTORIES(
	${Boost_LIBRARY_DIRS}
)

# use dbuggin flags
IF(UNIX)
	SET(CMAKE_CXX_FLAGS "-g -DUSE_MIDI -DRESOURCE_DIR='\"${CMAKE_INSTALL_PREFIX}/share/resound_gui\"'")
ELSEIF(APPLE)
	SET(CMAKE_CXX_FLAGS "-g")
ELSE(UNIX)
ENDIF(UNIX)

# build the excecutable
SET(sourcefiles 
	amclient.cpp
	resound_client/amclient.h
	automation.cpp
	resound_client/automation.h
	basebehaviour.cpp
	resound_client/basebehaviour.h
	behaviour.cpp
	resound_client/behaviour.h
	mmanager.cpp
	resound_client/mmanager.h
	mmanager_alsa.cpp #linux alsa
	resound_client/mmanager_alsa.h #linux alsa
	mmanagerwin32.cpp #win32
	resound_client/mmanagerwin32.h #win32
	performance.cpp
	resound_client/performance.h
	pvar.cpp
	resound_client/pvar.h
	resound_client/util.h
)

IF(UNIX)
	add_library (resound_client ${sourcefiles})
ELSEIF(APPLE)
	add_library (resound_client ${sourcefiles})
ELSE(UNIX)
ENDIF(UNIX)

#link with libs
IF(UNIX)
	target_link_libraries (resound_client boost_program_options resound_common boost_serialization asound)
ELSEIF(APPLE)
	target_link_libraries (resound_client boost_program_options resound_common boost_serialization)
ELSE(UNIX)
ENDIF(UNIX)

INSTALL(TARGETS resound_client DESTINATION lib)
