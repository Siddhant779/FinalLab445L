import socket
import threading

SERVER_HOST = "172.20.10.10"
SERVER_PORT = 8080

class Client():
    def __init__(self):
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            client_socket.connect((SERVER_HOST, SERVER_PORT))
            print(f"[+] Connected to the server at {SERVER_HOST}:{SERVER_PORT}")
            self.socket = client_socket
        except Exception as e:
            print(f"[-] Error while connecting to Client: {e}")
            client_socket.close()
        
    def start(self):
        input_thread = threading.Thread(target=self.input_func)
        output_thread = threading.Thread(target=self.output_func)
        input_thread.start()
        output_thread.start()

    
    def input_func(self):
        while True:
            try:
                recved = self.socket.recv(1024).decode()
                print(f"\n[+] RECV: {recved}")
            except Exception as e:
                print(f"[-] Error when receiving a message: {e}")
                self.socket.close()
                break                
    
    def output_func(self):
        while True:
            try:
                msg = input("Enter a message: ")
                self.socket.sendall(msg.encode())
            except Exception as e:
                print(f"[-] Error when sending a message: {e}")
                self.socket.close()
                break
    

if __name__ == "__main__":
    c = Client()
    c.start()
    
####### Test Code #######
# def main():
#     # Create the TCP connection
#     client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
#     try:
#         client_socket.connect((SERVER_HOST, SERVER_PORT))
#         print(f"[+] Connected to the server at {SERVER_HOST}:{SERVER_PORT}")
        
#         msg = "Hello world!"
#         client_socket.sendall(msg.encode())
        
#         print(f"[+] SEND: {msg}")
        
#         response = client_socket.recv(1024).decode()
        
#         print(f"[+] RECV: {response}")
#     except Exception as e:
#         print(f"[-] Error: {e}")
#     finally:
#         print(f"[-] Disconnecting from server")
#         client_socket.close()