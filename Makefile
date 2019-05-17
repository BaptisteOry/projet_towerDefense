CC     = gcc
CFLAGS = -Wall -O2 -Wno-unused-result -g
LDFLAGS	= -lSDL -lSDL_image -lGLU -lGL -lm

BINDIR	= bin/
SRCDIR	= src/
INCLUDEDIR	= include/
OBJDIR	= obj/
BIN    = $(BINDIR)towerdef.itd
OBJ    = $(OBJDIR)main.o $(OBJDIR)imageMap.o $(OBJDIR)controller.o $(OBJDIR)map.o $(OBJDIR)tower.o

RM     = rm -f
DIRNAME = $(shell basename $$PWD)
BACKUP  = $(shell date +`basename $$PWD`-%m.%d.%H.%M.zip)
STDNAME = $(DIRNAME).zip

all : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(BIN)
	@echo "--------------------------------------------------------------"
	@echo "            to execute type: $(BIN) &"
	@echo "--------------------------------------------------------------"

$(OBJDIR)main.o : $(SRCDIR)main.c $(SRCDIR)imageMap.c $(INCLUDEDIR)imageMap.h $(SRCDIR)controller.c $(INCLUDEDIR)controller.h $(SRCDIR)map.c $(INCLUDEDIR)map.h $(SRCDIR)tower.c $(INCLUDEDIR)tower.h
	@echo "compile main"
	mkdir -p `dirname $@`
	$(CC) -o $@ -c $< $(CFLAGS)
	@echo "done..."

$(OBJDIR)imageMap.o : $(SRCDIR)imageMap.c $(INCLUDEDIR)imageMap.h
	@echo "compile imageMap"
	mkdir -p `dirname $@`
	$(CC) -o $@ -c $< $(CFLAGS)
	@echo "done..."

$(OBJDIR)controller.o : $(SRCDIR)controller.c $(INCLUDEDIR)controller.h
	@echo "compile controller"
	mkdir -p `dirname $@`
	$(CC) -o $@ -c $< $(CFLAGS)
	@echo "done..."

$(OBJDIR)map.o : $(SRCDIR)map.c $(INCLUDEDIR)map.h
	@echo "compile map"
	mkdir -p `dirname $@`
	$(CC) -o $@ -c $< $(CFLAGS)
	@echo "done..."

$(OBJDIR)tower.o : $(SRCDIR)tower.c $(INCLUDEDIR)tower.h
	@echo "compile tower"
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
