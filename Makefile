run: shapeapp
	./shapeapp

shapeapp: main.cpp
	g++ -g -lsfml-graphics -lsfml-window -lsfml-system main.cpp -o shapeapp
