all : serwer klient
serwer : serwer.c libs.h user.h user.c active.h active.c
	gcc serwer.c user.c active.c -o serwer.o -lpthread
klient : klient.c libs.h
	gcc klient.c -o klient.o
