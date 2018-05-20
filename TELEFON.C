/*

                             *******************

******************************* C SOURCE FILE *******************************

**                           *******************                           **

**                                                                         **

** project   : Verwaltung einer Telefonliste                               ** 

** filename  : TELEFON.C                                                   **

** version   : 1                                                           **

** date      : Oktober 28, 2006                                            **

**                                                                         **

*****************************************************************************

**                                                                         **

** Copyright (c) 2006, A. Schreiber                                        **

** All rights reserved.                                                    **

**                                                                         **

*****************************************************************************

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *fp;


void anlegen(void)
{
  fp=fopen("TELEFON","w");
  if (fp==NULL)
    printf("Dateierstellungsfehler!");
  fclose(fp);
}

void eintragen()
{
  char name[30];
  char telnr[30];
  fp=fopen("TELEFON","a");
  if (fp==NULL)
    printf("Datei kann nicht geoeffnet werden!");
  else
    do
    {
      getchar();
      printf("\nName: ");
      gets(name);
      printf("\nTelefonnummer: ");
      gets(telnr);
      fputs(name,fp); fputs("\n",fp);
      fputs(telnr,fp); fputs("\n",fp);
      printf("Noch ein Datensatz (j/n)");
    }
    while(getchar()=='j');
  fclose (fp);
}

void ausgabe(void)
{
  char name[30];
  char telnr[30];
  fp=fopen("TELEFON","r");
  if (fp==NULL)
    printf("Datei kann nicht geoeffnet werden!");
  else
    do
      {
	if (!feof(fp))
	  {	  
	    printf("\n\nName: %s",name);
	    fgets(name,30,fp);
	    printf("\nTelefonnummer: %s",telnr);
            fgets(telnr,30,fp);
	  }
      }
	while(!feof(fp));
	fclose (fp);
}

void asuchen(void)
{
  char name[30],suchen[30];
  char telnr[30];
  int merk=1;
  fp=fopen("TELEFON","r");
  if (fp==NULL)
    printf("Datei kann nicht geoeffnet werden!");
  else
    {
      getchar();
      printf("\nTelefonnummer suchen\n");
      printf("====================\n");
      printf("Welchen Namen suchen? ");
      gets(suchen);
      strcat(suchen,"\n");
      do
	{
	  fgets(name,30,fp);
	  if(!strcmp(name,suchen)&&!feof(fp))
	    {
	      printf("\nName: %s",name);
	      fgets(telnr,30,fp);	  
	      printf("Telefonnummer: %s",telnr);
	      merk=0;
	    }
	}
	  while(!feof(fp));
	  if (merk)
	    printf("Name nicht gefunden!");
	  fclose (fp);
    }
}

void bsuchen(void)
{
  char name[30],suchen[30];
  char telnr[30];
  int merk=1;
  fp=fopen("TELEFON","r");
  if (fp==NULL)
    printf("Datei kann nicht geoeffnet werden!");
  else
    {
      getchar();
      printf("\nNamen suchen\n");
      printf("============\n");
      printf("Welche Telefonnummer suchen? ");
      gets(suchen);
      strcat(suchen,"\n");
      do
	{
	  fgets(name,30,fp);
	  if(!feof(fp))
	    {
	      fgets(telnr,30,fp);
	      if(!strcmp(telnr,suchen))
		{	      
		  printf("\nName: %s",name);  
		  printf("Telefonnummer: %s",telnr);
		  merk=0;
		}
	    }
	}
	  while(!feof(fp));
	  if (merk)
	    printf("Telefonnummer nicht gefunden!");
	  fclose (fp);
    }
}

main(void)
{
  int wahl;
  do
    {
      printf("\n\n\n");
      printf("\tDatei neu anlegen........1\n");
      printf("\tDaten anfuegen...........2\n");
      printf("\tDaten anzeigen...........3\n");
      printf("\tNamen suchen.............4\n");
      printf("\tTelefonnummer suchen.....5\n");
      printf("\tProgrammende.............6\n");
      printf("\n\n\n\tKennziffer eingeben ! ");
      switch(wahl=getchar())
	{
	case '1': anlegen();
	  break;
	case '2': eintragen();
	  break;
	case '3': ausgabe();
	  break;
	case '4': asuchen();
	  break;
	case '5': bsuchen();
	  break;
	case '6': exit(1);
	  break;
	default: printf("\007");
	}
      getchar();
    }
  while(1);
  return 0;
}
