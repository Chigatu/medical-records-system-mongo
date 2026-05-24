#pragma once
#include <memory>
#include <userver/storages/postgres/cluster.hpp>
#include "InMemoryUserRepository.h"
#include "InMemoryPatientRepository.h"
#include "InMemoryMedicalRecordRepository.h"
#include "PostgresUserRepository.h"
#include "PostgresPatientRepository.h"
#include "PostgresMedicalRecordRepository.h"
#include "../services/JWTService.h"

namespace medical_api {

class RepositoryFactory {
public:
    // In-memory репозитории (fallback)
    static std::shared_ptr<medical::infrastructure::InMemoryUserRepository> GetInMemoryUserRepo() {
        static auto repo = std::make_shared<medical::infrastructure::InMemoryUserRepository>();
        return repo;
    }
    
    static std::shared_ptr<medical::infrastructure::InMemoryPatientRepository> GetInMemoryPatientRepo() {
        static auto repo = std::make_shared<medical::infrastructure::InMemoryPatientRepository>();
        return repo;
    }
    
    static std::shared_ptr<medical::infrastructure::InMemoryMedicalRecordRepository> GetInMemoryMedicalRecordRepo() {
        static auto repo = std::make_shared<medical::infrastructure::InMemoryMedicalRecordRepository>();
        return repo;
    }
    
    // PostgreSQL репозитории
    static void InitPostgresRepositories(userver::storages::postgres::ClusterPtr pg_cluster) {
        pg_cluster_ = std::move(pg_cluster);
    }
    
    static std::shared_ptr<medical::infrastructure::PostgresUserRepository> GetPostgresUserRepo() {
        if (!postgres_user_repo_) {
            postgres_user_repo_ = std::make_shared<medical::infrastructure::PostgresUserRepository>(pg_cluster_);
        }
        return postgres_user_repo_;
    }
    
    static std::shared_ptr<medical::infrastructure::PostgresPatientRepository> GetPostgresPatientRepo() {
        if (!postgres_patient_repo_) {
            postgres_patient_repo_ = std::make_shared<medical::infrastructure::PostgresPatientRepository>(pg_cluster_);
        }
        return postgres_patient_repo_;
    }
    
    static std::shared_ptr<medical::infrastructure::PostgresMedicalRecordRepository> GetPostgresMedicalRecordRepo() {
        if (!postgres_medical_record_repo_) {
            postgres_medical_record_repo_ = std::make_shared<medical::infrastructure::PostgresMedicalRecordRepository>(pg_cluster_);
        }
        return postgres_medical_record_repo_;
    }
    
    static std::shared_ptr<medical::infrastructure::JWTService> GetJwtService() {
        static auto jwt = std::make_shared<medical::infrastructure::JWTService>();
        return jwt;
    }
    
    // Флаг использования PostgreSQL
    static bool UsePostgres() { return use_postgres_; }
    static void SetUsePostgres(bool value) { use_postgres_ = value; }

private:
    static inline userver::storages::postgres::ClusterPtr pg_cluster_;
    static inline std::shared_ptr<medical::infrastructure::PostgresUserRepository> postgres_user_repo_;
    static inline std::shared_ptr<medical::infrastructure::PostgresPatientRepository> postgres_patient_repo_;
    static inline std::shared_ptr<medical::infrastructure::PostgresMedicalRecordRepository> postgres_medical_record_repo_;
    static inline bool use_postgres_ = false;
};

}  // namespace medical_api
