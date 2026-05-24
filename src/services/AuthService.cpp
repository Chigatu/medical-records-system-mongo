#include "AuthService.h"
#include <iostream>

namespace medical::application {

AuthService::AuthService(std::shared_ptr<UserService> userService,
                        std::shared_ptr<infrastructure::JWTService> jwtService)
    : userService_(userService)
    , jwtService_(jwtService) {}

std::optional<dto::LoginResponseDTO> AuthService::login(const dto::LoginRequestDTO& request) {
    std::cout << "=== AuthService::login ===" << std::endl;
    std::cout << "Login: " << request.login << ", Password: " << request.password << std::endl;
    
    auto user = userService_->authenticateUser(request.login, request.password);
    if (!user.has_value()) {
        std::cout << "Authentication failed!" << std::endl;
        return std::nullopt;
    }
    
    std::cout << "User authenticated, ID: " << user->id << std::endl;
    
    std::string token = jwtService_->generateToken(user->id, user->login);
    std::cout << "Token generated: " << token << std::endl;
    
    dto::LoginResponseDTO response;
    response.token = token;
    response.user = user.value();
    
    return response;
}

std::optional<int> AuthService::validateToken(const std::string& token) {
    return jwtService_->validateToken(token);
}

} // namespace medical::application
