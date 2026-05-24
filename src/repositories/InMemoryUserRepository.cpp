#include "InMemoryUserRepository.h"
#include <algorithm>

namespace medical::infrastructure {

InMemoryUserRepository::InMemoryUserRepository() : nextId_(1) {}

std::optional<domain::User> InMemoryUserRepository::findById(int id) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = storage_.find(id);
    if (it != storage_.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::vector<domain::User> InMemoryUserRepository::findAll() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<domain::User> result;
    for (const auto& pair : storage_) {
        result.push_back(pair.second);
    }
    return result;
}

int InMemoryUserRepository::save(const domain::User& entity) {
    std::lock_guard<std::mutex> lock(mutex_);
    int id = entity.id();
    if (id == 0) {
        id = nextId_++;
        // Создаем копию с правильным ID
        domain::User userWithId(id, entity.login(), entity.email(), 
                                entity.fullName(), entity.passwordHash());
        storage_[id] = userWithId;
    } else {
        storage_[id] = entity;
    }
    return id;
}

void InMemoryUserRepository::update(const domain::User& entity) {
    std::lock_guard<std::mutex> lock(mutex_);
    storage_[entity.id()] = entity;
}

void InMemoryUserRepository::remove(int id) {
    std::lock_guard<std::mutex> lock(mutex_);
    storage_.erase(id);
}

bool InMemoryUserRepository::exists(int id) {
    std::lock_guard<std::mutex> lock(mutex_);
    return storage_.find(id) != storage_.end();
}

std::optional<domain::User> InMemoryUserRepository::findByLogin(const std::string& login) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (const auto& pair : storage_) {
        if (pair.second.login() == login) {
            return pair.second;
        }
    }
    return std::nullopt;
}

std::vector<domain::User> InMemoryUserRepository::findByFullNameMask(const std::string& mask) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<domain::User> result;
    for (const auto& pair : storage_) {
        if (pair.second.matchesNameMask(mask)) {
            result.push_back(pair.second);
        }
    }
    return result;
}

} // namespace medical::infrastructure
