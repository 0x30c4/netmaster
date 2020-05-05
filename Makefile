CC=gcc

tcps: server.c 
	$(CC) -o tcps server.c

# tcps: .c 
# 	$(CC) -o tcps server.c