#pragma once
#include <memory>
#include <vector>
#include <optional>
#include <chrono>
#include "models/IRepository.h"
#include "dto/DTOs.h"

namespace medical::application {

class PatientService {
private:
    std::shared_ptr<domain::IPatientRepository> patientRepository_;
    
public:
    explicit PatientService(std::shared_ptr<domain::IPatientRepository> repository);
    
    std::optional<dto::PatientDTO> createPatient(const dto::CreatePatientRequestDTO& request);
    std::vector<dto::PatientDTO> searchPatientsByName(const std::string& fullName);
    std::optional<dto::PatientDTO> findById(int id);
    
private:
    dto::PatientDTO mapToDTO(const domain::Patient& patient);
    std::chrono::system_clock::time_point parseDate(const std::string& dateStr);
};

} // namespace medical::application
