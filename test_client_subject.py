import socket

SERVER_IP = "127.0.0.1"
SERVER_PORT = 8888

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

print(f"UDP Client Active. Sending to {SERVER_IP}:{SERVER_PORT}")
print("Type your message and press Enter. Type 'quit' to stop.")

try:
    while True:
        # Get input from keyboard
        message = input("\nSend Message: ")

        if message.lower() == 'quit':
            print("Closing client...")
            break

        # Send to C++ server
        sock.sendto(message.encode(), (SERVER_IP, SERVER_PORT))
        
except KeyboardInterrupt:
    print("\nClient closed by user.")
finally:
    sock.close()
