import socket
import threading

HOST = "172.20.10.2"
PORT = 8080

client_dict = {}

class ClientHandler():
    def __init__(self, socket, num, addr, port):
        self.socket = socket
        self.num = num
        self.addr = addr
        self.port = port
    
    def handle_input(self):
        while True:
            try:
                data = self.socket.recv(1024).decode()
                if not data:
                    break
                
                print(f"[+] Received data from client {self.num}: {data}")
                
                self.send_msg(data)
            except Exception as e:
                print(f"[!] Error occurred while handling input from client {self.num}: {e}")
                break
        self.socket.close()
        del client_dict[self.num]
        
    
    def send_msg(self, msg):
        # XOR operation
        if len(client_dict) > 1:
            for k in client_dict.keys():
                if k != self.num:
                    client_socket = client_dict[k].get_socket()
                    client_socket.send(msg.encode())    
        else:
            print(f"[!] Sending message failed, only one client in client_dict")
        
    def get_socket(self):
        return self.socket
    
    def get_num(self):
        return self.num
    
    def get_info(self):
        return f"{self.addr}:{self.port}"

def main():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((HOST, PORT))
    server_socket.listen(2)                                                 # Only allow 2 connections
    print(f"[*] Listening on {HOST}:{PORT}")
    unique_num = 0
    
    try:
        while True:
            client_socket, client_addr = server_socket.accept()             # Blocking until client connects
            print(f"[+] Accepted connection from {client_addr[0]}:{client_addr[1]}")
            # welcome_msg = "Connection the server established!"
            # client_socket.sendall(welcome_msg.encode())
            handler = ClientHandler(client_socket, unique_num, client_addr[0], client_addr[1])
            client_dict[unique_num] = handler
            unique_num += 1
            
            client_thread = threading.Thread(target=handler.handle_input)
            client_thread.start()
    except KeyboardInterrupt:
        print(f"[*] Closing server...")
    finally:
        server_socket.close()

if __name__ == "__main__":
    main()