// ============================================
// Medical Records System - Aggregation Pipeline
// Variant 20
// ============================================

db = db.getSiblingDB('medical_records');

print('========================================');
print('Aggregation Pipelines');
print('========================================');

// ============================================
// 1. Количество записей по диагнозам
// ============================================
print('\n1. Records count by diagnosis:');
const recordsByDiagnosis = db.medical_records.aggregate([
    {
        $group: {
            _id: { code: '$diagnosisCode', description: '$diagnosisDescription' },
            count: { $sum: 1 }
        }
    },
    { $sort: { count: -1 } },
    { $project: { _id: 0, diagnosis: '$_id', count: 1 } }
]).toArray();
recordsByDiagnosis.forEach(r => print(`  ${r.diagnosis.code}: ${r.diagnosis.description} - ${r.count} records`));

// ============================================
// 2. Количество записей по врачам
// ============================================
print('\n2. Records count by doctor:');
const recordsByDoctor = db.medical_records.aggregate([
    {
        $lookup: {
            from: 'users',
            localField: 'doctorId',
            foreignField: '_id',
            as: 'doctor'
        }
    },
    { $unwind: '$doctor' },
    {
        $group: {
            _id: { id: '$doctor._id', name: { $concat: ['$doctor.lastName', ' ', '$doctor.firstName'] } },
            count: { $sum: 1 }
        }
    },
    { $sort: { count: -1 } },
    { $project: { _id: 0, doctor: '$_id.name', count: 1 } }
]).toArray();
recordsByDoctor.forEach(r => print(`  ${r.doctor}: ${r.count} records`));

// ============================================
// 3. Среднее количество симптомов на запись
// ============================================
print('\n3. Average symptoms per record:');
const avgSymptoms = db.medical_records.aggregate([
    {
        $project: {
            diagnosisCode: 1,
            symptomsCount: { $cond: { if: { $isArray: '$symptoms' }, then: { $size: '$symptoms' }, else: 0 } }
        }
    },
    {
        $group: {
            _id: null,
            avgSymptoms: { $avg: '$symptomsCount' },
            maxSymptoms: { $max: '$symptomsCount' },
            totalRecords: { $sum: 1 }
        }
    }
]).toArray();
if (avgSymptoms.length > 0) {
    print(`  Average: ${avgSymptoms[0].avgSymptoms.toFixed(2)} symptoms/record`);
    print(`  Max: ${avgSymptoms[0].maxSymptoms} symptoms`);
    print(`  Total records: ${avgSymptoms[0].totalRecords}`);
}

// ============================================
// 4. Пациенты с количеством записей
// ============================================
print('\n4. Patients with records count:');
const patientsWithRecords = db.medical_records.aggregate([
    {
        $group: {
            _id: '$patientId',
            recordCount: { $sum: 1 },
            lastRecord: { $max: '$createdAt' }
        }
    },
    {
        $lookup: {
            from: 'patients',
            localField: '_id',
            foreignField: '_id',
            as: 'patient'
        }
    },
    { $unwind: '$patient' },
    {
        $project: {
            _id: 0,
            patientName: { $concat: ['$patient.lastName', ' ', '$patient.firstName'] },
            recordCount: 1,
            lastRecord: 1
        }
    },
    { $sort: { recordCount: -1 } }
]).toArray();
patientsWithRecords.forEach(p => print(`  ${p.patientName}: ${p.recordCount} records (last: ${p.lastRecord.toISOString()})`));

// ============================================
// 5. Распределение записей по месяцам
// ============================================
print('\n5. Records distribution by month:');
const recordsByMonth = db.medical_records.aggregate([
    {
        $group: {
            _id: {
                year: { $year: '$createdAt' },
                month: { $month: '$createdAt' }
            },
            count: { $sum: 1 }
        }
    },
    { $sort: { '_id.year': 1, '_id.month': 1 } },
    {
        $project: {
            _id: 0,
            period: { $concat: [{ $toString: '$_id.year' }, '-', { $toString: '$_id.month' }] },
            count: 1
        }
    }
]).toArray();
recordsByMonth.forEach(r => print(`  ${r.period}: ${r.count} records`));

// ============================================
// 6. Поиск записей с тяжелыми симптомами
// ============================================
print('\n6. Records with severe symptoms:');
const severeRecords = db.medical_records.aggregate([
    { $unwind: '$symptoms' },
    { $match: { 'symptoms.severity': 'Сильная' } },
    {
        $lookup: {
            from: 'patients',
            localField: 'patientId',
            foreignField: '_id',
            as: 'patient'
        }
    },
    { $unwind: '$patient' },
    {
        $project: {
            _id: 0,
            code: 1,
            patient: { $concat: ['$patient.lastName', ' ', '$patient.firstName'] },
            symptom: '$symptoms.name',
            severity: '$symptoms.severity'
        }
    }
]).toArray();
severeRecords.forEach(r => print(`  ${r.code}: ${r.patient} - ${r.symptom} (${r.severity})`));

print('\n========================================');
print('Aggregation Pipelines Completed!');
print('========================================');
