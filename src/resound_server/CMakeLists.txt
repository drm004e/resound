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

# resound_server
# includes
include(FindBoost)

include_directories (
${RESOUND_SOURCE_DIR}/
/usr/include/jack
${Boost_INCLUDE_DIRS}
)

link_directories(
${Boost_LIBRARY_DIRS}
)

# use dbuggin flags
IF(UNIX)
	SET(CMAKE_CXX_FLAGS "-g")
ELSEIF(APPLE)
	SET(CMAKE_CXX_FLAGS "-g")
ELSE(UNIX)
ENDIF(UNIX)

# build the excecutable
SET(sourcefiles 
resound_server.cpp
dsp_manager.cpp
audio_buffer.cpp
)
IF(UNIX)
	add_executable (resound_server ${sourcefiles})
ELSEIF(APPLE)
	FILE(MAKE_DIRECTORY resound_server.app)
	FILE(MAKE_DIRECTORY resound_server.app/Contents)
	FILE(MAKE_DIRECTORY resound_server.app/Contents/MacOS)
	FILE(MAKE_DIRECTORY resound_server.app/Resources)
	add_executable (resound_server MACOSX_BUNDLE ${sourcefiles})
ELSE(UNIX)
ENDIF(UNIX)
#link with libs
target_link_libraries (resound_server resound_common jack boost_program_options)
