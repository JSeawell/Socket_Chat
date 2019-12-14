#!/usr/bin/env python3

# Name: Jake Seawell
# Date: 11/02/19
# Assignment: Project1 - Socket-Chat
# Decription: This program, called socket.py,
# connects to client.c on a certain port, and
# functions as a back-and-forth chat over
# sockets. See README.txt for compile/run
# instructions.

# Resources:
# https://realpython.com/python-sockets/

import sys
import socket

#HOST = '127.0.0.1' # Standard loopback interface address (localhost)
PORT = int(sys.argv[1]) # Port to listen on (non-privileged ports are > 1023)

#create socket
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind(("", PORT)) #bind socket to port number
    # Continue until sig-int (CTRL+C)
    while True:
        s.listen() #enable server to accept connections
        print('Waiting for client connection...')
        conn, addr = s.accept() #accept returns new connection socket
        #on connection socket
        clientQuit = 0
        serverQuit = 0
        with conn:
            #Receive username from client
            name = conn.recv(1024)
            print('Server connected to:', name.decode('utf-8'))
            #Until client or server quits
            while clientQuit == 0 and serverQuit == 0:
                #get client message
                data = conn.recv(1024) 
                #If client quits
                if data.decode('utf-8') == '\quit':
                    clientQuit = 1
                    print('Client ended connection.')
                #If client doesn't quit
                else:
                    #Print message to screen
                    print(name.decode('utf-8'), '>', data.decode('utf-8'))
                    del data
                    #Get reply message
                    reply = input('Server > ')
                    #If server quits
                    if reply == '\quit':
                        serverQuit = 1
                        print('Server ended connection.')
                    #Send reply to client
                    conn.send(reply.encode())
                    del reply

# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4

