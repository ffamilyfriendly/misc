ifndef FILE
    $(error Specify FILE=[colours|other])
endif

FILES = \
	./${FILE}.cpp \
	../bin/ps4fix.so

FLAGS = \
	-I /usr/include/libusb-1.0/ -lusb-1.0
all:
	g++ ${FILES} ${FLAGS} -o ./bin/${FILE}