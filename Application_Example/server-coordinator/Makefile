
CC = gcc
PWD = `pwd`
SRC_DIR = $(PWD)/source
INC = $(PWD)/include
BIN_DIR = $(PWD)/bin
LD_FLAG = -lpthread
EXE = server 

all: clean binary 

binary:
	
	$(CC) $(SRC_DIR)/*.c -I$(INC) $(LD_FLAG) -o $(EXE) 
	mv $(EXE) $(BIN_DIR)	
	@echo "\033[0;31m"
	@echo "========================================================="
	@echo "                 Compilation Finished                    "
	@echo "========================================================="
	@echo "\033[0m"

clean :
	
	rm -rf $(BIN_DIR)/$(EXE)
	rm -rf $(SRC_DIR)/*.o 
	rm -rf $(SRC_DIR)/*~ 
	rm -rf $(PWD)/*~
	
	@echo "\033[0;31m"
	@echo "========================================================="
	@echo "                 Cleaning Finished                       "
	@echo "========================================================="
	@echo "\033[0m"


