import socket
import threading

HOST = "127.0.0.1"
PORT = 8080

def client_handler(socket):
    while True:
        try:
            data = socket.recv(1024)
            
            if not data:
                break
            
            print(f"[!] Received {data.decode()} from client")
            
            # Echo for now
            socket.send(data)
        except Exception as e:
            print(f"Error handling client: {e}")
            break
    socket.close()

def main():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((HOST, PORT))
    server_socket.listen(2)                                                 # Only allow 2 connections
    print(f"[*] Listening on {HOST}:{PORT}")
    
    try:
        while True:
            client_socket, client_addr = server_socket.accept()             # Blocking until client connects
            print(f"[+] Accepted connection from {client_addr[0]}:{client_addr[1]}")
            
            client_thread = threading.Thread(target=client_handler, args=(client_socket,))
            client_thread.start()
    except KeyboardInterrupt:
        print(f"[*] Closing server...")
    finally:
        server_socket.close()

if __name__ == "__main__":
    main()