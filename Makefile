all:
	gcc main.c lib.c -o maze

c:	clean
clean:
	rm -f *.o maze; ls -l

