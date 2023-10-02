import socket
import os
from _thread import *
import time
import serial

ServerSocket = socket.socket()
host = '192.168.0.8'
port = 5000 

reply = "NOT DEFINED"

try:
    ServerSocket.bind((host, port))
except socket.error as e:
    print(str(e))

print('Waiting for a Connection...')
ServerSocket.listen(5)

def threaded_client(connection):
    global reply
    while True:
        # Lee la distancia desde Arduino a través del puerto serial
        
        data = connection.recv(2048).decode('utf-8')
        #distance = int(data)   
    
        if data:
                distance = int(data)
                print(distance)
        else:
             # Aquí puedes manejar el caso de cadena vacía o no válida, por ejemplo, asignando un valor predeterminado a 'distance'.
             distance = -1  # O cualquier otro valor predeterminado que desees

        # Actualiza la variable 'reply' según la distancia
        if distance < 0:
            reply = "NOT DEFINED"
        elif distance < 30:
            reply = "BLUE"
        elif 30 <= distance < 60:
            reply = "ORANGE"
        elif 60 <= distance < 120:
            reply = "RED"

        # Envía 'reply' al cliente
        try:
            connection.sendall(reply.encode('utf-8'))
        except ConnectionAbortedError as e:
            print("Error al enviar respuesta al cliente:", e)

        time.sleep(1)

    connection.close()

while True:
    Client, address = ServerSocket.accept()
    print('Connection from: ' + address[0] + ':' + str(address[1]))
    start_new_thread(threaded_client, (Client, ))

ServerSocket.close()