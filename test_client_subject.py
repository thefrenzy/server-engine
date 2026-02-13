import socket
import time

SERVER_IP = "127.0.0.1"
SERVER_PORT = 8888
TARGET_PPS = 3000
INTERVAL = 1.0 / TARGET_PPS

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
print(f"Pushing to {TARGET_PPS} Hz...")

# Use perf_counter for microsecond precision
next_tick = time.perf_counter()

try:
    while True:
        # Send the packet
        sock.sendto(b"this is a test", (SERVER_IP, SERVER_PORT))
        
        # Calculate when the next packet SHOULD go out
        next_tick += INTERVAL
        
        # BUSY WAIT: Loop until the clock hits the target time
        # This uses more CPU but is the ONLY way to get sub-1ms precision
        while time.perf_counter() < next_tick:
            pass 
except KeyboardInterrupt:
    sock.close()
