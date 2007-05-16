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
// Class automatically generated by Dev-C++ New Class wizard

#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include "systemlog.h" // class's header file

namespace SA
{

// class constructor
SystemLog::SystemLog()
		:fileStream(_T("system.log")), textStream(fileStream)
{
	textStream << _T("system.log begin... \n");
}

// class destructor
SystemLog::~SystemLog()
{
	textStream << _T("system.log end. \n");
}

SystemLog* SystemLog::singleton = 0;

SystemLog& SystemLog::GetSingleton()
{
	if(!singleton) {
		singleton = new SystemLog();
	}
	return *singleton;
}

void SystemLog::Destroy()
{
	if(singleton) {
		delete singleton;
	}
}

wxTextOutputStream& SystemLog::GetStream()
{
	return textStream;
}

} // namespace SA
