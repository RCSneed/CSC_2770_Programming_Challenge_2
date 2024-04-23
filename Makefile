CC = gcc
CFLAGS = -Wall -pthread
SRCS = HTTP_Client.c HTTP_Server.c
OBJS = $(SRCS:.c=.o)
TARGET1 = HTTP_Client
TARGET2 = HTTP_Server

all: $(TARGET1) $(TARGET2)

$(TARGET1): HTTP_Client.o
	$(CC) $(CFLAGS) HTTP_Client.o -o $(TARGET1)

$(TARGET2): HTTP_Server.o
	$(CC) $(CFLAGS) HTTP_Server.o -o $(TARGET2)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET1) $(TARGET2)

