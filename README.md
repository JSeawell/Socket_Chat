# Socket_Chat  
> CS-372 (Intro to Networks) Program  
  
A server/client chat service over sockets  

## Description: 
> This project functions using two  
> programs: server.py (written in python), and  
> client.c (written in C). The process is shown  
> below:  
> 1. Server starts listening on a given port  
> 2. Client connects to given host on given port  
> 3. Connection is made  
> 4. Client is prompted for a username  
> 5. Client sends first message to server  
> 6. Server replies  
> 7. Steps 5-6 are repeated until client or server quits  


## Files Needed:
> 1. server.py  
> 2. client.c  

## How to compile client.c into executable:
> g++ client.c -o client

## How to Run Program:
> 1. Start Server by typing: 'python3 server.py [PORTNUM]'  
> 2. Start Client by typing: 'client [HOSTNAME] [PORTNUM]'  
   *HOSTNAME should be the name of the server*	*PORTNUM should be same number*  
> 3. Client enters username  
> 4. Client sends message  
> 5. Server send message  
> 6. Continue as long as you want  
> 7. Quit at any time (in server or client)  
   by typing: '\quit' as your message  
   
## Tech and/or concepts learned/used:
> - C
> - Python
> - Socket Programming
> - Network Programming
> - Client/Server Communication
