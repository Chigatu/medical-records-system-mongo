#include "JWTService.h"
#include <sstream>
#include <iomanip>
#include <random>
#include <chrono>
#include <algorithm>
#include <iostream>

namespace medical::infrastructure {

JWTService::JWTService(const std::string& secret, int expirationSeconds)
    : secret_(secret)
    , expirationSeconds_(expirationSeconds) {}

std::string JWTService::generateToken(int userId, const std::string& login) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(100000, 999999);
    
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(
        now.time_since_epoch()).count();
    
    std::stringstream ss;
    ss << userId << "_" << login << "_" << timestamp << "_" << dis(gen);
    
    std::string token = base64Encode(ss.str());
    
    {
        std::lock_guard<std::mutex> lock(mutex_);
        validTokens_[token] = userId;
    }
    
    std::cout << "Generated token for user " << userId << ": " << token.substr(0, 20) << "..." << std::endl;
    
    return token;
}

std::optional<int> JWTService::validateToken(const std::string& token) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    std::cout << "Validating token: " << token.substr(0, 20) << "..." << std::endl;
    std::cout << "Total valid tokens: " << validTokens_.size() << std::endl;
    
    auto it = validTokens_.find(token);
    if (it != validTokens_.end()) {
        std::cout << "Token valid for user: " << it->second << std::endl;
        return it->second;
    }
    
    std::cout << "Token not found!" << std::endl;
    return std::nullopt;
}

std::string JWTService::base64Encode(const std::string& input) {
    static const char* base64_chars = 
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";
    
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    int in_len = input.size();
    const char* bytes_to_encode = input.c_str();
    
    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            
            for(i = 0; i < 4; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }
    
    if (i) {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';
        
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        
        for (j = 0; j < i + 1; j++)
            ret += base64_chars[char_array_4[j]];
        
        while((i++ < 3))
            ret += '=';
    }
    
    return ret;
}

std::string JWTService::base64Decode(const std::string& input) {
    // Упрощенная реализация для отладки
    return input;
}

} // namespace medical::infrastructure
