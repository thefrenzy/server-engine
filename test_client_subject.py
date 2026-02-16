import socket
import struct
import time
import random

def run_test(host="127.0.0.1", port=8888, target_pps=5000):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    
    header_fmt = "<HI"
    payload_fmt = "<fffI"
    
    print(f"--- High-Speed Test: Target {target_pps} PPS ---")
    
    # Pre-calculate a burst to save Python CPU cycles
    payload = struct.pack(payload_fmt, 1.0, 2.0, 3.0, 7)
    header = struct.pack(header_fmt, 101, len(payload))
    packet = header + payload

    packet_count = 0
    start_time = time.time()

    try:
        while True:
            cycle_start = time.perf_counter()
            
            # Send a burst for this millisecond
            # To get 5000 per second, we send 5 packets every 1ms
            burst_size = target_pps // 1000 
            for _ in range(burst_size):
                s.sendto(packet, (host, port))
                packet_count += 1

            # Precision Sleep to hit exactly the target PPS
            # Wait for the remainder of the 1ms window
            while time.perf_counter() - cycle_start < 0.001:
                pass 

            if packet_count % 5000 == 0:
                elapsed = time.time() - start_time
                actual_pps = packet_count / elapsed
                print(f"Total Sent: {packet_count} | Approx PPS: {actual_pps:.0f}")

    except KeyboardInterrupt:
        print(f"\n[TEST ENDED] Sent: {packet_count}")
        s.sendto(struct.pack(header_fmt, 999, 0), (host, port))

if __name__ == "__main__":
    # Test the 5000 PPS limit
    run_test(target_pps=5000)
