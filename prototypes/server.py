import socket
import struct
from enum import IntEnum

class Opcode(IntEnum):
    POSITION = 101
    COMMAND  = 102

def start_server():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(('localhost', 9999))
    server.listen(5)
    print("Server Engine Monitor: Listening for binary packets...")

    while True:
        conn, addr = server.accept()
        try:
            while True:
                # 1. Performance Step: Only read the Header (6 bytes: H + I)
                # H = Opcode (2 bytes), I = Length (4 bytes)
                header_data = conn.recv(6)
                if not header_data: break

                opcode, length = struct.unpack("<HI", header_data)

                # 2. Performance Step: Direct Integer Comparison (Binary Dispatch)
                if opcode == Opcode.POSITION:
                    # We know Position is exactly 8 bytes (two floats)
                    payload = conn.recv(length)
                    x, y = struct.unpack("<ff", payload)
                    print(f"[ROUTING] ID {opcode}: POS -> X:{x:.2f}, Y:{y:.2f}")

                elif opcode == Opcode.COMMAND:
                    # Commands are strings; we read exactly 'length' bytes
                    payload = conn.recv(length)
                    cmd = payload.decode('utf-8')
                    print(f"[ROUTING] ID {opcode}: CMD -> Executing: '{cmd}'")
                
                else:
                    print(f"[WARNING] Unknown Opcode {opcode}. Dropping packet.")
                    conn.recv(length) # Clear the buffer to prevent bottleneck

        except Exception as e:
            print(f"Connection error: {e}")
        finally:
            conn.close()

if __name__ == "__main__":
    start_server()
