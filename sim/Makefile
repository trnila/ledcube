LDFLAGS=-lglut -lGLEW -lGL -lm
CXXFLAGS=-g -I../common
NAME=sim

all: $(NAME)

$(NAME): main.o errors.o mesh.o program.o mapped_file.o viewport.o led.o
	$(CXX) $^ -o $@ $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f *.o $(NAME)
