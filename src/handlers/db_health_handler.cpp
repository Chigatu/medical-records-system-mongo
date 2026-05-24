#include "db_health_handler.hpp"
#include <userver/components/component_context.hpp>
#include <userver/storages/postgres/component.hpp>

namespace medical_api {

DbHealthHandler::DbHealthHandler(const userver::components::ComponentConfig& config,
                                   const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      pg_cluster_(context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster()) {}

std::string DbHealthHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& /*request*/,
    userver::server::request::RequestContext& /*context*/
) const {
    try {
        auto result = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kSlave,
            "SELECT 1 AS ok");
        
        return R"({"status":"ok","database":"connected"})";
    } catch (const std::exception& e) {
        return R"({"status":"error","database":"disconnected","message":")" + std::string(e.what()) + R"("})";
    }
}

}  // namespace medical_api
