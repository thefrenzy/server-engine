import socket
import struct
import time
import random

# --- CONFIGURATION ---
SERVER_IP = "127.0.0.1"
SERVER_PORT = 8888

# Opcodes matching your C++ TrafficHandler
OP_POSITION = 101
OP_EXIT     = 999

def send_position_packet(sock, player_id, x, y, z):
    """
    Constructs a DOD-compatible binary packet:
    Header: Opcode(H=2), Length(I=4)
    Payload: X(f=4), Y(f=4), Z(f=4), ID(I=4)
    Total size: 6 + 16 = 22 bytes
    """
    # 1. Prepare the payload (3 floats and 1 uint32)
    payload = struct.pack("<fffI", x, y, z, player_id)
    
    # 2. Prepare the header (Opcode and Length of payload)
    header = struct.pack("<HI", OP_POSITION, len(payload))
    
    # 3. Send the full binary "bboy"
    sock.sendall(header + payload)

def run_stress_test():
    print(f"Connecting to Server Engine at {SERVER_IP}:{SERVER_PORT}...")
    
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((SERVER_IP, SERVER_PORT))
            print("Connected! Sending high-frequency DOD traffic...")

            count = 0
            while True:
                # Generate random game data
                p_id = random.randint(1, 500)
                pos_x = random.uniform(-1000, 1000)
                pos_y = random.uniform(0, 50)
                pos_z = random.uniform(-1000, 1000)

                send_position_packet(s, p_id, pos_x, pos_y, pos_z)
                
                count += 1
                if count % 1000 == 0:
                    print(f"Sent {count} packets...")
                
                # Control the flow to test different load levels
                # Remove sleep for absolute max stress test
                time.sleep(0.001) 

    except ConnectionRefusedError:
        print("Error: Could not find the C++ Server. Is it running?")
    except KeyboardInterrupt:
        print("\nStopping stress test.")

if __name__ == "__main__":
    run_stress_test()
