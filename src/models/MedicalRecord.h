#pragma once
#include "models/ValueObjects.h"
#include <string>
#include <chrono>
#include <vector>
#include <optional>

namespace medical::domain {

struct Symptom {
    std::string name;
    std::string severity;
    std::chrono::system_clock::time_point startedAt;
    
    Symptom() = default;
    Symptom(const std::string& n, const std::string& s) 
        : name(n), severity(s) {
        startedAt = std::chrono::system_clock::now();
    }
};

struct Treatment {
    std::string name;
    std::string dosage;
    std::string frequency;
    std::chrono::system_clock::time_point startDate;
    std::optional<std::chrono::system_clock::time_point> endDate;
    
    Treatment() = default;
    Treatment(const std::string& n, const std::string& d, const std::string& f)
        : name(n), dosage(d), frequency(f) {
        startDate = std::chrono::system_clock::now();
    }
};

class MedicalRecord {
private:
    int id_;
    MedicalRecordCode code_;
    int patientId_;
    int doctorId_;
    Diagnosis diagnosis_;
    std::vector<Symptom> symptoms_;
    std::vector<Treatment> treatments_;
    std::string complaints_;
    std::string recommendations_;
    std::chrono::system_clock::time_point createdAt_;
    
public:
    // Конструктор по умолчанию - используем безопасные значения
    MedicalRecord() 
        : id_(0)
        , code_(MedicalRecordCode::generate())
        , patientId_(0)
        , doctorId_(0)
        , diagnosis_("A00", "Cholera")  // Безопасное значение по умолчанию
        , complaints_("") {
        createdAt_ = std::chrono::system_clock::now();
    }
    
    MedicalRecord(int id, const MedicalRecordCode& code, int patientId, int doctorId,
                  const Diagnosis& diagnosis, const std::string& complaints)
        : id_(id), code_(code), patientId_(patientId), doctorId_(doctorId),
          diagnosis_(diagnosis), complaints_(complaints) {
        createdAt_ = std::chrono::system_clock::now();
    }
    
    int id() const { return id_; }
    const MedicalRecordCode& code() const { return code_; }
    int patientId() const { return patientId_; }
    int doctorId() const { return doctorId_; }
    const Diagnosis& diagnosis() const { return diagnosis_; }
    const std::vector<Symptom>& symptoms() const { return symptoms_; }
    const std::vector<Treatment>& treatments() const { return treatments_; }
    const std::string& complaints() const { return complaints_; }
    const std::string& recommendations() const { return recommendations_; }
    const std::chrono::system_clock::time_point& createdAt() const { return createdAt_; }
    
    void addSymptom(const Symptom& symptom) {
        symptoms_.push_back(symptom);
    }
    
    void addTreatment(const Treatment& treatment) {
        treatments_.push_back(treatment);
    }
    
    void updateRecommendations(const std::string& recommendations) {
        recommendations_ = recommendations;
    }
};

} // namespace medical::domain
