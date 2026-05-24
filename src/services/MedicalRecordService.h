#pragma once
#include <memory>
#include <vector>
#include <optional>
#include <string>
#include "models/IRepository.h"
#include "dto/DTOs.h"

namespace medical::application {

class MedicalRecordService {
private:
    std::shared_ptr<domain::IMedicalRecordRepository> recordRepository_;
    std::shared_ptr<domain::IPatientRepository> patientRepository_;
    std::shared_ptr<domain::IUserRepository> userRepository_;
    
public:
    MedicalRecordService(
        std::shared_ptr<domain::IMedicalRecordRepository> recordRepo,
        std::shared_ptr<domain::IPatientRepository> patientRepo,
        std::shared_ptr<domain::IUserRepository> userRepo);
    
    std::optional<dto::MedicalRecordDTO> createRecord(const dto::CreateMedicalRecordRequestDTO& request);
    std::vector<dto::MedicalRecordDTO> getPatientRecords(int patientId);
    std::optional<dto::MedicalRecordDTO> findByCode(const std::string& code);
    
private:
    dto::MedicalRecordDTO mapToDTO(const domain::MedicalRecord& record);
};

} // namespace medical::application
