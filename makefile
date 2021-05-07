all: 	
	g++ -g *.cpp -o exec
	./exec

clean:
	rm -rf *.o *~ exec
