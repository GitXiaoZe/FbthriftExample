#include <iostream>
#include <string>
#include <thread>
#include <stdlib.h>
#include <time.h>

#include <folly/SocketAddress.h>
#include <folly/init/Init.h>
#include <folly/synchronization/Baton.h>
#include <folly/io/async/EventBase.h>
#include <thrift/lib/cpp/async/TAsyncSocket.h>
#include <thrift/lib/cpp2/async/HeaderClientChannel.h>
#include <thrift/lib/cpp2/async/RequestChannel.h>
#include "gen-cpp2/Echo.h"

using namespace apache::thrift;
using apache::thrift::HeaderClientChannel;
using apache::thrift::async::TAsyncSocket;
using apache::thrift::RequestCallback;
using apache::thrift::ClientReceiveState;
using namespace echoapp;
using namespace folly;

#define SERVER_IP "172.16.40.91"
#define SERVER_PORT 9999
class MyCallback : public RequestCallback{
	public:
		explicit MyCallback(){ }
		void requestSent() override {
			std::cout << "requestSend() has been called " << std::endl;
		}
		void replyReceived(ClientReceiveState&& state) override{
			std::cout << "replyReceived() has been called "  << std::endl;
		}
		void requestError(ClientReceiveState&& state) override{
			std::cout << "requestError() has been called " << std::endl;
		}
};
void MyEchoClient(){
	EventBase evb;
	auto addr = folly::SocketAddress(SERVER_IP,SERVER_PORT);
	TAsyncSocket::UniquePtr sock(new TAsyncSocket(&evb,addr));
	auto chan = HeaderClientChannel::newChannel(std::move(sock));
	chan->setProtocolId(apache::thrift::protocol::T_COMPACT_PROTOCOL);
	auto client = std::make_unique<EchoAsyncClient>(std::move(chan));
	int message_len = 1024;
	std::string message(1024,' ');	
	client->echo(std::make_unique<MyCallback>(),message);
	evb.loop();
}


int main(int argc,char** argv){
	folly::init(&argc,&argv);
	MyEchoClient();
	return 0;
}
