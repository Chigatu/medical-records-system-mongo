// ============================================
// Medical Records System - Schema Validation
// Variant 20
// ============================================

db = db.getSiblingDB('medical_records');

print('========================================');
print('Schema Validation Tests');
print('========================================');

// ============================================
// 1. Валидация коллекции users
// ============================================
print('\n--- USERS VALIDATION ---');

// Успешная вставка (валидные данные)
print('\n1. Valid user insert:');
try {
    const validUser = db.users.insertOne({
        login: 'valid.user',
        email: 'valid@hospital.ru',
        firstName: 'Valid',
        lastName: 'User',
        passwordHash: 'hash123',
        isActive: true,
        createdAt: new Date()
    });
    print(`  ✓ Inserted: ${validUser.insertedId}`);
} catch (e) {
    print(`  ✗ Error: ${e.message}`);
}

// Невалидный email
print('\n2. Invalid email:');
try {
    db.users.insertOne({
        login: 'invalid.email',
        email: 'not-an-email',
        firstName: 'Invalid',
        lastName: 'Email',
        passwordHash: 'hash123'
    });
    print('  ✗ Should have failed but succeeded!');
} catch (e) {
    print(`  ✓ Correctly rejected: ${e.message.includes('email') ? 'email validation' : e.message}`);
}

// Отсутствует обязательное поле passwordHash
print('\n3. Missing required field (passwordHash):');
try {
    db.users.insertOne({
        login: 'no.password',
        email: 'no@pass.com',
        firstName: 'No',
        lastName: 'Password'
    });
    print('  ✗ Should have failed!');
} catch (e) {
    print(`  ✓ Correctly rejected: missing required field`);
}

// Слишком короткий login
print('\n4. Too short login:');
try {
    db.users.insertOne({
        login: 'ab',
        email: 'short@login.com',
        firstName: 'Short',
        lastName: 'Login',
        passwordHash: 'hash123'
    });
    print('  ✗ Should have failed!');
} catch (e) {
    print(`  ✓ Correctly rejected: login too short`);
}

// ============================================
// 2. Валидация коллекции medical_records
// ============================================
print('\n\n--- MEDICAL RECORDS VALIDATION ---');

// Успешная вставка
print('\n5. Valid medical record insert:');
try {
    const patientId = db.patients.findOne()._id;
    const doctorId = db.users.findOne({ login: 'dr.ivanov' })._id;
    const validRecord = db.medical_records.insertOne({
        code: 'MED-20260524-00099',
        patientId: patientId,
        doctorId: doctorId,
        diagnosisCode: 'A00',
        diagnosisDescription: 'Холера',
        complaints: 'Диарея, рвота',
        symptoms: [
            { name: 'Диарея', severity: 'Сильная', startedAt: new Date() }
        ],
        treatments: [
            { name: 'Регидрон', dosage: '1 пакет', frequency: '3 раза в день', startDate: new Date() }
        ],
        createdAt: new Date()
    });
    print(`  ✓ Inserted: ${validRecord.insertedId}`);
} catch (e) {
    print(`  ✗ Error: ${e.message}`);
}

// Невалидный код записи (не соответствует паттерну)
print('\n6. Invalid record code:');
try {
    db.medical_records.insertOne({
        code: 'INVALID-CODE',
        patientId: db.patients.findOne()._id,
        doctorId: db.users.findOne()._id,
        diagnosisCode: 'A00',
        diagnosisDescription: 'Test'
    });
    print('  ✗ Should have failed!');
} catch (e) {
    print(`  ✓ Correctly rejected: invalid code format`);
}

// Отсутствует обязательное поле patientId
print('\n7. Missing required field (patientId):');
try {
    db.medical_records.insertOne({
        code: 'MED-20260524-00100',
        doctorId: db.users.findOne()._id,
        diagnosisCode: 'A00',
        diagnosisDescription: 'Test'
    });
    print('  ✗ Should have failed!');
} catch (e) {
    print(`  ✓ Correctly rejected: missing required field`);
}

// Симптом с недостающими полями
print('\n8. Symptom missing required field (severity):');
try {
    db.medical_records.insertOne({
        code: 'MED-20260524-00101',
        patientId: db.patients.findOne()._id,
        doctorId: db.users.findOne()._id,
        diagnosisCode: 'A00',
        diagnosisDescription: 'Test',
        symptoms: [
            { name: 'Боль' }  // Нет severity
        ]
    });
    print('  ✗ Should have failed!');
} catch (e) {
    print(`  ✓ Correctly rejected: symptom missing severity`);
}

// ============================================
// 3. Тестирование дубликатов (unique индекс)
// ============================================
print('\n\n--- UNIQUE CONSTRAINTS ---');

// Дубликат login
print('\n9. Duplicate login:');
try {
    db.users.insertOne({
        login: 'dr.ivanov',  // Уже существует
        email: 'duplicate@test.ru',
        firstName: 'Dup',
        lastName: 'Login',
        passwordHash: 'hash'
    });
    print('  ✗ Should have failed!');
} catch (e) {
    print(`  ✓ Correctly rejected: duplicate login`);
}

// Дубликат snils
print('\n10. Duplicate SNILS:');
try {
    db.patients.insertOne({
        firstName: 'Dup',
        lastName: 'Snils',
        snils: '123-456-789 01'  // Уже существует
    });
    print('  ✗ Should have failed!');
} catch (e) {
    print(`  ✓ Correctly rejected: duplicate SNILS`);
}

// Дубликат code записи
print('\n11. Duplicate record code:');
try {
    db.medical_records.insertOne({
        code: 'MED-20260320-00001',  // Уже существует
        patientId: db.patients.findOne()._id,
        doctorId: db.users.findOne()._id,
        diagnosisCode: 'A00',
        diagnosisDescription: 'Test'
    });
    print('  ✗ Should have failed!');
} catch (e) {
    print(`  ✓ Correctly rejected: duplicate record code`);
}

print('\n========================================');
print('Validation Tests Completed!');
print('========================================');
