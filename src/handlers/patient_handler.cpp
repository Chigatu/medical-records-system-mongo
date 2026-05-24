#include "patient_handler.hpp"
#include <userver/formats/json.hpp>
#include <userver/components/component_context.hpp>

namespace medical_api {

// ========== CreatePatientHandler ==========
CreatePatientHandler::CreatePatientHandler(const userver::components::ComponentConfig& config,
                                            const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context) {
    patient_service_ = std::make_shared<medical::application::PatientService>(
        RepositoryFactory::GetPatientRepo());
    jwt_service_ = RepositoryFactory::GetJwtService();
}

std::string CreatePatientHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& /*context*/
) const {
    // Проверяем JWT токен
    const auto& auth_header = request.GetHeader("Authorization");
    if (auth_header.empty() || auth_header.find("Bearer ") != 0) {
        request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kUnauthorized);
        return R"({"error":"Missing or invalid token"})";
    }
    
    std::string token = auth_header.substr(7);
    auto user_id = jwt_service_->validateToken(token);
    if (!user_id.has_value()) {
        request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kUnauthorized);
        return R"({"error":"Invalid token"})";
    }
    
    auto json = userver::formats::json::FromString(request.RequestBody());
    
    medical::application::dto::CreatePatientRequestDTO dto;
    dto.firstName = json["firstName"].As<std::string>();
    dto.lastName = json["lastName"].As<std::string>();
    dto.patronymic = json["patronymic"].As<std::string>("");
    dto.phone = json["phone"].As<std::string>();
    dto.address = json["address"].As<std::string>();
    dto.birthDate = json["birthDate"].As<std::string>();
    dto.snils = json["snils"].As<std::string>();
    dto.policyNumber = json["policyNumber"].As<std::string>();
    dto.userId = user_id.value();
    
    auto patient = patient_service_->createPatient(dto);
    if (!patient.has_value()) {
        request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kBadRequest);
        return R"({"error":"Patient with this SNILS already exists"})";
    }
    
    userver::formats::json::ValueBuilder response;
    response["id"] = patient->id;
    response["fullName"] = patient->fullName;
    response["phone"] = patient->phone;
    response["address"] = patient->address;
    response["age"] = patient->age;
    response["snils"] = patient->snils;
    response["policyNumber"] = patient->policyNumber;
    
    request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kCreated);
    return userver::formats::json::ToString(response.ExtractValue());
}

// ========== SearchPatientHandler ==========
SearchPatientHandler::SearchPatientHandler(const userver::components::ComponentConfig& config,
                                            const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context) {
    patient_service_ = std::make_shared<medical::application::PatientService>(
        RepositoryFactory::GetPatientRepo());
}

std::string SearchPatientHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& /*context*/
) const {
    const auto& fullName = request.GetArg("fullName");
    
    if (fullName.empty()) {
        request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kBadRequest);
        return R"({"error":"Missing fullName parameter"})";
    }
    
    auto patients = patient_service_->searchPatientsByName(fullName);
    
    userver::formats::json::ValueBuilder response;
    userver::formats::json::ValueBuilder patients_array(userver::formats::common::Type::kArray);
    
    for (const auto& p : patients) {
        userver::formats::json::ValueBuilder patient_obj;
        patient_obj["id"] = p.id;
        patient_obj["fullName"] = p.fullName;
        patient_obj["phone"] = p.phone;
        patient_obj["address"] = p.address;
        patient_obj["age"] = p.age;
        patient_obj["snils"] = p.snils;
        patients_array.PushBack(std::move(patient_obj));
    }
    
    response["patients"] = patients_array;
    return userver::formats::json::ToString(response.ExtractValue());
}

}  // namespace medical_api
