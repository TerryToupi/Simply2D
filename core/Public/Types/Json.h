#pragma once

#include <nlohmann/json.hpp>

//#include "EASTL/map.h"
//#include "EASTL/vector.h"
//#include "EASTL/string.h"

using json = nlohmann::json;

//using json = nlohmann::basic_json <
//	// ObjectType
//	eastl::map<
//	eastl::string,
//	nlohmann::basic_json<
//	eastl::map<eastl::string, void, eastl::less<eastl::string>, eastl::allocator>,
//	eastl::vector<void, eastl::allocator>,
//	eastl::string,
//	std::int64_t,
//	std::uint64_t,
//	double,
//	bool,
//	eastl::vector<std::uint8_t, eastl::allocator>,
//	eastl::allocator
//	>,
//	eastl::less<eastl::string>,
//	eastl::allocator
//	>,
//
//	// ArrayType
//	eastl::vector<
//	nlohmann::basic_json<
//	eastl::map<eastl::string, void, eastl::less<eastl::string>, eastl::allocator>,
//	eastl::vector<void, eastl::allocator>,
//	eastl::string,
//	std::int64_t,
//	std::uint64_t,
//	double,
//	bool,
//	eastl::vector<std::uint8_t, eastl::allocator>,
//	eastl::allocator
//	>,
//	eastl::allocator
//	>,
//
//	// StringType
//	eastl::string,
//
//	// IntegerType
//	std::int64_t,
//
//	// UnsignedType
//	std::uint64_t,
//
//	// FloatType
//	double,
//
//	// BooleanType
//	bool,
//
//	// BinaryType
//	eastl::vector<std::uint8_t, eastl::allocator>,
//
//	// AllocatorType
//	eastl::allocator
//> ;


