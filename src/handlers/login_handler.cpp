#include "login_handler.hpp"
#include <userver/formats/json.hpp>
#include <userver/components/component_context.hpp>
#include "repositories/RepositoryFactory.h"

namespace medical_api {

LoginHandler::LoginHandler(const userver::components::ComponentConfig& config,
                            const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context) {
    auto user_service = std::make_shared<medical::application::UserService>(
        RepositoryFactory::GetUserRepo());
    auth_service_ = std::make_shared<medical::application::AuthService>(
        user_service, RepositoryFactory::GetJwtService());
}

std::string LoginHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& /*context*/
) const {
    auto json = userver::formats::json::FromString(request.RequestBody());
    
    medical::application::dto::LoginRequestDTO dto;
    dto.login = json["login"].As<std::string>();
    dto.password = json["password"].As<std::string>();
    
    auto login_response = auth_service_->login(dto);
    if (!login_response.has_value()) {
        request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kUnauthorized);
        return R"({"error":"Invalid credentials"})";
    }
    
    userver::formats::json::ValueBuilder response;
    response["token"] = login_response->token;
    
    userver::formats::json::ValueBuilder user;
    user["id"] = login_response->user.id;
    user["login"] = login_response->user.login;
    user["email"] = login_response->user.email;
    user["fullName"] = login_response->user.fullName;
    response["user"] = user;
    
    return userver::formats::json::ToString(response.ExtractValue());
}

}  // namespace medical_api
