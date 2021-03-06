// Rollenmaß.cpp : Definiert den Einstiegspunkt der Anwendung.
//

#include "stdafx.h"
#include "Rollenmaß.h"
using namespace std;
#define MAX_LOADSTRING 100
Graphics* graphics;
thread* rollenmassMainThread;
bool run = true;
static mutex lockGraph;
static long double PI = 3.141592653589793238463;
// Globale Variablen:
HINSTANCE hInst;                                // Aktuelle Instanz
WCHAR szTitle[MAX_LOADSTRING];                  // Titelleistentext
WCHAR szWindowClass[MAX_LOADSTRING];            // Klassenname des Hauptfensters

// Vorwärtsdeklarationen der in diesem Codemodul enthaltenen Funktionen:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

/*
Globale Variablen
*/

int windowSizeX = 1200;									//Fenstergröße
int windowSizeY = 850;									//Fenstergröße
double posX = 600;										//Position des geplotteten Teils (mittelpunkt)
double posY = 400;										//Position des geplotteten Teils (mittelpunkt)
int vergroesserung = 8;									//Vergrößerungsfaktor des Teils
int vergroesserungNormale = 1;							//Vergrößerungsfaktor der Normalen an den Punkten
int windowPosX = 100;//-1920;								//Fensterposition
int windowPosY = 100;									//Fensterposition
double posEingabefeldX = (double)windowSizeX - 300.0f;	//Position des Hintergrundes für das Eingabefeld
double posEingabefeldY = 0.0;							//Position des Hintergrundes für das Eingabefeld
double groesseEingabefeldX = 300;						//Größe des Hintergrundes für das Eingabefeld
double groesseEingabefeldY = 180;						//Größe des Hintergrundes für das Eingabefeld
double alphaEingabefeld = 2.0;							//Durchsichtigkeit des Eingabefeld
double rollenmassGroesse = 15.0f;
double winkelRollenmass = 20.0f;
double winkelRollenmass2 = 0.0f;
double posRollenmassX = 0.0f;
double posRollenmassY = 0.0f;
double posRollenmassX2 = 0.0f;
double posRollenmassY2 = 0.0f;
bool weiterNachLinks = true;
bool weiterNachUnten = true;
bool weiterNachRechts = true;
bool weiterNachOben = true;
double geschwindigkeit1 = 1.0;
double geschwindigkeit2 = 1.0;
float rotWinkel = 0.0;
std::wstring textAbstandRollenmass;

void init() {
	posX = 600;										//Position des geplotteten Teils (mittelpunkt)
	posY = 400;										//Position des geplotteten Teils (mittelpunkt)
	vergroesserung = 8;								//Vergrößerungsfaktor des Teils
	vergroesserungNormale = 1;						//Vergrößerungsfaktor der Normalen an den Punkten
	alphaEingabefeld = 2.0;							//Durchsichtigkeit des Eingabefeld
	rollenmassGroesse = 15.0;
	winkelRollenmass = 20.0;
	winkelRollenmass2 = 0.0;
	posRollenmassX = 0.0;
	posRollenmassY = 0.0;
	posRollenmassX2 = 0.0;
	posRollenmassY2 = 0.0;
	weiterNachLinks = true;
	weiterNachUnten = true;
	weiterNachRechts = true;
	weiterNachOben = true;
	geschwindigkeit1 = 1.0;
	geschwindigkeit2 = 1.0;
	rotWinkel = 0.0;
}

/*
Text Variablen
*/

WCHAR* TextNormale = L"Vergrößerung Normale: ";
double posTextNormaleX = posEingabefeldX+5;
double posTextNormaleY = 15;
double posTextNormaleWidth = 200;
double posTextNormaleHeight = 100;

WCHAR* TextRollenmass = L"Rollenmaß größe: ";
double posTextRollenmassX = posEingabefeldX-5;
double posTextRollenmassY = 60;
double posTextRollenmassWidth = 200;
double posTextRollenmassHeight = 100;

WCHAR* TextWinkelRollenmass = L"Winkel für Rollenmaß: ";
double posTextWinkelRollenmassX = posEingabefeldX;
double posTextWinkelRollenmassY = 110;
double posTextWinkelRollenmassWidth = 220;
double posTextWinkelRollenmassHeight = 100;


WCHAR* TextPlusWinkelRollenmass = L"+";
double posPlusWinkelRollenmassX = posEingabefeldX + 220;
double posPlusWinkelRollenmassY = 110;
double posPlusRollenmassY = 15;
double posPlusNormaleY = 60;
double posPlusWinkelRollenmassWidth = 20;
double posPlusWinkelRollenmassHeight = 100;

WCHAR* TextMinusWinkelRollenmass = L"-";
double posMinusWinkelRollenmassX = posEingabefeldX + 240;
double posMinusWinkelRollenmassY = 109;
double posMinusRollenmassY = 13;
double posMinusNormaleY = 58;
double posMinusWinkelRollenmassWidth = 20;
double posMinusWinkelRollenmassHeight = 100;

WCHAR* TextZentrieren = L"Zentrieren/Zurücksetzen";
double posTextZentrierenX = posEingabefeldX+40;
double posTextZentrierenY = 150;
double posTextZentrierenWidth = 200;
double posTextZentrierenHeight = 100;

struct messpunkt {
	double x;
	double y;
	double z;
	double u;
	double v;
	double w;
};

int findeEnde(int i, char * buffer) {
	bool endeGefunden = false;
	while (!endeGefunden) {
		i++;
		if (buffer[i] == ',') {
			return i;
		}
		if (buffer[i] == '\n') {
			return i;
		}
	}
	return 0;
}

int parseZahl(int i, char* buffer, double* var) {
	int ende = findeEnde(i, buffer);
	double x = 0;
	char* zahlAlsChar = new char[ende - i];
	int g = 0;
	for (int j = i;j < ende;j++,g++) {
		zahlAlsChar[g] = buffer[j];
	}
	zahlAlsChar[g] = '\0';
	*var = (double)atof(zahlAlsChar);
	return ende;
}

int findeKomma(std::wstring tmp) {
	unsigned int i = 0;
	for (i;i < tmp.size();i++) {
		if (tmp[i] == '.') {
			break;
		}
	}
	return i;
}

void zeichneEingabefeld() {
	std::wstring tmp;
	std::wstring concartNormale;
	concartNormale += std::wstring(TextNormale);
	concartNormale += std::to_wstring(vergroesserungNormale);

	std::wstring concartRollenmass;
	concartRollenmass += std::wstring(TextRollenmass);
	tmp = std::to_wstring(rollenmassGroesse);
	concartRollenmass += tmp.substr(0, findeKomma(tmp) + 3);

	std::wstring concartWinkel;
	concartWinkel += std::wstring(TextWinkelRollenmass);
	tmp = std::to_wstring(winkelRollenmass);
	concartWinkel += tmp.substr(0, findeKomma(tmp) + 3);

	graphics->DrawRect(posEingabefeldX, posEingabefeldY, groesseEingabefeldX, groesseEingabefeldY, 0.4f, 0.4f, 0.4f, alphaEingabefeld);
	graphics->DrawTextS(posTextNormaleX, posTextNormaleY, posTextNormaleWidth, posTextNormaleHeight, &concartNormale,false);
	graphics->DrawTextS(posTextRollenmassX, posTextRollenmassY, posTextRollenmassWidth, posTextRollenmassHeight, &concartRollenmass, false);
	graphics->DrawTextS(posTextWinkelRollenmassX, posTextWinkelRollenmassY, posTextWinkelRollenmassWidth, posTextWinkelRollenmassHeight, &concartWinkel, false);
	graphics->DrawTextS(posPlusWinkelRollenmassX, posPlusWinkelRollenmassY, posPlusWinkelRollenmassWidth, posPlusWinkelRollenmassHeight, &std::wstring(TextPlusWinkelRollenmass), false);
	graphics->DrawTextS(posMinusWinkelRollenmassX, posMinusWinkelRollenmassY, posMinusWinkelRollenmassWidth, posMinusWinkelRollenmassHeight, &std::wstring(TextMinusWinkelRollenmass), false);
	graphics->DrawTextS(posPlusWinkelRollenmassX, posPlusRollenmassY, posMinusWinkelRollenmassWidth, posMinusWinkelRollenmassHeight, &std::wstring(TextPlusWinkelRollenmass), false);
	graphics->DrawTextS(posMinusWinkelRollenmassX, posMinusRollenmassY, posMinusWinkelRollenmassWidth, posMinusWinkelRollenmassHeight, &std::wstring(TextMinusWinkelRollenmass), false);
	graphics->DrawTextS(posPlusWinkelRollenmassX, posPlusNormaleY, posMinusWinkelRollenmassWidth, posMinusWinkelRollenmassHeight, &std::wstring(TextPlusWinkelRollenmass), false);
	graphics->DrawTextS(posMinusWinkelRollenmassX, posMinusNormaleY, posMinusWinkelRollenmassWidth, posMinusWinkelRollenmassHeight, &std::wstring(TextMinusWinkelRollenmass), false);
	graphics->DrawTextS(posTextZentrierenX, posTextZentrierenY, posTextZentrierenWidth, posTextZentrierenHeight, &std::wstring(TextZentrieren), false);
}

void rollenmass_singlethread(bool& run, OPENFILENAME ofn) {
	
	std::wstring output;
	output += std::wstring(TextNormale);
	output += std::to_wstring(vergroesserungNormale);
	FILE *datei;
	errno_t err;
	err = _wfopen_s(&datei, ofn.lpstrFile,__T("rb"));
	
	fseek(datei, 0, SEEK_END);
	unsigned int groesse = ftell(datei);
	rewind(datei);
	char * buffer = new char[groesse + 1];
	int gelesen;
	try {
		gelesen = fread_s(buffer, groesse, sizeof(char), groesse - 1, datei);
	}
	catch (exception e) {
		e.what();
	}
	bool weiter=true;
	vector<messpunkt> punkte;
	int countHeader = 0;
	int indexX = 0;
	int indexY = 0;
	int indexZ = 0;
	int indexU = 0;
	int indexV = 0;
	int indexW = 0;
	unsigned int i = 0;
	int indexCounter = 0;
	int indexLine = 0;
	while (weiter) {
		i++;
		if (buffer[i] == '(') {
			countHeader++;
		}
		else if (countHeader == 2) {
			bool weiterHeader = true;
			while (weiterHeader) {
				
				if (buffer[i] == '$') {
					indexCounter++;
					if (buffer[i + 1] == 'X') {
						indexX = indexCounter;
					}
					else if (buffer[i + 1] == 'Y') {
						indexY = indexCounter;
					}
					else if (buffer[i + 1] == 'Z') {
						indexZ = indexCounter;
					}
					else if (buffer[i + 1] == 'U') {
						indexU = indexCounter;
					}
					else if (buffer[i + 1] == 'V') {
						indexV = indexCounter;
					}
					else if (buffer[i + 1] == 'W') {
						indexW = indexCounter;
					}					
				}else if (buffer[i] == '^' || buffer[i] == '#') {
					indexCounter++;
				}else if (buffer[i] == ')' ) {
					weiterHeader = false;
					countHeader++;
				}
				i++;
			}
		}

		else if (countHeader == 3) {
			if (buffer[i] == '\r') {
				i++;
			}if (buffer[i] == '\n') {
				i++;
			}
			bool newLine = true;
			while (newLine) {
				bool weiterValues = true;
				messpunkt p;
				indexLine = 1;
				while (weiterValues) {					
					if (indexLine == indexX) {
						i = parseZahl(i, buffer, &p.x);
					}
					else if (indexLine == indexY) {
						i = parseZahl(i, buffer, &p.y);
						if (buffer[i] == '\n') {
							indexLine++;
						}
					}
					else if (indexLine == indexZ) {
						i = parseZahl(i, buffer, &p.z);
					}
					else if (indexLine == indexU) {
						i = parseZahl(i, buffer, &p.u);
					}
					else if (indexLine == indexV) {
						i = parseZahl(i, buffer, &p.v);
					}
					else if (indexLine == indexW) {
						i = parseZahl(i, buffer, &p.w);
						while (indexLine != indexCounter) {
							if (buffer[i] == ',') {
								indexLine++;
								if (indexLine == indexCounter - 1) {
									while (buffer[i] != 0x0a) {
										if (i > groesse) {
											break;
										}
										i++;
									}
									break;
								}
							}
							if (i > groesse) {
								break;
							}
							i++;
						}
						weiterValues = false;
					}
					if (buffer[i] == ',') {
						indexLine++;
					}
					if (indexLine>indexCounter) {
						weiterValues = false;
					}
					if (i > groesse) {
						newLine = false;
						break;	
					}
					i++;
				}
				punkte.push_back(p);
			}
		}
		if (i > groesse) {
			weiter=false;
		}
	}

	run = true;
	while (run) {
		lockGraph.lock();
		if (run) {
			graphics->BeginDraw();
			graphics->ClearScreen(0.0f, 0.0f, 0.1f);
			zeichneEingabefeld();
		}
		lockGraph.unlock();
		graphics->DrawLine(0, 0, ((cos((winkelRollenmass*PI) / 180.0)) * 50), ((sin((winkelRollenmass*PI) / 180.0f)*-1) * 50) , 0.8f, 0.2f, 0.2f, 10.0f);
		
		if (weiterNachLinks) {
			posRollenmassX -= (cos((winkelRollenmass*PI) / 180.0))/ geschwindigkeit1;
		}
		if (weiterNachUnten) {
			posRollenmassY -= (sin((winkelRollenmass*PI) / 180.0)*-1) / geschwindigkeit1;
		}
		graphics->DrawLine(0, 0, ((cos((winkelRollenmass2*PI) / 180.0)) * 50) , ((sin((winkelRollenmass2*PI) / 180.0f)*-1)  * 50), 0.8f, 0.2f, 0.2f, 10.0f);
		if (weiterNachRechts) {
			posRollenmassX2 += (cos((winkelRollenmass2*PI) / 180.0)) / geschwindigkeit2;
		}
		if (weiterNachOben) {
			posRollenmassY2 += (sin((winkelRollenmass2*PI) / 180.0)*-1) / geschwindigkeit2;
		}
		weiterNachLinks = true;
		weiterNachUnten = true;
		weiterNachOben = true;
		weiterNachRechts = true;

		Sleep(50);
		for (unsigned int i = 0; i < (punkte.size()-1); i++) {
			lockGraph.lock();
			if (run) {
				//graphics->DrawLine(punkte[i].x, punkte[i].y, ((punkte[i].x * (double)vergroesserung)+(punkte[i].u*(double)vergroesserungNormale)) + posX, ((punkte[i].y *(double)vergroesserung)+(punkte[i].v*(double)vergroesserungNormale)) + posY, 0.8f, 0.8f, 0.8f, 10.0f);
				//graphics->DrawLine(punkte[i].x, punkte[i].y, (punkte[i + 1].x * (double)vergroesserung) + posX, (punkte[i + 1].y * (double)vergroesserung) + posY, 0.8f, 0.8f, 0.8f, 10.0f);
				graphics->DrawLineRelativ(punkte[i].x, punkte[i].y, punkte[i].u, punkte[i].v, 0.8f, 0.8f, 0.8f, 10.0f,true);
				graphics->DrawLine(punkte[i].x, punkte[i].y, punkte[i + 1].x , punkte[i + 1].y, 0.8f, 0.8f, 0.8f, 10.0f);
				double x = punkte[i].x - posRollenmassX;
				double y = punkte[i].y - posRollenmassY;
				//graphics->DrawLine(posRollenmassX, posRollenmassY, punkte[i].x, punkte[i].y, 0.8f, 0.0f, 0.0f, 10.0f);
				//graphics->DrawLine(posRollenmassX2, posRollenmassY2, punkte[i].x, punkte[i].y, 0.8f, 0.0f, 0.0f, 10.0f);

				double x2 = punkte[i].x - posRollenmassX2;
				double y2 = punkte[i].y - posRollenmassY2;
				if (sqrt(x*x + y*y) < 7.8) {
					geschwindigkeit1 = 100.0;
				}
				if (sqrt(x*x + y*y) < 7.6) {
					geschwindigkeit1 = 500.0;
				}
				if (sqrt(x*x + y*y) < 7.55) {
					geschwindigkeit1 = 2000.0;
				}
				if (sqrt(x*x + y*y) < 7.50315) {
					if (x < 0) {
						weiterNachLinks = false;
					}if (y < 0) {
						weiterNachUnten = false;
					}
				}
				
				if (sqrt(x2*x2 + y2*y2) < 7.8) {
					geschwindigkeit2 = 100.0;
				}
				if (sqrt(x2*x2 + y2*y2) < 7.6) {
					geschwindigkeit2 = 500.0;
				}
				if (sqrt(x2*x2 + y2*y2) < 7.55) {
					geschwindigkeit2 = 2000.0;
				}
				if (sqrt(x2*x2 + y2*y2) < 7.50315) {
					if (x > 0) {
						weiterNachRechts = false;
					}if (y > 0) {
						weiterNachOben = false;
					}
				}
				
				
			}
			lockGraph.unlock();
		}

		graphics->DrawCircle(posRollenmassX, posRollenmassY, rollenmassGroesse, 0.2f, 0.2f, 0.8f, 10.0f);
		graphics->DrawCircle(posRollenmassX2, posRollenmassY2, rollenmassGroesse, 0.2f, 0.2f, 0.8f, 10.0f);
		double abstandHypotenuseRollenmass=0;
		if(weiterNachLinks == false && weiterNachUnten == false && weiterNachRechts == false ) {
			double abstandRollenmassX = posRollenmassX - posRollenmassX2;
			double abstandRollenmassY = posRollenmassY - posRollenmassY2;
			abstandHypotenuseRollenmass = sqrt(abstandRollenmassX*abstandRollenmassX + abstandRollenmassY*abstandRollenmassY);
			double abstandRollenmassXLaengeAufEins = abstandRollenmassX / abstandHypotenuseRollenmass;
			double abstandRollenmassYLaengeAufEins = abstandRollenmassY / abstandHypotenuseRollenmass;
			double coordMassStrichX = posRollenmassX - (7.5 *abstandRollenmassXLaengeAufEins);
			double coordMassStrichY = posRollenmassY - (7.5 *abstandRollenmassYLaengeAufEins);
			graphics->DrawLineRelativ(coordMassStrichX, coordMassStrichY, -abstandRollenmassYLaengeAufEins, abstandRollenmassXLaengeAufEins, 0.8f, 0.8f, 0.8f, 10.0f,false);
			double coordMassStrichX2 = posRollenmassX2 + (7.5 *abstandRollenmassXLaengeAufEins);
			double coordMassStrichY2 = posRollenmassY2 + (7.5 *abstandRollenmassYLaengeAufEins);
			graphics->DrawLineRelativ(coordMassStrichX2, coordMassStrichY2, -abstandRollenmassYLaengeAufEins, abstandRollenmassXLaengeAufEins, 0.8f, 0.8f, 0.8f, 10.0f,false);
			graphics->DrawLine(coordMassStrichX+(2 * -abstandRollenmassYLaengeAufEins), coordMassStrichY+(2 * abstandRollenmassXLaengeAufEins), coordMassStrichX2+(2 * -abstandRollenmassYLaengeAufEins), coordMassStrichY2 + (2 * abstandRollenmassXLaengeAufEins), 0.8f, 0.8f, 0.8f, 10.0f);			
			textAbstandRollenmass = std::to_wstring(abstandHypotenuseRollenmass-rollenmassGroesse);
			graphics->DrawTextS(-5, abstandRollenmassY/4, 100, 10, textAbstandRollenmass,true);

			graphics->DrawLineRelativ(posRollenmassX, posRollenmassY, -3, 3, 0.8f, 0.8f, 0.8f, 10.0f, false);
			graphics->DrawLineRelativ(posRollenmassX - (3 * 5), posRollenmassY + (3 * 5), -5, 0, 0.8f, 0.8f, 0.8f, 10.0f, false);

			textAbstandRollenmass = L"( ";
			textAbstandRollenmass += std::to_wstring(posRollenmassX);
			textAbstandRollenmass += L" , ";
			textAbstandRollenmass += std::to_wstring(posRollenmassY);
			textAbstandRollenmass += L" )";

			graphics->DrawTextS(posRollenmassX -(3 * 5) + 1, posRollenmassY + (3 * 5) - 3, -200, 10, textAbstandRollenmass, true);


			graphics->DrawLineRelativ(posRollenmassX2, posRollenmassY2, 3, 3, 0.8f, 0.8f, 0.8f, 10.0f, false);
			graphics->DrawLineRelativ(posRollenmassX2+(3*5), posRollenmassY2+ (3 * 5), 5, 0, 0.8f, 0.8f, 0.8f, 10.0f, false);

			textAbstandRollenmass = L"( ";
			textAbstandRollenmass += std::to_wstring(posRollenmassX2);
			textAbstandRollenmass += L" , ";
			textAbstandRollenmass += std::to_wstring(posRollenmassY2);
			textAbstandRollenmass += L" )";

			graphics->DrawTextS(posRollenmassX2 + (3 * 5)+1, posRollenmassY2 + (3 * 5)-3, 200, 10, textAbstandRollenmass, true);

		}
		graphics->DrawLine(posRollenmassX, posRollenmassY, posRollenmassX2, posRollenmassY2, 0.8f, 0.8f, 0.8f, 10.0f);
		lockGraph.lock();
		if (run) {
			graphics->EndDraw();
		}
		lockGraph.unlock();
	}
}

void clearScreen() {
	lockGraph.lock();
	if (run) {
		graphics->BeginDraw();
		graphics->ClearScreen(0.0f, 0.0f, 0.1f);
		zeichneEingabefeld();

	}
	lockGraph.unlock();
	lockGraph.lock();
	if (run) {
		graphics->EndDraw();
	}
	lockGraph.unlock();
}



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Hier Code einfügen.

    // Globale Zeichenfolgen initialisieren
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ROLLENMA, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Anwendungsinitialisierung ausführen:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ROLLENMA));
	clearScreen();
	

    MSG msg;

    // Hauptnachrichtenschleife:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNKTION: MyRegisterClass()
//
//  ZWECK: Registriert die Fensterklasse.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ROLLENMA));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ROLLENMA);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNKTION: InitInstance(HINSTANCE, int)
//
//   ZWECK: Speichert das Instanzenhandle und erstellt das Hauptfenster.
//
//   KOMMENTARE:
//
//        In dieser Funktion wird das Instanzenhandle in einer globalen Variablen gespeichert, und das
//        Hauptprogrammfenster wird erstellt und angezeigt.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Instanzenhandle in der globalen Variablen speichern

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   windowPosX, windowPosY, windowSizeX, windowSizeY, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   if (!hWnd) return -1;
   graphics = new Graphics();

   if (!graphics->Init(hWnd)) {
	   delete graphics;
	   return -1;
   }
   graphics->setCenterPosition(posX, posY);
   graphics->setVergroesserung(vergroesserung);
   graphics->setVergroesserungNormale(vergroesserungNormale);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

UINT nachricht;
LPARAM parameter;
int runOnce = 0;

void thread_mouse_clicked() {
	int posXAlt = LOWORD(parameter);
	int posYAlt = HIWORD(parameter);
	if (posXAlt > windowSizeX-80 && posXAlt < windowSizeX-60 && posYAlt>17 && posYAlt < 27) {		//Normale vergrößern
		graphics->setVergroesserungNormale(vergroesserungNormale++);
		clearScreen();
		return;
	}
	if (posXAlt >  windowSizeX - 60 && posXAlt <  windowSizeX - 40 && posYAlt>17 && posYAlt < 27) {		//Normale verkleinern
		graphics->setVergroesserungNormale(vergroesserungNormale--);
		clearScreen();
		return;
	}
	if (posXAlt >  windowSizeX - 80 && posXAlt <  windowSizeX - 60 && posYAlt>65 && posYAlt < 75) {		//Rollenmaß vergrößern
		rollenmassGroesse += 1.0f;
		clearScreen();
		return;
	}
	if (posXAlt >  windowSizeX - 60 && posXAlt <  windowSizeX - 40 && posYAlt>65 && posYAlt < 75) {//Rollenmaß verkleinern
		rollenmassGroesse -= 1.0f;
		clearScreen();
		return;
	}
	if (posXAlt >  windowSizeX - 80 && posXAlt <  windowSizeX - 60 && posYAlt>110 && posYAlt < 130) {//Winkel erhöhen
		winkelRollenmass += 5.0f;
		posRollenmassX = 0.0f;
		posRollenmassY = 0.0f;
		weiterNachLinks = true;
		weiterNachUnten = true;
		geschwindigkeit1 = 1.0;
		geschwindigkeit2 = 1.0;
		clearScreen();
		return;
	}
	if (posXAlt >  windowSizeX - 60 && posXAlt <  windowSizeX - 40 && posYAlt>110 && posYAlt < 130) {//Winkel verringern
		winkelRollenmass -= 5.0f;
		posRollenmassX = 0.0f;
		posRollenmassY = 0.0f;
		weiterNachLinks = true;
		weiterNachUnten = true;
		geschwindigkeit1 = 1.0;
		geschwindigkeit2 = 1.0;
		clearScreen();
		return;
	}
	if (posXAlt >  windowSizeX - 245 && posXAlt <  windowSizeX - 75 && posYAlt>140 && posYAlt < 160) {//Winkel verringern
		graphics->setCenterPosition(posX, posY);
		vergroesserung = 8;
		graphics->setVergroesserung(vergroesserung);
		vergroesserungNormale = 1;
		graphics->setVergroesserungNormale(vergroesserungNormale);
		graphics->SetRotationsWinkel(0);
		clearScreen();
		return;
	}
	int deltaX = 0;
	int deltaY = 0;
	while (nachricht != WM_LBUTTONUP) {
		deltaX = LOWORD(parameter) - posXAlt;
		deltaY = HIWORD(parameter) - posYAlt;
		graphics->moveCenterPosition((deltaX / 30),(deltaY / 30));
		Sleep(5);
	}

}


void thread_mouse_clickedR() {
	int posXAlt = LOWORD(parameter);
	int posYAlt = HIWORD(parameter);

	int deltaX = 0;
	int deltaY = 0;
	while (nachricht != WM_RBUTTONUP) {
		if (nachricht != WM_RBUTTONUP) {
			deltaY = HIWORD(parameter) - posYAlt;
		}
		graphics->SetRotationsWinkel(deltaY / 4);
		
		//Sleep(1);
	}

}

//
//  FUNKTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ZWECK:  Verarbeitet Meldungen vom Hauptfenster.
//
//  WM_COMMAND  - Verarbeiten des Anwendungsmenüs
//  WM_PAINT    - Darstellen des Hauptfensters
//  WM_DESTROY  - Ausgeben einer Beendenmeldung und zurückkehren
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	nachricht = message;
	parameter = lParam;
	OPENFILENAME ofn;
	LPWSTR file_name = new TCHAR[255];
    switch (message)
    {
	case WM_LBUTTONDOWN: {
		new thread(&thread_mouse_clicked);
		break;
	}
	case WM_RBUTTONDOWN: {
		new thread(&thread_mouse_clickedR);
		break;
	}
	case WM_MOUSEWHEEL: {
		int tmp = wParam;
		if (tmp >= 0) {
			graphics->setVergroesserung(vergroesserung++);
		}
		else {
			if (vergroesserung > 1) {
				graphics->setVergroesserung(vergroesserung--);
			}
		}
		break;
	}
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Menüauswahl bearbeiten:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
			case ID_DATEI_32771:
				run = false;
				init();
				ZeroMemory(&ofn, sizeof(OPENFILENAME));
				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.hwndOwner = hWnd;
				ofn.lpstrFile = file_name;
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = 255;
				ofn.lpstrFilter = __T("Alle Dateien *.*\0*.*\0Text Dateien *.txt\0*.txt\0");
				ofn.nFilterIndex = 2;
				GetOpenFileName(&ofn);
				if (ofn.lpstrFile != nullptr&&ofn.lpstrFile[0] != '\0') {
					rollenmassMainThread = new thread(&rollenmass_singlethread, std::ref(run), ofn);
				}
				break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
			clearScreen();
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            //TODO: Zeichencode, der hdc verwendet, hier einfügen...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
			lockGraph.lock();
			run = false;
			lockGraph.unlock();
			PostQuitMessage(0);
			return 0;
       
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Meldungshandler für Infofeld.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
