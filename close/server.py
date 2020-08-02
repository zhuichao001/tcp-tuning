import socket
import threading
import time

def handle_conn(cli_socket):
    time.sleep(3)
    cli_socket.send("hi.".encode())
    time.sleep(20)
    cli_socket.close()


def run():
    server = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    server.bind(("127.0.0.1", 9996))
    print "[*] Listening on 127.0.0.1:9996"
    
    server.listen(1024)
    while True:
        client, addr = server.accept()
        print "[*] Acception connection from %s:%d" % (addr[0], addr[1]) 
        th = threading.Thread(target=handle_conn, args=(client,))
        th.start()


if __name__ == '__main__':
    run()
