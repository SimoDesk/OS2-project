simulatore: src/simulatore.c
	@gcc -o simulatore src/simulatore.c src/operations.c src/util.c src/types.c src/stringManipolation.c src/dataGetter.c src/fileManagement.c -lm
	@./simulatore

clean:
	@rm -f *.o simulatore

.PHONY: clean simulatore

