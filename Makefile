source = main.cpp memory.cpp cache.cpp cachecontroller.cpp databus.cpp

main: $(source)
	g++ $(source) -o main
