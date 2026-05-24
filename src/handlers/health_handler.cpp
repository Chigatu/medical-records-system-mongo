#include "health_handler.hpp"

namespace medical_api {

std::string HealthHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& /*request*/,
    userver::server::request::RequestContext& /*context*/
) const {
    return R"({"status":"ok","service":"Medical Records API","version":"2.0.0"})";
}

}  // namespace medical_api
