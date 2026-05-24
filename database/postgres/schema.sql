-- ============================================
-- Medical Records System - Database Schema
-- Variant 20
-- ============================================

-- Таблица пользователей (врачей)
CREATE TABLE IF NOT EXISTS users (
    id              SERIAL PRIMARY KEY,
    login           VARCHAR(50) NOT NULL UNIQUE,
    email           VARCHAR(100) NOT NULL,
    first_name      VARCHAR(100) NOT NULL,
    last_name       VARCHAR(100) NOT NULL,
    patronymic      VARCHAR(100) DEFAULT '',
    password_hash   VARCHAR(255) NOT NULL,
    is_active       BOOLEAN DEFAULT TRUE,
    created_at      TIMESTAMP WITH TIME ZONE DEFAULT NOW(),
    
    CONSTRAINT chk_email CHECK (email LIKE '%@%.%')
);

-- Таблица пациентов
CREATE TABLE IF NOT EXISTS patients (
    id              SERIAL PRIMARY KEY,
    user_id         INTEGER REFERENCES users(id) ON DELETE SET NULL,
    first_name      VARCHAR(100) NOT NULL,
    last_name       VARCHAR(100) NOT NULL,
    patronymic      VARCHAR(100) DEFAULT '',
    phone           VARCHAR(20),
    address         TEXT,
    birth_date      DATE,
    snils           VARCHAR(20) UNIQUE,
    policy_number   VARCHAR(20),
    created_at      TIMESTAMP WITH TIME ZONE DEFAULT NOW()
);

-- Таблица медицинских записей
CREATE TABLE IF NOT EXISTS medical_records (
    id                  SERIAL PRIMARY KEY,
    code                VARCHAR(30) NOT NULL UNIQUE,
    patient_id          INTEGER NOT NULL REFERENCES patients(id) ON DELETE CASCADE,
    doctor_id           INTEGER REFERENCES users(id) ON DELETE SET NULL,
    diagnosis_code      VARCHAR(10),
    diagnosis_description TEXT,
    complaints          TEXT,
    recommendations     TEXT,
    created_at          TIMESTAMP WITH TIME ZONE DEFAULT NOW()
);

-- Индексы для внешних ключей
CREATE INDEX IF NOT EXISTS idx_patients_user_id ON patients(user_id);
CREATE INDEX IF NOT EXISTS idx_records_patient_id ON medical_records(patient_id);
CREATE INDEX IF NOT EXISTS idx_records_doctor_id ON medical_records(doctor_id);

-- Индексы для поиска
CREATE INDEX IF NOT EXISTS idx_patients_full_name ON patients(last_name, first_name, patronymic);
CREATE INDEX IF NOT EXISTS idx_patients_snils ON patients(snils);
CREATE INDEX IF NOT EXISTS idx_users_login ON users(login);
CREATE INDEX IF NOT EXISTS idx_records_code ON medical_records(code);
CREATE INDEX IF NOT EXISTS idx_records_created_at ON medical_records(created_at);

-- Полнотекстовый поиск по имени пациента
CREATE INDEX IF NOT EXISTS idx_patients_name_search ON patients 
    USING gin(to_tsvector('russian', last_name || ' ' || first_name || ' ' || COALESCE(patronymic, '')));
