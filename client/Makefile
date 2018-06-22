all: client

client: TCP_Client.c ConnectTCP.c errexit.c
	gcc TCP_Client.c ConnectTCP.c errexit.c -o client

clean: 
	rm -rf .*o client
