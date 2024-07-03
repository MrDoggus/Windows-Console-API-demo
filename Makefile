

CC = x86_64-w64-mingw32-gcc
CFLAGS = -Wall -Wshadow -Werror

SRCS = src/dvd.c src/console.c 
OBJS = $(SRCS:%.c=%.o)

%.o: %.c $(SRCS)
	$(CC) $(CFLAGS) -c $< -o $@

default: dvd



dvd: $(OBJS)
	$(CC) $(OBJS) -Wall -o $@

clean: 
	rm src/*.o