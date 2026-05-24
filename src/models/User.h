#pragma once
#include "models/ValueObjects.h"
#include <string>
#include <chrono>

namespace medical::domain {

class User {
private:
    int id_;
    std::string login_;
    Email email_;
    FullName fullName_;
    std::string passwordHash_;
    std::chrono::system_clock::time_point createdAt_;
    bool isActive_;
    
public:
    // Конструктор по умолчанию для контейнеров
    User() 
        : id_(0)
        , login_("")
        , email_(Email("default@example.com"))
        , fullName_("Default", "User")
        , passwordHash_("")
        , isActive_(false) {
        createdAt_ = std::chrono::system_clock::now();
    }
    
    User(int id, const std::string& login, const Email& email, 
         const FullName& fullName, const std::string& passwordHash)
        : id_(id), login_(login), email_(email), fullName_(fullName),
          passwordHash_(passwordHash), isActive_(true) {
        createdAt_ = std::chrono::system_clock::now();
    }
    
    int id() const { return id_; }
    const std::string& login() const { return login_; }
    const Email& email() const { return email_; }
    const FullName& fullName() const { return fullName_; }
    const std::string& passwordHash() const { return passwordHash_; }
    bool isActive() const { return isActive_; }
    
    void deactivate() {
        isActive_ = false;
    }
    
    bool matchesNameMask(const std::string& mask) const {
        return fullName_.matchesMask(mask);
    }
    
    bool matchesLogin(const std::string& login) const {
        return login_ == login;
    }
};

} // namespace medical::domain
