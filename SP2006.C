/**************************************************************************************************
 * Semesterprojekt 2006
 *
 * Grafische Temperaturanzeige der Werte von RS232 und Archivierung in Datei "HZTEMP.DAT"
 * (Diese Version des Programms l�uft ohne Daten von RS232 zu empfangen. Der Wert der Temperatur
 * ist simuliert und kann mit den Tasten "q" und "a" ver�ndert werden.)
 *
 * Autor: A. Schreiber
 * Date:  Oktober 2006
 **************************************************************************************************/

#include <stdio.h>
#include <graphics.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include "initcom.c"							/* Funktion Initialisierung COM1 */
												/* Vorgabe ist COM2, habe aber nur COM1 an meiner Kiste ;-) */
char zeit[8];									/* Feldvariable f�r Zeit hh:mm:ss */
int zeiten[60];									/* Feldvariable 60 Elemente (Zeit) */
int tempwerte[60];								/* Feldvariable 60 Elemente (Werte) */
int ibyte=100;									/* Variable f�r Einlesen COM (Anfangswert testweise vorgegeben) */
int twert;										/* Variable f�r Zwischenspeicherung Tempwert */
int fz,m;										/* Variablen f�r Schieberegister Feldvariablen */
int x,y,xold,yold;								/* Variablen f�r Grafikmodus */
int xa,ya,xrange,yrange,treiber,modus;
int key=0;										/* Variable f�r Tastenabfrage */

void zeitanzeige(void)							/* Funktion zur Erzeugung Uhrzeit im Format hh:mm:ss */
{
 time_t t1;
 struct tm t2;

 time(&t1);
 t2=*localtime(&t1);
 sprintf(zeit,"%02i:%02i:%02i\0",t2.tm_hour,t2.tm_min,t2.tm_sec);
}

void init_graphic(void)
{
	treiber=DETECT; modus=2;							/* Grafikinitialisierung */
	initgraph(&treiber, &modus,"");
	setcolor(LIGHTBLUE);								/* Farbauswahl */
}

void draw_diagram(void)
{
	settextstyle(0,0,1);								/* Textstyle:(Font/Richtung/Gr��e) */
	moveto(100,99);										/* zeichne die 2 Koordinatenachsen */
	lineto(100,355);
	lineto(585,355);
	for(ya=355;ya>=105;ya-=25)							/* zeichne Skala f�r y-Achse */
		{
			moveto(100,ya);
			lineto(95,ya);
		}
	for(xa=100;xa<=580;xa+=40)							/* zeichne Skala f�r x-Achse */
		{
			moveto(xa,355);
			lineto(xa,360);
		}
	outtextxy(65,352,"750");							/* Textbeschriftungen */
	outtextxy(65,327,"775");
	outtextxy(65,302,"800");
	outtextxy(65,277,"825");
	outtextxy(65,252,"850");
	outtextxy(65,227,"875");
	outtextxy(65,202,"900");
	outtextxy(65,177,"925");
	outtextxy(65,152,"950");
	outtextxy(65,127,"975");
	outtextxy(58,102,"1000");
	outtextxy(326,370,"Zeit");
	settextstyle(0,1,1);								/* Textstyle:(Font/Richtung/Gr��e) */
	outtextxy(45,190,"Temperatur");
}

int main (void)											/* Hauptprogramm */
{
	clock_t now,alt;
	FILE *datei;
	datei=fopen("HZTEMP.DAT", "w+");					/* Erstellt NEUE Datei zum Lesen/Schreiben */
	if (datei==NULL)									/* Fehler bei Dateierstellung */
		printf("Dateierstellungsfehler!");

	init_graphic();										/* Aufruf UP Grafikinitialisierung */
	draw_diagram();										/* Aufruf UP Diagramm zeichnen */

  initcom();											/* Ausf�hren von Programm "initcom.c" */
  alt=clock();
  while(key!=27)										/* Beginn Schleife Werteabfrage solange bis Taste ESC */
  {

	now=clock();
	if(kbhit())
		{
			key=getch();								/* Bei Tastatureingabe, Taste speichern */
			if (key!= 113)								/* bei "q" Wert um 1 erh�hen */
				ibyte--;
			if (key!= 97)								/* bei "a" Wert um 1 erniedrigen */
				ibyte++;
		}
	if((now-alt)/CLOCKS_PER_SEC>=1)						/* diese Schleife wird sek�ndlich ausgef�hrt */
	{
		twert=0;										/* R�cksetzen twert-Variable */
		outportb(com,150);								/* Ausgabe Wert �ber COM zum Testen */
		/* ibyte=inportb(com);							/* Einlesen von COM2 (nur mit Br�cke m�glich) */
		twert=ibyte;									/* Zwischenspeicherung Temperaturwert */
		for(fz=60;fz>=1;fz--)							/* Datenverschiebung um 1 Feld in Feldvariable */
			{
				tempwerte[fz-1]=tempwerte[fz-2];		/* aktuelles Feld = Feldwert von Feld davor */
				zeiten[fz]=fz;							/* Zuweisung 1-60 an Feldvariable zeiten[60] */
				/* printf("feld %u = %u\n",fz-1,tempwerte[fz-1]); */
			}
		tempwerte[0]= twert;							/* aktueller Wert an erstes Feld */
		clearviewport();								/* Grafikbildschirm r�cksetzen */
		draw_diagram();									/* Aufruf UP Diagramm zeichnen */
		for(m=0;m<59;m++)								/* stelle Werte grafisch dar */
		{
			moveto(8*m+100,(355-tempwerte[m]));
			lineto(8*(m+1)+100,(355-tempwerte[m+1]));
		}
		zeitanzeige();									/* Aufruf Funktion "Zeitanzeige" */
		alt=clock();

		datei=fopen("HZTEMP.DAT", "a+");				/* �ffnet bestehende Datei zum Lesen/Schreiben */
		if (datei==NULL)								/* Fehler bei Dateierstellung */
			printf("Dateierstellungsfehler!");
		else
		{
			fprintf(datei,"%s\t%u\n",zeit,twert+750);	/* Schreibe Werte in Datei */
			/* printf("%s\t%u\n",zeit,twert); */
		}
	    fclose(datei);									/* Schliesse Datei */
	}
  }
  closegraph();											/* Grafikmodus beenden */
  return 0;
}
