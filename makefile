simulatore: simulatore.c
	@gcc -o simulatore simulatore.c
	@./simulatore

clean:
	@rm -f *.o simulatore
	
.PHONY: clean, simulatore
