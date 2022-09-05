# Eron Ristich
# 8/26/2022

OBJS = GG1-C38-handler.o handler.o kernel.o main.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
LDLIBS = -Llib -lmingw32 -lopengl32 -lSDL2_ttf -lglew32 -lglu32 -lfreeglut -lSDL2main -lSDL2 -lSDL2_image -lglew32mx -lassimp.dll
INC = -Iinclude

main.exe : $(OBJS)
	$(CC) $(LFLAGS) $(INC) $(OBJS) -o main.exe $(LDLIBS)

kernel.o : util/kernel/kernel.h util/kernel/kernel.cpp
	$(CC) $(CFLAGS) $(INC) util/kernel/kernel.cpp

handler.o : util/kernel/kernel.h util/handler.h util/handler.cpp
	$(CC) $(CFLAGS) $(INC) util/handler.cpp

GG1-C38-handler.o : objects/helper.h util/handler.h util/glslInclude.h util/kernel/kernel.h GG1-C38-handler.h GG1-C38-handler.cpp
	$(CC) $(CFLAGS) $(INC) GG1-C38-handler.cpp

main.o : objects/helper.h util/kernel/kernel.h util/handler.h GG1-C38-handler.h main.cpp
	$(CC) $(CFLAGS) $(INC) main.cpp

clean:
	\rm *.o *~ main.exe
