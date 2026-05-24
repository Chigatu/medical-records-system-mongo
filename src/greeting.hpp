#pragma once

#include <string>
#include <string_view>

namespace medical_api {

enum class UserType { kFirstTime, kKnown };

std::string SayHelloTo(std::string_view name, UserType type);

}  // namespace medical_api