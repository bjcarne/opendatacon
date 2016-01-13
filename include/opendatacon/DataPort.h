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
 * DataPort.h
 *
 *  Created on: 15/07/2014
 *      Author: Neil Stephens <dearknarl@gmail.com>
 */

#ifndef DATAPORT_H_
#define DATAPORT_H_

#include "DataPortConf.h"
#include "IOHandler.h"
#include "ConfigParser.h"
#include "IUIResponder.h"
#include "Plugin.h"

namespace ODC
{

class DataPort: public Plugin, public IOHandler
{
public:
	DataPort(std::string aName, Context& parent, std::string aConfFilename, const Json::Value aConfOverrides):
		Plugin(aName, parent, aConfFilename, aConfOverrides),
		IOHandler(aName),
		pConf(nullptr)
	{};
	virtual ~DataPort(){};

	std::future<CommandStatus> Event(ConnectState state, uint16_t index, const std::string& SenderName) final
	{
		if (MuxConnectionEvents(state, SenderName))
			return ConnectionEvent(state, SenderName);
		else
			return IOHandler::CommandFutureUndefined();
	};

	virtual std::future<CommandStatus> ConnectionEvent(ConnectState state, const std::string& SenderName) = 0;

	virtual const Json::Value GetStatistics() const
	{
		return Json::Value();
	};

	virtual const Json::Value GetCurrentState() const
	{
		return Json::Value();
	};

	virtual const Json::Value GetStatus() const
	{
		return Json::Value();
	};

protected:
	std::unique_ptr<DataPortConf> pConf;
};

typedef ODC::DataPort*(NewPortFunctionT)(std::string&, Context&, std::string&, const Json::Value&);

}

#endif /* DATAPORT_H_ */
