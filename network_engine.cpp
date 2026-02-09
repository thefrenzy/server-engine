#include "network_engine.h"
#include "Logger.h"

static asio::io_context io_ctx;
static udp::socket* global_socket = nullptr;
static udp::endpoint remote_endpoint;
static std::string rx_buffer;
static std::thread* net_thread = nullptr;
static std::atomic<bool> is_running{false};

std::queue<std::string> message_queue;
std::mutex queue_mutex;

void rx_loop() {
    global_socket->async_receive_from(
        asio::buffer(&rx_buffer[0], rx_buffer.size()), remote_endpoint,
        [](const asio::error_code& ec, std::size_t bytes) {
            if (!ec && bytes > 0) {
                {
                    std::lock_guard<std::mutex> lock(queue_mutex);
                    message_queue.push(std::string(rx_buffer.data(), bytes));
                    if(message_queue.size() > 100) message_queue.pop();
                }
                rx_loop();
            }
        });
}

void net_init(int port) {
    rx_buffer.resize(1472);
    global_socket = new udp::socket(io_ctx, udp::endpoint(udp::v4(), port));
    asio::socket_base::receive_buffer_size opt(1024 * 1024);
    global_socket->set_option(opt);
}

void net_start() {
    if (is_running) return;
    is_running = true;
    rx_loop();
    net_thread = new std::thread([]() { 
        auto guard = asio::make_work_guard(io_ctx);
        io_ctx.run(); 
    });
}

void net_stop() {
    is_running = false;
    io_ctx.stop();
    if (net_thread && net_thread->joinable()) {
        net_thread->join();
    }
}

void net_send(const std::string& message, const std::string& host, int port) {
    udp::endpoint target(asio::ip::make_address(host), port);
    global_socket->async_send_to(asio::buffer(message), target, 
        [message](const asio::error_code&, std::size_t){});
}

bool net_get_next_message(std::string& out_msg) {
    std::lock_guard<std::mutex> lock(queue_mutex);
    if (message_queue.empty()) return false;
    out_msg = message_queue.front();
    message_queue.pop();
    return true;
}