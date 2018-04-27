#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>


/* portul folosit */
#define PORT 9921

#define DIMENSION 1000

extern int errno;		/* eroarea returnata de unele apeluri */


int callback(void *, int, char **, char **); //callback baza de date

void Introducere_User_Master(char user_master[DIMENSION]);

void Introducere_Password_Master(char passwd_master[DIMENSION], char user_master[DIMENSION]);

int Cauta_User_Master(char user_master[DIMENSION],char passwd_master[DIMENSION],int fd);

int Show_data_base(char user_master[DIMENSION], int fd);

int Show_cat_data_base(char user_master[DIMENSION],char categoria[DIMENSION], int fd);

void Insert_query(char titlu[DIMENSION], char parola[DIMENSION], char url[DIMENSION], char user_name[DIMENSION], char notite[DIMENSION],char categorie[DIMENSION],char user_master[DIMENSION]);

void Modify_query(char id[DIMENSION],char titlu[DIMENSION], char parola[DIMENSION], char url[DIMENSION], char user_name[DIMENSION], char notite[DIMENSION],char categorie[DIMENSION],char user_master[DIMENSION]);
/* functie de convertire a adresei IP a clientului in sir de caractere */
char * conv_addr (struct sockaddr_in address)
{
  static char str[25];
  char port[7];

  /* adresa IP a clientului */
  strcpy (str, inet_ntoa (address.sin_addr));	
  /* portul utilizat de client */
  bzero (port, 7);
  sprintf (port, ":%d", ntohs (address.sin_port));	
  strcat (str, port);
  return (str);
}


void Password_Manager(int fd, int sd)
{
    char buffer[DIMENSION];		/* mesajul */
    char msg[DIMENSION];		//mesajul primit de la client 
    char user_master[DIMENSION]; //user master
    char passwd_master[DIMENSION]; //parola master
    char comanda[DIMENSION]; //comanda pt afisare baza date

    if (read (fd, msg, DIMENSION) < 0)
    {
      perror ("[client]Eroare la read() de la client spre server.\n");
      //Inchidere(sd);
      close(sd);
    }
    fflush(stdout);
    if(!(strcmp(msg,"create\n")))
    {
        if (read (fd, user_master, DIMENSION) < 0)
            {
            perror ("[client]Eroare la read() pentru user master.\n");
           // Inchidere(sd);
           close(sd);
            }
            fflush(stdout);
        Introducere_User_Master(user_master);
        if(read(fd,passwd_master,DIMENSION) < 0)
        {
            perror ("[client]Eroare la read() pentru passwd master.\n");
            //Inchidere(sd);
            close(sd);
        }
        fflush(stdout);
        Introducere_Password_Master(passwd_master, user_master);
    }
    else if(!(strcmp(msg,"login\n")))
    {
      if(read(fd,user_master,DIMENSION)<0)
      {
        perror ("[client]Eroare la read() pentru user master.\n");
           // Inchidere(sd);
           close(sd);
      }
      fflush(stdout);
      if(read(fd,passwd_master,DIMENSION) < 0)
      {
        perror ("[client]Eroare la read() pentru passwd_master.\n");
           // Inchidere(sd);
           close(sd);
      }
      fflush(stdout);
      Cauta_User_Master(user_master,passwd_master,fd);
      printf("Cautarea a luat sfarsit :D !\n");
      fflush(stdout);
      CITIRECOMANDA:
      if(read(fd,comanda,DIMENSION) < 0)
      {
        perror ("[client]Eroare la read() pentru comanda.\n");
           // Inchidere(sd);
           close(sd);
      }
      if(read(fd,user_master,DIMENSION) < 0)
      {
        perror ("[client]Eroare la read() pentru user_master.\n");
           // Inchidere(sd);
           close(sd);
      }
      printf("comanda este %s\n",comanda);
      printf("useru este %s si lungimea %d\n",user_master,strlen(user_master));
      if(!(strcmp(comanda,"show")))
      {
        Show_data_base(user_master, fd);
        goto CITIRECOMANDA;
      }
      else if(!(strcmp(comanda,"quit")))
      {
          printf("\n");
      }
      else if(!(strcmp(comanda,"showcat")))
      {
          char categoria[DIMENSION];
        if(read(fd,categoria,DIMENSION) < 0)
          {
            perror ("[client]Eroare la read() pentru user_master.\n");
              close(sd);
          }
        Show_cat_data_base(user_master,categoria,fd);
        goto CITIRECOMANDA;
      }
      else if(!(strcmp(comanda,"add"))){
        char titlu[DIMENSION]="\0";
        char parola[DIMENSION]="\0";
        char url[DIMENSION]="\0";
        char user_name[DIMENSION]="\0";
        char notite[DIMENSION]="\0";
        char categorie[DIMENSION]="\0";
        if(read(fd,titlu,DIMENSION) < 0)
          {
            perror ("[client]Eroare la read() pentru user_master.\n");
              close(sd);
          }
          if(read(fd,parola,DIMENSION) < 0)
          {
            perror ("[client]Eroare la read() pentru user_master.\n");
              close(sd);
          }
          if(read(fd,url,DIMENSION) < 0)
          {
            perror ("[client]Eroare la read() pentru user_master.\n");
              close(sd);
          }
          if(read(fd,user_name,DIMENSION) < 0)
          {
            perror ("[client]Eroare la read() pentru user_master.\n");
              close(sd);
          }
          if(read(fd,notite,DIMENSION) < 0)
          {
            perror ("[client]Eroare la read() pentru user_master.\n");
              close(sd);
          }
          if(read(fd,categorie,DIMENSION) < 0)
          {
            perror ("[client]Eroare la read() pentru user_master.\n");
              close(sd);
          }
          Insert_query(titlu,parola,url,user_name,notite,categorie,user_master);
          goto CITIRECOMANDA;
      }
      else if(!(strcmp(comanda,"modify"))){
        char titlu[DIMENSION]="\0";
        char parola[DIMENSION]="\0";
        char url[DIMENSION]="\0";
        char user_name[DIMENSION]="\0";
        char notite[DIMENSION]="\0";
        char categorie[DIMENSION]="\0";
        char id[DIMENSION]="\0";
        if(read(fd,id,DIMENSION)){
           perror ("[client]Eroare la read() pentru user_master.\n");
              close(sd);
        }
        printf("%s acesta este id-ul\n",id);
        if(read(fd,titlu,DIMENSION) < 0)
          {
            perror ("[client]Eroare la read() pentru user_master.\n");
              close(sd);
          }
          if(read(fd,parola,DIMENSION) < 0)
          {
            perror ("[client]Eroare la read() pentru user_master.\n");
              close(sd);
          }
          if(read(fd,url,DIMENSION) < 0)
          {
            perror ("[client]Eroare la read() pentru user_master.\n");
              close(sd);
          }
          if(read(fd,user_name,DIMENSION) < 0)
          {
            perror ("[client]Eroare la read() pentru user_master.\n");
              close(sd);
          }
          if(read(fd,notite,DIMENSION) < 0)
          {
            perror ("[client]Eroare la read() pentru user_master.\n");
              close(sd);
          }
          if(read(fd,categorie,DIMENSION) < 0)
          {
            perror ("[client]Eroare la read() pentru user_master.\n");
              close(sd);
          }
          Modify_query(id,titlu,parola,url,user_name,notite,categorie,user_master);
          goto CITIRECOMANDA;
      }

    }
}

void Modify_query(char id[DIMENSION],char titlu[DIMENSION], char parola[DIMENSION], char url[DIMENSION], char user_name[DIMENSION], char notite[DIMENSION],char categorie[DIMENSION],char user_master[DIMENSION])
{
 sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    char sql[DIMENSION];
    char name[DIMENSION];

    //deschidem baza de date
  int  rc = sqlite3_open("tabel_password_manager.db", &db);


   if( rc ){
      fprintf(stderr, "Nu am putut deschide baza de date %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stdout, "Baza de date deschisa cu succes\n");
      fflush(stdout);
   }
        
        
        //User,Titlu,Parola,Url,UserName,Notite,Categorie
        printf("%d id-ul",id);
        strncpy(name,user_master,strlen(user_master)-1);
        printf("name %d %s",strlen(name),name);
        strcat(sql, "UPDATE Passwords SET Parola='");
        strcat(sql,titlu);
        strcat(sql,"', Url='");
        strcat(sql,url);
        strcat(sql,"', UserName='");
        strcat(sql,user_name);
        strcat(sql,"', Notite='");
        strcat(sql,notite);
        strcat(sql,"', Categorie='");
        strcat(sql,categorie);
        strcat(sql,"' WHERE ");
        strcat(sql,"Titlu = '");
        strcat(sql,titlu);
        strcat(sql,"' AND User ='");
        strcat(sql,name);
        strcat(sql,"';");

        rc = sqlite3_exec(db, sql, callback, 0 , &err_msg);					//o executam
			 
			   	
			   if( rc != SQLITE_OK ){
				  fprintf(stderr, "SQL error: %s\n", err_msg);
				  sqlite3_free(err_msg);
			   }

        rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
   	
		    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "A dat gres ca creeze data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        fflush(stdout);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        
        exit(0);
    }
    else{
        printf("Ati adaugat cu succes in baza de date !\n");
        fflush(stdout);
        
    }

	sqlite3_close(db);	
  
}

void Insert_query(char titlu[DIMENSION], char parola[DIMENSION], char url[DIMENSION], char user_name[DIMENSION], char notite[DIMENSION],char categorie[DIMENSION],char user_master[DIMENSION]){
    
    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    char sql[DIMENSION];
    char name[DIMENSION];

    //deschidem baza de date
  int  rc = sqlite3_open("tabel_password_manager.db", &db);


   if( rc ){
      fprintf(stderr, "Nu am putut deschide baza de date %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stdout, "Baza de date deschisa cu succes\n");
      fflush(stdout);
   }
        
        strncpy(name,user_master,strlen(user_master)-1);
        printf("name %d %s",strlen(name),name);
        strcpy(sql, "INSERT INTO Passwords (User,Titlu,Parola,Url,UserName,Notite,Categorie) VALUES ('");
       
		strcat(sql,name);
		strcat(sql,"' , '" );
    strcat(sql,titlu);
    strcat(sql,"' , '" );
    strcat(sql,parola);
    strcat(sql,"' , '" );
    strcat(sql,url);
    strcat(sql,"' , '" );
    strcat(sql,user_name);
    strcat(sql,"' , '" );
    strcat(sql,notite);
    strcat(sql,"' , '" );
    strcat(sql,categorie);
    strcat(sql,"');");
    

    
        rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
   	
		    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "A dat gres ca creeze data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        fflush(stdout);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        
        exit(0);
    }
    else{
        printf("Ati adaugat cu succes in baza de date !\n");
        fflush(stdout);
        
    }

	sqlite3_close(db);	
  
}
int Show_cat_data_base(char user_master[DIMENSION], char categoria[DIMENSION],int fd)
{

    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    char sql[DIMENSION];
    int rc;

  
  /* Open database */
				rc = sqlite3_open("tabel_password_manager.db", &db);
				if( rc ){
				  fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
				  return(0);
				}
				else
				{
				  fprintf(stdout, "Opened database successfully\n");
				}
        
		  
                //stocam in res rezultatul interogarii
				rc = sqlite3_prepare_v2(db,"SELECT * from Passwords", DIMENSION, &res, 0);

              if (rc != SQLITE_OK) {
                      printf("We did not get any data!\n");
              }
              else{
                printf("S-a produs interogarea corect ! \n");
              }
			  
			
			  char nume[DIMENSION]="\0";
        char cat[DIMENSION]="\0";

        strncpy(nume,user_master,strlen(user_master)-1);
        strncpy(cat,categoria,strlen(categoria));

      
			  char ok[DIMENSION]="\0";
        int i=0;
			  //verificam daca exista in baza de date cineva cu numele si parola asta
        printf("coloana 7 %s \n",sqlite3_column_text(res,1));
        printf("catu %s \n",cat);
          strcat(ok,"User:  Titlu:  Parola:       URL:        UserName:   Notite:            Categorie:\n");
			   while (sqlite3_step(res) == SQLITE_ROW) 
			  {    
                  if((!(strcmp(sqlite3_column_text(res,1),nume))) && (!(strcmp(sqlite3_column_text(res,7),cat))))
                  {
                     strcat(ok,"|");
                    for(i=1;i<=7;i++)
                    {
                      strcat(ok,"|");
                      strcat(ok,sqlite3_column_text(res,i));
                      strcat(ok," ");
                    }
                    strcat(ok,"||\n");
                  }
				}
        //  strcpy(ok,"un text in ok");
         printf("%s oke-ul ",ok);

         fflush(stdout);
          if (write (fd, ok, DIMENSION) <= 0)
                          {
                          perror ("[client]Eroare la write() spre client de la server cu ok.\n");
                          return errno;
                          }
                          fflush(stdout);

          rc = sqlite3_exec(db, sql, callback, 0 , &err_msg);					//o executam
			 
			   	
			   if( rc != SQLITE_OK ){
				  fprintf(stderr, "SQL error: %s\n", err_msg);
				  sqlite3_free(err_msg);
			   }
			   
			   else{
				  fprintf(stdout, "Operation done successfully\n");
			   }
             sqlite3_finalize(res);
            sqlite3_close(db);

}


int Show_data_base(char user_master[DIMENSION], int fd){

    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    char sql[DIMENSION];
    int rc;

  
  /* Open database */
				rc = sqlite3_open("tabel_password_manager.db", &db);
				if( rc ){
				  fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
				  return(0);
				}
				else
				{
				  fprintf(stdout, "Opened database successfully\n");
				}
        
		  
                //stocam in res rezultatul interogarii
				rc = sqlite3_prepare_v2(db,"SELECT * from Passwords", DIMENSION, &res, 0);

              if (rc != SQLITE_OK) {
                      printf("We did not get any data!\n");
              }
              else{
                printf("S-a produs interogarea corect ! \n");
              }
			  
		
			  char nume[DIMENSION]="\0";
        

      
        strncpy(nume,user_master,strlen(user_master)-1);
        
			  char ok[DIMENSION]="\0";
        int i=0;
			  //verificam daca exista in baza de date cineva cu numele si parola asta
         strcat(ok,"User:  Titlu:  Parola:       URL:        UserName:   Notite:            Categorie:\n");
			   while (sqlite3_step(res) == SQLITE_ROW) 
			  {    
                  if(!(strcmp(sqlite3_column_text(res,1),nume)))
                  {
                     strcat(ok,"|");
                    for(i=1;i<=7;i++)
                    {
                      strcat(ok,"|");
                      strcat(ok,sqlite3_column_text(res,i));
                      strcat(ok," ");
                    }
                    strcat(ok,"||\n");
                  }
				}
        //  strcpy(ok,"un text in ok");
         printf("%s oke-ul ",ok);

         fflush(stdout);
          if (write (fd, ok, DIMENSION) <= 0)
                          {
                          perror ("[client]Eroare la write() spre client de la server cu ok.\n");
                          return errno;
                          }
                          fflush(stdout);

          rc = sqlite3_exec(db, sql, callback, 0 , &err_msg);					//o executam
			 
			   	
			   if( rc != SQLITE_OK ){
				  fprintf(stderr, "SQL error: %s\n", err_msg);
				  sqlite3_free(err_msg);
			   }
			   
			   else{
				  fprintf(stdout, "Operation done successfully\n");
			   }
             sqlite3_finalize(res);
            sqlite3_close(db);

}


int Cauta_User_Master(char user_master[DIMENSION], char passwd_master[DIMENSION],int fd)
{
    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    char sql[DIMENSION];
    int rc;

  
  /* Open database */
				rc = sqlite3_open("tabel_password_manager.db", &db);
				if( rc ){
				  fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
				  return(0);
				}
				else
				{
				  fprintf(stdout, "Opened database successfully\n");
				}
		  
               //stocam in res reziltatul interogarii  
				rc = sqlite3_prepare_v2(db,"SELECT Nume,Parola from Utilizatori", DIMENSION, &res, 0);
       

              if (rc != SQLITE_OK) {
                      printf("We did not get any data!\n");
              }
              else{
                printf("S-a produs interogarea corect ! \n");
              }
			  
			 printf("passwd_master %s %d\n",passwd_master,strlen(passwd_master));
       printf("user_master %s %d\n",user_master,strlen(user_master));
      fflush(stdout);
			  char nume[DIMENSION];
        char parola[DIMENSION];

      
        strncpy(nume,user_master,strlen(user_master)-1);
        strncpy(parola,passwd_master,strlen(passwd_master)-1);

       printf("passwd_master %s %d\n",parola,strlen(parola));
       printf("user_master %s %d\n",nume,strlen(nume));
      fflush(stdout);
			  char ok[DIMENSION];
        bzero(ok,DIMENSION);
			  //verificam daca exista in baza de date cineva cu numele si parola asta
			   while (sqlite3_step(res) == SQLITE_ROW) 
			  {    
                  if(!(strcmp(sqlite3_column_text(res,0),nume)))
                  {
                    printf("a intrat pe primu if\n");
                      if(!(strcmp(sqlite3_column_text(res,1),parola)))
                      {
                            strcpy(ok,"true");
                      }
                  }
                  
				 }
         printf("%s oke-ul ",ok);

         fflush(stdout);
          if (write (fd, ok, DIMENSION) <= 0)
                          {
                          perror ("[client]Eroare la write() spre client de la server cu ok.\n");
                          return errno;
                          }
                          fflush(stdout);

          rc = sqlite3_exec(db, sql, callback, 0 , &err_msg);					//o executam
			 
			   	
			   if( rc != SQLITE_OK ){
				  fprintf(stderr, "SQL error: %s\n", err_msg);
				  sqlite3_free(err_msg);
			   }
			   
			   else{
				  fprintf(stdout, "Operation done successfully\n");
			   }
             sqlite3_finalize(res);
            sqlite3_close(db);

    }


void Introducere_Password_Master(char passwd_master[DIMENSION], char user_master[DIMENSION]){
    
    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    char sql[DIMENSION];

    int  rc = sqlite3_open("tabel_password_manager.db", &db);
    if( rc ){
      fprintf(stderr, "Nu am putut deschide baza de date %s\n", sqlite3_errmsg(db));
      exit(0);
            }
    else{
      fprintf(stdout, "Baza de date deschisa cu succes\n");
      fflush(stdout);
        }

        strcpy(sql,"UPDATE Utilizatori SET Parola = '");
				strcat(sql,passwd_master);
        strcat(sql,"'");
				strcat(sql,"WHERE Nume ='");	
				strcat(sql,user_master);
				strcat(sql,"';");

      rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
   	
		    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "A dat gres ca creeze data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        fflush(stdout);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        
        exit(0);
    }
    else{
        printf("Ati adaugat cu o parola master %s cu succes!\n",passwd_master);
        fflush(stdout);
    }
    sqlite3_close(db);
				
}

void Introducere_User_Master(char user_master[DIMENSION]){


    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    char sql[DIMENSION];

    //deschidem baza de date
  int  rc = sqlite3_open("tabel_password_manager.db", &db);


   if( rc ){
      fprintf(stderr, "Nu am putut deschide baza de date %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stdout, "Baza de date deschisa cu succes\n");
      fflush(stdout);
   }
        
        strcpy(sql, "INSERT INTO Utilizatori (Nume,Parola) VALUES ('");
       
		strcat(sql,user_master);
		strcat(sql,"' ," );
		strcat(sql,"NULL);");

    
        rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
   	
		    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "A dat gres ca creeze data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        fflush(stdout);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        
        exit(0);
    }
    else{
        printf("Ati adaugat cu succes utilizatorul %s!\n",user_master);
        fflush(stdout);
        
    }

	sqlite3_close(db);	
  
}


/* programul */
int
main ()
{
  struct sockaddr_in server;	/* structurile pentru server si clienti */
  struct sockaddr_in from;
  fd_set readfds;		/* multimea descriptorilor de citire */
  fd_set actfds;		/* multimea descriptorilor activi */
  struct timeval tv;		/* structura de timp pentru select() */
  int sd, client;		/* descriptori de socket */
  int optval=1; 			/* optiune folosita pentru setsockopt()*/ 
  int fd;			/* descriptor folosit pentru 
				   parcurgerea listelor de descriptori */
  int nfds;			/* numarul maxim de descriptori */
  int len;			/* lungimea structurii sockaddr_in */

  /* creare socket */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server] Eroare la socket().\n");
      return errno;
    }

  /*setam pentru socket optiunea SO_REUSEADDR */ 
  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR,&optval,sizeof(optval));

  /* pregatim structurile de date */
  bzero (&server, sizeof (server));

  /* umplem structura folosita de server */
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl (INADDR_ANY);
  server.sin_port = htons (PORT);

  /* atasam socketul */
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server] Eroare la bind().\n");
      return errno;
    }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen (sd, 5) == -1)
    {
      perror ("[server] Eroare la listen().\n");
      return errno;
    }
  
  /* completam multimea de descriptori de citire */
  FD_ZERO (&actfds);		/* initial, multimea este vida */
  FD_SET (sd, &actfds);		/* includem in multime socketul creat */

  tv.tv_sec = 1;		/* se va astepta un timp de 1 sec. */
  tv.tv_usec = 0;
  
  /* valoarea maxima a descriptorilor folositi */
  nfds = sd;

  printf ("[server] Asteptam la portul %d...\n", PORT);
  fflush (stdout);
        
  /* servim in mod concurent clientii... */
  while (1)
    {
      /* ajustam multimea descriptorilor activi (efectiv utilizati) */
      bcopy ((char *) &actfds, (char *) &readfds, sizeof (readfds));

      /* apelul select() */
      if (select (nfds+1, &readfds, NULL, NULL, &tv) < 0)
	{
	  perror ("[server] Eroare la select().\n");
	  return errno;
	}
      /* vedem daca e pregatit socketul pentru a-i accepta pe clienti */
      if (FD_ISSET (sd, &readfds))
	{
	  /* pregatirea structurii client */
	  len = sizeof (from);
	  bzero (&from, sizeof (from));

	  /* a venit un client, acceptam conexiunea */
	  client = accept (sd, (struct sockaddr *) &from, &len);

	  /* eroare la acceptarea conexiunii de la un client */
	  if (client < 0)
	    {
	      perror ("[server] Eroare la accept().\n");
	      continue;
	    }

          if (nfds < client) /* ajusteaza valoarea maximului */
            nfds = client;
            
	  /* includem in lista de descriptori activi si acest socket */
	  FD_SET (client, &actfds);

	  printf("[server] S-a conectat clientul cu descriptorul %d, de la adresa %s.\n",client, conv_addr (from));
	  fflush (stdout);
	}
      /* vedem daca e pregatit vreun socket client pentru a trimite raspunsul */
      for (fd = 0; fd <= nfds; fd++)	/* parcurgem multimea de descriptori */
	{
	  /* este un socket de citire pregatit? */
	  if (fd != sd && FD_ISSET (fd, &readfds))
	    {	
            Password_Manager(fd,sd);
            printf ("[server] S-a deconectat clientul cu descriptorul %d.\n",fd);
            fflush (stdout);
            close (fd);		// inchidem conexiunea cu clientul 
            FD_CLR (fd, &actfds);// scoatem si din multime 
            fflush(stdout);
		
	    }
	}			/* for */
    }				/* while */
}				/* main */


int callback(void *NotUsed, int argc, char **argv, 
                    char **azColName) {
    
    NotUsed = 0;
    
    for (int i = 0; i < argc; i++) {

        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    
    printf("\n");
    fflush(stdout);
    return 0;
}