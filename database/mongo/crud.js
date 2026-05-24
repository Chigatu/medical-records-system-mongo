// ============================================
// Medical Records System - CRUD Operations
// Variant 20
// ============================================

db = db.getSiblingDB('medical_records');

print('========================================');
print('CRUD Operations for Medical Records API');
print('========================================');

// ============================================
// 1. USERS (Пользователи/Врачи)
// ============================================

print('\n--- USERS ---');

// CREATE: Регистрация нового пользователя
print('\n1. CREATE - Register new user:');
const newUser = db.users.insertOne({
    login: 'dr.new_doctor',
    email: 'new_doctor@hospital.ru',
    firstName: 'Новый',
    lastName: 'Доктор',
    patronymic: 'Тестович',
    passwordHash: 'hashed_password_here',
    isActive: true,
    createdAt: new Date()
});
print(`Created user with _id: ${newUser.insertedId}`);

// READ: Поиск пользователя по логину (для аутентификации)
print('\n2. READ - Find user by login:');
const userByLogin = db.users.findOne({ login: 'dr.ivanov' });
printjson(userByLogin);

// READ: Поиск пользователей по маске имени
print('\n3. READ - Search users by name mask:');
const usersByName = db.users.find({
    $or: [
        { lastName: { $regex: 'Иван', $options: 'i' } },
        { firstName: { $regex: 'Иван', $options: 'i' } }
    ]
}).toArray();
print(`Found ${usersByName.length} users`);
usersByName.forEach(u => print(`  ${u.lastName} ${u.firstName} (${u.login})`));

// READ: Поиск по email (точное совпадение)
print('\n4. READ - Find user by email:');
const userByEmail = db.users.findOne({ email: 'ivanov@hospital.ru' });
print(userByEmail ? `Found: ${userByEmail.login}` : 'Not found');

// READ: Поиск активных пользователей
print('\n5. READ - Find active users:');
const activeUsers = db.users.find({ isActive: true }).toArray();
print(`Active users: ${activeUsers.length}`);

// READ: Поиск пользователей созданных после даты
print('\n6. READ - Users created after 2026-01-10:');
const recentUsers = db.users.find({
    createdAt: { $gt: new Date('2026-01-10') }
}).toArray();
print(`Users created after Jan 10: ${recentUsers.length}`);

// UPDATE: Обновление email пользователя
print('\n7. UPDATE - Update user email:');
const updateResult = db.users.updateOne(
    { login: 'dr.ivanov' },
    { $set: { email: 'ivanov.new@hospital.ru' } }
);
print(`Modified: ${updateResult.modifiedCount} document(s)`);

// UPDATE: Деактивация пользователя
print('\n8. UPDATE - Deactivate user:');
const deactivateResult = db.users.updateOne(
    { login: 'dr.novikov' },
    { $set: { isActive: false } }
);
print(`Modified: ${deactivateResult.modifiedCount} document(s)`);

// DELETE: Удаление пользователя (мягкое - деактивация)
print('\n9. DELETE - Soft delete user:');
const softDeleteResult = db.users.updateOne(
    { login: 'dr.volkova' },
    { $set: { isActive: false } }
);
print(`Soft deleted: ${softDeleteResult.modifiedCount} user(s)`);

// ============================================
// 2. PATIENTS (Пациенты)
// ============================================

print('\n\n--- PATIENTS ---');

// CREATE: Регистрация нового пациента
print('\n10. CREATE - Register new patient:');
const newPatient = db.patients.insertOne({
    firstName: 'Новый',
    lastName: 'Пациент',
    patronymic: 'Тестович',
    phone: '+79000000000',
    address: 'г. Москва, ул. Тестовая, д. 1',
    birthDate: new Date('2000-01-01'),
    snils: '999-888-777 01',
    policyNumber: '9998887776665554',
    createdAt: new Date()
});
print(`Created patient with _id: ${newPatient.insertedId}`);

// READ: Поиск пациента по СНИЛС
print('\n11. READ - Find patient by SNILS:');
const patientBySnils = db.patients.findOne({ snils: '123-456-789 01' });
print(patientBySnils ? `Found: ${patientBySnils.lastName} ${patientBySnils.firstName}` : 'Not found');

// READ: Поиск пациентов по ФИО
print('\n12. READ - Search patients by name:');
const patientsByName = db.patients.find({
    lastName: { $regex: 'Сидор', $options: 'i' }
}).toArray();
print(`Found ${patientsByName.length} patients`);
patientsByName.forEach(p => print(`  ${p.lastName} ${p.firstName} (SNILS: ${p.snils})`));

// READ: Поиск пациентов по возрасту (старше 1985 года)
print('\n13. READ - Patients born after 1985:');
const youngPatients = db.patients.find({
    birthDate: { $gt: new Date('1985-01-01') }
}).toArray();
print(`Patients born after 1985: ${youngPatients.length}`);

// READ: Поиск с $in оператором
print('\n14. READ - Find patients by multiple SNILS:');
const patientsBySnils = db.patients.find({
    snils: { $in: ['123-456-789 01', '123-456-789 02', '123-456-789 03'] }
}).toArray();
print(`Found ${patientsBySnils.length} patients`);

// READ: Поиск с $and оператором
print('\n15. READ - Find patients with phone AND address:');
const patientsWithContacts = db.patients.find({
    $and: [
        { phone: { $ne: null } },
        { address: { $ne: null } }
    ]
}).toArray();
print(`Patients with contacts: ${patientsWithContacts.length}`);

// UPDATE: Обновление телефона пациента
print('\n16. UPDATE - Update patient phone:');
const updatePhoneResult = db.patients.updateOne(
    { snils: '123-456-789 01' },
    { $set: { phone: '+79009999999' } }
);
print(`Modified: ${updatePhoneResult.modifiedCount} document(s)`);

// UPDATE: Добавление userId (связь с пользователем)
print('\n17. UPDATE - Link patient to user:');
const linkUserResult = db.patients.updateOne(
    { snils: '123-456-789 01' },
    { $set: { userId: newUser.insertedId } }
);
print(`Linked: ${linkUserResult.modifiedCount} patient(s)`);

// DELETE: Удаление пациента (по СНИЛС)
print('\n18. DELETE - Delete patient by SNILS:');
const deletePatientResult = db.patients.deleteOne({ snils: '999-888-777 01' });
print(`Deleted: ${deletePatientResult.deletedCount} patient(s)`);

// ============================================
// 3. MEDICAL RECORDS (Медицинские записи)
// ============================================

print('\n\n--- MEDICAL RECORDS ---');

// CREATE: Создание медицинской записи
print('\n19. CREATE - Create medical record:');
const newRecord = db.medical_records.insertOne({
    code: 'MED-20260524-00013',
    patientId: patientBySnils._id,
    doctorId: userByLogin._id,
    diagnosisCode: 'J11.1',
    diagnosisDescription: 'Грипп',
    complaints: 'Температура 39, ломота в теле',
    recommendations: 'Постельный режим, обильное питье',
    symptoms: [
        { name: 'Температура', severity: 'Высокая', startedAt: new Date('2026-05-24') },
        { name: 'Ломота в теле', severity: 'Средняя', startedAt: new Date('2026-05-24') }
    ],
    treatments: [
        { name: 'Осельтамивир', dosage: '75mg', frequency: '2 раза в день', startDate: new Date('2026-05-24') }
    ],
    createdAt: new Date()
});
print(`Created record with _id: ${newRecord.insertedId}`);

// READ: Поиск записи по коду
print('\n20. READ - Find record by code:');
const recordByCode = db.medical_records.findOne({ code: 'MED-20260320-00001' });
print(recordByCode ? `Found: ${recordByCode.diagnosisDescription}` : 'Not found');

// READ: Получение истории записей пациента
print('\n21. READ - Patient records history:');
const patientRecords = db.medical_records.find({
    patientId: patientBySnils._id
}).sort({ createdAt: -1 }).toArray();
print(`Records for patient: ${patientRecords.length}`);
patientRecords.forEach(r => print(`  ${r.code}: ${r.diagnosisDescription} (${r.createdAt.toISOString()})`));

// READ: Поиск записей по диагнозу
print('\n22. READ - Records by diagnosis code:');
const recordsByDiagnosis = db.medical_records.find({
    diagnosisCode: 'J06.9'
}).toArray();
print(`Records with J06.9: ${recordsByDiagnosis.length}`);

// READ: Поиск с $gt по дате
print('\n23. READ - Records after date:');
const recentRecords = db.medical_records.find({
    createdAt: { $gt: new Date('2026-03-25') }
}).sort({ createdAt: -1 }).toArray();
print(`Records after Mar 25: ${recentRecords.length}`);

// READ: Поиск с $and (диагноз + дата)
print('\n24. READ - Records with $and:');
const filteredRecords = db.medical_records.find({
    $and: [
        { diagnosisCode: { $in: ['J06.9', 'J03.9', 'J11.1'] } },
        { createdAt: { $gte: new Date('2026-03-20') } }
    ]
}).toArray();
print(`Filtered records: ${filteredRecords.length}`);

// UPDATE: Добавление симптома в запись ($push)
print('\n25. UPDATE - Add symptom ($push):');
const pushResult = db.medical_records.updateOne(
    { code: 'MED-20260320-00001' },
    { $push: { symptoms: { name: 'Головная боль', severity: 'Средняя', startedAt: new Date() } } }
);
print(`Modified: ${pushResult.modifiedCount} document(s)`);

// UPDATE: Добавление лечения ($push)
print('\n26. UPDATE - Add treatment ($push):');
const pushTreatmentResult = db.medical_records.updateOne(
    { code: 'MED-20260320-00001' },
    { $push: { treatments: { name: 'Ибупрофен', dosage: '400mg', frequency: 'при болях', startDate: new Date() } } }
);
print(`Modified: ${pushTreatmentResult.modifiedCount} document(s)`);

// UPDATE: Удаление симптома ($pull)
print('\n27. UPDATE - Remove symptom ($pull):');
const pullResult = db.medical_records.updateOne(
    { code: 'MED-20260320-00001' },
    { $pull: { symptoms: { name: 'Головная боль' } } }
);
print(`Modified: ${pullResult.modifiedCount} document(s)`);

// UPDATE: Обновление рекомендаций
print('\n28. UPDATE - Update recommendations:');
const updateRecResult = db.medical_records.updateOne(
    { code: 'MED-20260320-00001' },
    { $set: { recommendations: 'Обновленные рекомендации: покой и витамины' } }
);
print(`Modified: ${updateRecResult.modifiedCount} document(s)`);

// DELETE: Удаление записи
print('\n29. DELETE - Delete medical record:');
const deleteRecordResult = db.medical_records.deleteOne({ code: 'MED-20260524-00013' });
print(`Deleted: ${deleteRecordResult.deletedCount} record(s)`);

print('\n========================================');
print('CRUD Operations Completed!');
print('========================================');
