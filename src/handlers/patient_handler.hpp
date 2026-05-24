#pragma once

#include <userver/server/handlers/http_handler_base.hpp>
#include <memory>
#include "services/PatientService.h"
#include "services/JWTService.h"
#include "repositories/RepositoryFactory.h"

namespace medical_api {

// Создание пациента
class CreatePatientHandler final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-create-patient";

    CreatePatientHandler(const userver::components::ComponentConfig& config,
                          const userver::components::ComponentContext& context);

    std::string HandleRequestThrow(
        const userver::server::http::HttpRequest& request,
        userver::server::request::RequestContext& context
    ) const override;

private:
    std::shared_ptr<medical::application::PatientService> patient_service_;
    std::shared_ptr<medical::infrastructure::JWTService> jwt_service_;
};

// Поиск пациентов
class SearchPatientHandler final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-search-patient";

    SearchPatientHandler(const userver::components::ComponentConfig& config,
                          const userver::components::ComponentContext& context);

    std::string HandleRequestThrow(
        const userver::server::http::HttpRequest& request,
        userver::server::request::RequestContext& context
    ) const override;

private:
    std::shared_ptr<medical::application::PatientService> patient_service_;
};

}  // namespace medical_api
