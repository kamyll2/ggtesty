all : serwer klient
serwer : serwer.c libs.h user.h user.c
	gcc serwer.c user.c -o serwer.o
klient : klient.c libs.h
	gcc klient.c -o klient.o
