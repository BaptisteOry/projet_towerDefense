CC     = gcc
CFLAGS = -Wall -O2 -Wno-unused-result -g
LDFLAGS	= -lSDL -lSDL_image -lGLU -lGL -lm -lglut

BINDIR	= bin/
SRCDIR	= src/
INCLUDEDIR	= include/
OBJDIR	= obj/
BIN    = $(BINDIR)towerdef.itd
OBJ    = $(OBJDIR)main.o $(OBJDIR)game.o $(OBJDIR)interface.o $(OBJDIR)monster.o $(OBJDIR)building.o $(OBJDIR)tower.o $(OBJDIR)construction.o $(OBJDIR)imageMap.o $(OBJDIR)map.o $(OBJDIR)display.o $(OBJDIR)operations.o

RM     = rm -f
DIRNAME = $(shell basename $$PWD)
BACKUP  = $(shell date +`basename $$PWD`-%m.%d.%H.%M.zip)
STDNAME = $(DIRNAME).zip

all : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(BIN)
	@echo "--------------------------------------------------------------"
	@echo "            to execute type: $(BIN) &"
	@echo "--------------------------------------------------------------"

$(OBJDIR)main.o : $(SRCDIR)main.c $(SRCDIR)display.c $(INCLUDEDIR)display.h $(SRCDIR)operations.c $(INCLUDEDIR)operations.h $(SRCDIR)monster.c $(INCLUDEDIR)monster.h $(SRCDIR)building.c $(INCLUDEDIR)building.h $(SRCDIR)tower.c $(INCLUDEDIR)tower.h $(SRCDIR)construction.c $(INCLUDEDIR)construction.h $(SRCDIR)game.c $(INCLUDEDIR)game.h $(SRCDIR)interface.c $(INCLUDEDIR)interface.h $(SRCDIR)imageMap.c $(INCLUDEDIR)imageMap.h $(SRCDIR)map.c $(INCLUDEDIR)map.h
	@echo "compile main"
	mkdir -p `dirname $@`
	$(CC) -o $@ -c $< $(CFLAGS)
	@echo "done..."

$(OBJDIR)game.o : $(SRCDIR)game.c $(INCLUDEDIR)game.h $(SRCDIR)display.c $(INCLUDEDIR)display.h $(SRCDIR)operations.c $(INCLUDEDIR)operations.h $(SRCDIR)monster.c $(INCLUDEDIR)monster.h $(SRCDIR)tower.c $(INCLUDEDIR)tower.h
	@echo "compile building"
	mkdir -p `dirname $@`
	$(CC) -o $@ -c $< $(CFLAGS)
	@echo "done..."

$(OBJDIR)interface.o : $(SRCDIR)interface.c $(INCLUDEDIR)interface.h $(SRCDIR)display.c $(INCLUDEDIR)display.h $(SRCDIR)operations.c $(INCLUDEDIR)operations.h $(SRCDIR)monster.c $(INCLUDEDIR)monster.h $(SRCDIR)tower.c $(INCLUDEDIR)tower.h $(SRCDIR)game.c $(INCLUDEDIR)game.h
	@echo "compile interface"
	mkdir -p `dirname $@`
	$(CC) -o $@ -c $< $(CFLAGS)
	@echo "done..."

$(OBJDIR)monster.o : $(SRCDIR)monster.c $(INCLUDEDIR)monster.h $(SRCDIR)display.c $(INCLUDEDIR)display.h $(SRCDIR)operations.c $(INCLUDEDIR)operations.h
	@echo "compile monster"
	mkdir -p `dirname $@`
	$(CC) -o $@ -c $< $(CFLAGS)
	@echo "done..."	

$(OBJDIR)building.o : $(SRCDIR)building.c $(INCLUDEDIR)building.h $(SRCDIR)display.c $(INCLUDEDIR)display.h $(SRCDIR)operations.c $(INCLUDEDIR)operations.h
	@echo "compile building"
	mkdir -p `dirname $@`
	$(CC) -o $@ -c $< $(CFLAGS)
	@echo "done..."	

$(OBJDIR)tower.o : $(SRCDIR)tower.c $(INCLUDEDIR)tower.h $(SRCDIR)display.c $(INCLUDEDIR)display.h $(SRCDIR)operations.c $(INCLUDEDIR)operations.h
	@echo "compile tower"
	mkdir -p `dirname $@`
	$(CC) -o $@ -c $< $(CFLAGS)
	@echo "done..."	

$(OBJDIR)construction.o : $(SRCDIR)construction.c $(INCLUDEDIR)construction.h $(SRCDIR)display.c $(INCLUDEDIR)display.h $(SRCDIR)operations.c $(INCLUDEDIR)operations.h $(SRCDIR)building.c $(INCLUDEDIR)building.h $(SRCDIR)tower.c $(INCLUDEDIR)tower.h
	@echo "compile building"
	mkdir -p `dirname $@`
	$(CC) -o $@ -c $< $(CFLAGS)
	@echo "done..."

$(OBJDIR)imageMap.o : $(SRCDIR)imageMap.c $(INCLUDEDIR)imageMap.h $(SRCDIR)display.c $(INCLUDEDIR)display.h
	@echo "compile imageMap"
	mkdir -p `dirname $@`
	$(CC) -o $@ -c $< $(CFLAGS)
	@echo "done..."

$(OBJDIR)map.o : $(SRCDIR)map.c $(INCLUDEDIR)map.h $(SRCDIR)imageMap.c $(INCLUDEDIR)imageMap.h $(SRCDIR)operations.c $(INCLUDEDIR)operations.h
	@echo "compile map"
	mkdir -p `dirname $@`
	$(CC) -o $@ -c $< $(CFLAGS)
	@echo "done..."

$(OBJDIR)display.o : $(SRCDIR)display.c $(INCLUDEDIR)display.h
	@echo "compile display"
	mkdir -p `dirname $@`
	$(CC) -o $@ -c $< $(CFLAGS)
	@echo "done..."	

$(OBJDIR)operations.o : $(SRCDIR)operations.c $(INCLUDEDIR)operations.h $(SRCDIR)map.c $(INCLUDEDIR)map.h
	@echo "compile operations"
	mkdir -p `dirname $@`
	$(CC) -o $@ -c $< $(CFLAGS)
	@echo "done..."	

clean :	
	@echo "**************************"
	@echo "CLEAN"
	@echo "**************************"
	$(RM) *~ $(OBJ) $(BIN)    

bigclean :
	@echo "**************************"
	@echo "BIG CLEAN"
	@echo "**************************"
	find . -name '*~' -exec rm -fv {} \;
	$(RM) *~ $(OBJ) $(BIN)

zip : clean 
	@echo "**************************"
	@echo "ZIP"
	@echo "**************************"
	cd .. && zip -r $(BACKUP) $(DIRNAME)
