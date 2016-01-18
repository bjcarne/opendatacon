/*	opendatacon
 *
 *	Copyright (c) 2015:
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
 * IOTypes.h
 *
 *  Created on: 20/12/2015
 *      Author: Alan Murray <alan@atmurray.net>
 */

#ifndef IOTYPES_H_
#define IOTYPES_H_

#include <opendnp3/app/MeasurementTypes.h>
#include <opendnp3/app/ControlRelayOutputBlock.h>
#include <opendnp3/app/AnalogOutput.h>
#include <opendnp3/gen/ChannelState.h>

namespace ODC
{
    enum ConnectState : uint8_t
    {
        PORT_UP = 0,
        CONNECTED = 1,
        DISCONNECTED = 2,
        PORT_DOWN = 3
    };
    
    //typedef opendnp3::ChannelState ChannelState;
    enum class ChannelState : uint8_t
    {
        /// offline and idle
        CLOSED = 0,
        /// trying to open
        OPENING = 1,
        /// waiting to open
        WAITING = 2,
        /// open
        OPEN = 3,
        /// stopped and will never do anything again
        SHUTDOWN = 4
    };

#include <type_traits> //must include it

	/*
    template<typename A, typename B>
    constexpr bool enum_check(A a ,B b)
    {
        return static_cast<typename std::underlying_type<A>::type>(a) == static_cast<typename std::underlying_type<B>::type>(b);
    }
    static_assert(enum_check(ODC::ChannelState::CLOSED, opendnp3::ChannelState::CLOSED),"ChannelState enum check failed");
    static_assert(enum_check(ODC::ChannelState::OPENING, opendnp3::ChannelState::OPENING),"ChannelState enum check failed");
    static_assert(enum_check(ODC::ChannelState::WAITING, opendnp3::ChannelState::WAITING),"ChannelState enum check failed");
    static_assert(enum_check(ODC::ChannelState::OPEN, opendnp3::ChannelState::OPEN),"ChannelState enum check failed");
    static_assert(enum_check(ODC::ChannelState::SHUTDOWN, opendnp3::ChannelState::SHUTDOWN),"ChannelState enum check failed");
    */

    typedef opendnp3::CommandStatus CommandStatus;
    typedef opendnp3::ControlCode ControlCode;
    typedef opendnp3::DoubleBit DoubleBit;
    typedef opendnp3::DNPTime timestamp;
    
    /// Measurement types
    template <class T>
    class Measurement
    {
    public:
		Measurement() : value(T()), quality(0), time(0) {};
		Measurement(T v) : value(v), quality(0), time(0) {};
		Measurement(T v, uint8_t q ) : value(v), quality(q), time(0) {};
		Measurement(T v, uint8_t q, timestamp t) : value(v), quality(q), time(t) {};
        T value;
        uint8_t quality;
        timestamp time;
        typedef T value_type;
    };
    
    class Binary : public Measurement<bool> { using Measurement::Measurement; };
    class DoubleBitBinary : public Measurement<DoubleBit> { using Measurement::Measurement; };
    class Analog : public Measurement<double> { using Measurement::Measurement; };
    class Counter : public Measurement<uint32_t> { using Measurement::Measurement; };
    class FrozenCounter : public Measurement<uint32_t> { using Measurement::Measurement; };
    class BinaryOutputStatus : public Measurement<bool> { using Measurement::Measurement; };
    class AnalogOutputStatus : public Measurement<bool> { using Measurement::Measurement; };

    /// Control types
    class ControlRelayOutputBlock
    {
    public:
        // overloaded constructor that allows the user to set a raw control code for non-standard codes
		ControlRelayOutputBlock(uint8_t functionCode_,
                                uint8_t count_ = 1,
                                uint32_t onTimeMS_ = 100,
                                uint32_t offTimeMS_ = 100,
                                CommandStatus status_ = CommandStatus::SUCCESS) :
		functionCode(functionCode_),
        count(count_),
        onTimeMS(onTimeMS_),
        offTimeMS(offTimeMS_),
        status(status_)
        {};
		uint8_t functionCode;
        uint8_t count;
        uint32_t onTimeMS;
        uint32_t offTimeMS;
        CommandStatus status;
    };
    
    /// Analog Ouput types
    template <class T>
    class AnalogOutput
    {
    public:
		AnalogOutput() : value(T()), status(CommandStatus::SUCCESS) {};
		AnalogOutput(T v) : value(v), status(CommandStatus::SUCCESS) {};
		AnalogOutput(T v, CommandStatus s = CommandStatus::SUCCESS) : value(v), status(s) {};
		T value;
        CommandStatus status;
        typedef T value_type;
    };
    
    class AnalogOutputInt16 : public AnalogOutput<int16_t> { using AnalogOutput::AnalogOutput; };
    class AnalogOutputInt32 : public AnalogOutput<int32_t> { using AnalogOutput::AnalogOutput; };
    class AnalogOutputFloat32 : public AnalogOutput<float> { using AnalogOutput::AnalogOutput; };
    class AnalogOutputDouble64 : public AnalogOutput<double> { using AnalogOutput::AnalogOutput; };

    /// Quality types
typedef opendnp3::BinaryQuality BinaryQuality; // uint8_t
typedef opendnp3::DoubleBitBinaryQuality DoubleBitBinaryQuality; // uint8_t
typedef opendnp3::AnalogQuality AnalogQuality; // uint8_t
typedef opendnp3::CounterQuality CounterQuality; // uint8_t
typedef opendnp3::BinaryOutputStatusQuality BinaryOutputStatusQuality; // uint8_t
    
enum class FrozenCounterQuality: uint8_t
{
	/// set when the data is "good", meaning that rest of the system can trust the value
	ONLINE = 0x1,
	/// the quality all points get before we have established communication (or populated) the point
	RESTART = 0x2,
	/// set if communication has been lost with the source of the data (after establishing contact)
	COMM_LOST = 0x4,
	/// set if the value is being forced to a "fake" value somewhere in the system
	REMOTE_FORCED = 0x8,
	/// set if the value is being forced to a "fake" value on the original device
	LOCAL_FORCED = 0x10,
	/// Deprecated flag that indicates value has rolled over
	ROLLOVER = 0x20,
	/// indicates an unusual change in value
	DISCONTINUITY = 0x40,
	/// reserved bit
	RESERVED = 0x80
};

/**
Quality field bitmask for AnalogOutputStatus values
*/
enum class AnalogOutputStatusQuality: uint8_t
{
	/// set when the data is "good", meaning that rest of the system can trust the value
	ONLINE = 0x1,
	/// the quality all points get before we have established communication (or populated) the point
	RESTART = 0x2,
	/// set if communication has been lost with the source of the data (after establishing contact)
	COMM_LOST = 0x4,
	/// set if the value is being forced to a "fake" value somewhere in the system
	REMOTE_FORCED = 0x8,
	/// set if the value is being forced to a "fake" value on the original device
	LOCAL_FORCED = 0x10,
	/// set if a hardware input etc. is out of range and we are using a place holder value
	OVERRANGE = 0x20,
	/// set if calibration or reference voltage has been lost meaning readings are questionable
	REFERENCE_ERR = 0x40,
	/// reserved bit
	RESERVED = 0x80
};

}

#endif
