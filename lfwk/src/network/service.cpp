#include "network/service.h"

NS_BEGIN_LFWK

Service::Service(const string& name) :
    _name(name),
    _host("0.0.0.0"),
    _port(0),
    _accept_running(false),
    _client_running(false)
{
    _client_thread.Initialize(this, &Service::OnClientSvc);
    _accept_thread.Initialize(this, &Service::OnAcceptSvc);
}

Service::~Service()
{

}

bool Service::Startup()
{
    _client_running = true;
    _client_thread.Activate();

    _accept_running = true;
    _accept_thread.Activate();

    return true;
}

void Service::Shutdown()
{
    _accept_running = false;
    _accept_thread.Wait();

    _client_running = false;
    _client_thread.Wait();
}

void Service::OnAcceptSvc()
{
    while (_accept_running)
    {
        printf("[%s] OnAcceptSvc \n", _name.c_str());
        LFWK_Sleep(1000);
    }
}

void Service::OnClientSvc()
{
    while (_client_running)
    {
        printf("[%s] OnClientSvc \n", _name.c_str());
        LFWK_Sleep(1000);
    }
}

NS_END_LFWK
