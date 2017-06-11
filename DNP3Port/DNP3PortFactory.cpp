/*	opendatacon
 *
 *	Copyright (c) 2017:
 *
 *		DCrip3fJguWgVCLrZFfA7sIGgvx1Ou3fHfCxnrz4svAi
 *		yxeOtDhDCXf1Z4ApgXvX5ahqQmzRfJ2DoX8S05SqHA==
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *		http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 */
/*
 * DNP3PortFactory.cpp
 *
 *  Created on: 09/05/2017
 *      Author: Alan Murray <alan@atmurray.net>
 */

#include "DNP3PortFactory.h"
#include "DNP3OutstationPort.h"
#include "DNP3MasterPort.h"

DNP3PortFactory::DNP3PortFactory(std::shared_ptr<odc::IOManager> pIOMgr) : Manager(new DNP3PortManager(pIOMgr)) {
	std::cout << "Constructing DNP3PortFactory" << std::endl;
}

DNP3PortFactory::~DNP3PortFactory() {
	std::cout << "Destructing DNP3PortFactory" << std::endl;
}

DNP3PortFactory* DNP3PortFactory::Get(std::shared_ptr<odc::IOManager> pIOMgr) {
	return new DNP3PortFactory(pIOMgr);
}

DataPort* DNP3PortFactory::CreateDataPort(const std::string& Type, const std::string& Name, const std::string& File, const Json::Value& Overrides) {
	if (Type.compare("DNP3Outstation") == 0)
		return new DNP3OutstationPort(Manager, Name, File, Overrides);
	if (Type.compare("DNP3Master") == 0)
		return new DNP3MasterPort(Manager, Name, File, Overrides);
	return nullptr;
}