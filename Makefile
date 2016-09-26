# Declaration of variables
CC = g++
DEBUG_FLAG = -g
OPTI_FLAGS = #-O3
WARN_FLAGS = 
CC_FLAGS = -I/usr/include/libxml2 -I/usr/include/poppler/cpp/ $(DEBUG_FLAG) $(OPTI_FLAGS) $(WARN_FLAGS)

# File names
EXEC = reorg.out
SOURCES = $(wildcard *.cc)
OBJECTS = $(SOURCES:.cc=.o)
HEADERS = $(wildcard *.h)
LIBFLAGS = -lpoppler-cpp -lpoppler -llcms2 -lcurl -lxml2 -lz -llzma -lm -ldl

all: $(EXEC)

# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(CC_FLAGS) $(OBJECTS) -o $(EXEC) $(LIBFLAGS)

# To obtain object files
%.o: %.cc
	$(CC) -c $(CC_FLAGS) $< -o $@ 

# To remove generated files
clean:
	rm -f $(EXEC) *.o
