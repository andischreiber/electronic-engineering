/*

                             *******************

******************************* C SOURCE FILE *******************************

**                           *******************                           **

**                                                                         **

** project   : Reihenschwingkreis darstellen                               **

** filename  : RLC.C                                                       **

** version   : 1                                                           **

** date      : Oktober 24, 2006                                            **

**                                                                         **

*****************************************************************************

**                                                                         **

** Copyright (c) 2006, A. Schreiber                                        **

** All rights reserved.                                                    **

**                                                                         **

*****************************************************************************

*/

#include <stdio.h>
#include <graphics.h>
#include <math.h>

int px(double x)
{
  return (int)(getmaxx()*x/100);
}

int py(double y)
{
  return (int)(getmaxy()*y/100);
}


main(void)
{
  double x,y,f,omega,phi,pi=3.1416,r=500,l=0.020,c=0.00000001,im;
  int treiber,modus;
  treiber=DETECT;
  initgraph(&treiber,&modus,"");
  setcolor(YELLOW);
  moveto(px(10),py(20));
  lineto(px(10),py(80));
  moveto(px(10),py(50));
  lineto(px(90),py(50));
  outtextxy(px(12),py(20),"PHI");
  outtextxy(px(90),py(52),"f");
  setcolor(LIGHTGREEN);
  outtextxy(px(10),py(15),"Phasenverlauf eines Reihenschwingkreises");
  outtextxy(px(10),py(85),"Ende mit beliebiger Taste...");
  setcolor(LIGHTRED);
  moveto(px(10),py(80));
  for(f=0.1;f<=25500.1;f+=1000.0)
    {
      omega=2*pi*f;
      im=omega*l-1/(omega*c);
      phi=atan(im/r);
      x=px(10)+(px(80/25500.1*f));
      y=py(80)-py(60)/2-(py(60)/pi*phi);
      lineto(x,y);
    }
  getch();
  closegraph();
  return(0);
}
