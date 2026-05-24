#pragma once
#include <string>
#include <vector>
#include <optional>

namespace medical::application::dto {

struct UserDTO {
    int id;
    std::string login;
    std::string email;
    std::string fullName;
    bool isActive;
};

struct LoginRequestDTO {
    std::string login;
    std::string password;
};

struct LoginResponseDTO {
    std::string token;
    UserDTO user;
};

struct RegisterRequestDTO {
    std::string login;
    std::string email;
    std::string firstName;
    std::string lastName;
    std::string patronymic;
    std::string password;
};

struct PatientDTO {
    int id;
    std::string fullName;
    std::string phone;
    std::string address;
    int age;
    std::string snils;
    std::string policyNumber;
};

struct CreatePatientRequestDTO {
    std::string firstName;
    std::string lastName;
    std::string patronymic;
    std::string phone;
    std::string address;
    std::string birthDate;
    std::string snils;
    std::string policyNumber;
    std::optional<int> userId;
};

struct MedicalRecordDTO {
    int id;
    std::string code;
    int patientId;
    int doctorId;
    std::string diagnosisCode;
    std::string diagnosisDescription;
    std::string complaints;
    std::string createdAt;
};

struct CreateMedicalRecordRequestDTO {
    int patientId;
    int doctorId;
    std::string diagnosisCode;
    std::string diagnosisDescription;
    std::string complaints;
};

} // namespace medical::application::dto
