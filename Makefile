compile_output: main.o
	g++ main.o -o compile_output
main.o: main.cpp lexical1.h syntaxanalyzer_v3.h
	g++ -c main.cpp
clean:
	rm *.o compile_output
