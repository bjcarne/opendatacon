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

#ifndef opendatacon_suite_ILoggable_h
#define opendatacon_suite_ILoggable_h

#include <cstdint>
#include <time.h>
#include <string>

namespace platformtime
{
    tm localtime(const time_t* time);
    std::string time_string();
}


namespace ODC
{
    
    /**
     * Strongly typed wrapper for flags bitfield
     */
    class LogFilters
    {
        
    public:
        
        LogFilters() : filters(0)
        {}
        
        LogFilters(int32_t filters_) : filters(filters_)
        {}
        
        inline bool IsSet(int32_t levels) const
        {
            return (levels & filters) != 0;
        }
        
        bool operator &(const LogFilters& rhs) const
        {
            return IsSet(rhs.filters);
        }
        
        int32_t GetBitfield() const
        {
            return filters;
        }
        
        std::string toString() const;
        
    private:
        
        int32_t	filters;
    };
    
    /**
     * An event recorded by the logging framework
     */
    class LogEntry
    {
        
    public:
        
        LogEntry();
        
        LogEntry(char const* alias, const ODC::LogFilters& filters, char const* location, char const* message, int errorCode);
        
        /// @return The alias of the logger that recorded the message
        char const*	GetAlias() const
        {
            return alias;
        }
        
        /// @return The place in the source code where the message was recorded
        char const*	GetLocation() const
        {
            return location;
        }
        
        /// @return body of the log message
        char const* GetMessage() const
        {
            return message;
        }
        
        /// @return the log level of the message
        const ODC::LogFilters& GetFilters() const
        {
            return filters;
        }
        
        /// @return the error code associated with the message
        int	GetErrorCode() const
        {
            return errorCode;
        }
        
    private:
        
        char const*		alias;
        LogFilters		filters;
        char const*		location;
        char const*		message;
        int				errorCode;
    };
    
    class ILoggable
    {
    public:
        virtual void Log(const ODC::LogEntry& arEntry) = 0;
    };
}


#endif
