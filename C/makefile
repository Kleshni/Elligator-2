TARGET = libelligator-2-curve25519.a
OBJS = main.o

CFLAGS += -std=c99

ifdef DEBUG
	CFLAGS += -g3 -Wpedantic -Wall -Wextra
endif

$(TARGET): $(OBJS)
	$(AR) scru $@ $^

clean:
	rm -f $(TARGET) $(OBJS)

main.o: elligator-2-curve25519.h
