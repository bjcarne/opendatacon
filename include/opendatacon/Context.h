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
 *  Created by Alan Murray on 12/01/2016.
 */

#ifndef CONTEXT_H
#define CONTEXT_H

#include <set>
#include <mutex>
#include <asio.hpp>
#include <opendatacon/ILoggable.h>

namespace ODC
{
class Context: public virtual ILoggable
{
public:
	Context(const std::string& aName, asio::io_service &service):
		Name(aName),
		Parent(nullptr),
		IOS(&service),
		CollectionMutex()
	{};

	Context(const std::string &aName, Context &aParent):
		Name(aName),
		Parent(&aParent),
		IOS(Parent->IOS),
		CollectionMutex()
	{
		Parent->AddChild(*this);
	};

	void AddChild(Context &aChild)
	{
		std::unique_lock<std::mutex> lock(CollectionMutex);
		Children.insert(&aChild);
	}

	void RemoveChild(Context &aChild)
	{
		std::unique_lock<std::mutex> lock(CollectionMutex);
		Children.erase(&aChild);
	}

	~Context()
	{
		std::unique_lock<std::mutex> lock(CollectionMutex);
		for(auto child : Children)
		{
			child->Parent = nullptr;
		}
		if(Parent)
		{
			Parent->RemoveChild(*this);
		}
	};

	asio::io_service* GetIOService()
	{
		return IOS;
	}

	/* Post work to the shared thread pool */
	void Post(std::function<void(void)> func)
	{
		IOS->post(func);
	}

	void LogSubscribe(ILoggable& target)
	{
		std::unique_lock<std::mutex> lock(CollectionMutex);
		LogSubscribers.insert(&target);
	}

	void LogUnsubscribe(ILoggable& target)
	{
		std::unique_lock<std::mutex> lock(CollectionMutex);
		LogSubscribers.erase(&target);
	}

	/* Send log message to any subscribers */
	virtual inline void Log(const ODC::LogEntry& arEntry)
	{
		std::unique_lock<std::mutex> lock(CollectionMutex);
		for(auto target : LogSubscribers)
		{
			target->Log(arEntry);
		}
	};

protected:
	std::string Name;

private:
	Context* Parent;
	asio::io_service* IOS;

	std::set<Context*> Children;
	std::set<ILoggable*> LogSubscribers;

	std::mutex CollectionMutex;

};
}

#endif
