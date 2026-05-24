#pragma once
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include "models/User.h"
#include "models/ValueObjects.h"
#include "models/IRepository.h"
#include <optional>
#include <vector>
#include <string>

namespace medical::infrastructure {

class PostgresUserRepository : public domain::IUserRepository {
private:
    userver::storages::postgres::ClusterPtr pg_cluster_;

public:
    explicit PostgresUserRepository(userver::storages::postgres::ClusterPtr cluster);
    
    std::optional<domain::User> findById(int id) override;
    std::vector<domain::User> findAll() override;
    int save(const domain::User& entity) override;
    void update(const domain::User& entity) override;
    void remove(int id) override;
    bool exists(int id) override;
    
    std::optional<domain::User> findByLogin(const std::string& login) override;
    std::vector<domain::User> findByFullNameMask(const std::string& mask) override;
};

}  // namespace medical::infrastructure
