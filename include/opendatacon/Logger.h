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

#ifndef opendatacon_suite_Loggable_h
#define opendatacon_suite_Loggable_h
#include <opendnp3/LogLevels.h>

#include <opendatacon/ILoggable.h>
#include <sstream>
#include <iostream>

namespace ODC
{
    class Logger : public virtual ILoggable
    {
    public:
        Logger(const std::string& aName) {};
        
        virtual inline void Log(const ODC::LogEntry& arEntry)
        {
            std::ostringstream oss;

            std::string time_str = platformtime::time_string();

            oss <<time_str<<" - "<< arEntry.GetFilters().toString()<<" - "<<arEntry.GetAlias();
            if(!arEntry.GetLocation())
                oss << " - " << arEntry.GetLocation();
            oss << " - " << arEntry.GetMessage();
            if(arEntry.GetErrorCode() != -1)
                oss << " - " << arEntry.GetErrorCode();
            
            std::unique_lock<std::mutex> lock(mutex);
            std::cout << oss.str() << std::endl;
        };
        
    private:
        static std::mutex mutex;
    };
}

#endif
