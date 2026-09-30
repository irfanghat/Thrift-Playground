#include "Calculator.h"

#include <exception>
#include <iostream>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

using namespace calculator;

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

int
main()
{
    std::shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
    std::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    std::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    CalculatorClient client(protocol);

    try
    {
        transport->open();

        cout << "1 + 1 = " << client.add(1, 1) << endl;

        try
        {
            cout << client.subtract(1, 1) << endl;
        }
        catch (exception e)
        {
            cout << "Error: " << e.what() << endl;
        }

        transport->close();
    }
    catch (TException& tx)
    {
        cout << "ERROR: " << tx.what() << '\n';
    }
}