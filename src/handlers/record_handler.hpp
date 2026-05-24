#pragma once

#include <userver/server/handlers/http_handler_base.hpp>
#include <memory>
#include "services/MedicalRecordService.h"
#include "services/JWTService.h"
#include "repositories/RepositoryFactory.h"

namespace medical_api {

class CreateRecordHandler final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-create-record";

    CreateRecordHandler(const userver::components::ComponentConfig& config,
                         const userver::components::ComponentContext& context);

    std::string HandleRequestThrow(
        const userver::server::http::HttpRequest& request,
        userver::server::request::RequestContext& context
    ) const override;

private:
    std::shared_ptr<medical::application::MedicalRecordService> record_service_;
    std::shared_ptr<medical::infrastructure::JWTService> jwt_service_;
};

class GetPatientRecordsHandler final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-get-patient-records";

    GetPatientRecordsHandler(const userver::components::ComponentConfig& config,
                               const userver::components::ComponentContext& context);

    std::string HandleRequestThrow(
        const userver::server::http::HttpRequest& request,
        userver::server::request::RequestContext& context
    ) const override;

private:
    std::shared_ptr<medical::application::MedicalRecordService> record_service_;
    std::shared_ptr<medical::infrastructure::JWTService> jwt_service_;
};

}  // namespace medical_api
