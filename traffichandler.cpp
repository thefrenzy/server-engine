#include "Panel.h"
#include "mapmanager.h"
#include "network_engine.h"
#include "Logger.h"
#include <chrono>
#include <iostream>

#pragma pack(push, 1)
struct NetHeader { uint16_t opcode; uint32_t length; };
struct PosPacket  { float x, y, z; uint32_t playerID; };
#pragma pack(pop)

void Traffic_Run() {
    std::string buffer;
    auto last_tick = std::chrono::steady_clock::now();
    int pps = 0;

    Logger::Log("[SYSTEM] Traffic Handler Active.");

    while (g_running) {
        while (net_get_next_message(buffer)) {
			
            if (buffer.size() < sizeof(NetHeader)) continue;

            const uint8_t* raw = reinterpret_cast<const uint8_t*>(buffer.data());
            NetHeader* h = (NetHeader*)raw;

            switch (h->opcode) {
                case 101: 
                    if (buffer.size() >= sizeof(NetHeader) + sizeof(PosPacket)) {
                        PosPacket* p = (PosPacket*)(raw + sizeof(NetHeader));
                        Logger::Log("[MOVE] Player " + std::to_string(p->playerID)); 
                    }
                    break;
                case 999:
                    Logger::Log("[NET] Shutdown Command Received.");
                    g_running = false;
                    break;
            }
            pps++;
        }

        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(now - last_tick).count() >= 1) {
            if (pps > 0) Logger::Log("[TRAFFIC] PPS: " + std::to_string(pps));
            pps = 0;
            last_tick = now;
        }
        std::this_thread::yield();
    }
}