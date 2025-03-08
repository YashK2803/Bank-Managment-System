CPP = g++
RM = rm -f

# Executables
BANK=Bank

# Object files
BANK_OBJ=main.o

all: $(BANK) 

# Target to build the first executable
$(BANK): $(BANK_OBJ)
	$(CPP) $(BANK_OBJ) -o $(BANK)
	@echo " "
	@echo "Compilation done successfully......"
	@echo " "
	@echo "Name of executable : Bank"
	@echo " "




# Compile the source files into object files
$(BANK_OBJ): main.cpp
	$(CPP) $(FLAGS) -c main.cpp
	
	
# Clean up
clean:
	$(RM) $(BANK) $(BANK_OBJ)

