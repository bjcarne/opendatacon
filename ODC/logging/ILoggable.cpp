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
 *
 *  Created by Alan Murray on 09/01/2016.
 */

#include <opendatacon/ILoggable.h>
#include <opendatacon/LogLevels.h>
#include <chrono>
#include <ctime>

namespace platformtime
{
tm localtime(const time_t* time)
{
	std::tm tm_snapshot;
	#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__))
	localtime_s(&tm_snapshot, time);
	#else
	localtime_r(time, &tm_snapshot); // POSIX
	#endif
	return tm_snapshot;
}
std::string time_string()
{
	using namespace std::chrono;

	auto time_epoch = std::chrono::high_resolution_clock::now().time_since_epoch();
	//auto time_epoch = std::chrono::steady_clock::now().time_since_epoch();

	milliseconds ms = duration_cast<milliseconds>(time_epoch);
	seconds s = duration_cast<seconds>(time_epoch);

	std::time_t t = s.count();
	std::size_t fractional_seconds = ms.count() % 1000;
	auto local_time = platformtime::localtime(&t);
	char time_formatted[25];
	std::strftime(time_formatted, 25, "%Y-%m-%d %H:%M:%S", &local_time);
	return std::string(time_formatted)+"."+std::to_string(fractional_seconds);
}
}


namespace ODC
{
std::string LogFilters::toString() const
{
	switch (filters)
	{
		case (ODC::logflags::EVENT):
			return "EVENT";
		case (ODC::logflags::ERR):
			return "ERROR";
		case (ODC::logflags::WARN):
			return "WARN";
		case (ODC::logflags::INFO):
			return "INFO";
		case (ODC::logflags::DBG):
			return "DEBUG";
		case (ODC::logflags::LINK_RX):
		case (ODC::logflags::LINK_RX_HEX):
			return "<--LINK-";
		case (ODC::logflags::LINK_TX):
		case (ODC::logflags::LINK_TX_HEX):
			return "-LINK-->";
		case (ODC::logflags::TRANSPORT_RX):
			return "<--TRANS-";
		case (ODC::logflags::TRANSPORT_TX):
			return "-TRANS-->";
		case (ODC::logflags::APP_HEADER_RX):
		case (ODC::logflags::APP_OBJECT_RX):
			return "<--APP-";
		case (ODC::logflags::APP_HEADER_TX):
		case (ODC::logflags::APP_OBJECT_TX):
			return "-APP-->";
		default:
			return "UNKNOWN";
	}
}

LogEntry::LogEntry():
	alias(""),
	location(""),
	message(""),
	errorCode(-1)
{}


LogEntry::LogEntry(char const* alias_, const LogFilters& filters_, char const* location_, char const* message_, int errorCode_)
	:
	alias(alias_),
	filters(filters_),
	location(location_),
	message(message_),
	errorCode(errorCode_)
{}
}
