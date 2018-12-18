#ifndef RHIO_HPP
#define RHIO_HPP

#include "rhio_common/Protocol.hpp"
#include "rhio_server/IONode.hpp"
#include "rhio_server/Bind.hpp"

namespace RhIO {

/**
 * Main RhIO global instance
 * Root of ParameterNode tree
 */
extern IONode Root;

/**
 * Internal pointer to the instance of
 * the publisher server running in its thread
 */
class ServerPub;
extern ServerPub* ServerStream;

/**
 * Internal pointer to the instance of
 * the logging server running in its thread
 */
class ServerLog;
extern ServerLog* ServerLogging;

/**
 * Start the RhIO server.
 *
 * @param portRep_ Network TCP port for request server. 
 * @param portPub_ Network TCP port for streaming server.
 * @param period_ Target time period in milliseconds
 * of streaming pub server loop.
 */
void start(
    unsigned int portRep_ = PortServerRep,
    unsigned int portPub_ = PortServerPub,
    unsigned int period_ = 20);

/**
 * Wait for the RhIO server
 * Rep and Pub threads to stop
 */
void stop();

/**
 * Has RhIO servers started?
 */
bool started();

/**
 * Clear the whole RhIO 
 * tree and reset it
 */
void reset();

/**
 * Write all logged data into 
 * a file of given path
 */
void writeLogs(const std::string& filepath);

}

#endif

