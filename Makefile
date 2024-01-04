O_FILES := $(shell find . -name '*.cpp' | sed 's/cpp/o/')

run: shapeapp
	./shapeapp

shapeapp: $(O_FILES)
	g++ -O3 -ggdb -lsfml-graphics -lsfml-window -lsfml-system $(O_FILES) -o shapeapp

%.o: %.cpp
	g++ -O3 -ggdb  -c $<


