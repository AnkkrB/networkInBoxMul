PROGRAM = buffering
# comment out the following definition of CC to regain the default cc compiler
CC = g++
CFLAGS = -g -std=gnu++0x


${PROGRAM}:	buffering.o NALParser.o
		${CC} ${CFLAGS} -o ${PROGRAM} buffering.o NALParser.o
		
buffering.o: buffering.cpp
	${CC} ${CFLAGS} -c buffering.cpp

NALParser.o: classes/NALParser.cpp
	${CC} ${CFLAGS}  -c classes/NALParser.cpp
clean:
		rm -f *.o ${PROGRAM}
		
