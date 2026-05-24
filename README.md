# Medical Records Management System (MongoDB + PostgreSQL)

REST API сервис для управления медицинскими записями с MongoDB и PostgreSQL (Вариант 20, ДЗ №4).

## Функциональность

- Регистрация и аутентификация пользователей (JWT)
- Поиск пользователей по маске имени
- Регистрация пациентов
- Поиск пациентов по ФИО
- Создание медицинских записей
- Получение истории записей пациента
- Получение записи по уникальному коду

## Технологии

- C++20 + **userver** (асинхронный фреймворк Яндекса)
- **MongoDB 7** — документная база данных
- **PostgreSQL 15** — реляционная база данных
- JWT аутентификация
- Docker (docker-compose)

## Документная модель MongoDB

### Коллекция `users` — Пользователи (врачи)
```json
{
  "_id": ObjectId,
  "login": "dr.ivanov",        // String, unique
  "email": "ivanov@hospital.ru", // String, pattern validation
  "firstName": "Иван",          // String, required
  "lastName": "Иванов",         // String, required
  "patronymic": "Петрович",     // String
  "passwordHash": "hash",       // String, required
  "isActive": true,             // Boolean
  "createdAt": ISODate          // Date
}
```

### Коллекция `patients` — Пациенты
```json
{
  "_id": ObjectId,
  "userId": ObjectId,           // Reference → users._id
  "firstName": "Александр",     // String, required
  "lastName": "Сидоров",        // String, required
  "patronymic": "Иванович",     // String
  "phone": "+79001234567",      // String
  "address": "г. Москва...",    // String
  "birthDate": ISODate,         // Date
  "snils": "123-456-789 01",    // String, unique
  "policyNumber": "1234567890", // String
  "createdAt": ISODate          // Date
}
```

### Коллекция `medical_records` — Медицинские записи
```json
{
  "_id": ObjectId,
  "code": "MED-20260320-00001",  // String, unique, pattern
  "patientId": ObjectId,         // Reference → patients._id
  "doctorId": ObjectId,          // Reference → users._id
  "diagnosisCode": "J06.9",      // String
  "diagnosisDescription": "...", // String
  "complaints": "...",           // String
  "recommendations": "...",      // String
  "symptoms": [                  // Array of embedded documents
    {
      "name": "Кашель",
      "severity": "Средняя",     // required
      "startedAt": ISODate
    }
  ],
  "treatments": [               // Array of embedded documents
    {
      "name": "Парацетамол",
      "dosage": "500mg",        // required
      "frequency": "3 раза/день",
      "startDate": ISODate,
      "endDate": ISODate
    }
  ],
  "createdAt": ISODate          // Date
}
```

### Выбор Embedded vs References

| Связь | Решение | Обоснование |
|-------|---------|-------------|
| User → MedicalRecord | **References** | Записи часто обновляются независимо, могут быть большими |
| Patient → MedicalRecord | **References** | Записи создаются отдельно, пациент может иметь много записей |
| Symptoms в MedicalRecord | **Embedded** | Всегда читаются вместе с записью, не имеют смысла отдельно |
| Treatments в MedicalRecord | **Embedded** | Всегда читаются вместе с записью, не имеют смысла отдельно |

## Индексы MongoDB

### Коллекция `users`
- `{ login: 1 }` (unique) — быстрый поиск по логину
- `{ email: 1 }` — поиск по email
- `{ lastName: 1, firstName: 1 }` — поиск по ФИО

### Коллекция `patients`
- `{ snils: 1 }` (unique, sparse) — быстрая проверка уникальности СНИЛС
- `{ lastName: 1, firstName: 1 }` — поиск по ФИО
- `{ userId: 1 }` — поиск пациентов врача

### Коллекция `medical_records`
- `{ code: 1 }` (unique) — поиск записи по коду
- `{ patientId: 1 }` — история записей пациента
- `{ doctorId: 1 }` — записи врача
- `{ createdAt: -1 }` — сортировка по дате
- `{ diagnosisCode: 1 }` — поиск по коду диагноза

## Валидация схем ($jsonSchema)

Валидация настроена для коллекций `users` и `medical_records`:
- **users**: required поля (login, email, firstName, lastName, passwordHash), pattern для email, minLength для login
- **medical_records**: required поля (code, patientId, doctorId), pattern для code (MED-YYYYMMDD-XXXXX), вложенная валидация для symptoms и treatments

## Быстрый старт (Docker)

1. Установите Docker Desktop
2. Склонируйте репозиторий:
```
git clone https://github.com/Chigatu/medical-records-system-mongo.git
cd medical-records-system-mongo
```
3. Запустите сервисы (API + PostgreSQL + MongoDB):
```
docker-compose up --build
```
4. Проверьте:
```
curl http://localhost:8080/health
```

## Работа с MongoDB (отдельно)

```
# Запустить MongoDB shell в контейнере
docker exec -it medical-db-mongo mongosh medical_records

# Выполнить скрипты
docker exec -it medical-db-mongo mongosh medical_records /docker-entrypoint-initdb.d/01-init.js
```

## CRUD операции

Все операции реализованы в `database/mongo/crud.js`:
- **Create**: insertOne, insertMany
- **Read**: findOne, find с $regex, $gt, $lt, $in, $and, $or
- **Update**: updateOne с $set, $push, $pull
- **Delete**: deleteOne, мягкое удаление

Операторы: `$eq`, `$ne`, `$gt`, `$lt`, `$gte`, `$lte`, `$in`, `$and`, `$or`, `$regex`, `$push`, `$pull`, `$set`

## Aggregation Pipeline

Примеры в `database/mongo/aggregation.js`:
- Подсчет записей по диагнозам (`$group`, `$sort`)
- Записи по врачам (`$lookup`, `$unwind`, `$group`)
- Среднее количество симптомов (`$avg`, `$size`)
- Пациенты с количеством записей (`$group`, `$lookup`)
- Распределение по месяцам (`$year`, `$month`)
- Поиск тяжелых симптомов (`$unwind`, `$match`)

## Структура проекта

```
medical-records-system-mongo/
├── src/                        # Исходный код API
│   ├── handlers/               # HTTP обработчики
│   ├── models/                 # Доменные модели
│   ├── repositories/           # Репозитории (InMemory, PostgreSQL, MongoDB)
│   ├── services/               # Бизнес-логика
│   └── main.cpp                # Точка входа
├── configs/                    # Конфигурация userver
├── database/
│   ├── mongo/                  # MongoDB
│   │   ├── init.js             # Создание коллекций + индексы + валидация
│   │   ├── data.js             # Тестовые данные (12 врачей, 12 пациентов, 12 записей)
│   │   ├── crud.js             # CRUD операции
│   │   ├── validation.js       # Тесты валидации схем
│   │   └── aggregation.js      # Aggregation pipeline
│   └── postgres/               # PostgreSQL (из ДЗ №3)
├── Dockerfile                  # Docker образ API
├── docker-compose.yaml         # Docker Compose (API + PostgreSQL + MongoDB)
└── README.md                   # Документация
```

## API Endpoints

| Метод | URL | Описание | Требуется JWT |
|-------|-----|----------|---------------|
| GET | /health | Проверка работоспособности | Нет |
| GET | /db/health | Проверка подключения к PostgreSQL | Нет |
| POST | /api/auth/register | Регистрация пользователя | Нет |
| POST | /api/auth/login | Вход в систему | Нет |
| POST | /api/patients | Создание пациента | Да |
| GET | /api/patients/search?fullName={name} | Поиск пациентов по ФИО | Нет |
| POST | /api/medical-records | Создание медицинской записи | Да |
| GET | /api/medical-records/patient/{id} | Получение истории записей пациента | Да |

## Тестирование

```
./test_api_userver.sh
```

Для MongoDB:
```
docker exec -it medical-db-mongo mongosh medical_records /docker-entrypoint-initdb.d/01-init.js
docker exec -it medical-db-mongo mongosh medical_records /docker-entrypoint-initdb.d/02-data.js
docker exec -it medical-db-mongo mongosh medical_records /docker-entrypoint-initdb.d/../mongo/crud.js
```
