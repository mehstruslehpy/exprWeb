all: main translator

main: prooftree.o main.cpp context.o boolexp.o varexp.o notexp.o andexp.o orexp.o condexp.o
	g++ -std=c++11 -Wall -pedantic main.cpp varexp.o context.o boolexp.o notexp.o andexp.o orexp.o condexp.o prooftree.o -o main

context.o: context.h context.cpp                                                                                                                                                                             
	g++ -std=c++11 -Wall -pedantic context.cpp -o context.o -c
boolexp.o: boolexp.h boolexp.cpp
	g++ -std=c++11 -Wall -pedantic boolexp.cpp -o boolexp.o -c
varexp.o: varexp.h varexp.cpp
	g++ -std=c++11 -Wall -pedantic varexp.cpp -o varexp.o -c
notexp.o: notexp.h notexp.cpp
	g++ -std=c++11 -Wall -pedantic notexp.cpp -o notexp.o -c
andexp.o: andexp.h andexp.cpp
	g++ -std=c++11 -Wall -pedantic andexp.cpp -o andexp.o -c
orexp.o: orexp.h orexp.cpp
	g++ -std=c++11 -Wall -pedantic orexp.cpp -o orexp.o -c
condexp.o: condexp.h condexp.cpp
	g++ -std=c++11 -Wall -pedantic condexp.cpp -o condexp.o -c
prooftree.o: prooftree.h prooftree.cpp
	g++ -std=c++11 -Wall -pedantic prooftree.cpp -o prooftree.o -c

translator: ./proofTranslator/translator.cpp
	g++ -std=c++11 -Wall -pedantic ./proofTranslator/translator.cpp -o ./proofTranslator/translator

clean:
	./clean.sh
