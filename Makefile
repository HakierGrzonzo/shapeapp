O_FILES := $(shell find . -name '*.cpp' | sed 's/cpp/o/')

run: shapeapp
	./shapeapp

shapeapp: $(O_FILES)
	g++ -O2 -g -lsfml-graphics -lsfml-window -lsfml-system main.o drawer.o -o shapeapp

%.o: %.cpp
	g++ -c $<


