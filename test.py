import socket
import threading
from time import sleep

ya_socket = socket.socket()
addr = ("77.88.55.242", 443)
ya_socket.connect(addr)

data_out = b"GET / HTTP/1.1\r\nHost:ya.ru\r\n\r\n"
ya_socket.send(data_out)

data_in = b""

def recieving():
    global data_in
    while True:
        data_chunk = ya_socket.recv(1024)
        data_in = data_in + data_chunk

rec_thread = threading.Thread(target=recieving)
rec_thread.start()

sleep(4)
print(data_in)

ya_socket.close()