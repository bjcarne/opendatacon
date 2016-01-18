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

#include "opendnp3/UInt48Type.h"
#include "opendnp3/QualityMasks.h"

namespace ODC
{
    /// Underlying types
    enum class DoubleBit : uint8_t
    {
        /// Transitioning between end conditions
        INTERMEDIATE = 0x0,
        /// End condition, determined to be OFF
        DETERMINED_OFF = 0x1,
        /// End condition, determined to be ON
        DETERMINED_ON = 0x2,
        /// Abnormal or custom condition
        INDETERMINATE = 0x3
    };
    inline uint8_t DoubleBitToType(DoubleBit arg)
    {
        return static_cast<uint8_t>(arg);
    }
    inline DoubleBit DoubleBitFromType(uint8_t arg)
    {
        switch(arg)
        {
            case(0x0):
                return DoubleBit::INTERMEDIATE;
            case(0x1):
                return DoubleBit::DETERMINED_OFF;
            case(0x2):
                return DoubleBit::DETERMINED_ON;
            default:
                return DoubleBit::INDETERMINATE;
        }
    }
    
    
    class timestamp : public openpal::UInt48Type { using openpal::UInt48Type::UInt48Type; };
    
    ///
    enum ConnectState : uint8_t
    {
        PORT_UP = 0,
        CONNECTED = 1,
        DISCONNECTED = 2,
        PORT_DOWN = 3
    };

    //typedef opendnp3::CommandStatus CommandStatus;
    enum class CommandStatus : uint8_t
    {
        /// command was accepted, initiated, or queued
        SUCCESS = 0,
        /// command timed out before completing
        TIMEOUT = 1,
        /// command requires being selected before operate, configuration issue
        NO_SELECT = 2,
        /// bad control code or timing values
        FORMAT_ERROR = 3,
        /// command is not implemented
        NOT_SUPPORTED = 4,
        /// command is all ready in progress or its all ready in that mode
        ALREADY_ACTIVE = 5,
        /// something is stopping the command, often a local/remote interlock
        HARDWARE_ERROR = 6,
        /// the function governed by the control is in local only control
        LOCAL = 7,
        /// the command has been done too often and has been throttled
        TOO_MANY_OPS = 8,
        /// the command was rejected because the device denied it or an RTU intercepted it
        NOT_AUTHORIZED = 9,
        /// command not accepted because it was prevented or inhibited by a local automation process, such as interlocking logic or synchrocheck
        AUTOMATION_INHIBIT = 10,
        /// command not accepted because the device cannot process any more activities than are presently in progress
        PROCESSING_LIMITED = 11,
        /// command not accepted because the value is outside the acceptable range permitted for this point
        OUT_OF_RANGE = 12,
        /// command not accepted because the outstation is forwarding the request to another downstream device which reported LOCAL
        DOWNSTREAM_LOCAL = 13,
        /// command not accepted because the outstation has already completed the requested operation
        ALREADY_COMPLETE = 14,
        /// command not accepted because the requested function is specifically blocked at the outstation
        BLOCKED = 15,
        /// command not accepted because the operation was cancelled
        CANCELLED = 16,
        /// command not accepted because another master is communicating with the outstation and has exclusive rights to operate this control point
        BLOCKED_OTHER_MASTER = 17,
        /// command not accepted because the outstation is forwarding the request to another downstream device which cannot be reached or is otherwise incapable of performing the request
        DOWNSTREAM_FAIL = 18,
        /// (deprecated) indicates the outstation shall not issue or perform the control operation
        NON_PARTICIPATING = 126,
        /// 10 to 126 are currently reserved
        UNDEFINED = 127
    };
    
    inline uint8_t CommandStatusToType(CommandStatus arg)
    {
        return static_cast<uint8_t>(arg);
    };
    inline CommandStatus CommandStatusFromType(uint8_t arg)
    {
        switch(arg)
        {
            case(0):
                return CommandStatus::SUCCESS;
            case(1):
                return CommandStatus::TIMEOUT;
            case(2):
                return CommandStatus::NO_SELECT;
            case(3):
                return CommandStatus::FORMAT_ERROR;
            case(4):
                return CommandStatus::NOT_SUPPORTED;
            case(5):
                return CommandStatus::ALREADY_ACTIVE;
            case(6):
                return CommandStatus::HARDWARE_ERROR;
            case(7):
                return CommandStatus::LOCAL;
            case(8):
                return CommandStatus::TOO_MANY_OPS;
            case(9):
                return CommandStatus::NOT_AUTHORIZED;
            case(10):
                return CommandStatus::AUTOMATION_INHIBIT;
            case(11):
                return CommandStatus::PROCESSING_LIMITED;
            case(12):
                return CommandStatus::OUT_OF_RANGE;
            case(13):
                return CommandStatus::DOWNSTREAM_LOCAL;
            case(14):
                return CommandStatus::ALREADY_COMPLETE;
            case(15):
                return CommandStatus::BLOCKED;
            case(16):
                return CommandStatus::CANCELLED;
            case(17):
                return CommandStatus::BLOCKED_OTHER_MASTER;
            case(18):
                return CommandStatus::DOWNSTREAM_FAIL;
            case(126):
                return CommandStatus::NON_PARTICIPATING;
            default:
                return CommandStatus::UNDEFINED;
        }
    }
    
    //typedef opendnp3::ControlCode ControlCode;
    enum class ControlCode : uint8_t
    {
        /// Does not initiate an action or change an in-progress or pending command.
        NUL = 0x0,
        /// Cancel in-progress and pending commands. Take no additional action.
        NUL_CANCEL = 0x20,
        /// For activation model, set output to active for the duration of the On-time. For both complementary models, return NOT_SUPPORTED status.
        PULSE_ON = 0x1,
        /// Cancel in-progress and pending commands, process the remainder of the fields as if the control code were PULSE_ON
        PULSE_ON_CANCEL = 0x21,
        /// Non-interoperable code. Do not use for new applications. return NOT_SUPPORTED
        PULSE_OFF = 0x2,
        /// Non-interoperable code. Do not use for new applications. Cancel in-progress and pending commands, process remainder of fields as if the control code were PULSE_OFF
        PULSE_OFF_CANCEL = 0x22,
        /// For activation model, set output to active for the duration of the On-time. For complementary latch model, set the output to active. For complementary two-output model, set the close output to active for the duration of the On-time.
        LATCH_ON = 0x3,
        /// Cancel in-progress and pending commands, process the remainder of the fields as if the control code were LATCH_ON.
        LATCH_ON_CANCEL = 0x23,
        /// For activation model, set output to active for the duration of the On-time. For complementary latch model, set the output to inactive. For complementary two-output model, set the trip output to active for the duration of the On-time.
        LATCH_OFF = 0x4,
        /// Cancel in-progress and pending commands, process the remainder of the fields as if the control code were LATCH_OFF.
        LATCH_OFF_CANCEL = 0x24,
        /// For activation model, set output to active for the duration of the On-time. For complementary latch model, set the output to active. For complementary two-output model, set the close output to active for the duration of the On-time.
        CLOSE_PULSE_ON = 0x41,
        /// Cancel in-progress and pending commands, process the remainder of the fields as if the control code were CLOSE_PULSE_ON.
        CLOSE_PULSE_ON_CANCEL = 0x61,
        /// For activation model, set output to active for the duration of the On-time. For complementary latch model, set the output to inactive. For complementary two-output model, set the trip output to active for the duration of the On-time.
        TRIP_PULSE_ON = 0x81,
        /// Cancel in-progress and pending commands, process the remainder of the fields as if the control code were TRIP_PULSE_ON.
        TRIP_PULSE_ON_CANCEL = 0xA1,
        ///  undefined command (used by DNP standard)
        UNDEFINED = 0xFF
    };
    
    inline uint8_t ControlCodeToType(ControlCode arg)
    {
        return static_cast<uint8_t>(arg);
    }
    
    
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
		AnalogOutput(T v, CommandStatus s) : value(v), status(s) {};
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
