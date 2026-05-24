#include "PasswordHasher.h"
#include <sstream>
#include <iomanip>

namespace medical::infrastructure {

std::string PasswordHasher::hash(const std::string& password) {
    std::string salt = "medical_salt_2024";
    std::string combined = password + salt;
    size_t hash = std::hash<std::string>{}(combined);
    std::stringstream ss;
    ss << std::hex << hash;
    return ss.str();
}

bool PasswordHasher::verify(const std::string& password, const std::string& hash) {
    return hash == PasswordHasher::hash(password);
}

} // namespace medical::infrastructure
