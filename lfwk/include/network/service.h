#pragma once
#include <string>
#include "def.h"
#include "thread_agent.h"

using std::string;

NS_BEGIN_LFWK

class Service
{
public:
    Service(const string& name);
    virtual ~Service();

public:
    void SetHost(const string& host) { host_ = host; }
    void SetPort(int port) { port_ = port; }
    const string& GetName() const { return name_; }
    const string& GetHost() const { return host_; }
    int GetPort() const { return port_; }

    bool Startup();
    void Shutdown();

public:
    void OnAcceptSvc();
    void OnClientSvc();

private:
    string name_; // the service name
    string host_; // the listening host
    int port_;    // the listening port

    ThreadAgent<Service> accept_thread_;
    ThreadAgent<Service> client_thread_;
};

NS_END_LFWK
