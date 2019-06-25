
CXX	=	g++
CFLAGS	=	-O0 -fpermissive -DDEBUG -g -Wall -c -fmessage-length=0 -MMD 
LDFLAGS	=	
OBJS 	= 	register.o generator.o main.o
TARGET	=	red-hare

all: $(TARGET)
	@echo "Starting Building: $^"
	@echo "$(CXX) $(CFLAGS)"	

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o : %.cpp
	$(CXX) $(CFLAGS)  -o $@ $<		


.PHONY: clean

clean:
	rm -rf *.o $(TARGET) 
