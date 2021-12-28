makemake: config.o main.o generator.o target.o utils.o 
	g++ -o $@ $^ -lstdc++fs
config.o: config.cpp config.h
	g++ -c config.cpp -std=c++17
main.o: main.cpp utils.h target.h
	g++ -c main.cpp -std=c++17
generator.o: generator.cpp generator.h utils.h
	g++ -c generator.cpp -std=c++17
target.o: target.cpp target.h utils.h
	g++ -c target.cpp -std=c++17
utils.o: utils.cpp utils.h
	g++ -c utils.cpp -std=c++17

