import socket

SERVER_HOST = "127.0.0.1"
SERVER_PORT = 8080

def main():
    # Create the TCP connection
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    try:
        client_socket.connect((SERVER_HOST, SERVER_PORT))
        print(f"[+] Connected to the server at {SERVER_HOST}:{SERVER_PORT}")
        
        msg = "Hello world!"
        client_socket.sendall(msg.encode())
        
        print(f"[+] SEND: {msg}")
        
        response = client_socket.recv(1024).decode()
        
        print(f"[+] RECV: {response}")
    except Exception as e:
        print(f"[-] Error: {e}")
    finally:
        print(f"[-] Disconnecting from server")
        client_socket.close()

if __name__ == "__main__":
    main()