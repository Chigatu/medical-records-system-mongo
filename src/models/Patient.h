#pragma once
#include "models/ValueObjects.h"
#include <string>
#include <chrono>
#include <optional>

namespace medical::domain {

class Patient {
private:
    int id_;
    std::optional<int> userId_;
    FullName fullName_;
    std::string phone_;
    std::string address_;
    std::chrono::system_clock::time_point birthDate_;
    std::string snils_;
    std::string policyNumber_;
    std::chrono::system_clock::time_point createdAt_;
    
public:
    // Конструктор по умолчанию для контейнеров
    Patient() 
        : id_(0)
        , fullName_("Default", "Patient")
        , phone_("")
        , address_("")
        , birthDate_(std::chrono::system_clock::now())
        , snils_("")
        , policyNumber_("") {
        createdAt_ = std::chrono::system_clock::now();
    }
    
    Patient(int id, const FullName& fullName, const std::string& phone,
            const std::string& address, const std::chrono::system_clock::time_point& birthDate,
            const std::string& snils, const std::string& policyNumber,
            std::optional<int> userId = std::nullopt)
        : id_(id), userId_(userId), fullName_(fullName), phone_(phone),
          address_(address), birthDate_(birthDate), snils_(snils), 
          policyNumber_(policyNumber) {
        createdAt_ = std::chrono::system_clock::now();
    }
    
    int id() const { return id_; }
    std::optional<int> userId() const { return userId_; }
    const FullName& fullName() const { return fullName_; }
    const std::string& phone() const { return phone_; }
    const std::string& address() const { return address_; }
    auto birthDate() const { return birthDate_; }
    const std::string& snils() const { return snils_; }
    const std::string& policyNumber() const { return policyNumber_; }
    
    void updateContactInfo(const std::string& phone, const std::string& address) {
        phone_ = phone;
        address_ = address;
    }
    
    bool matchesNameMask(const std::string& mask) const {
        return fullName_.matchesMask(mask);
    }
    
    int age() const {
        auto now = std::chrono::system_clock::now();
        auto age = std::chrono::duration_cast<std::chrono::seconds>(
            now - birthDate_);
        return static_cast<int>(age.count() / 31556952.0);
    }
};

} // namespace medical::domain
