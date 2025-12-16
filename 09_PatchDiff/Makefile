CC=gcc
CFLAGS=-O2 -Wall -Wextra

all: main0 main1 main2 main3

main0: main.c
	$(CC) $(CFLAGS) -o main0 main.c

main1.c: main.c patch1.diff
	cp main.c main1.c
	patch main1.c < patch1.diff

main1: main1.c
	$(CC) $(CFLAGS) -o main1 main1.c

main2.c: main1.c patch2.diff
	cp main1.c main2.c
	patch main2.c < patch2.diff

main2: main2.c
	$(CC) $(CFLAGS) -o main2 main2.c

main3.c: main2.c patch3.diff
	cp main2.c main3.c
	patch main3.c < patch3.diff

main3: main3.c
	$(CC) $(CFLAGS) -o main3 main3.c

run: all
	./main0 > out0.txt
	./main1 6 > out1.txt
	./main2 ".#" 6 > out2.txt
	./main3 123 ".#" 6 > out3.txt

clean:
	rm -f main0 main1 main2 main3
	rm -f main1.c main2.c main3.c
	rm -f out0.txt out1.txt out2.txt out3.txt

