#pragma once
#include "stdafx.h"


class dreieck {
private:
	char str[150];
public:
	
	long double hypotenuse, ankatete, gegenkatete, alpha, beta, gamma; //gegenkatete = a; ankatete = b; hypotenuse = c;
	dreieck();
	~dreieck();
	char* toString();
	bool reset();
	bool ankateteMitWinkelBerechnen();
	bool ankateteMitWinkelohneHypBerechnen();
	bool gegenkateteMitWinkelBerechnen();
	bool ankateteBerechnen();
	bool gegenkateteBerechnen();
	bool hypotenuseBerechnen();
	bool sinAlphaBerechnen();
	bool tanAlphaBerechnen();
	bool sinBetaBerechnen();
	bool alphaBerechnen();
	bool betaBerechnen();
	long double getHypotenuse();
	void setHypotenuse(long double hypotenuse);
	long double getAnkatete();
	void setAnkatete(long double ankatete);
	long double getGegenkatete();
	void setGegenkatete(long double gegenkatete);
	long double getAlpha();
	void setAlpha(long double alpha);
	long double getBeta();
	void setBeta(long double beta);
	long double getGamma();
	void setGamma(long double gamma);
	long double degrees_to_radian(long double deg);
	char* getStr();

};

