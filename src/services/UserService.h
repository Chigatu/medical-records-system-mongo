#pragma once
#include <memory>
#include <vector>
#include <optional>
#include "models/IRepository.h"
#include "dto/DTOs.h"
#include "services/PasswordHasher.h"

namespace medical::application {

class UserService {
private:
    std::shared_ptr<domain::IUserRepository> userRepository_;
    
public:
    explicit UserService(std::shared_ptr<domain::IUserRepository> repository);
    
    std::optional<dto::UserDTO> registerUser(const dto::RegisterRequestDTO& request);
    std::optional<dto::UserDTO> authenticateUser(const std::string& login, const std::string& password);
    std::vector<dto::UserDTO> searchUsersByName(const std::string& mask);
    std::optional<dto::UserDTO> findById(int id);
    std::optional<dto::UserDTO> findByLogin(const std::string& login);
    
private:
    dto::UserDTO mapToDTO(const domain::User& user);
};

} // namespace medical::application
