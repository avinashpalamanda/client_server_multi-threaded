The client connects to the server and runs following command

Commands :

pingSites : The command sends the list of sites to the server and server returns back with handle ID..

showHandles : The command request the server to send status of all handles in the server.

showHandleStatus : The command send a handle ID and waits for the status of the handle.

Files :

client.c                       - The file contains the main function and takes the help of the client_helper file to run various commands.
client_helper.c                - This file has helper function to implement pingSites.

Example : ./client 127.0.0.1

Output :

		pingSites :

		Usage
		Pinging Site       -  pingSites
		Handle Status      -  showHandleStatus
		All Handles        -  showHandles
		Exit               -  exit
		Help               -  help

		Enter command : pingSites
		Enter the sites list : google.com,facebook.com,yahoo.com
		Handle Number 0

		showHandles :

		Enter command : showHandles

  	 	 0 facebook.com 12.500000 11.000000 15.000000 COMPLETED

		 0 google.com 12.300000 11.000000 14.000000 COMPLETED

		 0 yahoo.com 12.800000 12.000000 15.000000 COMPLETED

		showHandleStatus :

		Enter command : showHandleStatus
		Enter Handle ID : 0

 		0 facebook.com 12.500000 11.000000 15.000000 COMPLETED

 		0 google.com 12.300000 11.000000 14.000000 COMPLETED

 		0 yahoo.com 12.800000 12.000000 15.000000 COMPLETED
