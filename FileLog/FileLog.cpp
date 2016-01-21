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
#include "FileLog.h"

#include <sstream>
#include <fstream>
#include <ctime>
#include <cstdio>
#include <map>
#include <opendatacon/util.h>

#ifdef GetMessage
#undef GetMessage
#endif

void FileLog::Log( const ODC::LogEntry& arEntry )
{
	std::stringstream output;

	output << platformtime::time_string();
	output<<" - "<< arEntry.GetFilters().toString() <<" - "<<arEntry.GetAlias();
	if(mPrintLocation && !arEntry.GetLocation())
		output << " - " << arEntry.GetLocation();
	output << " - " << arEntry.GetMessage();
	if(arEntry.GetErrorCode() != -1)
		output << " - " << arEntry.GetErrorCode();
	output <<std::endl;

	std::lock_guard<std::mutex> get_lock(mMutex);
	if(!mLogFile.is_open())
	{
		mLogFile.open(LogConf.mLogName+std::to_string(mFileIndex)+".txt");
		if(mLogFile.fail())
			throw std::runtime_error("Failed to open log file");
	}

	mLogFile << output.str();
	if (mLogFile.tellp() > (int)LogConf.mFileSizekB*1024)
	{
		mLogFile.close();
		mFileIndex++;
		mFileIndex %= LogConf.mNumFiles;
	}
}

/*
void LogToFile::SetLogFileSizekB(size_t kB)
{
       mFileSizekB = kB;
}

void LogToFile::SetNumLogFiles(size_t num)
{
       mNumFiles = num;
}

void LogToFile::SetLogName(std::string name)
{
       mLogName = name;
       mFileIndex = 0;
}

size_t LogToFile::GetLogFileSizekB()
{
       return mFileSizekB;
}

size_t LogToFile::GetNumLogFiles()
{
       return mNumFiles;
}

std::string LogToFile::GetLogName()
{
       return mLogName;
}

void LogToFile::SetPrintLocation(bool print_loc)
{
       mPrintLocation = print_loc;
}
*/
