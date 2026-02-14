import socket
import struct
import time
import random

def run_test(host="127.0.0.1", port=8888, mode="steady"):
    # Create UDP Socket
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    
    header_fmt = "<HI"
    payload_fmt = "<fffI"
    
    print(f"--- Starting Test in [{mode}] mode ---")
    print(f"Target: {host}:{port}")
    
    try:
        packet_count = 0
        while True:
            # 1. Generate Random Game Data
            # This helps verify the 'Interface' is actually updating values
            player_id = random.randint(0, 9) # Matches your MAX_IDS = 10
            pos_x = random.uniform(-100.0, 100.0)
            pos_y = random.uniform(-100.0, 100.0)
            pos_z = 0.0
            
            # 2. Pack Binary Data
            payload = struct.pack(payload_fmt, pos_x, pos_y, pos_z, player_id)
            header = struct.pack(header_fmt, 101, len(payload))
            
            # 3. Send to C++ Server
            s.sendto(header + payload, (host, port))
            packet_count += 1
            
            # 4. Handle Modes (The Bottleneck Test)
            if mode == "steady":
                # 10 packets per second - Server should handle this easily
                time.sleep(0.1) 
                if packet_count % 10 == 0:
                    print(f"Sent {packet_count} packets...")
            
            elif mode == "stress":
                # No sleep! This will flood the C++ message_queue
                if packet_count % 1000 == 0:
                    print(f"STRESS TEST: Sent {packet_count} packets...")
                # Tiny sleep just to keep CPU sane, but fast enough to overflow 100-packet queue
                time.sleep(0.0001) 

    except KeyboardInterrupt:
        print(f"\n[TEST ENDED] Total packets sent: {packet_count}")
        # Send Shutdown (Opcode 999) to close the C++ engine threads
        print("Sending shutdown signal to server...")
        s.sendto(struct.pack(header_fmt, 999, 0), (host, port))

if __name__ == "__main__":
    # Change mode to "stress" to test your queue bottleneck!
    run_test(mode="steady")
