#include "PatientService.h"
#include "models/Patient.h"
#include "models/ValueObjects.h"
#include <sstream>
#include <iomanip>
#include <ctime>
#include <iostream>

namespace medical::application {

PatientService::PatientService(std::shared_ptr<domain::IPatientRepository> repository)
    : patientRepository_(repository) {}

std::chrono::system_clock::time_point PatientService::parseDate(const std::string& dateStr) {
    std::tm tm = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

std::optional<dto::PatientDTO> PatientService::createPatient(const dto::CreatePatientRequestDTO& request) {
    try {
        std::cout << "Creating patient with userId: " << request.userId.value_or(-1) << std::endl;
        
        // Проверяем уникальность СНИЛС
        auto existing = patientRepository_->findBySnils(request.snils);
        if (existing.has_value()) {
            std::cout << "Patient with SNILS already exists" << std::endl;
            return std::nullopt;
        }
        
        // Создаем FullName
        domain::FullName fullName(request.firstName, request.lastName, request.patronymic);
        
        // Парсим дату рождения
        auto birthDate = parseDate(request.birthDate);
        
        // Создаем пациента (userId может быть nullopt)
        domain::Patient patient(0, fullName, request.phone, request.address, 
                               birthDate, request.snils, request.policyNumber, 
                               request.userId);
        
        // Сохраняем
        int id = patientRepository_->save(patient);
        
        // Получаем сохраненного
        auto saved = patientRepository_->findById(id);
        if (saved.has_value()) {
            return mapToDTO(saved.value());
        }
        
        return std::nullopt;
        
    } catch (const std::exception& e) {
        std::cout << "Error creating patient: " << e.what() << std::endl;
        return std::nullopt;
    }
}

std::vector<dto::PatientDTO> PatientService::searchPatientsByName(const std::string& fullName) {
    auto patients = patientRepository_->findByFullNameMask(fullName);
    std::vector<dto::PatientDTO> result;
    for (const auto& patient : patients) {
        result.push_back(mapToDTO(patient));
    }
    return result;
}

std::optional<dto::PatientDTO> PatientService::findById(int id) {
    auto patient = patientRepository_->findById(id);
    if (patient.has_value()) {
        return mapToDTO(patient.value());
    }
    return std::nullopt;
}

dto::PatientDTO PatientService::mapToDTO(const domain::Patient& patient) {
    dto::PatientDTO dto;
    dto.id = patient.id();
    dto.fullName = patient.fullName().fullName();
    dto.phone = patient.phone();
    dto.address = patient.address();
    dto.age = patient.age();
    dto.snils = patient.snils();
    dto.policyNumber = patient.policyNumber();
    return dto;
}

} // namespace medical::application
