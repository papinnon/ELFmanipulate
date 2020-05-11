all: writer 101
writer:
	g++ src/writer.cpp -g -o writer
101:
	gcc src/101.c -o 101
clean:
	rm 101 writer
