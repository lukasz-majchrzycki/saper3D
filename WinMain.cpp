#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

HDC hdc,mem,hdc2;
static HGLRC hrc;
static HWND hwnd,but_hwnd[300],pas_hwnd,ramka_hwnd,dialog;
HBITMAP flaga,mina;
HINSTANCE gl;
HACCEL haccel;

RECT rect;
UINT timer;
POINT mysz,mysz2;

//****poziomy
int ppx=4,ppy=4,ppz=3,ppm=4;
int psx=5,psy=5,psz=5,psm=10;
int pzx=7,pzy=7,pzz=5,pzm=25;
int pex=10,pey=10,pez=5,pem=50;
//****koniec poziomów

int wys=570,szer=550;
int x=ppx,y=ppy,z=ppz,nr,nr2,max_min=ppm;
int x1=x,y1=y,z1=z;
int pozx,pozy,pozz,p;
bool miny[500];
int pl[500];
int stx=0,sty=40,pos=2;
int zwoln=0;
bool ruch=0;
int czas=0,poz_min,pol;
int ok=1;
int poziom;

int q,i,j,k,xy,mx,my;

const int rozm=15;

char str[120],str2[120],t_wynik[4][30];

LRESULT CALLBACK obsluga_kom (HWND,UINT,WPARAM,LPARAM);
BOOL CALLBACK niest_kom(HWND, UINT, WPARAM,LPARAM);
BOOL CALLBACK oprog_kom(HWND, UINT, WPARAM,LPARAM);
BOOL CALLBACK wyniki_kom(HWND, UINT, WPARAM,LPARAM);
BOOL CALLBACK wpis_kom(HWND, UINT, WPARAM,LPARAM);

void wynik()
{
/*HANDLE hExe,hUpdateRes;
HRSRC hRes;

hExe = LoadLibrary("Saper.exe");

hUpdateRes = BeginUpdateResource("Saper.exe", 1);
//hRes = FindResource(hExe, MAKEINTRESOURCE(4001), RT_STRING);

char *lpResLock="Majcher";
 if(!UpdateResource(hUpdateRes,RT_STRING,MAKEINTRESOURCE(4001),
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
                    lpResLock,100)  )
                    MessageBox(NULL, "NULL", "Saper",MB_OK);

EndUpdateResource(hUpdateRes, FALSE);*/

DWORD i,j,k;
int q=0,c;
char n[5];
HANDLE plik;
  plik=CreateFile("saper.lib",
              GENERIC_READ|GENERIC_WRITE,
              0,
              0,
              OPEN_ALWAYS,
              FILE_ATTRIBUTE_ARCHIVE,
              0);


ReadFile(plik,str,120,&i,NULL);
if(i!=120)
{
   sprintf(str,"9999s.          Nikt         *9999s.          Nikt         *9999s.          Nikt         *9999s.          Nikt         *");
   WriteFile(plik,str,120,&i,0);
}
CloseHandle(plik);
if(x==ppx&&y==ppy&&z==ppz&&max_min==ppm)  poziom=0;
else if(x==psx&&y==psy&&z==psz&&max_min==psm) poziom=1;
else if(x==pzx&&y==pzy&&z==pzz&&max_min==pzm) poziom=2;
else if(x==pex&&y==pey&&z==pez&&max_min==pem) poziom=3;
else return;

j=poziom*30;

for(k=0;;j++,k++)
{
 if(*(str+j)!='s') *(n+k)=*(str+j);
 else break;
}

c=atoi(n);
if(czas<c) DialogBox(gl,"Wpis",hwnd,wpis_kom);


SendMessage(hwnd,WM_COMMAND,1105,0);
}

void nowa()
{
  srand(time(0));
  xy=x*y;
  j=xy*z;

  for(i=0;i<j;i++)
   *(pl+i)=-1;

  for(i=0;i<max_min;i++)
  {
   q=rand()%j;
   if(*(miny+q)==1) i--;
   else *(miny+q)=1;
  }
}


void flagi(int q)
{
 int i,j,k;
 DWORD styl;



 if(pos==1)j=x*y;
 else j=0;

 if(pos==z)k=2*x*y;
 else k=3*x*y;

  for(i=j;i<k;i++)
     {
      if(*(pl+(pos-2)*x*y+i)==-2)
       {
        if(q==0&&(*(miny+(pos-2)*x*y+i)==0))  styl=NOTSRCCOPY;
        else styl=MERGECOPY;

        hdc2=GetDC(*(but_hwnd+i));
        mem=CreateCompatibleDC(hdc2);
        SelectObject(mem,flaga);

        BitBlt(hdc2,0,0,rozm,rozm,mem,0,0,styl);
        ReleaseDC(*but_hwnd+i,hdc2);
        DeleteDC(mem);
       }
      else if((*(miny+(pos-2)*x*y+i)==1)&&q==0)
       {
        hdc2=GetDC(*(but_hwnd+i));
        mem=CreateCompatibleDC(hdc2);
        SelectObject(mem,mina);

        BitBlt(hdc2,0,0,rozm,rozm,mem,0,0,MERGECOPY);
        ReleaseDC(*but_hwnd+i,hdc2);
        DeleteDC(mem);
       }
     }

}

void mina1(int i)
{
        hdc2=GetDC(*(but_hwnd+i));
        mem=CreateCompatibleDC(hdc2);
        SelectObject(mem,mina);

        BitBlt(hdc2,0,0,rozm,rozm,mem,0,0,MERGECOPY);
        ReleaseDC(*but_hwnd+i,hdc2);
        DeleteDC(mem);
}

void flaga1(int i)
{
        hdc2=GetDC(*(but_hwnd+i));
        mem=CreateCompatibleDC(hdc2);
        SelectObject(mem,flaga);

        BitBlt(hdc2,0,0,rozm,rozm,mem,0,0,MERGECOPY);
        ReleaseDC(*but_hwnd+i,hdc2);
        DeleteDC(mem);
}

void zlicz(int x1,int y1,int z1)
{
 int ile=0;
 int i,j,k,q;

 for(i=-1;i<=1;i++)        //z
  for(j=-1;j<=1;j++)       //y
   for(k=-1;k<=1;k++)      //x
   {
    if((x1+k)<0||(x1+k)>=x||(y1+j)<0||(y1+j)>=y||(z1+i)<0||(z1+i)>=z) continue;
    else if((k==0)&&(j==0)&&(i==0)) continue;
     else if(*(miny+(z1+i)*x*y+(x1+k)*y+(y1+j))==1)      ile++;
   }

   *(pl+z1*x*y+x1*y+y1)=ile;
    pol--;

   q=(z1-pos+2);
    if((q>=0)&&(q<=2))
    {
      EnableWindow(*(but_hwnd+q*x*y+x1*y+y1),0);
      SendMessage(*(but_hwnd+q*x*y+x1*y+y1), BM_SETSTATE,1,0);
      SetFocus(hwnd);

      if(ile!=0)
      {
       sprintf(str,"%d",ile);
       SetWindowText(*(but_hwnd+q*x*y+x1*y+y1),str);
      }
    }

    if(pol==0)
     {
        KillTimer(hwnd,timer);
        ruch=0;
        ok=0;
        j=2*x*y;
        for(i=x*y;i<j;i++)
         EnableWindow(*(but_hwnd+i),0);
       // MessageBox(NULL, "WYGRA£EŒ!!!", "Saper",MB_OK);
        wynik();
        return;
     }


  if(ile==0)
   {
   for(i=-1;i<=1;i++)        //z
    for(j=-1;j<=1;j++)       //y
     for(k=-1;k<=1;k++)      //x
      {
       if((x1+k)<0||(x1+k)>=x||(y1+j)<0||(y1+j)>=y||(z1+i)<0||(z1+i)>=z) continue;
       else if((k==0)&&(j==0)&&(i==0)) continue;
       else if(*(pl+(z1+i)*x*y+(x1+k)*y+(y1+j))==-1)  zlicz(x1+k,y1+j,z1+i);
      }
   }

}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PSTR szCmdLine,int iCmdShow)
{
 MSG msg;
 gl=hInstance;

 WNDCLASS okno;
 okno.style             =  CS_VREDRAW | CS_HREDRAW;
 okno.cbClsExtra		= 0;
 okno.cbWndExtra		= 0;
 okno.hInstance		= hInstance;
 okno.hIcon			= LoadIcon(hInstance,"Ikona");
 okno.hCursor		= LoadCursor(NULL,IDC_ARROW);
 okno.hbrBackground  =(HBRUSH)(COLOR_BTNFACE+1);
 okno.lpszMenuName	= "MENU1";
 okno.lpfnWndProc=obsluga_kom;
 okno.lpszClassName="okienko";

 if(!RegisterClass(&okno))
 {
  MessageBox(NULL,"Nie uda³o siê zarejestrowaæ klasy okna1","B£¥D",MB_ICONERROR);
 }

 hwnd=CreateWindow("okienko",
				   "Saper 3D   # by Majcher",
	               WS_VISIBLE | WS_SYSMENU | WS_POPUP | WS_CAPTION | WS_MINIMIZEBOX,            // styl okna
   				  100,0,  // pocz¹tkowa pozycja x,y
	               szer,wys,  // pocz¹tkowy rozmiar x,y
	               NULL,
				   NULL,
				   hInstance,
				   NULL);

  haccel=LoadAccelerators(hInstance,"Accel");
  flaga=LoadBitmap(hInstance,"FLAGA");
  mina=LoadBitmap(hInstance,"MINA");


 ShowWindow(hwnd,SW_SHOW);
 UpdateWindow(hwnd);

 while(GetMessage(&msg,NULL,0,0))
 {
  if(!TranslateAccelerator(hwnd,haccel,&msg))
  {
   TranslateMessage(&msg);
   DispatchMessage(&msg);
  }
 }
return msg.wParam;
}


LRESULT CALLBACK obsluga_kom(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
static PAINTSTRUCT ps;

switch(message)
 {

  case WM_CREATE:
  nowa();
  nr=0;
  i=HIWORD(GetDialogBaseUnits())/2;
  ramka_hwnd=CreateWindow("BUTTON",NULL,
                  BS_GROUPBOX|WS_CHILD|WS_VISIBLE,
                  (x+1)*rozm+stx-(rozm/2),
                  (y+1)*rozm+sty-(rozm/2)-i,
                  (x+1)*rozm,
                  (y+1)*rozm+i,
                  (HWND)hwnd,(HMENU)NULL,
                  GetModuleHandle(NULL),NULL);

  for(pozz=0;pozz<3;pozz++)
  {
  if(pozz==1)  nr2=99;
  else  nr2=0;
   for(pozx=1;pozx<=x;pozx++)
   {
    for(pozy=1;pozy<=y;pozy++)
    {
     if(pozz==1) nr2++;
     p=rozm*pozz;
     *(but_hwnd+nr)=CreateWindow("BUTTON","",
                  BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE,
                  (rozm*pozx)+p*x+stx,
                  (rozm*pozy)+p*y+sty,
                  rozm,
                  rozm,
                  (HWND)hwnd,(HMENU)nr2,
                  GetModuleHandle(NULL),NULL);
     nr++;
    }
   }
  }


  for(i=0;i<=2;i+=2)
   for(j=0;j<xy;j++)
    EnableWindow(*(but_hwnd+i*xy+j),0);

  pas_hwnd=CreateWindow("SCROLLBAR","",
                  SBS_VERT|WS_CHILD|WS_VISIBLE,
                  50,300+sty,15,150,
                  (HWND)hwnd,(HMENU)0,
                  GetModuleHandle(NULL),NULL);

  SetScrollRange(pas_hwnd,SB_CTL,1,z,1);
  SetScrollPos(pas_hwnd,SB_CTL,pos,1);

  poz_min=max_min;
  pol=x*y*z-max_min;

  GetCursorPos(&mysz);
  break;




  case WM_KEYDOWN:
  if(wParam==VK_CONTROL)
  {

    mysz2=mysz;
    GetCursorPos(&mysz);
    ScreenToClient(hwnd,&mysz);

     i=(mysz.x-stx)/rozm;
     j=(mysz.y-sty)/rozm;

     k=(mysz2.x-stx)/rozm;
     q=(mysz2.y-sty)/rozm;

     if((i==k)&&(j==q)&&zwoln==1) break;
     if(zwoln==0)
     {
     zwoln=1;
     if((i>x)&&(i<=(2*x))&&(j>y)&&(j<=(2*y)))
     {
       hdc = GetDC(hwnd);

       SelectObject(hdc,GetStockObject(NULL_BRUSH));
       SelectObject(hdc,CreatePen(PS_SOLID,2,RGB(255,0,0)));


       if(pos!=1)Ellipse(hdc,(i*rozm)-(rozm*x+3)+stx,(j*rozm)-(rozm*y+3)+sty,(i*rozm)-(rozm*x-17)+stx,(j*rozm)-(rozm*y-17)+sty);
       if(pos!=z)Ellipse(hdc,(i*rozm)+(rozm*x-3)+stx,(j*rozm)+(rozm*y-3)+sty,(i*rozm)+(rozm*x+17)+stx,(j*rozm)+(rozm*y+17)+sty);
       ReleaseDC(hwnd,hdc);
      }
   }

  }
  break;

  case WM_KEYUP:
   GetCursorPos(&mysz);
   ScreenToClient(hwnd,&mysz);
   zwoln=0;
   if(wParam==VK_CONTROL)
   {
    SetRect(&rect,stx,sty,(rozm*x)+20+stx,(rozm*y)+20+sty);
    InvalidateRect(hwnd,&rect,TRUE);
    SetRect(&rect,(30*x)-10+stx,(30*y)-10+sty,(45*x)+20+stx,(45*y)+20+sty);
    InvalidateRect(hwnd,&rect,TRUE);

   }
  break;

  case WM_CLOSE:
  DeleteObject(flaga);
  KillTimer(hwnd,timer);
   PostQuitMessage(0);
  break;

  case WM_PAINT:
   hdc=BeginPaint(hwnd,&ps);
   sprintf(str,"Warstwa : %d / %d",pos,z);
   SetBkMode(hdc,TRANSPARENT);
   TextOut(hdc,30,250+sty,str,strlen(str));

   sprintf(str,"Czas : %ds",czas);
   TextOut(hdc,15,15,str,strlen(str));

   sprintf(str,"Pozosta³o %d min",poz_min);
   TextOut(hdc,szer-150,15,str,strlen(str));

   EndPaint(hwnd,&ps);
   flagi(ok);

  break;



 case WM_COMMAND:

 if(LOWORD(wParam)<1000)
 {
    if(ruch==0&&pol!=0&&ok==1)
    {
     ruch=1;
     SetTimer(hwnd,timer,1000,NULL);
    }

   if((*(miny+(pos-1)*x*y+wParam-100)==1)&&(*(pl+(pos-1)*x*y+wParam-100)!=-2)&&pol!=0)
   {
    ok=0;
    ruch=0;
    j=2*x*y;
    for(i=x*y;i<j;i++)
     EnableWindow(*(but_hwnd+i),0);
//    MessageBox(NULL, "BUUUUM!!!!!", "Saper",MB_OK);
    flagi(0);


    if(ruch==0&&pol!=0)
       KillTimer(hwnd,timer);
    break;
   }



  if(*(pl+(pos-1)*x*y+wParam-100)==-1&&ok==1)
  {
   k=(wParam-100)%y;
   zlicz( ((wParam-100-k)/y), k, pos-1);
  /* *(pl+(pos-1)*x*y+wParam-100)=q;
   if(q!=0)
   {
    sprintf(str,"%d",q);

  //  CrateFontIndirect(15,5,0,0,400,0,0,0,0,0,0,0,0,"Arial");

    SetWindowText(*(but_hwnd+wParam-100+x*y),str);
   } */
   }
   else { flaga1(wParam-100+x*y); break; }

  }
  else
  {
   switch(LOWORD(wParam))
   {
    case 1001:                  //gra-nowa
     KillTimer(hwnd,timer);
     ok=1;
     ruch=0;
     czas=0;
     j=x*y*z;
     for(i=0;i<j;i++)
      *(miny+i)=0;
     j=3*x*y;
     for(i=0;i<j;i++)
      DestroyWindow(*(but_hwnd+i));
     DestroyWindow(pas_hwnd);
     x=x1;
     y=y1;
     z=z1;
     DestroyWindow(ramka_hwnd);
     SendMessage(hwnd,WM_CREATE,0,0);

     SetRect(&rect,szer-150,15,szer,30);
     InvalidateRect(hwnd,&rect,TRUE);
     SetRect(&rect,15,15,100,30);
     InvalidateRect(hwnd,&rect,TRUE);
     if(pos==1)
     {
      j=x*y;
      for(i=0;i<j;i++)
       ShowWindow(*(but_hwnd+i),SW_HIDE);
     }
     if(pos==z)
     {
      j=3*x*y;
       for(i=2*x*y;i<j;i++)
       ShowWindow(*(but_hwnd+i),SW_HIDE);
     }
     if(pos>z) SendMessage(hwnd,WM_VSCROLL,MAKEWPARAM(SB_THUMBTRACK,z),0);
     else
     {
       SetRect(&rect,30+stx,250+sty,150+stx,300+sty);
       InvalidateRect(hwnd,&rect,TRUE);
     }
    break;

    case 1002:                 //gra-wyjœcie
     DeleteObject(flaga);
     KillTimer(hwnd,timer);
     PostQuitMessage(0);
    break;

    case 1100:        //pocz¹tkuj¹cy
     x1=ppx;
     y1=ppy;
     z1=ppz;
     max_min=ppm;
     stx=0;
     SendMessage(hwnd,WM_COMMAND,1001,0);
    break;

    case 1101:        //Œredniozaawansowany
     x1=psx;
     y1=psy;
     z1=psz;
     max_min=psm;
     stx=0;
     SendMessage(hwnd,WM_COMMAND,1001,0);
    break;

    case 1102:       //Zaawansowany
     x1=pzx;
     y1=pzy;
     z1=pzz;
     max_min=pzm;
     stx=0;
     SendMessage(hwnd,WM_COMMAND,1001,0);
    break;

    case 1103:      //Ekspert
     x1=pex;
     y1=pey;
     z1=pez;
     max_min=pem;
     stx=0;
     SendMessage(hwnd,WM_COMMAND,1001,0);
    break;

    case 1104:
     if(DialogBox(gl,"Niest",hwnd,niest_kom)) SendMessage(hwnd,WM_COMMAND,1001,0);
    break;

    case 1011:     //O programie
     DialogBox(gl,"Oprog",hwnd,oprog_kom);
    break;

    case 1105:     //Najlepsze wyniki
     DialogBox(gl,"Wyniki",hwnd,wyniki_kom);
    break;
   }
  }
   break;

 case WM_CONTEXTMENU:
 q=0;
 for(i=x*y;i<2*x*y;i++)
  if(wParam==(int)*(but_hwnd+i))  {q=1;   break;   }

 if(q==1)
 {
  if(*(pl+(pos-2)*x*y+i)==-1)
  {
   *(pl+(pos-2)*x*y+i)=-2;
    flaga1(i);
    poz_min--;
    SetRect(&rect,szer-150,15,szer,30);
    InvalidateRect(hwnd,&rect,TRUE);
   }
   else if(*(pl+(pos-2)*x*y+i)==-2&&pol!=0)
   {
    *(pl+(pos-2)*x*y+i)=-1;
    SetWindowText(*(but_hwnd+i)," ");
    poz_min++;
    SetRect(&rect,szer-150,15,szer,30);
    InvalidateRect(hwnd,&rect,TRUE);
   }

 }
 break;

 case WM_TIMER:
   czas++;
   SetRect(&rect,15,15,100,30);
   InvalidateRect(hwnd,&rect,TRUE);
 break;

 case WM_VSCROLL:
  if(((LOWORD(wParam)==1)||(LOWORD(wParam)==3))&&(pos<z))  pos++;
  else if(((LOWORD(wParam)==0)||(LOWORD(wParam)==2))&&(pos>1))pos--;
  else if(LOWORD(wParam)==SB_THUMBTRACK) pos=HIWORD(wParam);
  else break;


  SetScrollPos(pas_hwnd,SB_CTL,pos,1);


  //--------------------------
  xy=x*y;

 if(pos!=1)
  {
    for(i=0;i<xy;i++)
    {

     q=*(pl+(pos-2)*xy+i);
     ShowWindow(*(but_hwnd+i),SW_SHOWDEFAULT);

     if(q==-1)            SendMessage(*(but_hwnd+i),BM_SETSTATE,0,0);
     else if(q>=0)        SendMessage(*(but_hwnd+i),BM_SETSTATE,1,0);


     if(q>0)
     {
      sprintf(str,"%d",q);
      SetWindowText(*(but_hwnd+i),str);
     }
     else SetWindowText(*(but_hwnd+i)," ");
    }
   }
   else
    for(i=0;i<xy;i++)
     ShowWindow(*(but_hwnd+i),SW_HIDE);
//------------------------------------------------
  for(i=0;i<xy;i++)
   {
    q=*(pl+(pos-1)*xy+i);
    if(q==-1)             {SendMessage(*(but_hwnd+i+xy),BM_SETSTATE,0,0); j=1;}
    else if(q>=0)         { SendMessage(*(but_hwnd+i+xy),BM_SETSTATE,1,0); j=0;}
    else j=1;

    if(ok==1)EnableWindow(*(but_hwnd+i+xy),j);

     if(q>0)
     {
      sprintf(str,"%d",q);
      SetWindowText(*(but_hwnd+i+xy),str);
     }
     else SetWindowText(*(but_hwnd+i+xy)," ");



   }
//-----------------
  k=2*xy;

 if(pos!=z)
  {
    for(i=0;i<xy;i++)
    {
     ShowWindow(*(but_hwnd+i+k),SW_SHOWNORMAL);

     q=*(pl+pos*xy+i);
     if(q==-1)        SendMessage(*(but_hwnd+i+k),BM_SETSTATE,0,0);
     else if(q>=0)    SendMessage(*(but_hwnd+i+k),BM_SETSTATE,1,0);

     if(q>0)
      {
       sprintf(str,"%d",q);
       SetWindowText(*(but_hwnd+i+k),str);
      }
      else SetWindowText(*(but_hwnd+i+k)," ");
    }
   }
   else
    for(i=0;i<xy;i++)
     ShowWindow(*(but_hwnd+i+k),SW_HIDE);


 SetFocus(hwnd);

  SetRect(&rect,30,250+sty,150,300+sty);
  InvalidateRect(hwnd,&rect,TRUE);

 break;

 case WM_SIZE:
  if(wParam==SIZE_MINIMIZED) KillTimer(hwnd,timer);
  else if(ruch==1&&pol!=0) SetTimer(hwnd,timer,1000,NULL);
 break;

 default: return DefWindowProc(hwnd,message,wParam,lParam);
 }
}


BOOL CALLBACK niest_kom(HWND hDlg, UINT iMsg, WPARAM wParam,LPARAM lParam)
{
switch(iMsg)
 {
  case WM_COMMAND:
   switch(LOWORD(wParam))
   {
   case 2005:
   {
   HWND okno;
   POINT po;
//-----------
   po.x=116;
   po.y=42;
   ClientToScreen(hDlg,&po);
   okno=WindowFromPoint(po);
    if(!GetWindowText(okno,str,GetWindowTextLength(okno)+1))
    {
     EndDialog(hDlg,0);
     return 1;
    }
    else
    {
     i=atoi(str);
     if(i!=0)
      x1=i;
     else
     {
      EndDialog(hDlg,0);
      return 1;
     }
    }
//----------------------
   po.x=116;
   po.y=75;
   ClientToScreen(hDlg,&po);
   okno=WindowFromPoint(po);
   if(!GetWindowText(okno,str,GetWindowTextLength(okno)+1))
    {
     EndDialog(hDlg,0);
     return 1;
    }
    else
    {
     i=atoi(str);
     if(i!=0)
      y1=i;
     else
     {
      EndDialog(hDlg,0);
      return 1;
     }
    }
 //-----------------------------
   po.x=116;
   po.y=108;
   ClientToScreen(hDlg,&po);
   okno=WindowFromPoint(po);
   if(!GetWindowText(okno,str,GetWindowTextLength(okno)+1))
    {
     EndDialog(hDlg,0);
     return 1;
    }
    else
    {
     i=atoi(str);
     if(i!=0)
      z1=i;
     else
     {
      EndDialog(hDlg,0);
      return 1;
     }
    }
//----------------------------
   po.x=130;
   po.y=141;
   ClientToScreen(hDlg,&po);
   okno=WindowFromPoint(po);
   if(!GetWindowText(okno,str,GetWindowTextLength(okno)+1))
    {
     EndDialog(hDlg,0);
     return 1;
    }
    else
    {
     i=atoi(str);
     if(i!=0)
      max_min=i;
     else
     {
      EndDialog(hDlg,0);
      return 1;
     }
    }
//-----------------------------
    if(x1>10) x1=10;
    if(y1>10) y1=10;
    if((x1*y1*z1)>500) z1=500/(x1*y1);
    if(max_min>=(x1*y1*z1)) max_min=(x1*y1*z1)-1;
    stx=150-x1*rozm;
    EndDialog(hDlg,1);
   return 1;
   }      
//////////////////////
   case IDCANCEL:
    EndDialog(hDlg,0);
   return 1;
  default: return 0;
  }
  default: return 0;
 }
}

BOOL CALLBACK oprog_kom(HWND hDlg, UINT iMsg, WPARAM wParam,LPARAM lParam)
{
 switch (iMsg)
 {
  case WM_COMMAND:
   EndDialog(hDlg,0);
  return 1;

  default: return 0;
 }
}

BOOL CALLBACK wyniki_kom(HWND hDlg, UINT iMsg, WPARAM wParam,LPARAM lParam)
{
 switch (iMsg)
 {
  case WM_COMMAND:
  if(wParam==IDOK||wParam==IDCANCEL)
   EndDialog(hDlg,0);
  else if(wParam==4002)
  {
  DWORD i;
      HANDLE plik;
      plik=CreateFile("saper.lib",
              GENERIC_READ|GENERIC_WRITE,
              0,
              0,
              OPEN_ALWAYS,
              FILE_ATTRIBUTE_ARCHIVE,
              0);
   sprintf(str,"9999s.          Nikt         *9999s.          Nikt         *9999s.          Nikt         *9999s.          Nikt         *");
   WriteFile(plik,str,120,&i,0);
   CloseHandle(plik);

   SendMessage(hDlg,WM_INITDIALOG,0,0);
   //InvalidateRect(hDlg,NULL,TRUE);
   SendMessage(hDlg,DM_REPOSITION ,0,0);
//   RedrawWindow(hDlg,NULL,NULL,RDW_INVALIDATE);
//   RedrawWindow(hDlg,NULL,NULL,RDW_VALIDATE);
   EndDialog(hDlg,0);


  }
  return 1;

  case WM_PAINT:
   hdc2=GetDC(hDlg);
    SetBkMode(hdc2,TRANSPARENT);
    SelectObject(hdc2,CreateFont(8,4,0,0,400,0,0,0,0,0,0,0,0,"Helv"));
    sprintf(str,"Pocz¹tkuj¹cy :");
    TextOut(hdc2,30,40,str,strlen(str));
    sprintf(str,"Œredniozaawansowany :");
    TextOut(hdc2,30,60,str,strlen(str));
    sprintf(str,"Zaawansowany :");
    TextOut(hdc2,30,80,str,strlen(str));
    sprintf(str,"Expert :");
    TextOut(hdc2,30,100,str,strlen(str));
//    sprintf(str,"Niestandardowy:");
//    TextOut(hdc2,30,120,str,strlen(str));
    TextOut(hdc2,160,40,t_wynik[0],strlen(t_wynik[0]));
    TextOut(hdc2,160,60,t_wynik[1],strlen(t_wynik[1]));
    TextOut(hdc2,160,80,t_wynik[2],strlen(t_wynik[2]));
    TextOut(hdc2,160,100,t_wynik[3],strlen(t_wynik[3]));
//    TextOut(hdc2,160,120,t_wynik[4],strlen(t_wynik[4]));
   ReleaseDC(hDlg,hdc2);
  return 0;

  case WM_INITDIALOG:
  {
   HANDLE h;
   DWORD i,j,k,l=0,m=0;
   h=CreateFile("saper.lib",GENERIC_READ|GENERIC_WRITE,0,0,OPEN_ALWAYS,FILE_ATTRIBUTE_ARCHIVE,0);
   ReadFile(h,str2,120,&i,NULL);
   CloseHandle(h);
   if(i!=120)   SendMessage(hDlg,WM_COMMAND,4002,0);
   for(j=0;j<=i;j++)
    {
     if(*(str2+j)=='*')
      {
       for(k=0;k<=(j-l-1);k++)
          t_wynik[m][k]=*(str2+k+l);
       j++;
       l=j;
       m++;
      }
    }

  }
  return 0;

  default: return 0;
 }
}


BOOL CALLBACK wpis_kom(HWND hDlg, UINT iMsg, WPARAM wParam,LPARAM lParam)
{
 switch (iMsg)
 {
  case WM_COMMAND:
  if(wParam==IDOK)
  {
   DWORD i,j,q;
   int dl;
   POINT po;
   HWND okno;
   char n[5];
   HANDLE plik;

   i=poziom*30;
   po.x=117;
   po.y=19;
   ClientToScreen(hDlg,&po);
   okno=WindowFromPoint(po);
   dl=GetWindowTextLength(okno);
   if(dl>13) {  MessageBox(NULL, "Imiê jest zbyt d³ugie", "Saper",MB_OK); return 1; }
   GetWindowText(okno,str2,dl+1);
   plik=CreateFile("saper.lib",
              GENERIC_READ|GENERIC_WRITE,
              0,
              0,
              OPEN_ALWAYS,
              FILE_ATTRIBUTE_ARCHIVE,
              0);
   itoa(czas,n,10);

   for(j=0;j<=5;j++)
    if(*(n+j)=='\x0') {q=j; break; }

   for(j=0;j<4-q;j++,i++)
      *(str+i)=' ';

   for(j=0;j<q;i++,j++)
      *(str+i)=*(n+j);

  i+=2;

  for(j=0;j<10;j++,i++)
    *(str+i)=' ';

   for(j=0;j<dl;j++,i++)
      *(str+i)=*(str2+j);

  for(;j<12;j++,i++)
      *(str+i)=' ';

  // MessageBox(NULL, str, "Saper",MB_OK);

   WriteFile(plik,str,120,&i,0);
   CloseHandle(plik);

   EndDialog(hDlg,0);
  }

  return 1;

  default: return 0;
 }
}

