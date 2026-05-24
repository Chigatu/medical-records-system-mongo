#include "UserService.h"
#include "models/User.h"
#include "models/ValueObjects.h"
#include <stdexcept>
#include <iostream>

namespace medical::application {

UserService::UserService(std::shared_ptr<domain::IUserRepository> repository)
    : userRepository_(repository) {}

std::optional<dto::UserDTO> UserService::registerUser(const dto::RegisterRequestDTO& request) {
    try {
        std::cout << "=== UserService::registerUser ===" << std::endl;
        std::cout << "Login: " << request.login << ", Email: " << request.email << std::endl;
        
        // Проверяем, не существует ли уже пользователь с таким логином
        auto existing = userRepository_->findByLogin(request.login);
        if (existing.has_value()) {
            std::cout << "User already exists!" << std::endl;
            return std::nullopt;
        }
        
        // Создаем email value object
        domain::Email email(request.email);
        
        // Создаем FullName value object
        domain::FullName fullName(request.firstName, request.lastName, request.patronymic);
        
        // Хешируем пароль
        std::string passwordHash = infrastructure::PasswordHasher::hash(request.password);
        std::cout << "Password hash: " << passwordHash << std::endl;
        
        // Создаем пользователя
        domain::User user(0, request.login, email, fullName, passwordHash);
        
        // Сохраняем
        int id = userRepository_->save(user);
        std::cout << "Saved user with ID: " << id << std::endl;
        
        // Получаем сохраненного пользователя
        auto saved = userRepository_->findById(id);
        if (saved.has_value()) {
            std::cout << "User retrieved successfully" << std::endl;
            return mapToDTO(saved.value());
        }
        
        std::cout << "Failed to retrieve saved user" << std::endl;
        return std::nullopt;
        
    } catch (const std::exception& e) {
        std::cout << "Exception in registerUser: " << e.what() << std::endl;
        return std::nullopt;
    }
}

std::optional<dto::UserDTO> UserService::authenticateUser(const std::string& login, const std::string& password) {
    std::cout << "=== UserService::authenticateUser ===" << std::endl;
    std::cout << "Login: " << login << std::endl;
    
    auto user = userRepository_->findByLogin(login);
    if (!user.has_value()) {
        std::cout << "User not found!" << std::endl;
        return std::nullopt;
    }
    
    std::cout << "User found, stored hash: " << user->passwordHash() << std::endl;
    std::string inputHash = infrastructure::PasswordHasher::hash(password);
    std::cout << "Input password hash: " << inputHash << std::endl;
    
    if (!infrastructure::PasswordHasher::verify(password, user->passwordHash())) {
        std::cout << "Password verification failed!" << std::endl;
        return std::nullopt;
    }
    
    if (!user->isActive()) {
        std::cout << "User is not active!" << std::endl;
        return std::nullopt;
    }
    
    std::cout << "Authentication successful!" << std::endl;
    return mapToDTO(user.value());
}

std::vector<dto::UserDTO> UserService::searchUsersByName(const std::string& mask) {
    auto users = userRepository_->findByFullNameMask(mask);
    std::vector<dto::UserDTO> result;
    for (const auto& user : users) {
        result.push_back(mapToDTO(user));
    }
    return result;
}

std::optional<dto::UserDTO> UserService::findById(int id) {
    auto user = userRepository_->findById(id);
    if (user.has_value()) {
        return mapToDTO(user.value());
    }
    return std::nullopt;
}

std::optional<dto::UserDTO> UserService::findByLogin(const std::string& login) {
    auto user = userRepository_->findByLogin(login);
    if (user.has_value()) {
        return mapToDTO(user.value());
    }
    return std::nullopt;
}

dto::UserDTO UserService::mapToDTO(const domain::User& user) {
    dto::UserDTO dto;
    dto.id = user.id();
    dto.login = user.login();
    dto.email = user.email().value();
    dto.fullName = user.fullName().fullName();
    dto.isActive = user.isActive();
    return dto;
}

} // namespace medical::application
