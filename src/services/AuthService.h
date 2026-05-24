#pragma once
#include <memory>
#include <optional>
#include "UserService.h"
#include "services/JWTService.h"
#include "dto/DTOs.h"

namespace medical::application {

class AuthService {
private:
    std::shared_ptr<UserService> userService_;
    std::shared_ptr<infrastructure::JWTService> jwtService_;
    
public:
    AuthService(std::shared_ptr<UserService> userService,
                std::shared_ptr<infrastructure::JWTService> jwtService);
    
    std::optional<dto::LoginResponseDTO> login(const dto::LoginRequestDTO& request);
    std::optional<int> validateToken(const std::string& token);
};

} // namespace medical::application
