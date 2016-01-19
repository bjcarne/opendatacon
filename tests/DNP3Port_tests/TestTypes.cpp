/*	opendatacon
 *
 *	Copyright (c) 2016:
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
#include <catch.hpp>

#include <opendatacon/IOTypes.h>
#include <opendnp3/app/MeasurementTypes.h>
#include <chrono>
#include "../../DNP3Port/OpenDNP3Helpers.h"

#define SUITE(name) "DNP3PortTypeTestSuite - " name

template <class A, class B>
bool MeasurementEqual(const A& a, const B& b)
{
    return (
            (a.value == b.value) &&
            (a.quality == b.quality) &&
            ((uint64_t)a.time == (uint64_t)b.time)
             );
};

TEST_CASE(SUITE("Binary"))
{
    opendnp3::DNPTime t(9);
    
    opendnp3::Binary b_d(true, 7, t);
    opendnp3::DoubleBitBinary db_d(opendnp3::DoubleBit::INDETERMINATE, 7, t);
    opendnp3::Analog a_d(3.3, 7, t);
    opendnp3::Counter c_d(123, 7, t);
    opendnp3::FrozenCounter fc_d(123, 7, t);
    opendnp3::BinaryOutputStatus bos_d(true, 7, t);
    opendnp3::AnalogOutputStatus aos_d(3.3, 7, t);
    
    auto b_o = ToODC(b_d);
    auto db_o = ToODC(db_d);
    auto a_o = ToODC(a_d);
    auto c_o = ToODC(c_d);
    auto fc_o = ToODC(fc_d);
    auto bos_o = ToODC(bos_d);
    auto aos_o = ToODC(aos_d);

    assert(MeasurementEqual(b_d, b_o));
    //assert(MeasurementEqual(db_d, db_o));
    assert(MeasurementEqual(a_d, a_o));
    assert(MeasurementEqual(c_d, c_o));
    assert(MeasurementEqual(fc_d, fc_o));
    assert(MeasurementEqual(bos_d, bos_o));
    assert(MeasurementEqual(aos_d, aos_o));

}
