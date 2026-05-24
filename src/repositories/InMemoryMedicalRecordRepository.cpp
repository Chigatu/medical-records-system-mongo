#include "InMemoryMedicalRecordRepository.h"
#include <algorithm>

namespace medical::infrastructure {

InMemoryMedicalRecordRepository::InMemoryMedicalRecordRepository() : nextId_(1) {}

std::optional<domain::MedicalRecord> InMemoryMedicalRecordRepository::findById(int id) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = storage_.find(id);
    if (it != storage_.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::vector<domain::MedicalRecord> InMemoryMedicalRecordRepository::findAll() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<domain::MedicalRecord> result;
    for (const auto& pair : storage_) {
        result.push_back(pair.second);
    }
    return result;
}

int InMemoryMedicalRecordRepository::save(const domain::MedicalRecord& entity) {
    std::lock_guard<std::mutex> lock(mutex_);
    int id = entity.id();
    if (id == 0) {
        id = nextId_++;
        // Создаем копию с правильным ID
        domain::MedicalRecord recordWithId(id, entity.code(), entity.patientId(),
                                           entity.doctorId(), entity.diagnosis(),
                                           entity.complaints());
        storage_[id] = recordWithId;
    } else {
        storage_[id] = entity;
        if (id >= nextId_) {
            nextId_ = id + 1;
        }
    }
    return id;
}

void InMemoryMedicalRecordRepository::update(const domain::MedicalRecord& entity) {
    std::lock_guard<std::mutex> lock(mutex_);
    storage_[entity.id()] = entity;
}

void InMemoryMedicalRecordRepository::remove(int id) {
    std::lock_guard<std::mutex> lock(mutex_);
    storage_.erase(id);
}

bool InMemoryMedicalRecordRepository::exists(int id) {
    std::lock_guard<std::mutex> lock(mutex_);
    return storage_.find(id) != storage_.end();
}

std::vector<domain::MedicalRecord> InMemoryMedicalRecordRepository::findByPatientId(int patientId) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<domain::MedicalRecord> result;
    for (const auto& pair : storage_) {
        if (pair.second.patientId() == patientId) {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::optional<domain::MedicalRecord> InMemoryMedicalRecordRepository::findByCode(const domain::MedicalRecordCode& code) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (const auto& pair : storage_) {
        if (pair.second.code().value() == code.value()) {
            return pair.second;
        }
    }
    return std::nullopt;
}

std::vector<domain::MedicalRecord> InMemoryMedicalRecordRepository::findByDoctorId(int doctorId) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<domain::MedicalRecord> result;
    for (const auto& pair : storage_) {
        if (pair.second.doctorId() == doctorId) {
            result.push_back(pair.second);
        }
    }
    return result;
}

} // namespace medical::infrastructure
