#pragma once
#include "models/IRepository.h"
#include "models/Patient.h"
#include <unordered_map>
#include <mutex>

namespace medical::infrastructure {

class InMemoryPatientRepository : public domain::IPatientRepository {
private:
    std::unordered_map<int, domain::Patient> storage_;
    std::mutex mutex_;
    int nextId_;
    
public:
    InMemoryPatientRepository();
    
    std::optional<domain::Patient> findById(int id) override;
    std::vector<domain::Patient> findAll() override;
    int save(const domain::Patient& entity) override;
    void update(const domain::Patient& entity) override;
    void remove(int id) override;
    bool exists(int id) override;
    
    std::optional<domain::Patient> findByUserId(int userId) override;
    std::vector<domain::Patient> findByFullNameMask(const std::string& mask) override;
    std::optional<domain::Patient> findBySnils(const std::string& snils) override;
};

} // namespace medical::infrastructure
