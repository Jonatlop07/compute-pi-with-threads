compile_exersise:
	gcc -c compute_pi_thread.c
	gcc compute_pi_thread.o -lm -lpthread -o compute_pi_thread
	./compute_pi_thread

