all : serwer klient
serwer : serwer.c libs.h user.h  user.c active.h active.c parser.h parser.c
	gcc serwer.c user.c active.c parser.c -o serwer.o -lpthread
klient : klient.c libs.h parser.h parser.c
	gcc klient.c parser.c -o klient.o
