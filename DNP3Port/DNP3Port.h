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
 * DNP3Port.h
 *
 *  Created on: 18/07/2014
 *      Author: Neil Stephens <dearknarl@gmail.com>
 */

#ifndef DNP3PORT_H_
#define DNP3PORT_H_

#include <opendatacon/DataPort.h>
#include <opendnp3/gen/LinkStatus.h>
#include <asiodnp3/DNP3Manager.h>
#include <openpal/logging/ILogHandler.h>
#include <openpal/logging/LogEntry.h>
#include "DNP3PortConf.h"

class DNP3LogWrapper : public openpal::ILogHandler
{
public:
    DNP3LogWrapper(const std::string& aName, ODC::ILoggable& target) :
    name(aName),
    theLogger(target)
    {
        
    }
    
    virtual void Log( const openpal::LogEntry& entry )
    {
        if (entry.GetAlias() != name)
        {
            //return;
        }
        /* TODO: filter down to only log messages for this port */
        ODC::LogEntry newentry(entry.GetAlias(), entry.GetFilters().GetBitfield(), entry.GetLocation(), entry.GetMessage(), entry.GetErrorCode());
        try
        {
            theLogger.Log(newentry);
        }
        catch (std::exception& e)
        {
            
        }
    }
    
private:
    const std::string name;
    ODC::ILoggable& theLogger;
    
};

class DNP3Port: public ODC::DataPort
{
public:
	DNP3Port(std::string aName, std::string aConfFilename, const Json::Value aConfOverrides);
    using DataPort::IOHandler::Event;

	virtual void Enable()=0;
	virtual void Disable()=0;
	virtual void BuildOrRebuild()=0;

	//Override DataPort for UI
	const Json::Value GetStatus() const override;

	virtual std::future<ODC::CommandStatus> Event(const ODC::Binary& meas, uint16_t index, const std::string& SenderName) { return IOHandler::CommandFutureNotSupported(); };
	virtual std::future<ODC::CommandStatus> Event(const ODC::DoubleBitBinary& meas, uint16_t index, const std::string& SenderName) { return IOHandler::CommandFutureNotSupported(); };
	virtual std::future<ODC::CommandStatus> Event(const ODC::Analog& meas, uint16_t index, const std::string& SenderName) { return IOHandler::CommandFutureNotSupported(); };
	virtual std::future<ODC::CommandStatus> Event(const ODC::Counter& meas, uint16_t index, const std::string& SenderName) { return IOHandler::CommandFutureNotSupported(); };
	virtual std::future<ODC::CommandStatus> Event(const ODC::FrozenCounter& meas, uint16_t index, const std::string& SenderName) { return IOHandler::CommandFutureNotSupported(); };
	virtual std::future<ODC::CommandStatus> Event(const ODC::BinaryOutputStatus& meas, uint16_t index, const std::string& SenderName) { return IOHandler::CommandFutureNotSupported(); };
	virtual std::future<ODC::CommandStatus> Event(const ODC::AnalogOutputStatus& meas, uint16_t index, const std::string& SenderName) { return IOHandler::CommandFutureNotSupported(); };

	virtual std::future<ODC::CommandStatus> Event(const ODC::ControlRelayOutputBlock& arCommand, uint16_t index, const std::string& SenderName) { return IOHandler::CommandFutureNotSupported(); };
	virtual std::future<ODC::CommandStatus> Event(const ODC::AnalogOutputInt16& arCommand, uint16_t index, const std::string& SenderName) { return IOHandler::CommandFutureNotSupported(); };
	virtual std::future<ODC::CommandStatus> Event(const ODC::AnalogOutputInt32& arCommand, uint16_t index, const std::string& SenderName) { return IOHandler::CommandFutureNotSupported(); };
	virtual std::future<ODC::CommandStatus> Event(const ODC::AnalogOutputFloat32& arCommand, uint16_t index, const std::string& SenderName) { return IOHandler::CommandFutureNotSupported(); };
	virtual std::future<ODC::CommandStatus> Event(const ODC::AnalogOutputDouble64& arCommand, uint16_t index, const std::string& SenderName) { return IOHandler::CommandFutureNotSupported(); };

	/// Quality change events
	virtual std::future<ODC::CommandStatus> Event(const ODC::BinaryQuality qual, uint16_t index, const std::string& SenderName) { return IOHandler::CommandFutureNotSupported(); };
	virtual std::future<ODC::CommandStatus> Event(const ODC::DoubleBitBinaryQuality qual, uint16_t index, const std::string& SenderName) { return IOHandler::CommandFutureNotSupported(); };
	virtual std::future<ODC::CommandStatus> Event(const ODC::AnalogQuality qual, uint16_t index, const std::string& SenderName) { return IOHandler::CommandFutureNotSupported(); };
	virtual std::future<ODC::CommandStatus> Event(const ODC::CounterQuality qual, uint16_t index, const std::string& SenderName) { return IOHandler::CommandFutureNotSupported(); };
	virtual std::future<ODC::CommandStatus> Event(const ODC::FrozenCounterQuality qual, uint16_t index, const std::string& SenderName) { return IOHandler::CommandFutureNotSupported(); };
	virtual std::future<ODC::CommandStatus> Event(const ODC::BinaryOutputStatusQuality qual, uint16_t index, const std::string& SenderName) { return IOHandler::CommandFutureNotSupported(); };
	virtual std::future<ODC::CommandStatus> Event(const ODC::AnalogOutputStatusQuality qual, uint16_t index, const std::string& SenderName) { return IOHandler::CommandFutureNotSupported(); };

	void ProcessElements(const Json::Value& JSONRoot);

protected:
    
    inline asiodnp3::IChannel* getTCPServer(const DNP3AddrConf& mAddrConf)
    {
        auto IPPort = "TCPS_" + mAddrConf.IP +":"+ std::to_string(mAddrConf.Port);
        
        //create a new channel if one isn't already up
        if(!TCPServers.count(IPPort))
        {
            TCPServers[IPPort] = DNP3Mgr.AddTCPServer(IPPort.c_str(), opendnp3::levels::ALL,
                                                       opendnp3::ChannelRetry::Default(),
                                                       mAddrConf.IP,
                                                       mAddrConf.Port);
        }
        return TCPServers[IPPort];
    }

    inline asiodnp3::IChannel* getTCPClient(const DNP3AddrConf& mAddrConf)
    {
        auto IPPort = "TCPC_" + mAddrConf.IP +":"+ std::to_string(mAddrConf.Port);
        
        //create a new channel if one isn't already up
        if(!TCPClients.count(IPPort))
        {
            TCPClients[IPPort] = DNP3Mgr.AddTCPClient(IPPort.c_str(), opendnp3::levels::ALL,
                                                       opendnp3::ChannelRetry::Default(),
                                                       mAddrConf.IP,
                                                       "0.0.0.0",
                                                       mAddrConf.Port);
        }
        return TCPClients[IPPort];
    }
    
    DNP3LogWrapper LogWrapper;
	asiodnp3::IChannel* pChannel;
	static std::unordered_map<std::string, asiodnp3::IChannel*> TCPServers;
    static std::unordered_map<std::string, asiodnp3::IChannel*> TCPClients;
	static asiodnp3::DNP3Manager DNP3Mgr;
	opendnp3::LinkStatus status;
	bool link_dead;
};

#endif /* DNP3PORT_H_ */
