/*
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#ifndef ODC_QUALITYMASKS_H
#define ODC_QUALITYMASKS_H

#include "gen/BinaryQuality.h"
#include "gen/DoubleBitBinaryQuality.h"
#include "gen/CounterQuality.h"
#include "gen/AnalogQuality.h"
#include "gen/BinaryOutputStatusQuality.h"

namespace ODC
{
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
