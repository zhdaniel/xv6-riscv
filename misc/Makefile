all: pipe primes

pipe: pipe.c
	gcc -g pipe.c -o pipe

primes: primes.c
	gcc -g primes.c -o primes

.PHONY: clean
clean:
	rm -rf pipe primes *.dSYM
