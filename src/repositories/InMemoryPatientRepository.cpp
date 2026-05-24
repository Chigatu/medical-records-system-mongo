#include "InMemoryPatientRepository.h"
#include <algorithm>

namespace medical::infrastructure {

InMemoryPatientRepository::InMemoryPatientRepository() : nextId_(1) {}

std::optional<domain::Patient> InMemoryPatientRepository::findById(int id) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = storage_.find(id);
    if (it != storage_.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::vector<domain::Patient> InMemoryPatientRepository::findAll() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<domain::Patient> result;
    for (const auto& pair : storage_) {
        result.push_back(pair.second);
    }
    return result;
}

int InMemoryPatientRepository::save(const domain::Patient& entity) {
    std::lock_guard<std::mutex> lock(mutex_);
    int id = entity.id();
    if (id == 0) {
        id = nextId_++;
        // Создаем копию с правильным ID
        domain::Patient patientWithId(id, entity.fullName(), entity.phone(), 
                                      entity.address(), entity.birthDate(),
                                      entity.snils(), entity.policyNumber(),
                                      entity.userId());
        storage_[id] = patientWithId;
    } else {
        storage_[id] = entity;
        if (id >= nextId_) {
            nextId_ = id + 1;
        }
    }
    return id;
}

void InMemoryPatientRepository::update(const domain::Patient& entity) {
    std::lock_guard<std::mutex> lock(mutex_);
    storage_[entity.id()] = entity;
}

void InMemoryPatientRepository::remove(int id) {
    std::lock_guard<std::mutex> lock(mutex_);
    storage_.erase(id);
}

bool InMemoryPatientRepository::exists(int id) {
    std::lock_guard<std::mutex> lock(mutex_);
    return storage_.find(id) != storage_.end();
}

std::optional<domain::Patient> InMemoryPatientRepository::findByUserId(int userId) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (const auto& pair : storage_) {
        if (pair.second.userId() && pair.second.userId().value() == userId) {
            return pair.second;
        }
    }
    return std::nullopt;
}

std::vector<domain::Patient> InMemoryPatientRepository::findByFullNameMask(const std::string& mask) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<domain::Patient> result;
    for (const auto& pair : storage_) {
        if (pair.second.matchesNameMask(mask)) {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::optional<domain::Patient> InMemoryPatientRepository::findBySnils(const std::string& snils) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (const auto& pair : storage_) {
        if (pair.second.snils() == snils) {
            return pair.second;
        }
    }
    return std::nullopt;
}

} // namespace medical::infrastructure
