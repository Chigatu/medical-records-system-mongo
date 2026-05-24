// ============================================
// Medical Records System - Test Data (MongoDB)
// Variant 20
// ============================================

db = db.getSiblingDB('medical_records');

// Очищаем коллекции перед вставкой
db.users.deleteMany({});
db.patients.deleteMany({});
db.medical_records.deleteMany({});

print('Inserting test users...');

// Вставляем пользователей (врачей)
const users = db.users.insertMany([
    {
        login: 'dr.ivanov',
        email: 'ivanov@hospital.ru',
        firstName: 'Иван',
        lastName: 'Иванов',
        patronymic: 'Петрович',
        passwordHash: 'c002055f8ebbbbd9',
        isActive: true,
        createdAt: new Date('2026-01-01')
    },
    {
        login: 'dr.petrova',
        email: 'petrova@hospital.ru',
        firstName: 'Мария',
        lastName: 'Петрова',
        patronymic: 'Сергеевна',
        passwordHash: 'c002055f8ebbbbd9',
        isActive: true,
        createdAt: new Date('2026-01-01')
    },
    {
        login: 'dr.smirnov',
        email: 'smirnov@hospital.ru',
        firstName: 'Алексей',
        lastName: 'Смирнов',
        patronymic: 'Иванович',
        passwordHash: 'c002055f8ebbbbd9',
        isActive: true,
        createdAt: new Date('2026-01-01')
    },
    {
        login: 'dr.kuznetsova',
        email: 'kuznetsova@hospital.ru',
        firstName: 'Елена',
        lastName: 'Кузнецова',
        patronymic: 'Андреевна',
        passwordHash: 'c002055f8ebbbbd9',
        isActive: true,
        createdAt: new Date('2026-01-01')
    },
    {
        login: 'dr.popov',
        email: 'popov@hospital.ru',
        firstName: 'Дмитрий',
        lastName: 'Попов',
        patronymic: 'Александрович',
        passwordHash: 'c002055f8ebbbbd9',
        isActive: true,
        createdAt: new Date('2026-01-01')
    },
    {
        login: 'dr.vasileva',
        email: 'vasileva@hospital.ru',
        firstName: 'Анна',
        lastName: 'Васильева',
        patronymic: 'Николаевна',
        passwordHash: 'c002055f8ebbbbd9',
        isActive: true,
        createdAt: new Date('2026-01-01')
    },
    {
        login: 'dr.sokolov',
        email: 'sokolov@hospital.ru',
        firstName: 'Сергей',
        lastName: 'Соколов',
        patronymic: 'Викторович',
        passwordHash: 'c002055f8ebbbbd9',
        isActive: true,
        createdAt: new Date('2026-01-01')
    },
    {
        login: 'dr.mikhailova',
        email: 'mikhailova@hospital.ru',
        firstName: 'Ольга',
        lastName: 'Михайлова',
        patronymic: 'Павловна',
        passwordHash: 'c002055f8ebbbbd9',
        isActive: true,
        createdAt: new Date('2026-01-01')
    },
    {
        login: 'dr.novikov',
        email: 'novikov@hospital.ru',
        firstName: 'Андрей',
        lastName: 'Новиков',
        patronymic: 'Сергеевич',
        passwordHash: 'c002055f8ebbbbd9',
        isActive: false,
        createdAt: new Date('2026-01-01')
    },
    {
        login: 'dr.fedorova',
        email: 'fedorova@hospital.ru',
        firstName: 'Татьяна',
        lastName: 'Федорова',
        patronymic: 'Игоревна',
        passwordHash: 'c002055f8ebbbbd9',
        isActive: true,
        createdAt: new Date('2026-01-01')
    },
    {
        login: 'dr.morozov',
        email: 'morozov@hospital.ru',
        firstName: 'Павел',
        lastName: 'Морозов',
        patronymic: 'Дмитриевич',
        passwordHash: 'c002055f8ebbbbd9',
        isActive: true,
        createdAt: new Date('2026-01-15')
    },
    {
        login: 'dr.volkova',
        email: 'volkova@hospital.ru',
        firstName: 'Светлана',
        lastName: 'Волкова',
        patronymic: 'Алексеевна',
        passwordHash: 'c002055f8ebbbbd9',
        isActive: true,
        createdAt: new Date('2026-01-15')
    }
]);

print(`Inserted ${users.insertedIds.length} users`);

// Вставляем пациентов
print('Inserting test patients...');
const patients = db.patients.insertMany([
    {
        firstName: 'Александр',
        lastName: 'Сидоров',
        patronymic: 'Иванович',
        phone: '+79001234567',
        address: 'г. Москва, ул. Ленина, д. 1, кв. 10',
        birthDate: new Date('1980-03-15'),
        snils: '123-456-789 01',
        policyNumber: '1234567890123456',
        createdAt: new Date()
    },
    {
        firstName: 'Наталья',
        lastName: 'Сидорова',
        patronymic: 'Петровна',
        phone: '+79002345678',
        address: 'г. Москва, ул. Ленина, д. 1, кв. 10',
        birthDate: new Date('1982-07-22'),
        snils: '123-456-789 02',
        policyNumber: '1234567890123457',
        createdAt: new Date()
    },
    {
        firstName: 'Михаил',
        lastName: 'Козлов',
        patronymic: 'Александрович',
        phone: '+79003456789',
        address: 'г. Москва, ул. Пушкина, д. 5, кв. 22',
        birthDate: new Date('1975-11-08'),
        snils: '123-456-789 03',
        policyNumber: '1234567890123458',
        createdAt: new Date()
    },
    {
        firstName: 'Екатерина',
        lastName: 'Козлова',
        patronymic: 'Дмитриевна',
        phone: '+79004567890',
        address: 'г. Москва, ул. Пушкина, д. 5, кв. 22',
        birthDate: new Date('1978-04-30'),
        snils: '123-456-789 04',
        policyNumber: '1234567890123459',
        createdAt: new Date()
    },
    {
        firstName: 'Дмитрий',
        lastName: 'Николаев',
        patronymic: 'Сергеевич',
        phone: '+79005678901',
        address: 'г. Москва, ул. Гагарина, д. 12, кв. 5',
        birthDate: new Date('1990-09-14'),
        snils: '123-456-789 05',
        policyNumber: '1234567890123460',
        createdAt: new Date()
    },
    {
        firstName: 'Анна',
        lastName: 'Николаева',
        patronymic: 'Владимировна',
        phone: '+79006789012',
        address: 'г. Москва, ул. Гагарина, д. 12, кв. 5',
        birthDate: new Date('1992-01-25'),
        snils: '123-456-789 06',
        policyNumber: '1234567890123461',
        createdAt: new Date()
    },
    {
        firstName: 'Сергей',
        lastName: 'Морозов',
        patronymic: 'Алексеевич',
        phone: '+79007890123',
        address: 'г. Москва, ул. Мира, д. 8, кв. 15',
        birthDate: new Date('1985-06-18'),
        snils: '123-456-789 07',
        policyNumber: '1234567890123462',
        createdAt: new Date()
    },
    {
        firstName: 'Ольга',
        lastName: 'Морозова',
        patronymic: 'Игоревна',
        phone: '+79008901234',
        address: 'г. Москва, ул. Мира, д. 8, кв. 15',
        birthDate: new Date('1987-12-03'),
        snils: '123-456-789 08',
        policyNumber: '1234567890123463',
        createdAt: new Date()
    },
    {
        firstName: 'Андрей',
        lastName: 'Волков',
        patronymic: 'Павлович',
        phone: '+79009012345',
        address: 'г. Москва, ул. Цветочная, д. 3, кв. 7',
        birthDate: new Date('1970-08-20'),
        snils: '123-456-789 09',
        policyNumber: '1234567890123464',
        createdAt: new Date()
    },
    {
        firstName: 'Татьяна',
        lastName: 'Волкова',
        patronymic: 'Сергеевна',
        phone: '+79000123456',
        address: 'г. Москва, ул. Цветочная, д. 3, кв. 7',
        birthDate: new Date('1972-02-14'),
        snils: '123-456-789 10',
        policyNumber: '1234567890123465',
        createdAt: new Date()
    },
    {
        firstName: 'Павел',
        lastName: 'Федоров',
        patronymic: 'Николаевич',
        phone: '+79001234568',
        address: 'г. Москва, ул. Солнечная, д. 7, кв. 12',
        birthDate: new Date('1995-05-10'),
        snils: '123-456-789 11',
        policyNumber: '1234567890123466',
        createdAt: new Date()
    },
    {
        firstName: 'Марина',
        lastName: 'Федорова',
        patronymic: 'Александровна',
        phone: '+79002345679',
        address: 'г. Москва, ул. Солнечная, д. 7, кв. 12',
        birthDate: new Date('1997-09-28'),
        snils: '123-456-789 12',
        policyNumber: '1234567890123467',
        createdAt: new Date()
    }
]);

print(`Inserted ${patients.insertedIds.length} patients`);

// Вставляем медицинские записи
print('Inserting medical records...');
const records = db.medical_records.insertMany([
    {
        code: 'MED-20260320-00001',
        patientId: patients.insertedIds[0],
        doctorId: users.insertedIds[0],
        diagnosisCode: 'J06.9',
        diagnosisDescription: 'Острая респираторная инфекция верхних дыхательных путей',
        complaints: 'Кашель, насморк, температура 38.5',
        recommendations: 'Постельный режим, обильное питье',
        symptoms: [
            { name: 'Кашель', severity: 'Средняя', startedAt: new Date('2026-03-20') },
            { name: 'Насморк', severity: 'Легкая', startedAt: new Date('2026-03-19') }
        ],
        treatments: [
            { name: 'Парацетамол', dosage: '500mg', frequency: '3 раза в день', startDate: new Date('2026-03-20') }
        ],
        createdAt: new Date('2026-03-20')
    },
    {
        code: 'MED-20260321-00002',
        patientId: patients.insertedIds[2],
        doctorId: users.insertedIds[1],
        diagnosisCode: 'K29.1',
        diagnosisDescription: 'Хронический гастрит',
        complaints: 'Боли в эпигастрии, изжога',
        symptoms: [
            { name: 'Боль в эпигастрии', severity: 'Сильная', startedAt: new Date('2026-03-21') }
        ],
        treatments: [
            { name: 'Омепразол', dosage: '20mg', frequency: '2 раза в день', startDate: new Date('2026-03-21') }
        ],
        createdAt: new Date('2026-03-21')
    },
    {
        code: 'MED-20260322-00003',
        patientId: patients.insertedIds[4],
        doctorId: users.insertedIds[0],
        diagnosisCode: 'I10',
        diagnosisDescription: 'Гипертоническая болезнь',
        complaints: 'Головная боль, давление 160/100',
        recommendations: 'Контроль АД, прием гипотензивных',
        symptoms: [
            { name: 'Головная боль', severity: 'Средняя', startedAt: new Date('2026-03-22') }
        ],
        treatments: [
            { name: 'Эналаприл', dosage: '10mg', frequency: '1 раз в день', startDate: new Date('2026-03-22') }
        ],
        createdAt: new Date('2026-03-22')
    },
    {
        code: 'MED-20260323-00004',
        patientId: patients.insertedIds[6],
        doctorId: users.insertedIds[2],
        diagnosisCode: 'M54.1',
        diagnosisDescription: 'Остеохондроз поясничного отдела',
        complaints: 'Боли в пояснице, онемение левой ноги',
        symptoms: [
            { name: 'Боль в пояснице', severity: 'Сильная', startedAt: new Date('2026-03-23') },
            { name: 'Онемение ноги', severity: 'Средняя', startedAt: new Date('2026-03-23') }
        ],
        treatments: [
            { name: 'Диклофенак', dosage: '50mg', frequency: '2 раза в день', startDate: new Date('2026-03-23') }
        ],
        createdAt: new Date('2026-03-23')
    },
    {
        code: 'MED-20260324-00005',
        patientId: patients.insertedIds[1],
        doctorId: users.insertedIds[3],
        diagnosisCode: 'J03.9',
        diagnosisDescription: 'Острый тонзиллит',
        complaints: 'Боли в горле, температура 39.0',
        symptoms: [
            { name: 'Боль в горле', severity: 'Сильная', startedAt: new Date('2026-03-24') }
        ],
        treatments: [
            { name: 'Амоксициллин', dosage: '500mg', frequency: '3 раза в день', startDate: new Date('2026-03-24') }
        ],
        createdAt: new Date('2026-03-24')
    },
    {
        code: 'MED-20260325-00006',
        patientId: patients.insertedIds[8],
        doctorId: users.insertedIds[5],
        diagnosisCode: 'E11.9',
        diagnosisDescription: 'Сахарный диабет 2 типа',
        complaints: 'Повышенный сахар крови, жажда',
        symptoms: [
            { name: 'Жажда', severity: 'Средняя', startedAt: new Date('2026-03-25') }
        ],
        treatments: [
            { name: 'Метформин', dosage: '1000mg', frequency: '2 раза в день', startDate: new Date('2026-03-25') }
        ],
        createdAt: new Date('2026-03-25')
    },
    {
        code: 'MED-20260326-00007',
        patientId: patients.insertedIds[3],
        doctorId: users.insertedIds[1],
        diagnosisCode: 'H52.1',
        diagnosisDescription: 'Миопия средней степени',
        complaints: 'Снижение зрения вдаль',
        symptoms: [
            { name: 'Снижение зрения', severity: 'Средняя', startedAt: new Date('2026-03-26') }
        ],
        createdAt: new Date('2026-03-26')
    },
    {
        code: 'MED-20260326-00008',
        patientId: patients.insertedIds[5],
        doctorId: users.insertedIds[6],
        diagnosisCode: 'F41.2',
        diagnosisDescription: 'Тревожно-депрессивное расстройство',
        complaints: 'Бессонница, тревожность',
        symptoms: [
            { name: 'Бессонница', severity: 'Сильная', startedAt: new Date('2026-03-26') },
            { name: 'Тревожность', severity: 'Средняя', startedAt: new Date('2026-03-26') }
        ],
        treatments: [
            { name: 'Сертралин', dosage: '50mg', frequency: '1 раз в день', startDate: new Date('2026-03-26') }
        ],
        createdAt: new Date('2026-03-26')
    },
    {
        code: 'MED-20260327-00009',
        patientId: patients.insertedIds[7],
        doctorId: users.insertedIds[4],
        diagnosisCode: 'L20.8',
        diagnosisDescription: 'Атопический дерматит',
        complaints: 'Зуд, высыпания на коже',
        symptoms: [
            { name: 'Зуд', severity: 'Сильная', startedAt: new Date('2026-03-27') }
        ],
        treatments: [
            { name: 'Цетиризин', dosage: '10mg', frequency: '1 раз в день', startDate: new Date('2026-03-27') }
        ],
        createdAt: new Date('2026-03-27')
    },
    {
        code: 'MED-20260328-00010',
        patientId: patients.insertedIds[9],
        doctorId: users.insertedIds[7],
        diagnosisCode: 'K80.1',
        diagnosisDescription: 'Желчнокаменная болезнь',
        complaints: 'Боли в правом подреберье',
        symptoms: [
            { name: 'Боль в правом подреберье', severity: 'Сильная', startedAt: new Date('2026-03-28') }
        ],
        recommendations: 'Диета, УЗИ контроль, консультация хирурга',
        createdAt: new Date('2026-03-28')
    },
    {
        code: 'MED-20260329-00011',
        patientId: patients.insertedIds[10],
        doctorId: users.insertedIds[9],
        diagnosisCode: 'N20.0',
        diagnosisDescription: 'Мочекаменная болезнь',
        complaints: 'Боли в пояснице',
        symptoms: [
            { name: 'Боль в пояснице', severity: 'Сильная', startedAt: new Date('2026-03-29') }
        ],
        treatments: [
            { name: 'Дротаверин', dosage: '40mg', frequency: 'при болях', startDate: new Date('2026-03-29') }
        ],
        createdAt: new Date('2026-03-29')
    },
    {
        code: 'MED-20260330-00012',
        patientId: patients.insertedIds[11],
        doctorId: users.insertedIds[10],
        diagnosisCode: 'G43.0',
        diagnosisDescription: 'Мигрень без ауры',
        complaints: 'Пульсирующая головная боль, светобоязнь',
        symptoms: [
            { name: 'Головная боль', severity: 'Сильная', startedAt: new Date('2026-03-30') },
            { name: 'Светобоязнь', severity: 'Средняя', startedAt: new Date('2026-03-30') }
        ],
        treatments: [
            { name: 'Суматриптан', dosage: '50mg', frequency: 'при приступе', startDate: new Date('2026-03-30') }
        ],
        createdAt: new Date('2026-03-30')
    }
]);

print(`Inserted ${records.insertedIds.length} medical records`);
print('Test data inserted successfully!');
