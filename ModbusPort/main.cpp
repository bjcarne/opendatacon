/*	opendatacon
 *
 *	Copyright (c) 2014:
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
 * main.cpp
 *
 *  Created on: 16/10/2014
 *      Author: Alan Murray
 */

#include "ModbusOutstationPort.h"
#include "ModbusMasterPort.h"

extern "C" ODC::NewPortFunctionT new_ModbusMasterPort;
extern "C" ODC::DataPort* new_ModbusMasterPort(std::string& aName, Context& aParent, std::string& aFilename, const Json::Value& aOverrides)
{
	return new ModbusMasterPort(aName,aParent,aFilename,aOverrides);
}

extern "C" ODC::NewPortFunctionT new_ModbusOutstationPort;
extern "C" ODC::DataPort* new_ModbusOutstationPort(std::string& aName, Context& aParent, std::string& aFilename, const Json::Value& aOverrides)
{
	return new ModbusOutstationPort(aName,aParent,aFilename,aOverrides);
}
