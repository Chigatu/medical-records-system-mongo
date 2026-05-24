// ============================================
// Medical Records System - MongoDB Init
// Variant 20
// ============================================

// Переключаемся на базу данных
db = db.getSiblingDB('medical_records');

// ============================================
// 1. Создание коллекции users (врачи)
// ============================================
db.createCollection('users', {
    validator: {
        $jsonSchema: {
            bsonType: 'object',
            required: ['login', 'email', 'firstName', 'lastName', 'passwordHash'],
            properties: {
                login: {
                    bsonType: 'string',
                    description: 'Уникальный логин',
                    minLength: 3,
                    maxLength: 50
                },
                email: {
                    bsonType: 'string',
                    pattern: '^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$',
                    description: 'Email пользователя'
                },
                firstName: {
                    bsonType: 'string',
                    description: 'Имя'
                },
                lastName: {
                    bsonType: 'string',
                    description: 'Фамилия'
                },
                patronymic: {
                    bsonType: 'string',
                    description: 'Отчество'
                },
                passwordHash: {
                    bsonType: 'string',
                    description: 'Хеш пароля'
                },
                isActive: {
                    bsonType: 'bool',
                    description: 'Активен ли пользователь'
                },
                createdAt: {
                    bsonType: 'date',
                    description: 'Дата создания'
                }
            }
        }
    }
});

// Индексы для users
db.users.createIndex({ login: 1 }, { unique: true });
db.users.createIndex({ email: 1 });
db.users.createIndex({ lastName: 1, firstName: 1 });

// ============================================
// 2. Создание коллекции patients
// ============================================
db.createCollection('patients', {
    validator: {
        $jsonSchema: {
            bsonType: 'object',
            required: ['firstName', 'lastName'],
            properties: {
                firstName: { bsonType: 'string' },
                lastName: { bsonType: 'string' },
                patronymic: { bsonType: 'string' },
                phone: { bsonType: 'string' },
                address: { bsonType: 'string' },
                birthDate: { bsonType: 'date' },
                snils: { bsonType: 'string' },
                policyNumber: { bsonType: 'string' },
                userId: { bsonType: 'objectId' },
                createdAt: { bsonType: 'date' }
            }
        }
    }
});

// Индексы для patients
db.patients.createIndex({ snils: 1 }, { unique: true, sparse: true });
db.patients.createIndex({ lastName: 1, firstName: 1 });
db.patients.createIndex({ userId: 1 });

// ============================================
// 3. Создание коллекции medical_records
// ============================================
db.createCollection('medical_records', {
    validator: {
        $jsonSchema: {
            bsonType: 'object',
            required: ['code', 'patientId', 'doctorId'],
            properties: {
                code: {
                    bsonType: 'string',
                    pattern: '^MED-\\d{8}-\\d{5}$',
                    description: 'Код записи в формате MED-YYYYMMDD-XXXXX'
                },
                patientId: { bsonType: 'objectId' },
                doctorId: { bsonType: 'objectId' },
                diagnosisCode: { bsonType: 'string' },
                diagnosisDescription: { bsonType: 'string' },
                complaints: { bsonType: 'string' },
                recommendations: { bsonType: 'string' },
                symptoms: {
                    bsonType: 'array',
                    items: {
                        bsonType: 'object',
                        required: ['name', 'severity'],
                        properties: {
                            name: { bsonType: 'string' },
                            severity: { bsonType: 'string' },
                            startedAt: { bsonType: 'date' }
                        }
                    }
                },
                treatments: {
                    bsonType: 'array',
                    items: {
                        bsonType: 'object',
                        required: ['name', 'dosage'],
                        properties: {
                            name: { bsonType: 'string' },
                            dosage: { bsonType: 'string' },
                            frequency: { bsonType: 'string' },
                            startDate: { bsonType: 'date' },
                            endDate: { bsonType: 'date' }
                        }
                    }
                },
                createdAt: { bsonType: 'date' }
            }
        }
    }
});

// Индексы для medical_records
db.medical_records.createIndex({ code: 1 }, { unique: true });
db.medical_records.createIndex({ patientId: 1 });
db.medical_records.createIndex({ doctorId: 1 });
db.medical_records.createIndex({ createdAt: -1 });
db.medical_records.createIndex({ diagnosisCode: 1 });

print('MongoDB collections and indexes created successfully!');
