#pragma once
#include <string>
#include <optional>
#include <unordered_map>
#include <mutex>

namespace medical::infrastructure {

class JWTService {
private:
    std::string secret_;
    int expirationSeconds_;
    std::unordered_map<std::string, int> validTokens_;
    std::mutex mutex_;
    
public:
    JWTService(const std::string& secret = "medical_secret_key_2024", 
               int expirationSeconds = 3600);
    
    std::string generateToken(int userId, const std::string& login);
    std::optional<int> validateToken(const std::string& token);
    
private:
    std::string base64Encode(const std::string& input);
    std::string base64Decode(const std::string& input);
};

} // namespace medical::infrastructure
