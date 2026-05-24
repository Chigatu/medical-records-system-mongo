#pragma once
#include "models/IRepository.h"
#include "models/MedicalRecord.h"
#include <unordered_map>
#include <mutex>

namespace medical::infrastructure {

class InMemoryMedicalRecordRepository : public domain::IMedicalRecordRepository {
private:
    std::unordered_map<int, domain::MedicalRecord> storage_;
    std::mutex mutex_;
    int nextId_;
    
public:
    InMemoryMedicalRecordRepository();
    
    std::optional<domain::MedicalRecord> findById(int id) override;
    std::vector<domain::MedicalRecord> findAll() override;
    int save(const domain::MedicalRecord& entity) override;
    void update(const domain::MedicalRecord& entity) override;
    void remove(int id) override;
    bool exists(int id) override;
    
    std::vector<domain::MedicalRecord> findByPatientId(int patientId) override;
    std::optional<domain::MedicalRecord> findByCode(const domain::MedicalRecordCode& code) override;
    std::vector<domain::MedicalRecord> findByDoctorId(int doctorId) override;
};

} // namespace medical::infrastructure
