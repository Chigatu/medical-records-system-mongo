#include "PostgresUserRepository.h"
#include <userver/storages/postgres/io/chrono.hpp>
#include <iostream>

namespace medical::infrastructure {

PostgresUserRepository::PostgresUserRepository(userver::storages::postgres::ClusterPtr cluster)
    : pg_cluster_(std::move(cluster)) {}

std::optional<domain::User> PostgresUserRepository::findById(int id) {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT id, login, email, first_name, last_name, "
        "COALESCE(patronymic, ''), password_hash, is_active, created_at "
        "FROM users WHERE id = $1", id);
    
    if (result.IsEmpty()) return std::nullopt;
    
    auto row = result.Front();
    return domain::User(
        row["id"].As<int>(),
        row["login"].As<std::string>(),
        domain::Email(row["email"].As<std::string>()),
        domain::FullName(
            row["first_name"].As<std::string>(),
            row["last_name"].As<std::string>(),
            row["patronymic"].As<std::string>()),
        row["password_hash"].As<std::string>()
    );
}

std::vector<domain::User> PostgresUserRepository::findAll() {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT id, login, email, first_name, last_name, "
        "COALESCE(patronymic, ''), password_hash, is_active, created_at "
        "FROM users ORDER BY id");
    
    std::vector<domain::User> users;
    for (const auto& row : result) {
        users.emplace_back(
            row["id"].As<int>(),
            row["login"].As<std::string>(),
            domain::Email(row["email"].As<std::string>()),
            domain::FullName(
                row["first_name"].As<std::string>(),
                row["last_name"].As<std::string>(),
                row["patronymic"].As<std::string>()),
            row["password_hash"].As<std::string>()
        );
    }
    return users;
}

int PostgresUserRepository::save(const domain::User& entity) {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        "INSERT INTO users (login, email, first_name, last_name, patronymic, password_hash) "
        "VALUES ($1, $2, $3, $4, $5, $6) RETURNING id",
        entity.login(),
        entity.email().value(),
        entity.fullName().firstName(),
        entity.fullName().lastName(),
        entity.fullName().patronymic(),
        entity.passwordHash());
    
    return result.Front()["id"].As<int>();
}

void PostgresUserRepository::update(const domain::User& entity) {
    pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        "UPDATE users SET login=$1, email=$2, first_name=$3, last_name=$4, "
        "patronymic=$5, is_active=$6 WHERE id=$7",
        entity.login(), entity.email().value(),
        entity.fullName().firstName(), entity.fullName().lastName(),
        entity.fullName().patronymic(), entity.isActive(), entity.id());
}

void PostgresUserRepository::remove(int id) {
    pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        "DELETE FROM users WHERE id = $1", id);
}

bool PostgresUserRepository::exists(int id) {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT EXISTS(SELECT 1 FROM users WHERE id = $1) AS exists", id);
    return result.Front()["exists"].As<bool>();
}

std::optional<domain::User> PostgresUserRepository::findByLogin(const std::string& login) {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT id, login, email, first_name, last_name, "
        "COALESCE(patronymic, ''), password_hash, is_active, created_at "
        "FROM users WHERE login = $1", login);
    
    if (result.IsEmpty()) return std::nullopt;
    
    auto row = result.Front();
    return domain::User(
        row["id"].As<int>(),
        row["login"].As<std::string>(),
        domain::Email(row["email"].As<std::string>()),
        domain::FullName(
            row["first_name"].As<std::string>(),
            row["last_name"].As<std::string>(),
            row["patronymic"].As<std::string>()),
        row["password_hash"].As<std::string>()
    );
}

std::vector<domain::User> PostgresUserRepository::findByFullNameMask(const std::string& mask) {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT id, login, email, first_name, last_name, "
        "COALESCE(patronymic, ''), password_hash, is_active, created_at "
        "FROM users "
        "WHERE last_name || ' ' || first_name || ' ' || COALESCE(patronymic, '') LIKE $1 "
        "ORDER BY id",
        "%" + mask + "%");
    
    std::vector<domain::User> users;
    for (const auto& row : result) {
        users.emplace_back(
            row["id"].As<int>(),
            row["login"].As<std::string>(),
            domain::Email(row["email"].As<std::string>()),
            domain::FullName(
                row["first_name"].As<std::string>(),
                row["last_name"].As<std::string>(),
                row["patronymic"].As<std::string>()),
            row["password_hash"].As<std::string>()
        );
    }
    return users;
}

}  // namespace medical::infrastructure
