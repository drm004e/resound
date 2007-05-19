/**********************************************************************************
    Resound
    Copyright 2007 David Moore and James Mooney 

    This file is part of Resound.

    Resound is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Resound is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Resound; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

************************************************************************************/

#ifndef __AUDIO_MATH_H
#define __AUDIO_MATH_H

#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

namespace Resound{

inline float dB_to_float(const float& dB){
	return std::pow(10.0f,dB/20.0f);
}
inline double dB_to_double(const double& dB){
	return std::pow(10.0,dB/20.0);
}
inline float float_to_dB(const float& v){
	return 20.0f * std::log10(fabs(v));
}
inline double double_to_dB(const float& v){
	return 20.0 * std::log10(fabs(v));
}

// usefull ascii codes +- = Plus or minus = 177
// infinity 8 on side = 165 ??

/// return a string containing a formated dB value such as -60.00 dB dp specifies decimal places
/// out of range errors return -infinity character char
inline std::string float_to_dB_string(const float& v, int dp){
	std::stringstream s;
	s << std::setprecision(dp)  << std::fixed << float_to_dB(v) << " dB";
	return s.str();
}

}

#endif