/**************************************************************************************************
 * Semesterprojekt 2006
 *
 * Grafische Temperaturanzeige der Werte von RS232 und Archivierung in Datei "HZTEMP.DAT"
 * (Diese Version des Programms läuft nur bei Dateneingang an COM1. Zum Testen gibt dieses Programm selbst
 * einen Wert über COM1 aus, welcher beim Stecken einer Brücke wieder eingelesen wird.
 * Im Praxisfall würde jede Sekunde ein Wert an COM1 anstehen. Beim Test des Programmes steht der Wert
 * welcher über COM1 ausgegeben wird aber öfters als 1x pro Sekunde neu an; deshalb ist die Zeitskala des
 * Diagramms nicht mehr laut Vorgabe (60 Sek.).
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
char zeit[8];									/* Feldvariable für Zeit hh:mm:ss */
int zeiten[60];									/* Feldvariable 60 Elemente (Zeit) */
int tempwerte[60];								/* Feldvariable 60 Elemente (Werte) */
int ibyte;										/* Variable für Einlesen COM */
int twert;										/* Variable für Zwischenspeicherung Tempwert */
int fz,m;										/* Variablen für Schieberegister Feldvariablen */
int x,y,xold,yold;								/* Variablen für Grafikmodus */
int xa,ya,xrange,yrange,treiber,modus;
int key=0;										/* Variable für Tastenabfrage */

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
	settextstyle(0,0,1);								/* Textstyle:(Font/Richtung/Größe) */
	moveto(100,99);										/* zeichne die 2 Koordinatenachsen */
	lineto(100,355);
	lineto(585,355);
	for(ya=355;ya>=105;ya-=25)							/* zeichne Skala für y-Achse */
		{
			moveto(100,ya);
			lineto(95,ya);
		}
	for(xa=100;xa<=580;xa+=40)							/* zeichne Skala für x-Achse */
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
	settextstyle(0,1,1);								/* Textstyle:(Font/Richtung/Größe) */
	outtextxy(45,190,"Temperatur");
}

int main (void)											/* Hauptprogramm */
{
	FILE *datei;
	datei=fopen("HZTEMP.DAT", "w+");					/* Erstellt NEUE Datei zum Lesen/Schreiben */
	if (datei==NULL)									/* Fehler bei Dateierstellung */
		printf("Dateierstellungsfehler!");

	init_graphic();										/* Aufruf UP Grafikinitialisierung */
	draw_diagram();										/* Aufruf UP Diagramm zeichnen */

  initcom();											/* Ausführen von Programm "initcom.c" */
  while(key!=27)										/* Beginn Schleife Werteabfrage solange bis Taste ESC */
  {
	/* Um die Programmfunktion der COM-Schnittstelle zu Testen muss Pin 2 und 3 der COM1 gebrückt werden.
	   Über OUTPORT wird ein Wert seriell ausgegeben und sogleich über INPORT wieder eingelesen. */

	if(kbhit())
		{
			key=getch();								/* Bei Tastatureingabe, Taste speichern */
		}
	outportb(com,150);									/* Ausgabe Wert über COM zum Testen */
	if(inportb(com+5)&1)								/* Neuer Wert an COM, Schleife ausführen */
	{
		twert=0;										/* Rücksetzen twert-Variable */
		ibyte=inportb(com);								/* Einlesen von COM (nur mit Brücke möglich) */
		twert=ibyte;									/* Zwischenspeicherung Temperaturwert */
		for(fz=60;fz>=1;fz--)							/* Datenverschiebung um 1 Feld in Feldvariable */
			{
				tempwerte[fz-1]=tempwerte[fz-2];		/* aktuelles Feld = Feldwert von Feld davor */
				zeiten[fz]=fz;							/* Zuweisung 1-60 an Feldvariable zeiten[60] */
				/* printf("feld %u = %u\n",fz-1,tempwerte[fz-1]); */

			}
		tempwerte[0]= twert;							/* aktueller Wert an erstes Feld */
		clearviewport();								/* Grafikbildschirm rücksetzen */
		draw_diagram();									/* Aufruf UP Diagramm zeichnen */
		for(m=0;m<59;m++)								/* stelle Werte grafisch dar */
		{
			moveto(8*m+100,(355-tempwerte[m]));
			lineto(8*(m+1)+100,(355-tempwerte[m+1]));
		}
		zeitanzeige();									/* Aufruf Funktion "Zeitanzeige" */

		datei=fopen("HZTEMP.DAT", "a+");				/* Öffnet bestehende Datei zum Lesen/Schreiben */
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
