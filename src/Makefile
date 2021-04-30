##
# Rgbtterm
#
# @file
# @version 0.1

CC = clang
RM = rm -f
DFLAG = -g
GFLAGS = -Wall -Wextra
CFLAGS = $(GFLAGS) $(DFLAG) -c
LFLAGS = $(GFLAGS) -lm

all: rgbtterm

rgbtterm.o: rgbtterm.c rgbtterm.h errcodes.h termv.h
	$(CC) $(CFLAGS) rgbtterm.c

termv.o: termv.c termv.h errcodes.h
	$(CC) $(CFLAGS) termv.c

rgbtterm: rgbtterm.o termv.o
	$(CC) $(LFLAGS) rgbtterm.o termv.o -o rgbtterm

clean:
	$(RM) *.o rgbtterm

# end
