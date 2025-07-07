simulatore: src/simulatore.c
	@gcc -std=c90 -o simulatore src/simulatore.c src/operations.c src/util.c src/types.c src/stringManipolation.c src/dataGetter.c src/fileManagement.c src/queue.c src/thread.c -lm

clean:
	@rm -f *.o simulatore
	
debug: src/simulatore.c
	@gcc -g -pthread -o simulatore src/simulatore.c src/operations.c src/util.c src/types.c src/stringManipolation.c src/dataGetter.c src/fileManagement.c src/queue.c src/thread.c -lm

.PHONY: clean simulatore

