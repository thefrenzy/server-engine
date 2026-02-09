import socket
import time

SERVER_IP = "127.0.0.1"
SERVER_PORT = 8888
FREQUENCY = 60
INTERVAL = 1.0 / FREQUENCY

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

print(f"Sending 'test1' to {SERVER_IP}:{SERVER_PORT} at {FREQUENCY} Hz")

try:
    while True:
        start = time.time()

        sock.sendto(b"test3", (SERVER_IP, SERVER_PORT))

        # maintain 60 Hz
        elapsed = time.time() - start
        time.sleep(max(0, INTERVAL - elapsed))

except KeyboardInterrupt:
    print("Stopped.")

finally:
    sock.close()
