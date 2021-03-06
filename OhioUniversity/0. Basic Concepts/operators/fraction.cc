#include "fraction.h"
#include <iostream>
using namespace std;

Fraction::Fraction(int t,int b){
	top=t;
	divider='/';
	bottom=b;
}

/*Fraction Fraction::operator +(const Fraction f1,const Fraction f2){
	//Fraction temp;
	return gcd(f1,f2);
	//temp.top=f1.top+f2.top;
	
}

Fraction Fraction::operator -(const Fraction f1,const Fraction f2){
	return gcd(f1,f2);
}*/

int Fraction::getTop(){
	return top;
}

int Fraction::getBottom(){
	return bottom;
}

int Fraction::gcd(int a,int b){
	if(a==b){
		return a;
	}else if(a>b){
		return gcd(a-b,b);
	}else if(a<b){
		return gcd(a,b-a);
	}
}
