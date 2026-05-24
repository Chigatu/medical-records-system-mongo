#include "PostgresPatientRepository.h"
#include <userver/storages/postgres/io/chrono.hpp>

namespace medical::infrastructure {

PostgresPatientRepository::PostgresPatientRepository(userver::storages::postgres::ClusterPtr cluster)
    : pg_cluster_(std::move(cluster)) {}

std::optional<domain::Patient> PostgresPatientRepository::findById(int id) {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT id, user_id, first_name, last_name, COALESCE(patronymic, ''), "
        "phone, address, birth_date, snils, policy_number "
        "FROM patients WHERE id = $1", id);
    
    if (result.IsEmpty()) return std::nullopt;
    
    auto row = result.Front();
    auto birth_date = row["birth_date"].As<std::chrono::system_clock::time_point>();
    
    return domain::Patient(
        row["id"].As<int>(),
        domain::FullName(
            row["first_name"].As<std::string>(),
            row["last_name"].As<std::string>(),
            row["patronymic"].As<std::string>()),
        row["phone"].As<std::string>(),
        row["address"].As<std::string>(),
        birth_date,
        row["snils"].As<std::string>(),
        row["policy_number"].As<std::string>(),
        row["user_id"].As<std::optional<int>>()
    );
}

std::vector<domain::Patient> PostgresPatientRepository::findAll() {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT id, user_id, first_name, last_name, COALESCE(patronymic, ''), "
        "phone, address, birth_date, snils, policy_number "
        "FROM patients ORDER BY id");
    
    std::vector<domain::Patient> patients;
    for (const auto& row : result) {
        auto birth_date = row["birth_date"].As<std::chrono::system_clock::time_point>();
        patients.emplace_back(
            row["id"].As<int>(),
            domain::FullName(
                row["first_name"].As<std::string>(),
                row["last_name"].As<std::string>(),
                row["patronymic"].As<std::string>()),
            row["phone"].As<std::string>(),
            row["address"].As<std::string>(),
            birth_date,
            row["snils"].As<std::string>(),
            row["policy_number"].As<std::string>(),
            row["user_id"].As<std::optional<int>>()
        );
    }
    return patients;
}

int PostgresPatientRepository::save(const domain::Patient& entity) {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        "INSERT INTO patients (first_name, last_name, patronymic, phone, address, "
        "birth_date, snils, policy_number, user_id) "
        "VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9) RETURNING id",
        entity.fullName().firstName(),
        entity.fullName().lastName(),
        entity.fullName().patronymic(),
        entity.phone(),
        entity.address(),
        entity.birthDate(),
        entity.snils(),
        entity.policyNumber(),
        entity.userId());
    
    return result.Front()["id"].As<int>();
}

void PostgresPatientRepository::update(const domain::Patient& entity) {
    pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        "UPDATE patients SET first_name=$1, last_name=$2, patronymic=$3, "
        "phone=$4, address=$5, birth_date=$6, snils=$7, policy_number=$8 "
        "WHERE id=$9",
        entity.fullName().firstName(), entity.fullName().lastName(),
        entity.fullName().patronymic(), entity.phone(), entity.address(),
        entity.birthDate(), entity.snils(), entity.policyNumber(), entity.id());
}

void PostgresPatientRepository::remove(int id) {
    pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        "DELETE FROM patients WHERE id = $1", id);
}

bool PostgresPatientRepository::exists(int id) {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT EXISTS(SELECT 1 FROM patients WHERE id = $1) AS exists", id);
    return result.Front()["exists"].As<bool>();
}

std::optional<domain::Patient> PostgresPatientRepository::findByUserId(int userId) {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT id, user_id, first_name, last_name, COALESCE(patronymic, ''), "
        "phone, address, birth_date, snils, policy_number "
        "FROM patients WHERE user_id = $1 LIMIT 1", userId);
    
    if (result.IsEmpty()) return std::nullopt;
    return findById(result.Front()["id"].As<int>());
}

std::vector<domain::Patient> PostgresPatientRepository::findByFullNameMask(const std::string& mask) {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT id, user_id, first_name, last_name, COALESCE(patronymic, ''), "
        "phone, address, birth_date, snils, policy_number "
        "FROM patients "
        "WHERE last_name || ' ' || first_name || ' ' || COALESCE(patronymic, '') LIKE $1 "
        "ORDER BY id",
        "%" + mask + "%");
    
    std::vector<domain::Patient> patients;
    for (const auto& row : result) {
        auto birth_date = row["birth_date"].As<std::chrono::system_clock::time_point>();
        patients.emplace_back(
            row["id"].As<int>(),
            domain::FullName(
                row["first_name"].As<std::string>(),
                row["last_name"].As<std::string>(),
                row["patronymic"].As<std::string>()),
            row["phone"].As<std::string>(),
            row["address"].As<std::string>(),
            birth_date,
            row["snils"].As<std::string>(),
            row["policy_number"].As<std::string>(),
            row["user_id"].As<std::optional<int>>()
        );
    }
    return patients;
}

std::optional<domain::Patient> PostgresPatientRepository::findBySnils(const std::string& snils) {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT id FROM patients WHERE snils = $1 LIMIT 1", snils);
    
    if (result.IsEmpty()) return std::nullopt;
    return findById(result.Front()["id"].As<int>());
}

}  // namespace medical::infrastructure
