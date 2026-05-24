#pragma once
#include <vector>
#include <optional>
#include <memory>

namespace medical::domain {

// Forward declarations
class User;
class Patient;
class MedicalRecord;
class MedicalRecordCode;

template<typename T, typename Id>
class IRepository {
public:
    virtual ~IRepository() = default;
    
    virtual std::optional<T> findById(Id id) = 0;
    virtual std::vector<T> findAll() = 0;
    virtual Id save(const T& entity) = 0;
    virtual void update(const T& entity) = 0;
    virtual void remove(Id id) = 0;
    virtual bool exists(Id id) = 0;
};

class IUserRepository : public IRepository<User, int> {
public:
    virtual std::optional<User> findByLogin(const std::string& login) = 0;
    virtual std::vector<User> findByFullNameMask(const std::string& mask) = 0;
};

class IPatientRepository : public IRepository<Patient, int> {
public:
    virtual std::optional<Patient> findByUserId(int userId) = 0;
    virtual std::vector<Patient> findByFullNameMask(const std::string& mask) = 0;
    virtual std::optional<Patient> findBySnils(const std::string& snils) = 0;
};

class IMedicalRecordRepository : public IRepository<MedicalRecord, int> {
public:
    virtual std::vector<MedicalRecord> findByPatientId(int patientId) = 0;
    virtual std::optional<MedicalRecord> findByCode(const MedicalRecordCode& code) = 0;
    virtual std::vector<MedicalRecord> findByDoctorId(int doctorId) = 0;
};

} // namespace medical::domain
