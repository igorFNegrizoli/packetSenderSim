all: 	
	g++ -g -O2 *.cpp -o exec
	./exec

clean:
	rm -rf *.o *~ exec
