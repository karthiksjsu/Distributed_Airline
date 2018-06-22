all: server

server: Server.c error.c errexit.c admin_flights.c admin_Airliner.c agent_airliner.c Customer_main.c Customer_sub.c
	gcc -pthread Server.c error.c errexit.c  admin_flights.c admin_Airliner.c agent_airliner.c Customer_main.c Customer_sub.c -l sqlite3 -o server

clean:
	rm -rf .*o server
