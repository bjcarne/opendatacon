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
 * SimPort.h
 *
 *  Created on: 29/07/2015
 *      Author: Neil Stephens <dearknarl@gmail.com>
 */

#ifndef SIMPORT_H
#define SIMPORT_H

#include <opendatacon/DataPort.h>
#include <opendatacon/util.h>
#include <random>

class SimPort: public ODC::DataPort
{
public:
	//Implement DataPort interface
	SimPort(std::string& aName, Context& parent, std::string& File, const Json::Value& Overrides);
	void Enable() final;
	void Disable() final;
	void BuildOrRebuild() final;
	void ProcessElements(const Json::Value& JSONRoot) final;
	std::future<ODC::CommandStatus> ConnectionEvent(ODC::ConnectState state, const std::string& SenderName) final;

	//Implement Event handlers from IOHandler

	// measurement events
	std::future<ODC::CommandStatus> Event(const ODC::Binary& meas, uint16_t index, const std::string& SenderName) final;
	std::future<ODC::CommandStatus> Event(const ODC::DoubleBitBinary& meas, uint16_t index, const std::string& SenderName) final;
	std::future<ODC::CommandStatus> Event(const ODC::Analog& meas, uint16_t index, const std::string& SenderName) final;
	std::future<ODC::CommandStatus> Event(const ODC::Counter& meas, uint16_t index, const std::string& SenderName) final;
	std::future<ODC::CommandStatus> Event(const ODC::FrozenCounter& meas, uint16_t index, const std::string& SenderName) final;
	std::future<ODC::CommandStatus> Event(const ODC::BinaryOutputStatus& meas, uint16_t index, const std::string& SenderName) final;
	std::future<ODC::CommandStatus> Event(const ODC::AnalogOutputStatus& meas, uint16_t index, const std::string& SenderName) final;

	// change of quality events
	std::future<ODC::CommandStatus> Event(const ODC::BinaryQuality qual, uint16_t index, const std::string& SenderName) final;
	std::future<ODC::CommandStatus> Event(const ODC::DoubleBitBinaryQuality qual, uint16_t index, const std::string& SenderName) final;
	std::future<ODC::CommandStatus> Event(const ODC::AnalogQuality qual, uint16_t index, const std::string& SenderName) final;
	std::future<ODC::CommandStatus> Event(const ODC::CounterQuality qual, uint16_t index, const std::string& SenderName) final;
	std::future<ODC::CommandStatus> Event(const ODC::FrozenCounterQuality qual, uint16_t index, const std::string& SenderName) final;
	std::future<ODC::CommandStatus> Event(const ODC::BinaryOutputStatusQuality qual, uint16_t index, const std::string& SenderName) final;
	std::future<ODC::CommandStatus> Event(const ODC::AnalogOutputStatusQuality qual, uint16_t index, const std::string& SenderName) final;

	// control events
	std::future<ODC::CommandStatus> Event(const ODC::ControlRelayOutputBlock& arCommand, uint16_t index, const std::string& SenderName) final;
	std::future<ODC::CommandStatus> Event(const ODC::AnalogOutputInt16& arCommand, uint16_t index, const std::string& SenderName) final;
	std::future<ODC::CommandStatus> Event(const ODC::AnalogOutputInt32& arCommand, uint16_t index, const std::string& SenderName) final;
	std::future<ODC::CommandStatus> Event(const ODC::AnalogOutputFloat32& arCommand, uint16_t index, const std::string& SenderName) final;
	std::future<ODC::CommandStatus> Event(const ODC::AnalogOutputDouble64& arCommand, uint16_t index, const std::string& SenderName) final;

private:
	typedef asio::basic_waitable_timer<std::chrono::steady_clock> Timer_t;
	typedef std::shared_ptr<Timer_t> pTimer_t;
	std::vector<pTimer_t> Timers;
	void SpawnEvent(std::shared_ptr<ODC::Analog> pMean, double std_dev, unsigned int interval, size_t index, pTimer_t pTimer, rand_t seed);
	void PortUp();
	void PortDown();

	std::unique_ptr<asio::strand> pEnableDisableSync;
	bool enabled;
	std::mt19937 RandNumGenerator;
};

#endif // SIMPORT_H
