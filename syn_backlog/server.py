import socket
import threading
import time

def handle_client(client_socket):
    request = client_socket.recv(1024)
    print("[*] Received: %s" % request)
    client_socket.send("hi~".encode())
    client_socket.close()

server = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
server.bind(("0.0.0.0", 8723))
print "[*] Listening on :8723"
server.listen(1024) #backlog=8
time.sleep(120)
while True:
    client, addr = server.accept()
    print "[*] Acception connection from %s:%d" % (addr[0], addr[1]) 
    th = threading.Thread(target=handle_client, args=(client,))
    th.start()
