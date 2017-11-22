#include "dreieck.h"
#include "stdafx.h"


using namespace std;
#pragma comment(lib, "d2d1")
#pragma comment(lib, "Dwrite.lib")

static long double PI = 3.141592653589793238463;

dreieck::dreieck() :gamma(90), alpha(0), beta(0), ankatete(0), gegenkatete(0), hypotenuse(0) {
}


dreieck::~dreieck() {
}

bool dreieck::reset() {
	gamma = 90;
	alpha = 0;
	beta = 0;
	ankatete = 0;
	gegenkatete = 0;
	hypotenuse = 0;
	return true;
}

char* dreieck::getStr() {
	return str;
}

char* dreieck::toString() {
	char buf[65];
	string s = "************************************\n";
	s.append("Hypotenuse: ");
	_itoa_s(hypotenuse, buf, 65, 10);
	s.append(buf);
	s.append(" Ankatete: ");
	_itoa_s(ankatete, buf, 65, 10);
	s.append(buf);
	s.append(" Gegenkatete: ");
	_itoa_s(gegenkatete, buf, 65, 10);
	s.append(buf);
	s.append(" Alpha: ");
	_itoa_s(alpha, buf, 65, 10);
	s.append(buf);
	s.append(" Beta: ");
	_itoa_s(beta, buf, 65, 10);
	s.append(buf);

	for (int i = 0; i < s.length(); i++) {
		str[i] = s[i];
	}
	str[s.length()] = 0;

	return str;
}


bool dreieck::ankateteMitWinkelBerechnen() {
	if (hypotenuse <= 0 || beta <= 0 || gamma <= 0) {
		cout << "Kann die Ankatete nicht berechnen!" << endl;
		return false;
	}
	ankatete = (hypotenuse*sin(beta * PI / 180)) / sin(gamma * PI / 180);
	//cout << "Ankatete ist nun " + ankatete);
	return true;
}

bool dreieck::ankateteMitWinkelohneHypBerechnen() {
	if (alpha <= 0 || beta <= 0 || gegenkatete <= 0) {
		cout << "Kann die Ankatete nicht berechnen!" << endl;
		return false;
	}
	ankatete = (gegenkatete*sin(alpha * PI / 180)) / sin(beta * PI / 180);
	//cout << "Ankatete ist nun " + ankatete);
	return true;
}

bool dreieck::gegenkateteMitWinkelBerechnen() {
	if (hypotenuse <= 0 || alpha<0 || gamma <= 0) {
		cout << "Kann die Gegenkatete nicht berechnen!" << endl;
		return false;
	}
	gegenkatete = (hypotenuse*sin(alpha * PI / 180)) / sin(gamma * PI / 180);
	//cout << "Gegenkatete ist nun " + gegenkatete);
	return true;
}


bool dreieck::ankateteBerechnen() {
	if (hypotenuse <= 0 || gegenkatete <= 0) {
		cout << "Kann die Ankatete nicht berechnen!" << endl;
		return false;
	}
	ankatete = sqrt(hypotenuse*hypotenuse - gegenkatete*gegenkatete);
	//cout << "Ankatete ist nun " + ankatete);
	return true;
}

bool dreieck::gegenkateteBerechnen() {
	if (hypotenuse <= 0 || ankatete <= 0) {
		cout << "Kann die Gegenkatete nicht berechnen!" << endl;
		return false;
	}
	gegenkatete = sqrt(hypotenuse*hypotenuse - ankatete*ankatete);
	//cout << "Gegenkatete ist nun " + gegenkatete);
	return true;
}

bool dreieck::hypotenuseBerechnen() {
	hypotenuse = sqrt(gegenkatete*gegenkatete + ankatete*ankatete);
	//cout << "Hypotenuse ist nun " + hypotenuse);
	return true;
}

bool dreieck::sinAlphaBerechnen() {
	if (hypotenuse <= 0) {
		cout << "Keine Winkelberechnung möglich!" << endl;
		return false;
	}
	//cout << "gegenkatete/hypotenuse = " + gegenkatete/hypotenuse);
	alpha = asin(gegenkatete / hypotenuse) * 180 / PI;
	//cout << "wintel berechnet. Alpha ist " + alpha);
	return true;
}

bool dreieck::tanAlphaBerechnen() {
	if (ankatete == 0 ) {
		alpha = 0;
		cout << "Keine Winkelberechnung möglich!" << endl;
		return false;
	}
	//cout << "gegenkatete/ankatete = " + gegenkatete/ankatete);
	alpha = atan(gegenkatete / ankatete) * 57.2958;
	//cout << "wintel berechnet. Alpha ist " + alpha);
	return true;
}

bool dreieck::sinBetaBerechnen() {
	if (hypotenuse <= 0 || ankatete <= 0) {
		cout << "Keine Winkelberechnung möglich!" << endl;
		return false;
	}
	//cout << "ankatete/hypotenuse = " + ankatete/hypotenuse);
	beta = asin(ankatete / hypotenuse) * 180 / PI;
	//cout << "wintel berechnet. Beta ist " + beta);
	return true;
}

bool dreieck::alphaBerechnen() {
	if (beta <= 0 || gamma <= 0) {
		cout << "Keine Winkelberechnung möglich!" << endl;
		return false;
	}
	alpha = 180 - gamma - beta;
	return true;
}

bool dreieck::betaBerechnen() {
	if (alpha <= 0 || gamma <= 0) {
		cout << "Keine Winkelberechnung möglich!" << endl;
		return false;
	}
	beta = 180 - gamma - alpha;
	return true;
}

long double dreieck::getHypotenuse() {
	return hypotenuse;
}

void dreieck::setHypotenuse(long double hypo) {
	hypotenuse = hypo;
}

long double dreieck::getAnkatete() {
	return ankatete;
}

void dreieck::setAnkatete(long double anka) {
	ankatete = anka;
}

long double dreieck::getGegenkatete() {
	return gegenkatete;
}

void dreieck::setGegenkatete(long double gegen) {
	gegenkatete = gegen;
	
}

long double dreieck::degrees_to_radian(long double deg) {
	return deg / 180.0;
}


long double dreieck::getAlpha() {
	return alpha;
}

void dreieck::setAlpha(long double a) {
	alpha = a;
}

long double dreieck::getBeta() {
	return beta;
}

void dreieck::setBeta(long double b) {
	beta = b;
}

long double dreieck::getGamma() {
	return gamma;
}

void dreieck::setGamma(long double g) {
	gamma = g;
}


