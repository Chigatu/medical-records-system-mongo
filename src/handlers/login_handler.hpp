#pragma once

#include <userver/server/handlers/http_handler_base.hpp>
#include <memory>
#include "services/UserService.h"
#include "services/AuthService.h"
#include "repositories/InMemoryUserRepository.h"
#include "services/JWTService.h"

namespace medical_api {

class LoginHandler final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-login";

    LoginHandler(const userver::components::ComponentConfig& config,
                 const userver::components::ComponentContext& context);

    std::string HandleRequestThrow(
        const userver::server::http::HttpRequest& request,
        userver::server::request::RequestContext& context
    ) const override;

private:
    std::shared_ptr<medical::application::AuthService> auth_service_;
};

}  // namespace medical_api
