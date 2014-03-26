/* 
 * File:   AbstractSender.hpp
 * Author: Ivan Gubochkin
 *
 * Created on 27 Март 2014 г., 0:39
 */

#ifndef ABSTRACTSENDER_HPP
#define	ABSTRACTSENDER_HPP

#include <zmq.hpp>
#include <type_traits>
#include <google/protobuf/message.h>

#include "Exceptions.hpp"

extern zmq::context_t gContext;

template <typename T> class AbstractSender
{
public:
    AbstractSender(const std::string& connectAddr)
    : socket(gContext, ZMQ_PUSH)
    {
        try
        {
            socket.connect(connectAddr.c_str());
        }
        catch (zmq::error_t& e)
        {
            COMM_EXCEPTION(InternalError, e.what());
        }
    }
    
    virtual ~AbstractSender()
    {
        socket.close();
    }

protected:
    void sendMessage(const T& msg)
    {
        static_assert(std::is_base_of<google::protobuf::Message, T>::value,
                "Type must be derived from google message");
        
        int messageSize = msg.ByteSize();
        std::vector<int8_t> outArray(messageSize);
        msg.SerializeToArray(&outArray[0], messageSize);

        socket.send(&outArray[0], messageSize, 0);
    }

private:
    zmq::socket_t socket;
};

#endif	/* ABSTRACTSENDER_HPP */
