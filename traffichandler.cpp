#include "Panel.h"
#include "mapmanager.h"
#include "network_engine.h"
#include "Logger.h"
#include <chrono>
#include <thread>
#include <vector>

#pragma pack(push, 1)
struct NetHeader { uint16_t opcode; uint32_t length; };
struct PosPacket  { float x, y, z; uint32_t playerID; };
#pragma pack(pop)

void Traffic_Run() {
    // Reuse a vector/buffer to avoid heap reallocations
    std::string buffer; 
    buffer.reserve(1024); 

    auto last_tick = std::chrono::steady_clock::now();
    uint32_t pps = 0;

    Logger::Log("[SYSTEM] Optimized Traffic Handler Active.");

    while (g_running) {
        bool processed_any = false;

        // Process all available messages in the queue
        while (net_get_next_message(buffer)) {
            processed_any = true;
            
            if (buffer.size() < sizeof(NetHeader)) continue;

            const NetHeader* h = reinterpret_cast<const NetHeader*>(buffer.data());

            switch (h->opcode) {
                case 101: 
                    // Optimization: Remove per-packet logging for movement.
                    // Instead, update the MapManager or internal state directly.
                    if (buffer.size() >= sizeof(NetHeader) + sizeof(PosPacket)) {
                        // PosPacket* p = (PosPacket*)(buffer.data() + sizeof(NetHeader));
                        // UpdateGameState(p); 
                    }
                    break;

                case 999:
                    Logger::Log("[NET] Shutdown Command Received.");
                    g_running = false;
                    break;
            }
            pps++;
        }

        // Metrics Tracking
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();
        
        if (elapsed >= 1000) {
            if (pps > 0) Logger::Log("[TRAFFIC] PPS: " + std::to_string(pps));
            pps = 0;
            last_tick = now;
        }

        // CPU Relief: If no messages were found, sleep for a tiny window.
        // This prevents 100% core usage while maintaining low latency.
        if (!processed_any) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}