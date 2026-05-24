#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component_list.hpp>
#include <userver/components/component.hpp>
#include <userver/components/component_list.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/congestion_control/component.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>
#include <userver/storages/postgres/component.hpp>

#include <hello.hpp>
#include <handlers/health_handler.hpp>
#include <handlers/register_handler.hpp>
#include <handlers/login_handler.hpp>
#include <handlers/patient_handler.hpp>
#include <handlers/record_handler.hpp>

#include <repositories/RepositoryFactory.h>

#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "========================================" << std::endl;
    std::cout << "Medical Records API Server (with PostgreSQL)" << std::endl;
    std::cout << "Architecture: Domain-Driven Design + userver" << std::endl;
    std::cout << "========================================" << std::endl;
    
    auto component_list =
        userver::components::MinimalServerComponentList()
            .Append<userver::server::handlers::Ping>()
            .Append<userver::components::TestsuiteSupport>()
            .AppendComponentList(userver::clients::http::ComponentList())
            .Append<userver::clients::dns::Component>()
            .Append<userver::server::handlers::TestsControl>()
            .Append<userver::congestion_control::Component>()
            .Append<userver::components::Postgres>("postgres-db-1")
            .Append<medical_api::Hello>()
            .Append<medical_api::HealthHandler>()
            .Append<medical_api::RegisterHandler>()
            .Append<medical_api::LoginHandler>()
            .Append<medical_api::CreatePatientHandler>()
            .Append<medical_api::SearchPatientHandler>()
            .Append<medical_api::CreateRecordHandler>()
            .Append<medical_api::GetPatientRecordsHandler>()
        ;

    return userver::utils::DaemonMain(argc, argv, component_list);
}
