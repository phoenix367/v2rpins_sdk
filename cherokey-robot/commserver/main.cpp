/* 
 * File:   main.cpp
 * Author: Ivan Gubochkin
 *
 * Created on November 12, 2013, 11:05 PM
 */

#include <cstdlib>
#include <zmq.hpp>

using namespace std;
/*
 * 
 */
int main(int argc, char** argv) 
{
    // Prepare our context and publisher
    zmq::context_t context(1);
    zmq::socket_t publisher(context, ZMQ_PUB);
    publisher.bind("tcp://*:5556");

    while (1) 
    {
        // Send message to all subscribers
        zmq::message_t message(20);
        //snprintf((char *) message.data(), 20,
        //        "%05d %d %d", zipcode, temperature, relhumidity);
        publisher.send(message);

    }
    
    return 0;
}
