cmake_minimum_required(VERSION 3.12...3.31)
project(medical_api CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(DownloadUserver)

find_package(
    userver
    COMPONENTS core postgresql
    QUIET
)
if(NOT userver_FOUND)
    download_userver(TRY_DIR third_party/userver)
endif()

userver_setup_environment()

include_directories(src)

add_library(
    ${PROJECT_NAME}_objs OBJECT
    src/greeting.cpp
    src/hello.cpp
    src/handlers/health_handler.cpp
    src/handlers/register_handler.cpp
    src/handlers/login_handler.cpp
    src/handlers/patient_handler.cpp
    src/handlers/record_handler.cpp
    src/repositories/InMemoryUserRepository.cpp
    src/repositories/InMemoryPatientRepository.cpp
    src/repositories/InMemoryMedicalRecordRepository.cpp
    src/repositories/PostgresUserRepository.cpp
    src/repositories/PostgresPatientRepository.cpp
    src/repositories/PostgresMedicalRecordRepository.cpp
    src/services/PasswordHasher.cpp
    src/services/JWTService.cpp
    src/services/UserService.cpp
    src/services/PatientService.cpp
    src/services/MedicalRecordService.cpp
    src/services/AuthService.cpp
)
target_link_libraries(
    ${PROJECT_NAME}_objs
    PUBLIC userver::core
    PUBLIC userver::postgresql
)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_objs)

add_executable(${PROJECT_NAME}_unittest src/greeting_test.cpp)
target_link_libraries(${PROJECT_NAME}_unittest PRIVATE ${PROJECT_NAME}_objs userver::utest)
add_google_tests(${PROJECT_NAME}_unittest)

add_executable(${PROJECT_NAME}_benchmark src/greeting_benchmark.cpp)
target_link_libraries(${PROJECT_NAME}_benchmark PRIVATE ${PROJECT_NAME}_objs userver::ubench)
add_google_benchmark_tests(${PROJECT_NAME}_benchmark)

userver_testsuite_add_simple()

include(GNUInstallDirs)

if(DEFINED ENV{PREFIX})
    message(STATUS "Set install prefix: $ENV{PREFIX}")
    file(TO_CMAKE_PATH "$ENV{PREFIX}" PREFIX_PATH)
    set(CMAKE_INSTALL_PREFIX "${PREFIX_PATH}")
endif()

file(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)

install(
    TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT ${PROJECT_NAME}
)
install(
    FILES ${CONFIGS_FILES}
    DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME}
    COMPONENT ${PROJECT_NAME}
)
