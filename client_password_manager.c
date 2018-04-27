#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

#define DIMENSION 1000

void Create( int sd); //functia de creat conturi
int Login(int sd); //Login
int Inchidere(int sd); //close
int Baza_date_parole(char user_master[DIMENSION],int sd);//afisare si management parole
/* codul de eroare returnat de anumite apeluri */
extern int errno;



/* portul de conectare la server*/
int port;

int main (int argc, char *argv[])
{
  int sd;			// descriptorul de socket
  struct sockaddr_in server;	// structura folosita pentru conectare 
  char msg[DIMENSION];		// mesajul trimis
  
  /* exista toate argumentele in linia de comanda? */
  if (argc != 3)
    {
      printf ("[client] Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  /* stabilim portul */
  port = atoi (argv[2]);

  /* cream socketul */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[client] Eroare la socket().\n");
      return errno;
    }
  

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(argv[1]);
  /* portul de conectare */
  server.sin_port = htons (port);
  
  /* ne conectam la server */
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }

printf("██████╗  █████╗ ███████╗███████╗██╗    ██╗ ██████╗ ██████╗ ██████╗     ███╗   ███╗ █████╗ ███╗   ██╗ █████╗  ██████╗ ███████╗██████╗\n");  
printf("██╔══██╗██╔══██╗██╔════╝██╔════╝██║    ██║██╔═══██╗██╔══██╗██╔══██╗    ████╗ ████║██╔══██╗████╗  ██║██╔══██╗██╔════╝ ██╔════╝██╔══██╗\n");
printf("██████╔╝███████║███████╗███████╗██║ █╗ ██║██║   ██║██████╔╝██║  ██║    ██╔████╔██║███████║██╔██╗ ██║███████║██║  ███╗█████╗  ██████╔╝\n");
printf("██╔═══╝ ██╔══██║╚════██║╚════██║██║███╗██║██║   ██║██╔══██╗██║  ██║    ██║╚██╔╝██║██╔══██║██║╚██╗██║██╔══██║██║   ██║██╔══╝  ██╔══██╗\n");
printf("██║     ██║  ██║███████║███████║╚███╔███╔╝╚██████╔╝██║  ██║██████╔╝    ██║ ╚═╝ ██║██║  ██║██║ ╚████║██║  ██║╚██████╔╝███████╗██║  ██║\n");
printf("╚═╝     ╚═╝  ╚═╝╚══════╝╚══════╝ ╚══╝╚══╝  ╚═════╝ ╚═╝  ╚═╝╚═════╝     ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚═╝  ╚═╝\n");
printf("\n");
printf ("Bun venit !\n ");
printf ("Alege din urmatoarele optiuni ce vrei sa faci :\n");
printf ("~Login : login\n");
printf ("~Creeaza cont : create\n");
printf ("~Inchide aplicatie : quit\n");
fflush(stdout);

  /* citirea mesajului */
  bzero (msg, DIMENSION);
  printf ("[client]Introduceti o comanda: ");
  fflush (stdout);
  read (0, msg, DIMENSION);
  fflush(stdout); 
  if(!(strcmp(msg,"create\n")))
  {
    if (write (sd, msg, DIMENSION) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    fflush(stdout);
    Create(sd);
     
  }
  else if(!(strcmp(msg,"quit\n")))
  {
      Inchidere(sd);
  }
  else if(!(strcmp(msg,"login\n")))
  {
    if (write (sd, msg, DIMENSION) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    fflush(stdout);
    Login(sd);
    
  }

  close (sd);
}
int Login(int sd)
{
  char user_master[DIMENSION];
  char passwd_master[DIMENSION];
  char ok[DIMENSION];
  char msg[DIMENSION];

	bzero(user_master,DIMENSION);
  printf("Introduceti numele de utilizator master\n");
	fflush(stdout);
  read(0,user_master,DIMENSION);
  bzero(passwd_master,DIMENSION);
  printf("Introduceti parola master\n");
	fflush(stdout);
  read(0,passwd_master,DIMENSION);

  if (write (sd, user_master, DIMENSION) <= 0)
        {
        perror ("[client]Eroare la write() user master spre server.\n");
        return errno;
        }
        fflush(stdout);
  if (write (sd,passwd_master, DIMENSION) < 0)
        {
          perror ("[client]Eroare la write()parola master spre server.\n");
          return errno;
        }
        fflush(stdout);
  if (read (sd, ok, DIMENSION) < 0)
    {
      perror ("[client]Eroare la read() de la server cu ok.\n");
      return errno;
    }
fflush(stdout);
    
  if(!(strcmp(ok,"true")))
  {
    printf("Sunteti logat! \n");
    fflush(stdout);
    label:
    Baza_date_parole(user_master,sd);
    goto label;
  }
  else{
    printf("Nu aveti cont! Reintrati in aplicatie si creati-va cont !\n");
    fflush(stdout);
    Create(sd);
  
  }
  
}

int Baza_date_parole(char user_master[DIMENSION],int sd){

char comanda[DIMENSION];
char ok[DIMENSION];

printf("Aveti la dispozitie urmatoarele comenzi :\n");
printf ("~Afiseaza parolele : show\n");
printf ("~Afiseaza pe categorii : showcat\n");
printf ("~Adauga parole : add\n");
printf ("~Modifica : modify\n");
printf ("~Inchide aplicatie : quit\n");
printf("\n");
fflush(stdout);
scanf("%s",comanda);
fflush(stdout);
if(!(strcmp(comanda,"quit")))
{
  
  strcpy(comanda,"quit");
  if(write(sd,comanda,DIMENSION)<=0)
          {
          perror ("[client]Eroare la write()  comanda spre server.\n");
          return errno;
        }
  Inchidere(sd);
}
if(!(strcmp(comanda,"show")))
{
    if(write(sd,comanda,DIMENSION)<=0)
          {
          perror ("[client]Eroare la write()  comanda spre server.\n");
          return errno;
        }
        
    if(write(sd,user_master,DIMENSION)<=0)
          {
          perror ("[client]Eroare la write() user master comanda spre server.\n");
          return errno;
        }

  if (read (sd, ok, DIMENSION) < 0)
      {
        perror ("[client]Eroare la read() de la server cu ok.\n");
        return errno;
      }
      printf("%s\n",ok);
      strcpy(ok,"");
      strcpy(comanda,"");
}
if(!(strcmp(comanda,"showcat")))
{
  char categoria[DIMENSION]="\0";
    if(write(sd,comanda,DIMENSION)<=0)
          {
          perror ("[client]Eroare la write()  comanda spre server.\n");
          return errno;
        }
    if(write(sd,user_master,DIMENSION)<=0)
          {
          perror ("[client]Eroare la write() user master comanda spre server.\n");
          return errno;
        }
    printf("Introduceti categoria pe care o vreti afisata : \n");
    scanf("%s",categoria);
    if(write(sd,categoria,DIMENSION)<=0)
          {
          perror ("[client]Eroare la write() user master comanda spre server.\n");
          return errno;
        }

  if (read (sd, ok, DIMENSION) < 0)
      {
        perror ("[client]Eroare la read() de la server cu ok.\n");
        return errno;
      }
      printf("%s\n",ok);
      strcpy(ok,"");
      strcpy(comanda,"");
}
if(!(strcmp(comanda,"add")))
{
    char titlu[DIMENSION];
    char parola[DIMENSION];
    char url[DIMENSION];
    char user_name[DIMENSION];
    char notite[DIMENSION];
    char categorie[DIMENSION];

    if(write(sd,comanda,DIMENSION)<=0)
          {
          perror ("[client]Eroare la write()  comanda spre server.\n");
          return errno;
        }
        
    if(write(sd,user_master,DIMENSION)<=0)
          {
          perror ("[client]Eroare la write() user master comanda spre server.\n");
          return errno;
        }
    printf("Introduceti titlul : \n");
    fflush(stdout);
    scanf("%s",titlu);
    if(write(sd,titlu,DIMENSION)<=0)
          {
          perror ("[client]Eroare la write() user master comanda spre server.\n");
          return errno;
        }
     printf("Introduceti parola  : \n");
     fflush(stdout);
    scanf("%s",parola);
    if(write(sd,parola,DIMENSION)<=0)
          {
          perror ("[client]Eroare la write() user master comanda spre server.\n");
          return errno;
        }
        printf("Introduceti url-ul  : \n");
        fflush(stdout);
    scanf("%s",url);
    if(write(sd,url,DIMENSION)<=0)
          {
          perror ("[client]Eroare la write() user master comanda spre server.\n");
          return errno;
        }
         printf("Introduceti user_name-ul  : \n");
         fflush(stdout);
    scanf("%s",user_name);
    if(write(sd,user_name,DIMENSION)<=0)
          {
          perror ("[client]Eroare la write() user master comanda spre server.\n");
          return errno;
        }
        printf("Introduceti notite : \n");
        fflush(stdout);
    scanf("%s",notite);
    if(write(sd,notite,DIMENSION)<=0)
          {
          perror ("[client]Eroare la write() user master comanda spre server.\n");
          return errno;
        }
 printf("Introduceti categoria : \n");
 fflush(stdout);
    scanf("%s",categorie);
    if(write(sd,categorie,DIMENSION)<=0)
          {
          perror ("[client]Eroare la write() user master comanda spre server.\n");
          return errno;
        }
      strcpy(comanda,"");
}
if(!(strcmp(comanda,"modify")))
{
    char titlu[DIMENSION];
    char parola[DIMENSION];
    char url[DIMENSION];
    char user_name[DIMENSION];
    char notite[DIMENSION];
    char categorie[DIMENSION];
    char id[DIMENSION];

    if(write(sd,comanda,DIMENSION)<=0)
          {
          perror ("[client]Eroare la write()  comanda spre server.\n");
          return errno;
        }
        
    if(write(sd,user_master,DIMENSION)<=0)
          {
          perror ("[client]Eroare la write() user master comanda spre server.\n");
          return errno;
        }
        printf("Introduceti un indentificator pentru randul pe care vreti sa il modificati : \n");
    fflush(stdout);
    scanf("%s",id);
    if(write(sd,id,DIMENSION)<=0)
          {
          perror ("[client]Eroare la write() user master comanda spre server.\n");
          return errno;
        }
     
    printf("Introduceti titlul : \n");
    fflush(stdout);
    scanf("%s",titlu);
    if(write(sd,titlu,DIMENSION)<=0)
          {
          perror ("[client]Eroare la write() user master comanda spre server.\n");
          return errno;
        }
     printf("Introduceti parola  : \n");
     fflush(stdout);
    scanf("%s",parola);
    if(write(sd,parola,DIMENSION)<=0)
          {
          perror ("[client]Eroare la write() user master comanda spre server.\n");
          return errno;
        }
        printf("Introduceti url-ul  : \n");
        fflush(stdout);
    scanf("%s",url);
    if(write(sd,url,DIMENSION)<=0)
          {
          perror ("[client]Eroare la write() user master comanda spre server.\n");
          return errno;
        }
         printf("Introduceti user_name-ul  : \n");
         fflush(stdout);
    scanf("%s",user_name);
    if(write(sd,user_name,DIMENSION)<=0)
          {
          perror ("[client]Eroare la write() user master comanda spre server.\n");
          return errno;
        }
        printf("Introduceti notite : \n");
        fflush(stdout);
    scanf("%s",notite);
    if(write(sd,notite,DIMENSION)<=0)
          {
          perror ("[client]Eroare la write() user master comanda spre server.\n");
          return errno;
        }
 printf("Introduceti categoria : \n");
 fflush(stdout);
    scanf("%s",categorie);
    if(write(sd,categorie,DIMENSION)<=0)
          {
          perror ("[client]Eroare la write() user master comanda spre server.\n");
          return errno;
        }
      strcpy(comanda,"");
}


}



void Create(int sd)
{   
    
      char user_master[DIMENSION];     // userul master
      char passwd_master[DIMENSION]; //parola master    
      
      printf("Introduceti un user master:\n");
      scanf("%s",user_master);
      if (write (sd, user_master, DIMENSION) <= 0)
        {
        perror ("[client]Eroare la write() user master spre server.\n");
        Inchidere(sd);
        }
        fflush(stdout);

      printf("Introduceti si o parola master pentru contul dumneavoastra : \n");
      fflush(stdout);
      scanf("%s",passwd_master);
      if (write (sd, passwd_master, DIMENSION) <= 0)
        {
        perror ("[client]Eroare la write() parola master spre server.\n");
        Inchidere(sd);
        }
        fflush(stdout);
      printf("Ati creat cu succes contul, logati-va !\n");
      fflush(stdout);
      Login(sd);
      
}

int Inchidere(int sd)
{
        printf("Deconectare!\n");
        fflush(stdout);
        close(sd);
        exit(1);
}

