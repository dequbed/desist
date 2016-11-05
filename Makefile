CC = clang

CFLAGS = -Wall -g

LFLAGS = -L/usr/lib/ipsec/

LIBS = -lvici

SRCS = src/opennhrp-script.c src/util.c src/conn.c

# Suffix replace syntax: $(name:string1=string2)
OBJS = $(SRCS:.c=.o)

BIN = opennhrp-script

.PHONY: clean

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS) $(LFLAGS) $(LIBS)

# Suffix replacement rule for building objects
# $< and $@ are automatic variables
# aka: GNU make magic
.c.o:
	$(CC) $(CFLAGS) -c $<  -o $@

clean:
	$(RM) src/*.o *~ $(BIN)
