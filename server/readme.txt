The client connects to the server and runs following command

Commands :

showHandles : The command request the server to send status of all handles in the server.

showHandleStatus : The command send a handle ID and waits for the status of the handle.

Files :

server.c                       - The file contains the main function and takes the help of the server_helper and ping file to run various commands.
server_helper.c                - This file has helper function to implement threads for each client and function to print and send handle details to client.
ping.c                         - This file has implementation of queue and thread function to perform pinging function

Example : ./server

Output :

		showHandles :

		Usage
		Handle Status      -  showHandleStatus [Handle Id]
		All Handles        -  showHandles
		Exit               -  exit
		Help               -  help
		Connection from 127.0.0.1 accepted
		showHandles
		0 facebook.com 12.600000 12.000000 13.000000 COMPLETED
		0 google.com 13.400000 12.000000 16.000000 COMPLETED

		showHandleStatus :

		showHandleStatus 0
		0 facebook.com 12.600000 12.000000 13.000000 COMPLETED
		0 google.com 13.400000 12.000000 16.000000 COMPLETED
