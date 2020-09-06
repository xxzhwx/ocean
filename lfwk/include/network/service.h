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
    void SetHost(const string& host) { _host = host; }
    void SetPort(int port) { _port = port; }
    const string& GetName() const { return _name; }
    const string& GetHost() const { return _host; }
    int GetPort() const { return _port; }

    bool Startup();
    void Shutdown();

public:
    void OnAcceptSvc();
    void OnClientSvc();

private:
    string _name; // the service name
    string _host; // the listening host
    int _port;    // the listening port

    ThreadAgent<Service> _accept_thread;
    ThreadAgent<Service> _client_thread;
    bool _accept_running;
    bool _client_running;
};

NS_END_LFWK
