//   
//   Copyright (c) 2007 David Moore, James Mooney, All Rights Reserved.
//   
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the GNU General Public License as
//   published by the Free Software Foundation; either version 2 of
//   the License, or (at your option) any later version.
//   
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//   GNU General Public License for more details.
//   
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 59 Temple Place, Suite 330, Boston,
//   MA 02111-1307 USA
//   
#ifndef __PCH_H__
#define __PCH_H__

// precompiled header
// makefile has been altered to deal with this

#include <exception>
#include <cstdlib>
#include <cmath>
#include <list>
#include <string>
#include <vector>
#include <map>


// wx headers
#include <wx/wx.h>
#include <wx/socket.h>
#include <wx/event.h>
#include <wx/dcbuffer.h>
#include <wx/notebook.h>
#include <wx/wfstream.h>
#include <wx/datstrm.h>
#include <wx/txtstrm.h>
#include <wx/tglbtn.h> // not sure bout this one
#include <wx/gbsizer.h>

// library headers
#include <resound_common/array2.h>
#include <resound_common/exception.h>
#include <resound_common/verbose.h>

// constants
const float PI = 3.141592f;
const float TWOPI = PI * 2.0f;

#endif
