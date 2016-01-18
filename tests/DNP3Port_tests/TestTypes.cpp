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
#include <chrono>

#define SUITE(name) "DNP3PortTypeTestSuite - " name

TEST_CASE(SUITE("Binary"))
{
    opendnp3::Binary b(true, 7, opendnp3::DNPTime(9));
	ODC::Binary c(b.value, b.quality, b.time); // maintains reference to original (a wrapper)

    REQUIRE(c.value == b.value);
    REQUIRE(c.quality == b.quality);
    REQUIRE(c.time == b.time);

}
