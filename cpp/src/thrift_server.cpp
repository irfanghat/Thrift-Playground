#include <cstdint>
#include <thrift/TToString.h>
#include <thrift/concurrency/ThreadFactory.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include <iostream>

#include "Calculator.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using namespace calculator;

class CalculatorHandler : public CalculatorIf
{
  public:
    CalculatorHandler() = default;

    int32_t
    add(int32_t const n1, int32_t const n2) override
    {
        cout << "add(" << n1 << ", " << n2 << ")" << std::endl;
        return n1 + n2;
    }

    int32_t
    subtract(int32_t const n1, int32_t n2) override
    {
        cout << "subtract(" << n1 << ", " << n2 << ")" << endl;
        return n1 - n2;
    }
};

/*
  CalculatorIfFactory is code generated.
  CalculatorCloneFactory is useful for getting access to the server side of the
  transport.  It is also useful for making per-connection state.  Without this
  CloneFactory, all connections will end up sharing the same handler instance.
*/
class CalculatorCloneFactory : virtual public CalculatorIfFactory
{
  public:
    ~CalculatorCloneFactory() override = default;
    CalculatorIf*
    getHandler(::apache::thrift::TConnectionInfo const& connInfo) override
    {
        std::shared_ptr<TSocket> sock =
            std::dynamic_pointer_cast<TSocket>(connInfo.transport);
        cout << "Incoming connection\n";
        cout << "\tSocketInfo: " << sock->getSocketInfo() << "\n";
        cout << "\tPeerHost: " << sock->getPeerHost() << "\n";
        cout << "\tPeerAddress: " << sock->getPeerAddress() << "\n";
        cout << "\tPeerPort: " << sock->getPeerPort() << "\n";
        return new CalculatorHandler;
    }
    void
    releaseHandler(CalculatorIf* handler) override
    {
        delete handler;
    }
};

int
main()
{
    TThreadedServer server(std::make_shared<CalculatorProcessorFactory>(
                               std::make_shared<CalculatorCloneFactory>()),
                           std::make_shared<TServerSocket>(9090), // port
                           std::make_shared<TBufferedTransportFactory>(),
                           std::make_shared<TBinaryProtocolFactory>());

    server.serve();
}
