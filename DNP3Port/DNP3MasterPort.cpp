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
 * DNP3ClientPort.cpp
 *
 *  Created on: 15/07/2014
 *      Author: Neil Stephens <dearknarl@gmail.com>
 */

#include <asiodnp3/DefaultMasterApplication.h>
#include <opendnp3/app/ClassField.h>
#include <opendnp3/app/MeasurementTypes.h>
#include <opendnp3/LogLevels.h>
#include <asiopal/UTCTimeSource.h>

#include <array>

#include <opendatacon/LogLevels.h>

#include "OpenDNP3Helpers.h"
#include "DNP3MasterPort.h"
#include "CommandCallbackPromise.h"

void DNP3MasterPort::Enable()
{
	if(enabled)
		return;
	if(nullptr == pMaster)
	{
		std::string msg = Name + ": Port not configured.";
		auto log_entry = ODC::LogEntry("DNP3MasterPort", ODC::logflags::ERR, "", msg.c_str(), -1);
		Log(log_entry);
		return;
	}

	enabled = true;
	//initialise as comms down - in case they never come up
	PortDown();

	DNP3PortConf* pConf = static_cast<DNP3PortConf*>(this->pConf.get());
	if(!stack_enabled && pConf->mAddrConf.ServerType == server_type_t::PERSISTENT)
		EnableStack();

}
void DNP3MasterPort::Disable()
{
	if(!enabled)
		return;
	enabled = false;

	if(stack_enabled)
	{
		PortDown();

		stack_enabled = false;
		pMaster->Disable();
	}
}

void DNP3MasterPort::PortUp()
{
    auto eventTime = ODC::timestamp(asiopal::UTCTimeSource::Instance().Now().msSinceEpoch);
	DNP3PortConf* pConf = static_cast<DNP3PortConf*>(this->pConf.get());

	// Update the comms state point if configured
	if (pConf->mCommsPoint.first.quality & static_cast<uint8_t>(opendnp3::BinaryQuality::ONLINE))
	{
		for (auto IOHandler_pair : Subscribers)
		{
			{
				std::string msg = Name + ": Updating comms state point to good";
				auto log_entry = ODC::LogEntry("DNP3MasterPort", ODC::logflags::DBG, "", msg.c_str(), -1);
				this->Log(log_entry);
			}
			ODC::Binary commsUpEvent(!pConf->mCommsPoint.first.value, static_cast<uint8_t>(opendnp3::BinaryQuality::ONLINE), eventTime);
			IOHandler_pair.second->Event(commsUpEvent, pConf->mCommsPoint.second, this->Name);
		}
	}
}

void DNP3MasterPort::PortDown()
{
    auto eventTime = ODC::timestamp(asiopal::UTCTimeSource::Instance().Now().msSinceEpoch);
	DNP3PortConf* pConf = static_cast<DNP3PortConf*>(this->pConf.get());

	for (auto IOHandler_pair : Subscribers)
	{
		{
			std::string msg = Name + ": Setting point quality to COMM_LOST";
			auto log_entry = ODC::LogEntry("DNP3MasterPort", ODC::logflags::DBG, "", msg.c_str(), -1);
			Log(log_entry);
		}

		for (auto index : pConf->BinaryIndicies)
			IOHandler_pair.second->Event(opendnp3::BinaryQuality::COMM_LOST, index, this->Name);
		for (auto index : pConf->AnalogIndicies)
			IOHandler_pair.second->Event(opendnp3::AnalogQuality::COMM_LOST, index, this->Name);

		// Update the comms state point if configured
		if (pConf->mCommsPoint.first.quality & static_cast<uint8_t>(opendnp3::BinaryQuality::ONLINE))
		{
			{
				std::string msg = Name + ": Updating comms state point to bad";
				auto log_entry = ODC::LogEntry("DNP3MasterPort", ODC::logflags::DBG, "", msg.c_str(), -1);
				Log(log_entry);
			}
			ODC::Binary commsDownEvent(pConf->mCommsPoint.first.value, static_cast<uint8_t>(opendnp3::BinaryQuality::ONLINE), eventTime);
			IOHandler_pair.second->Event(commsDownEvent, pConf->mCommsPoint.second, this->Name);
		}
	}
}

// Called by OpenDNP3 Thread Pool
// Called when a the reset/unreset status of the link layer changes (and on link up)
void DNP3MasterPort::OnStateChange(opendnp3::LinkStatus status)
{
	this->status = status;
	if(link_dead)
	{

		link_dead = false;
		// Update the comms state point and qualities
		PortUp();
	}
	//TODO: track a statistic - reset count
}
// Called by OpenDNP3 Thread Pool
// Called when a keep alive message (request link status) receives no response
void DNP3MasterPort::OnKeepAliveFailure()
{
	if(!link_dead)
	{
		link_dead = true;
		PortDown();

		// Notify subscribers that a disconnect event has occured
		for (auto IOHandler_pair : Subscribers)
		{
			IOHandler_pair.second->Event(ODC::ConnectState::DISCONNECTED, 0, this->Name);
		}

		DNP3PortConf* pConf = static_cast<DNP3PortConf*>(this->pConf.get());
		if (stack_enabled && pConf->mAddrConf.ServerType != server_type_t::PERSISTENT && !InDemand())
		{
			std::string msg = Name + ": disabling stack following disconnect on non-persistent port.";
			auto log_entry = ODC::LogEntry("DNP3MasterPort", ODC::logflags::INFO, "", msg.c_str(), -1);
			Log(log_entry);

			// For all but persistent connections, and in-demand ONDEMAND connections, disable the stack
			Post([&]()
			     {
			           DisableStack();
			     });
		}
	}
}
// Called by OpenDNP3 Thread Pool
// Called when a keep alive message receives a valid response
void DNP3MasterPort::OnKeepAliveSuccess()
{
	if(link_dead)
	{
		link_dead = false;
		// Update the comms state point and qualities
		PortUp();
	}
}

void DNP3MasterPort::BuildOrRebuild()
{
	DNP3PortConf* pConf = static_cast<DNP3PortConf*>(this->pConf.get());

	pChannel = getTCPClient(pConf->mAddrConf);
	if (pChannel == nullptr)
	{
		std::string msg = Name + ": TCP channel not found for masterstation.";
		auto log_entry = ODC::LogEntry("DNP3MasterPort", ODC::logflags::ERR, "", msg.c_str(), -1);
		Log(log_entry);

		return;
	}

	opendnp3::MasterStackConfig StackConfig;

	// Link layer configuration
	StackConfig.link.LocalAddr = pConf->mAddrConf.MasterAddr;
	StackConfig.link.RemoteAddr = pConf->mAddrConf.OutstationAddr;
	StackConfig.link.NumRetry = pConf->LinkNumRetry;
	StackConfig.link.Timeout = openpal::TimeDuration::Milliseconds(pConf->LinkTimeoutms);
	if(pConf->LinkKeepAlivems == 0)
		StackConfig.link.KeepAliveTimeout = openpal::TimeDuration::Max();
	else
		StackConfig.link.KeepAliveTimeout = openpal::TimeDuration::Milliseconds(pConf->LinkKeepAlivems);
	StackConfig.link.UseConfirms = pConf->LinkUseConfirms;

	// Master station configuration
	StackConfig.master.responseTimeout = openpal::TimeDuration::Milliseconds(pConf->MasterResponseTimeoutms);
	StackConfig.master.timeSyncMode = pConf->MasterRespondTimeSync ? opendnp3::TimeSyncMode::SerialTimeSync : opendnp3::TimeSyncMode::None;
	StackConfig.master.disableUnsolOnStartup = !pConf->DoUnsolOnStartup;
	StackConfig.master.unsolClassMask = pConf->GetUnsolClassMask();
	StackConfig.master.startupIntegrityClassMask = pConf->GetStartupIntegrityClassMask(); //TODO: report/investigate bug - doesn't recognise response to integrity scan if not ALL_CLASSES
	StackConfig.master.integrityOnEventOverflowIIN = pConf->IntegrityOnEventOverflowIIN;
	StackConfig.master.taskRetryPeriod = openpal::TimeDuration::Milliseconds(pConf->TaskRetryPeriodms);

	pMaster = pChannel->AddMaster(Name.c_str(), *this, asiodnp3::DefaultMasterApplication::Instance(), StackConfig);
	if (pMaster == nullptr)
	{
		std::string msg = Name + ": Error creating masterstation.";
		auto log_entry = ODC::LogEntry("DNP3MasterPort", ODC::logflags::ERR, "", msg.c_str(), -1);
		Log(log_entry);

		return;
	}

	// Master Station scanning configuration
	if(pConf->IntegrityScanRatems > 0)
		IntegrityScan = pMaster->AddClassScan(opendnp3::ClassField::ALL_CLASSES, openpal::TimeDuration::Milliseconds(pConf->IntegrityScanRatems));
	else
		IntegrityScan = pMaster->AddClassScan(opendnp3::ClassField::ALL_CLASSES, openpal::TimeDuration::Minutes(600000000)); //ten million hours
	if(pConf->EventClass1ScanRatems > 0)
		pMaster->AddClassScan(opendnp3::ClassField::CLASS_1, openpal::TimeDuration::Milliseconds(pConf->EventClass1ScanRatems));
	if(pConf->EventClass2ScanRatems > 0)
		pMaster->AddClassScan(opendnp3::ClassField::CLASS_2, openpal::TimeDuration::Milliseconds(pConf->EventClass2ScanRatems));
	if(pConf->EventClass3ScanRatems > 0)
		pMaster->AddClassScan(opendnp3::ClassField::CLASS_3, openpal::TimeDuration::Milliseconds(pConf->EventClass3ScanRatems));
}

// Called by OpenDNP3 Thread Pool
//implement ISOEHandler
void DNP3MasterPort::Process(const HeaderInfo& info, const ICollection<Indexed<opendnp3::Binary> >& meas){ LoadT(meas); }
void DNP3MasterPort::Process(const HeaderInfo& info, const ICollection<Indexed<opendnp3::DoubleBitBinary> >& meas){ LoadT(meas); }
void DNP3MasterPort::Process(const HeaderInfo& info, const ICollection<Indexed<opendnp3::Analog> >& meas){ LoadT(meas); }
void DNP3MasterPort::Process(const HeaderInfo& info, const ICollection<Indexed<opendnp3::Counter> >& meas){ LoadT(meas); }
void DNP3MasterPort::Process(const HeaderInfo& info, const ICollection<Indexed<opendnp3::FrozenCounter> >& meas){ LoadT(meas); }
void DNP3MasterPort::Process(const HeaderInfo& info, const ICollection<Indexed<opendnp3::BinaryOutputStatus> >& meas){ LoadT(meas); }
void DNP3MasterPort::Process(const HeaderInfo& info, const ICollection<Indexed<opendnp3::AnalogOutputStatus> >& meas){ LoadT(meas); }
void DNP3MasterPort::Process(const HeaderInfo& info, const ICollection<Indexed<opendnp3::OctetString> >& meas){ /*LoadT(meas);*/ }
void DNP3MasterPort::Process(const HeaderInfo& info, const ICollection<Indexed<opendnp3::TimeAndInterval> >& meas){ /*LoadT(meas);*/ }
void DNP3MasterPort::Process(const HeaderInfo& info, const ICollection<Indexed<opendnp3::BinaryCommandEvent> >& meas){ /*LoadT(meas);*/ }
void DNP3MasterPort::Process(const HeaderInfo& info, const ICollection<Indexed<opendnp3::AnalogCommandEvent> >& meas){ /*LoadT(meas);*/ }
void DNP3MasterPort::Process(const HeaderInfo& info, const ICollection<Indexed<opendnp3::SecurityStat> >& meas){ /*LoadT(meas);*/ }

template<typename T>
inline void DNP3MasterPort::LoadT(const ICollection<Indexed<T> >& meas)
{
	meas.ForeachItem([&](const Indexed<T>&pair)
	                 {
	                       for(auto IOHandler_pair: Subscribers)
	                       {
                               IOHandler_pair.second->Event(ToODC(pair.value),pair.index,this->Name);
				     }
			     });
}

//Implement some IOHandler - parent DNP3Port implements the rest to return NOT_SUPPORTED
std::future<ODC::CommandStatus> DNP3MasterPort::Event(const ODC::ControlRelayOutputBlock& arCommand, uint16_t index, const std::string& SenderName){ return EventT(arCommand, index, SenderName); }
std::future<ODC::CommandStatus> DNP3MasterPort::Event(const ODC::AnalogOutputInt16& arCommand, uint16_t index, const std::string& SenderName){ return EventT(arCommand, index, SenderName); }
std::future<ODC::CommandStatus> DNP3MasterPort::Event(const ODC::AnalogOutputInt32& arCommand, uint16_t index, const std::string& SenderName){ return EventT(arCommand, index, SenderName); }
std::future<ODC::CommandStatus> DNP3MasterPort::Event(const ODC::AnalogOutputFloat32& arCommand, uint16_t index, const std::string& SenderName){ return EventT(arCommand, index, SenderName); }
std::future<ODC::CommandStatus> DNP3MasterPort::Event(const ODC::AnalogOutputDouble64& arCommand, uint16_t index, const std::string& SenderName){ return EventT(arCommand, index, SenderName); }

std::future<ODC::CommandStatus> DNP3MasterPort::ConnectionEvent(ODC::ConnectState state, const std::string& SenderName)
{
	if(!enabled)
	{
		return IOHandler::CommandFutureUndefined();
	}

	// If an upstream port has been enabled after the stack has already been enabled, do an integrity scan
	if (stack_enabled && state == ODC::ConnectState::PORT_UP)
	{
		std::string msg = Name + ": upstream port enabled, performing integrity scan.";
		auto log_entry = ODC::LogEntry("DNP3MasterPort", ODC::logflags::INFO, "", msg.c_str(), -1);
		Log(log_entry);

		IntegrityScan.Demand();
	}

	DNP3PortConf* pConf = static_cast<DNP3PortConf*>(this->pConf.get());

	// If an upstream port is connected, attempt a connection (if on demand)
	if (!stack_enabled && state == ODC::ConnectState::CONNECTED && pConf->mAddrConf.ServerType == server_type_t::ONDEMAND)
	{
		std::string msg = Name + ": upstream port connected, performing on-demand connection.";
		auto log_entry = ODC::LogEntry("DNP3MasterPort", ODC::logflags::INFO, "", msg.c_str(), -1);
		Log(log_entry);

		Post([&]()
		     {
		           EnableStack();
		     });
	}

	// If an upstream port is disconnected, disconnect ourselves if it was the last active connection (if on demand)
	if (stack_enabled && state == ODC::ConnectState::DISCONNECTED && pConf->mAddrConf.ServerType == server_type_t::ONDEMAND)
	{
		Post([&]()
		     {
		           DisableStack();
		           PortDown();
		     });
	}

	return IOHandler::CommandFutureSuccess();
}

template<typename T>
inline std::future<ODC::CommandStatus> DNP3MasterPort::EventT(T& arCommand, uint16_t index, const std::string& SenderName)
{
	// If the port is disabled, fail the command
	if(!enabled)
	{
		return IOHandler::CommandFutureUndefined();
	}

	// If the stack is disabled, fail the command
	if (!stack_enabled)
	{
		return IOHandler::CommandFutureUndefined();
	}

	auto pConf = static_cast<DNP3PortConf*>(this->pConf.get());
	for(auto i : pConf->ControlIndicies)
	{
		if(i == index)
		{
			auto cmd_promise = std::promise<ODC::CommandStatus>();
			auto cmd_future = cmd_promise.get_future();

			//make a copy of the command, so we can change it if needed
			auto lCommand = ToOpenDNP3(arCommand);
			//this will change the control code if the command is binary, and there's a defined override
			DoOverrideControlCode(lCommand);

			std::string msg = "Executing direct operate to index: " + std::to_string(index);
			auto log_entry = ODC::LogEntry("DNP3MasterPort", ODC::logflags::INFO, "", msg.c_str(), -1);
			Log(log_entry);

			this->pMaster->DirectOperate(lCommand,index,*CommandCorrespondant::GetCallback(std::move(cmd_promise)));

			return cmd_future;
		}
	}
	std::string msg = "Control sent to invalid DNP3 index: " + std::to_string(index);
	auto log_entry = ODC::LogEntry("DNP3MasterPort", ODC::logflags::WARN, "", msg.c_str(), -1);
	Log(log_entry);
	return IOHandler::CommandFutureUndefined();
}

//DataPort function for UI
const Json::Value DNP3MasterPort::GetStatistics() const
{
	Json::Value event;

	if (pChannel != nullptr)
	{
		auto ChanStats = this->pChannel->GetChannelStatistics();
		event["numCrcError"] = ChanStats.numCrcError;             /// Number of frames discared due to CRC errors
		event["numLinkFrameTx"] = ChanStats.numLinkFrameTx;       /// Number of frames transmitted
		event["numLinkFrameRx"] = ChanStats.numLinkFrameRx;       /// Number of frames received
		event["numBadLinkFrameRx"] = ChanStats.numBadLinkFrameRx; /// Number of frames detected with bad / malformed contents
		event["numBytesRx"] = ChanStats.numBytesRx;
		event["numBytesTx"] = ChanStats.numBytesTx;
		event["numClose"] = ChanStats.numClose;
		event["numOpen"] = ChanStats.numOpen;
		event["numOpenFail"] = ChanStats.numOpenFail;
	}
	if (pMaster != nullptr)
	{
		auto StackStats = this->pMaster->GetStackStatistics();
		event["numTransportErrorRx"] = StackStats.numTransportErrorRx;
		event["numTransportRx"] = StackStats.numTransportRx;
		event["numTransportTx"] = StackStats.numTransportTx;
	}

	return event;
}
