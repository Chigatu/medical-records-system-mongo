#include "MedicalRecordService.h"
#include "models/MedicalRecord.h"
#include "models/ValueObjects.h"
#include <chrono>
#include <sstream>
#include <iomanip>
#include <iostream>

namespace medical::application {

MedicalRecordService::MedicalRecordService(
    std::shared_ptr<domain::IMedicalRecordRepository> recordRepo,
    std::shared_ptr<domain::IPatientRepository> patientRepo,
    std::shared_ptr<domain::IUserRepository> userRepo)
    : recordRepository_(recordRepo)
    , patientRepository_(patientRepo)
    , userRepository_(userRepo) {}

std::optional<dto::MedicalRecordDTO> MedicalRecordService::createRecord(
    const dto::CreateMedicalRecordRequestDTO& request) {
    try {
        std::cout << "=== MedicalRecordService::createRecord ===" << std::endl;
        std::cout << "PatientId: " << request.patientId << ", DoctorId: " << request.doctorId << std::endl;
        
        // Проверяем существование пациента
        bool patientExists = patientRepository_->exists(request.patientId);
        std::cout << "Patient exists: " << patientExists << std::endl;
        if (!patientExists) {
            std::cout << "Patient not found!" << std::endl;
            return std::nullopt;
        }
        
        // Проверяем существование врача
        bool doctorExists = userRepository_->exists(request.doctorId);
        std::cout << "Doctor exists: " << doctorExists << std::endl;
        if (!doctorExists) {
            std::cout << "Doctor not found!" << std::endl;
            return std::nullopt;
        }
        
        // Генерируем код записи
        auto code = domain::MedicalRecordCode::generate();
        std::cout << "Generated code: " << code.value() << std::endl;
        
        // Создаем диагноз
        domain::Diagnosis diagnosis(request.diagnosisCode, request.diagnosisDescription);
        
        // Создаем запись
        domain::MedicalRecord record(0, code, request.patientId, request.doctorId,
                                    diagnosis, request.complaints);
        
        // Сохраняем
        int id = recordRepository_->save(record);
        std::cout << "Saved record with ID: " << id << std::endl;
        
        // Получаем сохраненную запись
        auto saved = recordRepository_->findById(id);
        if (saved.has_value()) {
            return mapToDTO(saved.value());
        }
        
        return std::nullopt;
        
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
        return std::nullopt;
    }
}

std::vector<dto::MedicalRecordDTO> MedicalRecordService::getPatientRecords(int patientId) {
    auto records = recordRepository_->findByPatientId(patientId);
    std::vector<dto::MedicalRecordDTO> result;
    for (const auto& record : records) {
        result.push_back(mapToDTO(record));
    }
    return result;
}

std::optional<dto::MedicalRecordDTO> MedicalRecordService::findByCode(const std::string& code) {
    domain::MedicalRecordCode recordCode(code);
    auto record = recordRepository_->findByCode(recordCode);
    if (record.has_value()) {
        return mapToDTO(record.value());
    }
    return std::nullopt;
}

dto::MedicalRecordDTO MedicalRecordService::mapToDTO(const domain::MedicalRecord& record) {
    dto::MedicalRecordDTO dto;
    dto.id = record.id();
    dto.code = record.code().value();
    dto.patientId = record.patientId();
    dto.doctorId = record.doctorId();
    dto.diagnosisCode = record.diagnosis().code();
    dto.diagnosisDescription = record.diagnosis().description();
    dto.complaints = record.complaints();
    
    auto time = std::chrono::system_clock::to_time_t(record.createdAt());
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    dto.createdAt = ss.str();
    
    return dto;
}

} // namespace medical::application
