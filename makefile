OBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADER = $(wildcard *.h)
TARGET = lsh
CC = g++
CFLAGS = -O3 -g -Wall 


%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
		$(CC) -g $(OBJS) -o $@


clean:
	-rm -f *.o
	-rm -f $(TARGET)
