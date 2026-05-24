#!/bin/bash
BASE_URL="http://localhost:8080"
PASS=0
FAIL=0

check() {
    if [ $1 -eq 0 ]; then
        echo -e "\033[0;32m✓ PASSED\033[0m: $2"
        ((PASS++))
    else
        echo -e "\033[0;31m✗ FAILED\033[0m: $2"
        ((FAIL++))
    fi
}

echo "========================================"
echo "Medical Records API Testing (userver)"
echo "========================================"

# 1. Health check
echo -e "\n1. Health Check"
RESP=$(curl -s $BASE_URL/health)
echo "   $RESP"
[[ "$RESP" == *"ok"* ]]; check $? "Health check"

# 2. Register
echo -e "\n2. Register User"
LOGIN="test_$(date +%s)"
RESP=$(curl -s -X POST $BASE_URL/api/auth/register \
  -H "Content-Type: application/json" \
  -d "{\"login\":\"${LOGIN}\",\"email\":\"${LOGIN}@test.ru\",\"firstName\":\"Test\",\"lastName\":\"User\",\"password\":\"123\"}")
echo "   $RESP"
[[ "$RESP" == *"\"id\":"* ]]; check $? "Register"

# 3. Login
echo -e "\n3. Login"
RESP=$(curl -s -X POST $BASE_URL/api/auth/login \
  -H "Content-Type: application/json" \
  -d "{\"login\":\"${LOGIN}\",\"password\":\"123\"}")
echo "   $RESP"
TOKEN=$(echo "$RESP" | python3 -c "import sys,json; print(json.load(sys.stdin)['token'])" 2>/dev/null)
[[ -n "$TOKEN" ]]; check $? "Login (token: ${TOKEN:0:20}...)"

# 4. Create patient
echo -e "\n4. Create Patient"
SNILS="SNILS-$(date +%s)"
RESP=$(curl -s -X POST $BASE_URL/api/patients \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $TOKEN" \
  -d "{\"firstName\":\"Anna\",\"lastName\":\"Smirnova\",\"phone\":\"+79991112233\",\"address\":\"Moscow\",\"birthDate\":\"1985-03-15\",\"snils\":\"${SNILS}\",\"policyNumber\":\"POL123\"}")
echo "   $RESP"
[[ "$RESP" == *"\"id\":"* ]]; check $? "Create patient"

# 5. Search patients
echo -e "\n5. Search Patients"
RESP=$(curl -s "$BASE_URL/api/patients/search?fullName=Smirnova")
echo "   $RESP"
[[ "$RESP" == *"patients"* ]]; check $? "Search patients"

# 6. Create medical record
echo -e "\n6. Create Medical Record"
RESP=$(curl -s -X POST $BASE_URL/api/medical-records \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $TOKEN" \
  -d "{\"patientId\":1,\"doctorId\":1,\"diagnosisCode\":\"J06.9\",\"diagnosisDescription\":\"Flu\",\"complaints\":\"Cough\"}")
echo "   $RESP"
[[ "$RESP" == *"\"code\":"* ]]; check $? "Create medical record"

# 7. Get patient records
echo -e "\n7. Get Patient Records"
RESP=$(curl -s "$BASE_URL/api/medical-records/patient/1" \
  -H "Authorization: Bearer $TOKEN")
echo "   $RESP"
[[ "$RESP" == *"records"* ]]; check $? "Get patient records"

echo -e "\n========================================"
echo "Results: $PASS passed, $FAIL failed"
echo "========================================"
