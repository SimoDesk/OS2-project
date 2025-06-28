simulatore: src/simulatore.c
	@gcc -o simulatore src/simulatore.c src/operations.c src/util.c src/types.c src/stringManipolation.c src/dataGetter.c src/fileManagement.c src/queue.c src/thread.c -lm
	@./simulatore

clean:
	@rm -f *.o simulatore
	
debug: src/simulatore.c
	@gcc -g -pthread -o simulatore src/simulatore.c src/operations.c src/util.c src/types.c src/stringManipolation.c src/dataGetter.c src/fileManagement.c src/queue.c src/thread.c -lm
	@gdb ./simulatore

.PHONY: clean simulatore

