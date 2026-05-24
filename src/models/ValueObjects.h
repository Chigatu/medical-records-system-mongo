#pragma once
#include <string>
#include <regex>
#include <stdexcept>
#include <chrono>
#include <ctime>
#include <cstdio>
#include <algorithm>
#include <iostream>

namespace medical::domain {

// Value Object: Email
class Email {
private:
    std::string value_;
    
    bool isValid(const std::string& email) {
        const std::regex pattern(R"((\w+)(\.\w+)*@(\w+)(\.\w+)+)");
        return std::regex_match(email, pattern);
    }
    
public:
    Email() : value_("default@example.com") {}
    
    explicit Email(const std::string& email) : value_(email) {
        if (!isValid(email)) {
            throw std::invalid_argument("Invalid email format");
        }
    }
    
    std::string value() const { return value_; }
    
    bool operator==(const Email& other) const {
        return value_ == other.value_;
    }
};

// Value Object: FullName
class FullName {
private:
    std::string firstName_;
    std::string lastName_;
    std::string patronymic_;
    
public:
    FullName() : firstName_("Default"), lastName_("User"), patronymic_("") {}
    
    FullName(const std::string& firstName, const std::string& lastName, 
             const std::string& patronymic = "")
        : firstName_(firstName), lastName_(lastName), patronymic_(patronymic) {
        if (firstName.empty() || lastName.empty()) {
            throw std::invalid_argument("First name and last name are required");
        }
    }
    
    std::string firstName() const { return firstName_; }
    std::string lastName() const { return lastName_; }
    std::string patronymic() const { return patronymic_; }
    
    std::string fullName() const {
        if (patronymic_.empty()) {
            return lastName_ + " " + firstName_;
        }
        return lastName_ + " " + firstName_ + " " + patronymic_;
    }
    
    bool matchesMask(const std::string& mask) const {
        std::string full = fullName();
        return full.find(mask) != std::string::npos;
    }
};

// Value Object: MedicalRecordCode
class MedicalRecordCode {
private:
    std::string code_;
    
public:
    MedicalRecordCode() : code_("MED-00000000-00000") {}
    
    explicit MedicalRecordCode(const std::string& code) : code_(code) {
        if (code.empty()) {
            throw std::invalid_argument("Record code cannot be empty");
        }
    }
    
    std::string value() const { return code_; }
    
    static MedicalRecordCode generate() {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::tm* tm = std::localtime(&time);
        
        char date[9];
        std::strftime(date, sizeof(date), "%Y%m%d", tm);
        
        static int counter = 0;
        char code[20];
        std::snprintf(code, sizeof(code), "MED-%s-%05d", date, ++counter);
        
        return MedicalRecordCode(std::string(code));
    }
    
    bool operator==(const MedicalRecordCode& other) const {
        return code_ == other.code_;
    }
};

// Value Object: Diagnosis
class Diagnosis {
private:
    std::string code_;
    std::string description_;
    
    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, last - first + 1);
    }
    
public:
    Diagnosis() : code_(""), description_("") {
        std::cout << "Diagnosis default constructor called" << std::endl;
    }
    
    Diagnosis(const std::string& code, const std::string& description)
        : code_(trim(code)), description_(trim(description)) {
        std::cout << "Diagnosis constructor called with code: '" << code_ << "', desc: '" << description_ << "'" << std::endl;
        if (code_.empty() || description_.empty()) {
            std::cout << "Diagnosis validation FAILED - code empty: " << code_.empty() << ", desc empty: " << description_.empty() << std::endl;
            throw std::invalid_argument("Diagnosis code and description required");
        }
        std::cout << "Diagnosis created successfully" << std::endl;
    }
    
    std::string code() const { return code_; }
    std::string description() const { return description_; }
};

} // namespace medical::domain

// Специализация std::hash для MedicalRecordCode
namespace std {
    template<>
    struct hash<medical::domain::MedicalRecordCode> {
        size_t operator()(const medical::domain::MedicalRecordCode& code) const {
            return hash<string>()(code.value());
        }
    };
}
