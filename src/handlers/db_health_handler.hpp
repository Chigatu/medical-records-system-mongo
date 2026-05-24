#pragma once

#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace medical_api {

class DbHealthHandler final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-db-health";

    DbHealthHandler(const userver::components::ComponentConfig& config,
                    const userver::components::ComponentContext& context);

    std::string HandleRequestThrow(
        const userver::server::http::HttpRequest& request,
        userver::server::request::RequestContext& context
    ) const override;

private:
    userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace medical_api
