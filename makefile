simulatore: simulatore.c
	cd src
	@gcc -o simulatore simulatore.c -lm
	@./simulatore

clean:
	cd src
	@rm -f *.o simulatore

.PHONY: clean simulatore

