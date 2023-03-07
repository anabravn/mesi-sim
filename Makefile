main: src/* 
	g++ src/* -o main -I include/

clean: main
	rm main
