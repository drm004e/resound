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

#ifndef __VIRTUAL_SURFACE_H
#define __VIRTUAL_SURFACE_H

namespace resound{
/// an abstract base class for virtual representations of physical controls on hardware surfaces
class VirtualPhysicalControl{
public:
	VirtualPhysicalControl();
	virtual VirtualPhysicalControl();
	
	/// this should be overloaded to send feedback to the slaved physical control, ie update virtual to real
	virtual void send_feedback();
	
protected:
	Collective m_collective;
};

typedef boost::shared_ptr<VirtualPhysicalControl> VirtualPhysicalControlPtr;
typedef std::vector<VirtualPhysicalControlPtr> VirtualPhysicalControlList;

/// a class for managing VirtualPhysicalControls as would be found on a control surface
class VirtualControlSurface{
public:
	VirtualSurface();
	virtual ~VirtualSurface();
	void add(VirtualPhysicalControlPtr control);
protected:
	VirtualPhysicalControlList m_controls;
};

/*
/// a virtual representation of a fader
class VirtualFader : public VirtualPhysicalControl{
public:
	VirtualFader();
	virtual ~VirtualFader();
	void set_value(int value);
	
};*/

}

#endif
