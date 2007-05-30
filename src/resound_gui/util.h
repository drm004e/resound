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
#ifndef SA_UTIL_H
#define SA_UTIL_H

// usefull utils globaly defined

namespace SA{

inline float RANGEMAPF(float v, float iMin, float iMax, float oMin, float oMax)
{
	// (iMax - iMin) // i range
	// (oMax - oMin) // o range
	return ((v - iMin) / (iMax - iMin)) * (oMax - oMin) + oMin;
}

// return a mapped value from one range to another
inline int RANGEMAP(int v, int iMin, int iMax, int oMin, int oMax)
{
	// (iMax - iMin) // i range
	// (oMax - oMin) // o range
	return int( RANGEMAPF(float(v), float(iMin), float(iMax), float(oMin), float(oMax)));
}

inline int CLAMP(int v, int min, int max)
{
	if(v < min)
		return min;
	if(v > max)
		return max;
	return v;
}

inline float CLAMPF(float v, float min, float max)
{
	if(v < min)
		return min;
	if(v > max)
		return max;
	return v;
}

}

#endif
