-- ============================================
-- Medical Records System - Queries
-- Variant 20
-- ============================================

-- 1. Регистрация нового пользователя (врача)
-- POST /api/auth/register
INSERT INTO users (login, email, first_name, last_name, patronymic, password_hash)
VALUES ('dr.new_doctor', 'new_doctor@hospital.ru', 'Новый', 'Доктор', 'Тестович', 'hashed_password')
RETURNING id, login, email, last_name || ' ' || first_name || ' ' || COALESCE(patronymic, '') AS full_name;

-- 2. Поиск пользователя по логину (для аутентификации)
-- POST /api/auth/login
SELECT id, login, email, password_hash, is_active,
       last_name || ' ' || first_name || ' ' || COALESCE(patronymic, '') AS full_name
FROM users
WHERE login = 'dr.ivanov';

-- 3. Поиск пользователей по маске имени
-- GET /api/users/search?mask=Иван
SELECT id, login, email,
       last_name || ' ' || first_name || ' ' || COALESCE(patronymic, '') AS full_name
FROM users
WHERE last_name || ' ' || first_name || ' ' || COALESCE(patronymic, '') LIKE '%Иван%';

-- 4. Регистрация нового пациента
-- POST /api/patients
INSERT INTO patients (first_name, last_name, patronymic, phone, address, birth_date, snils, policy_number)
VALUES ('Новый', 'Пациент', 'Тестович', '+79000000000', 'г. Москва, ул. Тестовая, д. 1', '2000-01-01', '999-888-777 01', '9998887776665554')
RETURNING id, last_name || ' ' || first_name || ' ' || COALESCE(patronymic, '') AS full_name;

-- 5. Поиск пациента по СНИЛС (проверка уникальности)
SELECT id FROM patients WHERE snils = '123-456-789 01';

-- 6. Поиск пациентов по ФИО (полнотекстовый или LIKE)
-- GET /api/patients/search?fullName=Смирнова
SELECT id, 
       last_name || ' ' || first_name || ' ' || COALESCE(patronymic, '') AS full_name,
       phone, address, 
       EXTRACT(YEAR FROM AGE(birth_date)) AS age,
       snils, policy_number
FROM patients
WHERE last_name || ' ' || first_name || ' ' || COALESCE(patronymic, '') LIKE '%Сидоров%';

-- 7. Создание медицинской записи
-- POST /api/medical-records
INSERT INTO medical_records (code, patient_id, doctor_id, diagnosis_code, diagnosis_description, complaints)
VALUES ('MED-20260327-00013', 1, 1, 'J11.1', 'Грипп', 'Температура 39, ломота в теле')
RETURNING id, code, patient_id, doctor_id, diagnosis_code, diagnosis_description, complaints, created_at;

-- 8. Получение истории записей пациента
-- GET /api/medical-records/patient/{id}
SELECT mr.id, mr.code, mr.diagnosis_code, mr.diagnosis_description, 
       mr.complaints, mr.created_at,
       u.last_name || ' ' || u.first_name AS doctor_name
FROM medical_records mr
LEFT JOIN users u ON mr.doctor_id = u.id
WHERE mr.patient_id = 1
ORDER BY mr.created_at DESC;

-- 9. Получение записи по коду
-- GET /api/medical-records/{code}
SELECT mr.id, mr.code, mr.patient_id, mr.doctor_id,
       mr.diagnosis_code, mr.diagnosis_description, 
       mr.complaints, mr.recommendations, mr.created_at,
       p.last_name || ' ' || p.first_name AS patient_name,
       u.last_name || ' ' || u.first_name AS doctor_name
FROM medical_records mr
JOIN patients p ON mr.patient_id = p.id
LEFT JOIN users u ON mr.doctor_id = u.id
WHERE mr.code = 'MED-20260320-00001';

-- 10. Проверка существования пациента по ID
SELECT EXISTS(SELECT 1 FROM patients WHERE id = 1) AS patient_exists;

-- 11. Проверка существования врача по ID
SELECT EXISTS(SELECT 1 FROM users WHERE id = 1 AND is_active = true) AS doctor_exists;

-- 12. Статистика: количество записей по врачам
SELECT u.last_name || ' ' || u.first_name AS doctor_name,
       COUNT(mr.id) AS total_records
FROM users u
LEFT JOIN medical_records mr ON u.id = mr.doctor_id
GROUP BY u.id, u.last_name, u.first_name
ORDER BY total_records DESC;

-- 13. Статистика: количество записей по диагнозам
SELECT diagnosis_code, diagnosis_description, COUNT(*) AS count
FROM medical_records
GROUP BY diagnosis_code, diagnosis_description
ORDER BY count DESC;
