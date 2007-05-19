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

# resound_common
# includes
include_directories (
${RESOUND_SOURCE_DIR}/
/usr/include/wx-2.6/
)
SET(CMAKE_CXX_FLAGS -g)
# now include the wx_script, must be done after setting cxx_flags because it adds to them
INCLUDE(Use_wxWindows)

# build the excecutable
add_library (resound_common
comms.cpp
systemlog.cpp
osc_manager.cpp
)
#link with libs
target_link_libraries (resound_common lo)
