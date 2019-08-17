CXX=g++-5
CXXFLAGS=-std=c++14 -Wall -Werror=vla -MMD
EXEC=quadris
COMMAND=command/command.o
BLOCK=block/block.o
BOARD=board/board.o board/subject.o
DISPLAY=display/graphicsdisplay.o display/window.o display/textdisplay.o
LEVEL=level/level.o level/level0.o level/level1.o level/level2.o level/level3.o level/level4.o level/levelfactory.o
OBJECTS=main.o ${COMMAND} ${BLOCK} ${BOARD} ${DISPLAY} ${LEVEL}
DEPENDS=${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lX11

-include ${DEPENDS}

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
.PHONY: clean
