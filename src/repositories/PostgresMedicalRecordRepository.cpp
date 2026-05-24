#include "PostgresMedicalRecordRepository.h"
#include <userver/storages/postgres/io/chrono.hpp>

namespace medical::infrastructure {

PostgresMedicalRecordRepository::PostgresMedicalRecordRepository(userver::storages::postgres::ClusterPtr cluster)
    : pg_cluster_(std::move(cluster)) {}

std::optional<domain::MedicalRecord> PostgresMedicalRecordRepository::findById(int id) {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT id, code, patient_id, doctor_id, diagnosis_code, "
        "COALESCE(diagnosis_description, ''), COALESCE(complaints, ''), "
        "COALESCE(recommendations, ''), created_at "
        "FROM medical_records WHERE id = $1", id);
    
    if (result.IsEmpty()) return std::nullopt;
    
    auto row = result.Front();
    domain::MedicalRecordCode code(row["code"].As<std::string>());
    domain::Diagnosis diagnosis(
        row["diagnosis_code"].As<std::string>(),
        row["diagnosis_description"].As<std::string>());
    
    return domain::MedicalRecord(
        row["id"].As<int>(),
        code,
        row["patient_id"].As<int>(),
        row["doctor_id"].As<int>(),
        diagnosis,
        row["complaints"].As<std::string>()
    );
}

std::vector<domain::MedicalRecord> PostgresMedicalRecordRepository::findAll() {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT id, code, patient_id, doctor_id, diagnosis_code, "
        "COALESCE(diagnosis_description, ''), COALESCE(complaints, ''), "
        "COALESCE(recommendations, ''), created_at "
        "FROM medical_records ORDER BY created_at DESC");
    
    std::vector<domain::MedicalRecord> records;
    for (const auto& row : result) {
        domain::MedicalRecordCode code(row["code"].As<std::string>());
        domain::Diagnosis diagnosis(
            row["diagnosis_code"].As<std::string>(),
            row["diagnosis_description"].As<std::string>());
        
        records.emplace_back(
            row["id"].As<int>(),
            code,
            row["patient_id"].As<int>(),
            row["doctor_id"].As<int>(),
            diagnosis,
            row["complaints"].As<std::string>()
        );
    }
    return records;
}

int PostgresMedicalRecordRepository::save(const domain::MedicalRecord& entity) {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        "INSERT INTO medical_records (code, patient_id, doctor_id, "
        "diagnosis_code, diagnosis_description, complaints) "
        "VALUES ($1, $2, $3, $4, $5, $6) RETURNING id",
        entity.code().value(),
        entity.patientId(),
        entity.doctorId(),
        entity.diagnosis().code(),
        entity.diagnosis().description(),
        entity.complaints());
    
    return result.Front()["id"].As<int>();
}

void PostgresMedicalRecordRepository::update(const domain::MedicalRecord& entity) {
    pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        "UPDATE medical_records SET diagnosis_code=$1, diagnosis_description=$2, "
        "complaints=$3, recommendations=$4 WHERE id=$5",
        entity.diagnosis().code(), entity.diagnosis().description(),
        entity.complaints(), entity.recommendations(), entity.id());
}

void PostgresMedicalRecordRepository::remove(int id) {
    pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        "DELETE FROM medical_records WHERE id = $1", id);
}

bool PostgresMedicalRecordRepository::exists(int id) {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT EXISTS(SELECT 1 FROM medical_records WHERE id = $1) AS exists", id);
    return result.Front()["exists"].As<bool>();
}

std::vector<domain::MedicalRecord> PostgresMedicalRecordRepository::findByPatientId(int patientId) {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT id, code, patient_id, doctor_id, diagnosis_code, "
        "COALESCE(diagnosis_description, ''), COALESCE(complaints, ''), "
        "COALESCE(recommendations, ''), created_at "
        "FROM medical_records WHERE patient_id = $1 ORDER BY created_at DESC", patientId);
    
    std::vector<domain::MedicalRecord> records;
    for (const auto& row : result) {
        domain::MedicalRecordCode code(row["code"].As<std::string>());
        domain::Diagnosis diagnosis(
            row["diagnosis_code"].As<std::string>(),
            row["diagnosis_description"].As<std::string>());
        
        records.emplace_back(
            row["id"].As<int>(),
            code,
            row["patient_id"].As<int>(),
            row["doctor_id"].As<int>(),
            diagnosis,
            row["complaints"].As<std::string>()
        );
    }
    return records;
}

std::optional<domain::MedicalRecord> PostgresMedicalRecordRepository::findByCode(
    const domain::MedicalRecordCode& code) {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT id FROM medical_records WHERE code = $1", code.value());
    
    if (result.IsEmpty()) return std::nullopt;
    return findById(result.Front()["id"].As<int>());
}

std::vector<domain::MedicalRecord> PostgresMedicalRecordRepository::findByDoctorId(int doctorId) {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT id, code, patient_id, doctor_id, diagnosis_code, "
        "COALESCE(diagnosis_description, ''), COALESCE(complaints, ''), "
        "COALESCE(recommendations, ''), created_at "
        "FROM medical_records WHERE doctor_id = $1 ORDER BY created_at DESC", doctorId);
    
    std::vector<domain::MedicalRecord> records;
    for (const auto& row : result) {
        domain::MedicalRecordCode code(row["code"].As<std::string>());
        domain::Diagnosis diagnosis(
            row["diagnosis_code"].As<std::string>(),
            row["diagnosis_description"].As<std::string>());
        
        records.emplace_back(
            row["id"].As<int>(),
            code,
            row["patient_id"].As<int>(),
            row["doctor_id"].As<int>(),
            diagnosis,
            row["complaints"].As<std::string>()
        );
    }
    return records;
}

}  // namespace medical::infrastructure
