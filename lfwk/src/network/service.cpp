#include "network/service.h"

NS_BEGIN_LFWK

Service::Service(const string& name) :
    name_(name),
    host_("0.0.0.0"),
    port_(0)
{
    accept_thread_.Initialize(this, &Service::OnAcceptSvc);
    client_thread_.Initialize(this, &Service::OnClientSvc);
}

Service::~Service()
{

}

bool Service::Startup()
{
    accept_thread_.Activate();
    client_thread_.Activate();

    return false;
}

void Service::Shutdown()
{

}

void Service::OnAcceptSvc()
{

}

void Service::OnClientSvc()
{

}

NS_END_LFWK
