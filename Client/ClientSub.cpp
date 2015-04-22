#include <stdexcept>
#include <chrono>
#include "ClientSub.hpp"
#include "Protocol.hpp"
#include "DataBuffer.hpp"

namespace RhIO {
        
ClientSub::ClientSub(const std::string& endpoint) :
    _mutex(),
    _isContinue(true),
    _handlerBool(StreamBoolHandler()),
    _handlerInt(StreamIntHandler()),
    _handlerFloat(StreamFloatHandler()),
    _handlerStr(StreamStrHandler()),
    //Starting receiver thread
    _thread(&ClientSub::subscriberThread, this, endpoint)
{
}
        
ClientSub::~ClientSub()
{
    //Asking subscriber thread stop
    _isContinue = false;
    //Waiting thread end
    _thread.join();
}

void ClientSub::setHandlerBool(StreamBoolHandler handler)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _handlerBool = handler;
}
void ClientSub::setHandlerInt(StreamIntHandler handler)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _handlerInt = handler;
}
void ClientSub::setHandlerFloat(StreamFloatHandler handler)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _handlerFloat = handler;
}
void ClientSub::setHandlerStr(StreamStrHandler handler)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _handlerStr = handler;
}
        
void ClientSub::subscriberThread(const std::string& endpoint)
{
    //ZMQ context
    zmq::context_t context(1);
    //ZMQ socket
    zmq::socket_t socket(context, ZMQ_SUB);
    //Connection to Server
    socket.connect(endpoint.c_str());
    //Subscribing to all
    socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);

    while (_isContinue) {
        //Wait for Server next packet
        zmq::message_t packet;
        if (!socket.recv(&packet, ZMQ_NOBLOCK)) {
            //If no data is available wait a bit
            std::this_thread::sleep_for(
                std::chrono::milliseconds(10));
            continue;
        }
        DataBuffer sub(packet.data(), packet.size());

        //Check empty message
        if (sub.size() == 0) {
            throw std::logic_error(
                "RhIOClient empty server message");
        }
        //Retrieve message type
        MsgType type = (MsgType)sub.readType();
        if (type == MsgStreamBool) {
            //Stream Bool value
            std::string name = sub.readStr();
            long timestamp = sub.readInt();
            bool val = sub.readBool();
            std::lock_guard<std::mutex> lock(_mutex);
            if (_handlerBool) {
                _handlerBool(name, timestamp, val);
            }
            continue;
        } else if (type == MsgStreamInt) {
            //Stream Int value
            std::string name = sub.readStr();
            long timestamp = sub.readInt();
            long val = sub.readInt();
            std::lock_guard<std::mutex> lock(_mutex);
            if (_handlerInt) {
                _handlerInt(name, timestamp, val);
            }
            continue;
        } else if (type == MsgStreamFloat) {
            //Stream Float value
            std::string name = sub.readStr();
            long timestamp = sub.readInt();
            double val = sub.readFloat();
            std::lock_guard<std::mutex> lock(_mutex);
            if (_handlerFloat) {
                _handlerFloat(name, timestamp, val);
            }
            continue;
        } else if (type == MsgStreamStr) {
            //Stream Str value
            std::string name = sub.readStr();
            long timestamp = sub.readInt();
            std::string val = sub.readStr();
            std::lock_guard<std::mutex> lock(_mutex);
            if (_handlerStr) {
                _handlerStr(name, timestamp, val);
            }
            continue;
        } else {
            throw std::logic_error(
                "RhIOClient invalid stream message type");
        }
    }
}

}
