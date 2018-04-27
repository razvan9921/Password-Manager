all:
	gcc server_password_manager.c -o server_password_manager.exe -lsqlite3
	gcc client_password_manager.c -o client_password_manager.exe
	gcc -Wall baza_date.c -o baza_date.exe -lsqlite3
clean:
	rm -f *~client_password_manager.exe server_password_manager.exe
