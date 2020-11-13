FILES = \
	./src/main.cpp \

FLAGS = \
	-I /usr/include/libusb-1.0/ -lusb-1.0 \
	-fPIC \
	-c

all:
	g++ ${FILES} ${FLAGS} -o ./bin/ps4fix.so