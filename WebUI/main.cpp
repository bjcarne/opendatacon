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
//
//  main.cpp
//  opendatacon
//
//  Created by Alan Murray on 30/11/2014.
//
//

#include "WebUI.h"

extern "C" ODC::NewPluginFunctionT new_WebUIPlugin;
extern "C" IUI* new_WebUIPlugin(const std::string& aName, Context& aParent, const std::string& Filename, const Json::Value& aOverrides)
{
	std::string ip = "0.0.0.0";
	uint16_t port = 443;
	if(aOverrides.isObject())
	{
		if(!aOverrides["IP"].isNull())
			ip= aOverrides["IP"].asString();

		if(!aOverrides["Port"].isNull())
			port = aOverrides["Port"].asUInt();
	}

	return new WebUI(aName, aParent, port);
}
