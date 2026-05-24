#pragma once
#include <userver/storages/postgres/cluster.hpp>
#include "models/MedicalRecord.h"
#include "models/IRepository.h"
#include <optional>
#include <vector>

namespace medical::infrastructure {

class PostgresMedicalRecordRepository : public domain::IMedicalRecordRepository {
private:
    userver::storages::postgres::ClusterPtr pg_cluster_;

public:
    explicit PostgresMedicalRecordRepository(userver::storages::postgres::ClusterPtr cluster);
    
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

}  // namespace medical::infrastructure
