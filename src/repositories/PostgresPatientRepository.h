#pragma once
#include <userver/storages/postgres/cluster.hpp>
#include "models/Patient.h"
#include "models/IRepository.h"
#include <optional>
#include <vector>

namespace medical::infrastructure {

class PostgresPatientRepository : public domain::IPatientRepository {
private:
    userver::storages::postgres::ClusterPtr pg_cluster_;

public:
    explicit PostgresPatientRepository(userver::storages::postgres::ClusterPtr cluster);
    
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

}  // namespace medical::infrastructure
