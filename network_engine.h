#ifndef NETWORK_ENGINE_HPP
#define NETWORK_ENGINE_HPP

#include <asio.hpp>
#include <string>
#include <vector>
#include <mutex>
#include <queue>
#include <thread>
#include <atomic>

#include "Logger.h"

using asio::ip::udp;

void net_init(int port);
void net_start();
void net_stop();
void net_send(const std::string& message, const std::string& host, int port);
bool net_get_next_message(std::string& out_msg);

extern std::mutex queue_mutex;
extern std::queue<std::string> message_queue;

#endif