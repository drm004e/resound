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



# includes
INCLUDE_DIRECTORIES(
${RESOUND_SOURCE_DIR}/
/usr/include/wx-2.6/
)


# use dbuggin flags
SET(CMAKE_CXX_FLAGS -g )


FIND_PACKAGE(wxWidgets)
INCLUDE(UsewxWidgets)

# build the excecutable
add_executable (resound_gui
	amclient.cpp
	amclient.h
	amclient_widgets.h
	amclient_widgets.cpp
	app.cpp
	app.h
	automation.cpp
	automation.h
	basebehaviour.cpp
	basebehaviour.h
	base.cpp
	base.h
	behaviour.cpp
	behaviour.h
	behaviourview.cpp
	behaviourview.h
	coherency.cpp
	coherency.h
	faderwidget.cpp
	faderwidget.h
	masterfader.cpp
	masterfader.h
	mmanager.cpp
	mmanager.h
	mmanagerwin32.cpp #win32
	mmanagerwin32.h #win32
	monitorframe.cpp
	monitorframe.h
	monitornodewidget.cpp
	monitornodewidget.h
	monitorview.cpp
	monitorview.h
	performance.cpp
	performance.h
	performanceview.cpp
	performanceview.h
	pvar.cpp
	pvar.h
	pvarwidgets.cpp
	pvarwidgets.h
	vumeterwidget.cpp
	vumeterwidget.h
	util.h
)
#link with libs
target_link_libraries (resound_gui resound_common boost_serialization ${wxWidgets_LIBRARIES})
