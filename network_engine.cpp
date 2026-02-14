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
    if (!is_running) return;

    global_socket->async_receive_from(
        asio::buffer(&rx_buffer[0], rx_buffer.size()), remote_endpoint,
        [](const asio::error_code& ec, std::size_t bytes) {
            if (!ec && bytes > 0) {

                {
                    std::lock_guard<std::mutex> lock(queue_mutex);
                    message_queue.push(std::string(rx_buffer.data(), bytes));
                    
                    // Prevent bottleneck overflow
                    if(message_queue.size() > 100) {
                        message_queue.pop();
                    }
                }
                
                // Continue listening
                rx_loop();
            } else if (ec) {
                Logger::Log("[NET ERROR] Receive failed: " + ec.message());
            }
        });
}

void net_init(int port) {
    try {
        rx_buffer.resize(1472); // Max MTU for UDP
        global_socket = new udp::socket(io_ctx, udp::endpoint(udp::v4(), port));
        
        asio::socket_base::receive_buffer_size opt(1024 * 1024);
        global_socket->set_option(opt);
        
        Logger::Log("[NET] Socket bound to port " + std::to_string(port));
    } catch (const std::exception& e) {
        Logger::Log("[NET FATAL] Initialization failed: " + std::string(e.what()));
    }
}

void net_start() {
    if (is_running) return;
    is_running = true;

    // Start listening BEFORE the thread runs
    rx_loop();

    net_thread = new std::thread([]() { 
        try {
            auto guard = asio::make_work_guard(io_ctx);
            io_ctx.run(); 
        } catch (const std::exception& e) {
            Logger::Log("[NET ERROR] IO Context stopped: " + std::string(e.what()));
        }
    });

    Logger::Log("[NET] Network engine thread started");
}

void net_stop() {
    is_running = false;
    io_ctx.stop();
    if (net_thread && net_thread->joinable()) {
        net_thread->join();
    }
    Logger::Log("[NET] Network engine stopped");
}

void net_send(const std::string& message, const std::string& host, int port) {
    try {
        udp::endpoint target(asio::ip::make_address(host), port);
        global_socket->async_send_to(asio::buffer(message), target, 
            [](const asio::error_code& ec, std::size_t){
                if(ec) Logger::Log("[NET ERROR] Send failed: " + ec.message());
            });
    } catch (...) {
        Logger::Log("[NET ERROR] Invalid address: " + host);
    }
}

bool net_get_next_message(std::string& out_msg) {
    std::lock_guard<std::mutex> lock(queue_mutex);
    if (message_queue.empty()) return false;
    
    out_msg = message_queue.front();
    message_queue.pop();
    return true;
}