#include "register_handler.hpp"
#include <userver/formats/json.hpp>
#include <userver/components/component_context.hpp>
#include "repositories/RepositoryFactory.h"

namespace medical_api {

RegisterHandler::RegisterHandler(const userver::components::ComponentConfig& config,
                                  const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context) {
    user_service_ = std::make_shared<medical::application::UserService>(
        RepositoryFactory::GetUserRepo());
}

std::string RegisterHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& /*context*/
) const {
    auto json = userver::formats::json::FromString(request.RequestBody());
    
    medical::application::dto::RegisterRequestDTO dto;
    dto.login = json["login"].As<std::string>();
    dto.email = json["email"].As<std::string>();
    dto.firstName = json["firstName"].As<std::string>();
    dto.lastName = json["lastName"].As<std::string>();
    dto.patronymic = json["patronymic"].As<std::string>("");
    dto.password = json["password"].As<std::string>();
    
    auto user = user_service_->registerUser(dto);
    if (!user.has_value()) {
        request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kConflict);
        return R"({"error":"User already exists"})";
    }
    
    userver::formats::json::ValueBuilder response;
    response["id"] = user->id;
    response["login"] = user->login;
    response["email"] = user->email;
    response["fullName"] = user->fullName;
    
    request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kCreated);
    return userver::formats::json::ToString(response.ExtractValue());
}

}  // namespace medical_api
