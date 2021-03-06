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

# resound_gui


include(FindBoost)

# includes
IF(UNIX)
	INCLUDE_DIRECTORIES(
		${RESOUND_SOURCE_DIR}/
		${RESOUND_SOURCE_DIR}/libresound_client
		${Boost_INCLUDE_DIRS}
	)
ELSEIF(APPLE)
	INCLUDE_DIRECTORIES(
		${RESOUND_SOURCE_DIR}/
		${Boost_INCLUDE_DIRS}
		/opt/local/lib/wx/include/mac-unicode-release-2.8 
		/opt/local/include/wx-2.8
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
	SET(CMAKE_CXX_FLAGS "-g -D__WXMAC__")
ELSE(UNIX)
ENDIF(UNIX)

IF(UNIX)
	FIND_PACKAGE(wxWidgets)
	INCLUDE(UsewxWidgets)
ENDIF(UNIX)

# build the excecutable
SET(sourcefiles 
	amclient_widgets.h
	amclient_widgets.cpp
	app.cpp
	app.h
	base.cpp
	base.h
	behaviourview.cpp
	behaviourview.h
	faderwidget.cpp
	faderwidget.h
	masterfader.cpp
	masterfader.h
	monitorframe.cpp
	monitorframe.h
	monitornodewidget.cpp
	monitornodewidget.h
	monitorview.cpp
	monitorview.h
	performanceview.cpp
	performanceview.h
	pvarwidgets.cpp
	pvarwidgets.h
	vumeterwidget.cpp
	vumeterwidget.h
)

IF(UNIX)
	add_executable (resound_gui ${sourcefiles})
ELSEIF(APPLE)
	FILE(MAKE_DIRECTORY resound_gui.app)
	FILE(MAKE_DIRECTORY resound_gui.app/Contents)
	FILE(MAKE_DIRECTORY resound_gui.app/Contents/MacOS)
	FILE(MAKE_DIRECTORY resound_gui.app/Resources)
	add_executable (resound_gui MACOSX_BUNDLE ${sourcefiles})
ELSE(UNIX)
ENDIF(UNIX)

#link with libs
IF(UNIX)
	target_link_libraries (resound_gui resound_client boost_program_options resound_common boost_serialization asound ${wxWidgets_LIBRARIES})
ELSEIF(APPLE)
	target_link_libraries (resound_gui resound_client boost_program_options resound_common boost_serialization wx_macu-2.8)
ELSE(UNIX)
ENDIF(UNIX)

INSTALL(DIRECTORY image DESTINATION share/resound_gui)

INSTALL(TARGETS resound_gui DESTINATION bin)
