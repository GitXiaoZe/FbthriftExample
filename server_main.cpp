#include <iostream>
#include <memory>
#include <mutex>
#include <time.h>
#include <folly/init/Init.h>
#include "gen-cpp2/Echo.h"

#include <thrift/lib/cpp2/server/ThriftServer.h>

using namespace apache::thrift;
using namespace echoapp;
class EchoSvIfImple : public EchoSvIf{
	
	
	void async_tm_echo(std::unique_ptr<apache::thrift::HandlerCallback<std::unique_ptr<std::string>>> callback,std::unique_ptr<std::string> message) override {
		std::cout << "Do Nothing" << std::endl;
		callback->result("server return");

	}
	
	
};

int main(int argc,char** argv){
	folly::init(&argc,&argv);
	
	auto handler = std::make_shared<EchoSvIfImple>();
	auto server = std::make_shared<ThriftServer>();
	server->setInterface(handler);
	server->setPort(9999);
	std::cout << "Starting the server..." << std::endl;
	server->serve();

	return 0;
}
