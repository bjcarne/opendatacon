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
/*
* OpenDNP3Helpers.h
*
*  Created on: 24/11/2014
*      Author: Alan Murray
*/

#ifndef OPENDNP3HELPERS_H_
#define OPENDNP3HELPERS_H_
#include <string>

#include <opendnp3/app/MeasurementTypes.h>
#include <opendnp3/app/ControlRelayOutputBlock.h>
#include <opendnp3/app/AnalogOutput.h>

#include <openpal/container/ArrayView.h>
#include <opendatacon/IOTypes.h>

/// Map underlying types
inline ODC::DoubleBit DoubleBitToODC(const opendnp3::DoubleBit& a)
{
    return ODC::DoubleBitFromType(opendnp3::DoubleBitToType(a));
};
inline opendnp3::DoubleBit DoubleBitToOpenDNP3(const ODC::DoubleBit& a)
{
    return opendnp3::DoubleBitFromType(ODC::DoubleBitToType(a));
};

inline ODC::timestamp Convert(const opendnp3::DNPTime& a)
{
    return ODC::timestamp((uint64_t)a);
};
inline opendnp3::DNPTime Convert(const ODC::timestamp& a)
{
    return opendnp3::DNPTime((uint64_t)a);
};

/// Map measurement types
template <class A, class B>
inline B Converter(const A& a)
{
    return B(a.value, a.quality, Convert(a.time));
};

template <>
inline ODC::DoubleBitBinary Converter(const opendnp3::DoubleBitBinary& a)
{
    return ODC::DoubleBitBinary(DoubleBitToODC(a.value), a.quality, Convert(a.time));
};
template <>
inline opendnp3::DoubleBitBinary Converter(const ODC::DoubleBitBinary& a)
{
    return opendnp3::DoubleBitBinary(DoubleBitToOpenDNP3(a.value), a.quality, Convert(a.time));
};

inline ODC::Binary ToODC(const opendnp3::Binary& a) {
    return Converter<opendnp3::Binary, ODC::Binary>(a); };
inline ODC::DoubleBitBinary ToODC(const opendnp3::DoubleBitBinary& a) {
    return Converter<opendnp3::DoubleBitBinary, ODC::DoubleBitBinary>(a); };
inline ODC::Analog ToODC(const opendnp3::Analog& a) {
    return Converter<opendnp3::Analog, ODC::Analog>(a); };
inline ODC::Counter ToODC(const opendnp3::Counter a) {
    return Converter<opendnp3::Counter, ODC::Counter>(a); };
inline ODC::FrozenCounter ToODC(const opendnp3::FrozenCounter a) {
    return Converter<opendnp3::FrozenCounter, ODC::FrozenCounter>(a); };
inline ODC::BinaryOutputStatus ToODC(const opendnp3::BinaryOutputStatus a) {
    return Converter<opendnp3::BinaryOutputStatus, ODC::BinaryOutputStatus>(a); };
inline ODC::AnalogOutputStatus ToODC(const opendnp3::AnalogOutputStatus a) {
    return Converter<opendnp3::AnalogOutputStatus, ODC::AnalogOutputStatus>(a); };

inline opendnp3::Binary ToOpenDNP3(const ODC::Binary& a) {
    return Converter<ODC::Binary, opendnp3::Binary>(a); };
inline opendnp3::DoubleBitBinary ToOpenDNP3(const ODC::DoubleBitBinary& a) {
    return Converter<ODC::DoubleBitBinary, opendnp3::DoubleBitBinary>(a); };
inline opendnp3::Analog ToOpenDNP3(const ODC::Analog& a) {
    return Converter<ODC::Analog, opendnp3::Analog>(a); };
inline opendnp3::Counter ToOpenDNP3(const ODC::Counter& a) {
    return Converter<ODC::Counter, opendnp3::Counter>(a); };
inline opendnp3::FrozenCounter ToOpenDNP3(const ODC::FrozenCounter& a) {
    return Converter<ODC::FrozenCounter, opendnp3::FrozenCounter>(a); };
inline opendnp3::BinaryOutputStatus ToOpenDNP3(const ODC::BinaryOutputStatus& a) {
    return Converter<ODC::BinaryOutputStatus, opendnp3::BinaryOutputStatus>(a); };
inline opendnp3::AnalogOutputStatus ToOpenDNP3(const ODC::AnalogOutputStatus& a) {
    return Converter<ODC::AnalogOutputStatus, opendnp3::AnalogOutputStatus>(a); };

/// Map control types
inline ODC::CommandStatus ToODC(const opendnp3::CommandStatus& a) {
    return ODC::CommandStatusFromType(opendnp3::CommandStatusToType(a));
}
inline opendnp3::CommandStatus ToOpenDNP3(const ODC::CommandStatus& a) {
    return opendnp3::CommandStatusFromType(ODC::CommandStatusToType(a));
}

inline ODC::ControlRelayOutputBlock ToODC(const opendnp3::ControlRelayOutputBlock& a) {
    return ODC::ControlRelayOutputBlock(a.rawCode,a.count,a.onTimeMS,a.offTimeMS,ToODC(a.status)); };
inline opendnp3::ControlRelayOutputBlock ToOpenDNP3(const ODC::ControlRelayOutputBlock& a) {
    return opendnp3::ControlRelayOutputBlock(a.functionCode,a.count,a.onTimeMS,a.offTimeMS,ToOpenDNP3(a.status)); };

/// Map output types
template <class A, class B>
inline B AnalogOutputToODC(const A& a)
{
    return B(a.value, ToODC(a.status));
};
template <class A, class B>
inline B AnalogOutputToOpenDNP3(const A& a)
{
    return B(a.value, ToOpenDNP3(a.status));
};

inline ODC::AnalogOutputInt16 ToODC(const opendnp3::AnalogOutputInt16& a) {
    return AnalogOutputToODC<opendnp3::AnalogOutputInt16, ODC::AnalogOutputInt16>(a); };
inline ODC::AnalogOutputInt32 ToODC(const opendnp3::AnalogOutputInt32& a) {
    return AnalogOutputToODC<opendnp3::AnalogOutputInt32, ODC::AnalogOutputInt32>(a); };
inline ODC::AnalogOutputFloat32 ToODC(const opendnp3::AnalogOutputFloat32& a) {
    return AnalogOutputToODC<opendnp3::AnalogOutputFloat32, ODC::AnalogOutputFloat32>(a); };
inline ODC::AnalogOutputDouble64 ToODC(const opendnp3::AnalogOutputDouble64& a) {
    return AnalogOutputToODC<opendnp3::AnalogOutputDouble64, ODC::AnalogOutputDouble64>(a); };

inline opendnp3::AnalogOutputInt16 ToOpenDNP3(const ODC::AnalogOutputInt16& a) {
    return AnalogOutputToOpenDNP3<ODC::AnalogOutputInt16, opendnp3::AnalogOutputInt16>(a); };
inline opendnp3::AnalogOutputInt32 ToOpenDNP3(const ODC::AnalogOutputInt32& a) {
    return AnalogOutputToOpenDNP3<ODC::AnalogOutputInt32, opendnp3::AnalogOutputInt32>(a); };
inline opendnp3::AnalogOutputFloat32 ToOpenDNP3(const ODC::AnalogOutputFloat32& a) {
    return AnalogOutputToOpenDNP3<ODC::AnalogOutputFloat32, opendnp3::AnalogOutputFloat32>(a); };
inline opendnp3::AnalogOutputDouble64 ToOpenDNP3(const ODC::AnalogOutputDouble64& a) {
    return AnalogOutputToOpenDNP3<ODC::AnalogOutputDouble64, opendnp3::AnalogOutputDouble64>(a); };

/*
ODC::BinaryQuality ToODC(const opendnp3::BinaryQuality a) { return Converter<opendnp3::BinaryQuality, ODC::BinaryQuality>(a); };
ODC::DoubleBitBinaryQuality ToODC(const opendnp3::DoubleBitBinaryQuality a) { return Converter<opendnp3::DoubleBitBinaryQuality, ODC::DoubleBitBinaryQuality>(a); };
ODC::AnalogQuality ToODC(const opendnp3::AnalogQuality a) { return Converter<opendnp3::AnalogQuality, ODC::AnalogQuality>(a); };
ODC::CounterQuality ToODC(const opendnp3::CounterQuality a) { return Converter<opendnp3::CounterQuality, ODC::CounterQuality>(a); };
ODC::BinaryOutputStatusQuality ToODC(const opendnp3::BinaryOutputStatusQuality a) { return Converter<opendnp3::BinaryOutputStatusQuality, ODC::BinaryOutputStatusQuality>(a); };

opendnp3::BinaryQuality ToOpenDNP3(const ODC::BinaryQuality& a) { return Converter<ODC::BinaryQuality, opendnp3::BinaryQuality>(a); };
opendnp3::DoubleBitBinaryQuality ToOpenDNP3(const ODC::DoubleBitBinaryQuality& a) { return Converter<ODC::DoubleBitBinaryQuality, opendnp3::DoubleBitBinaryQuality>(a); };
opendnp3::AnalogQuality ToOpenDNP3(const ODC::AnalogQuality& a) { return Converter<ODC::AnalogQuality, opendnp3::AnalogQuality>(a); };
opendnp3::CounterQuality ToOpenDNP3(const ODC::CounterQuality& a) { return Converter<ODC::CounterQuality, opendnp3::CounterQuality>(a); };
opendnp3::BinaryOutputStatusQuality ToOpenDNP3(const ODC::BinaryOutputStatusQuality& a) { return Converter<ODC::BinaryOutputStatusQuality, opendnp3::BinaryOutputStatusQuality>(a); };
*/


/*
template <typename T>
opendnp3::PointIndexes GetIndexes(const opendnp3::Database& database);

template <>
opendnp3::PointIndexes GetIndexes<opendnp3::Binary>(const opendnp3::Database& database)
{
        return database.staticData.binaries.indexes;
}

template <>
opendnp3::PointIndexes GetIndexes<opendnp3::DoubleBitBinary>(const opendnp3::Database& database)
{
        return database.staticData.doubleBinaries.indexes;
}

template <>
opendnp3::PointIndexes GetIndexes<opendnp3::Analog>(const opendnp3::Database& database)
{
        return database.staticData.analogs.indexes;
}

template <>
opendnp3::PointIndexes GetIndexes<opendnp3::Counter>(const opendnp3::Database& database)
{
        return database.staticData.counters.indexes;
}

template <>
opendnp3::PointIndexes GetIndexes<opendnp3::FrozenCounter>(const opendnp3::Database& database)
{
        return database.staticData.frozenCounters.indexes;
}

template <>
opendnp3::PointIndexes GetIndexes<opendnp3::BinaryOutputStatus>(const opendnp3::Database& database)
{
        return database.staticData.binaryOutputStatii.indexes;
}

template <>
opendnp3::PointIndexes GetIndexes<opendnp3::AnalogOutputStatus>(const opendnp3::Database& database)
{
        return database.staticData.analogOutputStatii.indexes;
}

template<typename T>
T GetCurrentValue(opendnp3::IDatabase& database, uint16_t index)
{
        database.Modify()
        auto idx = GetIndexes<T>(database).GetPosition(index);
        return database.Values<T>()[idx].current;
}

template<typename T>
T UpdateQuality(opendnp3::IDatabase& database, uint8_t qual, uint16_t index)
{
        T meas(GetCurrentValue<T>(database, index));
        meas.quality = qual;
        return meas;
}*/

opendnp3::Binary::StaticVariation StringToStaticBinaryResponse(const std::string& str);
opendnp3::Analog::StaticVariation StringToStaticAnalogResponse(const std::string& str);
opendnp3::Counter::StaticVariation StringToStaticCounterResponse(const std::string& str);
opendnp3::Binary::EventVariation StringToEventBinaryResponse(const std::string& str);
opendnp3::Analog::EventVariation StringToEventAnalogResponse(const std::string& str);
opendnp3::Counter::EventVariation StringToEventCounterResponse(const std::string& str);

template <class ValueType, class IndexType>
class ArrayViewIterator
{
public:
	ArrayViewIterator(openpal::ArrayView<ValueType, IndexType>* data, IndexType pos)
		: _pos(pos)
		, _data(data)
	{ }

	bool
	operator!= (const ArrayViewIterator<ValueType, IndexType>& other) const
	{
		return _pos != other._pos;
	}

	ValueType& operator* () const
	{
		return (*_data)[_pos];
	}

	const ArrayViewIterator& operator++ ()
	{
		++_pos;
		// although not strictly necessary for a range-based for loop
		// following the normal convention of returning a value from
		// operator++ is a good idea.
		return *this;
	}

private:
	IndexType _pos;
	openpal::ArrayView<ValueType, IndexType>* _data;
};

namespace openpal
{
template <class ValueType, class IndexType>
ArrayViewIterator<ValueType, IndexType> begin(openpal::ArrayView<ValueType, IndexType>& data)
{
	return ArrayViewIterator<ValueType, IndexType>(&data, 0);
}

template <class ValueType, class IndexType>
ArrayViewIterator<ValueType, IndexType> end(openpal::ArrayView<ValueType, IndexType>& data)
{
	return ArrayViewIterator<ValueType, IndexType>(&data, data.Size());
}
}

#endif
