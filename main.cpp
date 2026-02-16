#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>
#include <random> // Added for workload simulation
#include <cmath>

#include "PlayersManager.h"
#include "WeaponsManager.h"
#include "network_engine.h"
#include "mapmanager.h"
#include "Panel.h"
#include "Logger.h"

using namespace std;
std::atomic<bool> g_running{ true };

// Simulation Tools: Normal distribution (Mean: 250us, StdDev: 50us)
// This simulates the "G" (General) part of the M/G/1 queuing math
std::default_random_engine generator;
std::normal_distribution<double> workload_dist(250.0, 50.0);

void SimulatePacketProcessing() {
    double work_time = workload_dist(generator);
    if (work_time < 10) work_time = 10; // Floor

    auto start = chrono::steady_clock::now();
    volatile double math_burner = 1.1; 
    
    // Busy-loop to simulate actual CPU cycles being consumed
    while (chrono::duration_cast<chrono::microseconds>(
           chrono::steady_clock::now() - start).count() < work_time) {
        math_burner = std::sqrt(std::pow(math_burner + 1.01, 2.0));
    }
}

int main() {
    // Standard Inits
    PlayerManager pl; InitPlayers(pl);
    MapState ms; LoadMapConfig(ms);
    WeaponManager wm; InitWeapons(wm);

    net_init(8888);
    net_start();

    thread uiThread(RunGraphicsPanel, ref(ms));

    string incoming_data;
    Logger::Log("[SYSTEM] Stress-Test Mode: Enabled");

    int msg_counter = 0;
    auto last_tick = chrono::steady_clock::now();

    while (g_running) {
        // --- THE DRAIN LOOP ---
        // We process EVERYTHING in the queue immediately
        while (net_get_next_message(incoming_data)) {
            ++msg_counter;

            // NEW: Simulate the mathematical cost of a game request
            SimulatePacketProcessing();

            if (incoming_data == "exit") {
                g_running = false;
            }
        }

        // --- PERFORMANCE MONITORING ---
        auto now = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - last_tick).count();
        
        if (elapsed >= 1000) {
            // Calculate how much of that second was spent working vs sleeping
            // At 3000 PPS * 250us = 750,000us (75% Load)
            Logger::Log("[NET] PPS: " + to_string(msg_counter) + " | Status: " + 
                        (msg_counter > 2800 ? "HEAVY LOAD" : "STABLE"));
            
            msg_counter = 0;
            last_tick = now;
        }

        // We use a tiny yield instead of a full sleep to keep responsiveness high
        std::this_thread::yield(); 
    }

    net_stop();
    if (uiThread.joinable()) uiThread.join();

    return 0;
}