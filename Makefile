CC= gcc
CFLAGS = -Wall -linput -fPIC -ludev
TARGET = main

all: $(TARGET)

$(TARGET): src/$(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) src/$(TARGET).c

clean:
	$(RM) $(TARGET)
