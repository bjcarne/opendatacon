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
/**
 */
#include <catch.hpp>

#include "DNP3OutstationPort.h"
#include "DNP3MasterPort.h"
#include "PortLoader.h"

#define SUITE(name) "DNP3PortEndToEndTestSuite - " name

TEST_CASE(SUITE("ConstructEnableDisableDestroy"))
{
	asio::io_service IOS(std::thread::hardware_concurrency());
	ODC::Context context("TEST", IOS);
	
	{
		//make an outstation port
		ODC::NewPortFunctionT* newOutstation = GetPortCreator("DNP3Port", "DNP3Outstation");
		REQUIRE(newOutstation);
		
		Json::Value Oconf;
		Oconf["IP"] = "0.0.0.0";
		Oconf["Port"] = 20001;
		ODC::DataPort* OPUT = newOutstation("OutstationUnderTest", context, "", Oconf);
		
		//make a master port
		ODC::NewPortFunctionT* newMaster = GetPortCreator("DNP3Port", "DNP3Master");
		REQUIRE(newMaster);
		
		Json::Value Mconf;
		Mconf["ServerType"] = "PERSISTENT";
		Mconf["Port"] = 20001;
		ODC::DataPort* MPUT = newMaster("MasterUnderTest", context, "", Mconf);
		
		auto pDNP3Man = new asiodnp3::DNP3Manager(std::thread::hardware_concurrency());
		auto pLOG_LEVEL = new openpal::LogFilters();
		
		//get them to build themselves using their configs
		OPUT->BuildOrRebuild();
		MPUT->BuildOrRebuild();
		
		//turn them on
		OPUT->Enable();
		MPUT->Enable();
		
		//TODO: write a better way to wait for GetStatus and timeout (when decouple gets merged)
		uint count = 0;
		while(OPUT->GetStatus()["Result"].asString() == "Port enabled - link down" && count < 10000)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			count++;
		}
		
		REQUIRE(MPUT->GetStatus()["Result"].asString() == "Port enabled - link up (unreset)");
		REQUIRE(OPUT->GetStatus()["Result"].asString() == "Port enabled - link up (unreset)");
		
		delete MPUT;
		delete OPUT;
		delete pLOG_LEVEL;
		delete pDNP3Man;
	};
}
