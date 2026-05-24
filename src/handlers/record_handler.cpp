#include "record_handler.hpp"
#include <userver/formats/json.hpp>
#include <userver/components/component_context.hpp>

namespace medical_api {

// ========== CreateRecordHandler ==========
CreateRecordHandler::CreateRecordHandler(const userver::components::ComponentConfig& config,
                                           const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context) {
    record_service_ = std::make_shared<medical::application::MedicalRecordService>(
        RepositoryFactory::GetMedicalRecordRepo(),
        RepositoryFactory::GetPatientRepo(),
        RepositoryFactory::GetUserRepo());
    jwt_service_ = RepositoryFactory::GetJwtService();
}

std::string CreateRecordHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& /*context*/
) const {
    // JWT
    const auto& auth_header = request.GetHeader("Authorization");
    if (auth_header.empty() || auth_header.find("Bearer ") != 0) {
        request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kUnauthorized);
        return R"({"error":"Missing or invalid token"})";
    }
    std::string token = auth_header.substr(7);
    if (!jwt_service_->validateToken(token).has_value()) {
        request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kUnauthorized);
        return R"({"error":"Invalid token"})";
    }
    
    auto json = userver::formats::json::FromString(request.RequestBody());
    
    medical::application::dto::CreateMedicalRecordRequestDTO dto;
    dto.patientId = json["patientId"].As<int>();
    dto.doctorId = json["doctorId"].As<int>();
    dto.diagnosisCode = json["diagnosisCode"].As<std::string>();
    dto.diagnosisDescription = json["diagnosisDescription"].As<std::string>();
    dto.complaints = json["complaints"].As<std::string>();
    
    auto record = record_service_->createRecord(dto);
    if (!record.has_value()) {
        request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kBadRequest);
        return R"({"error":"Invalid patient or doctor ID"})";
    }
    
    userver::formats::json::ValueBuilder response;
    response["id"] = record->id;
    response["code"] = record->code;
    response["patientId"] = record->patientId;
    response["doctorId"] = record->doctorId;
    response["diagnosisCode"] = record->diagnosisCode;
    response["diagnosisDescription"] = record->diagnosisDescription;
    response["complaints"] = record->complaints;
    response["createdAt"] = record->createdAt;
    
    request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kCreated);
    return userver::formats::json::ToString(response.ExtractValue());
}

// ========== GetPatientRecordsHandler ==========
GetPatientRecordsHandler::GetPatientRecordsHandler(const userver::components::ComponentConfig& config,
                                                     const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context) {
    record_service_ = std::make_shared<medical::application::MedicalRecordService>(
        RepositoryFactory::GetMedicalRecordRepo(),
        RepositoryFactory::GetPatientRepo(),
        RepositoryFactory::GetUserRepo());
    jwt_service_ = RepositoryFactory::GetJwtService();
}

std::string GetPatientRecordsHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& /*context*/
) const {
    // JWT
    const auto& auth_header = request.GetHeader("Authorization");
    if (auth_header.empty() || auth_header.find("Bearer ") != 0) {
        request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kUnauthorized);
        return R"({"error":"Missing or invalid token"})";
    }
    std::string token = auth_header.substr(7);
    if (!jwt_service_->validateToken(token).has_value()) {
        request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kUnauthorized);
        return R"({"error":"Invalid token"})";
    }
    
    int patient_id = std::stoi(request.GetPathArg("patientId"));
    auto records = record_service_->getPatientRecords(patient_id);
    
    userver::formats::json::ValueBuilder response;
    userver::formats::json::ValueBuilder records_array(userver::formats::common::Type::kArray);
    
    for (const auto& r : records) {
        userver::formats::json::ValueBuilder record_obj;
        record_obj["id"] = r.id;
        record_obj["code"] = r.code;
        record_obj["diagnosisCode"] = r.diagnosisCode;
        record_obj["diagnosisDescription"] = r.diagnosisDescription;
        record_obj["complaints"] = r.complaints;
        record_obj["createdAt"] = r.createdAt;
        records_array.PushBack(std::move(record_obj));
    }
    
    response["records"] = records_array;
    return userver::formats::json::ToString(response.ExtractValue());
}

}  // namespace medical_api
