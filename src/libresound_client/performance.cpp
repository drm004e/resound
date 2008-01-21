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

#include "resound_client/performance.h" // class's header file

#include <iostream>
#include <fstream>

#include "resound_client/amclient.h"


namespace Resound
{

//---------------------- ParameterLockPreset -------------------------------

ParameterLockPreset::ParameterLockPreset()
{}
ParameterLockPreset::~ParameterLockPreset()
{}


//-------------------- MasterFaderPreset ------------------------------

MasterFaderPreset::MasterFaderPreset() :
m_value(0)
{}
MasterFaderPreset::~MasterFaderPreset()
{}


//------------------------- PreformancePreset --------------------------

PerformancePreset::PerformancePreset()
{}
PerformancePreset::~PerformancePreset()
{}


//------------------------- Performance ------------------------------------

Performance::Performance() :
m_parameterNamespaceManager(new ParameterNamespaceManager())
{

}

Performance::~Performance()
{}
// -------------------------- PerformanceManager ---------------------------
PerformanceManager::PerformanceManager(){};
PerformanceManager::~PerformanceManager(){};
PerformanceManager* PerformanceManager::s_instance = 0;
PerformanceManager& PerformanceManager::get_instance(){
	if(!s_instance) s_instance = new PerformanceManager;
	return *s_instance;
}

void PerformanceManager::new_performance(){
	m_activePerformance = PerformancePtr(new Performance());
}

void PerformanceManager::save_performance_xml(const std::string& filepath){
    // make an archive

    std::ofstream ofs(filepath.c_str());
    assert(ofs.good());
    boost::archive::xml_oarchive oa(ofs);
    oa << BOOST_SERIALIZATION_NVP(m_activePerformance);
}

void PerformanceManager::load_performance_xml(const std::string& filepath){
    // open an archive
	PerformancePtr performance;
    std::ifstream ifs(filepath.c_str());
    assert(ifs.good());
    boost::archive::xml_iarchive ia(ifs);

    // restore the schedule from the archive
    ia >> BOOST_SERIALIZATION_NVP(m_activePerformance);

}

} // namespace Resound
