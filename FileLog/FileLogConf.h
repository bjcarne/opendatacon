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
 *  Created on: 15/01/2016
 *      Author: Alan Murray <alan@atmurray.net>
 */

#ifndef FILELOGCONF_H_
#define FILELOGCONF_H_

#include <memory>
#include <opendatacon/Context.h>
#include <opendatacon/DataPortConf.h>

class FileLogConf: public ODC::Context, public DataPortConf
{
public:
	FileLogConf(const std::string& aFileName, ODC::Context& aParent):
		Context(aFileName, aParent),
	    mLogName("opendatacon.log"),
	    mNumFiles(5),
	    mFileSizekB(5*1024)
	{};

    std::string mLogName;
    size_t mNumFiles;
    size_t mFileSizekB;
};

#endif /* FILELOGCONF_H_ */
