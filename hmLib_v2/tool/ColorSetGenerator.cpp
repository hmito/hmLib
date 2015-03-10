#include <fstream>
#include <vector>
#include "winTime.hpp"
#include "hmLib_v2_05.hpp"
using namespace std;
using namespace hmstd;
int main(){
	vector<string> ClrHue;
	vector<string> ClrLgh;
	vector<string> ClrSat;

	ClrHue.push_back("Red");
	ClrHue.push_back("Orenge");
	ClrHue.push_back("Yellow");
	ClrHue.push_back("Yelgreen");
	ClrHue.push_back("Green");
	ClrHue.push_back("Seagreen");
	ClrHue.push_back("Cyan");
	ClrHue.push_back("Skyblue");
	ClrHue.push_back("Blue");
	ClrHue.push_back("Violet");
	ClrHue.push_back("Magenta");
	ClrHue.push_back("Pink");

	ClrLgh.push_back("Dim");
	ClrLgh.push_back("Dark");
	ClrLgh.push_back("Deep");
	ClrLgh.push_back("");
	ClrLgh.push_back("Light");
	ClrLgh.push_back("Thin");
	ClrLgh.push_back("Pale");

	ClrSat.push_back("Dull");
	ClrSat.push_back("Soft");
	ClrSat.push_back("");

	ofstream ofsh("dxColorSet.h");
	ofstream ofsc("dxColorSet.cpp");
	ofsh<<"/*"<<endl
		<<"dxColorSet.h"<<endl;
	ofsh<<"Hue"<<endl;
	for(int i=0;i<ClrHue.size();i++){
		ofsh<<"\t"<<ClrHue.at(i)<<endl;
	}
	ofsh<<"Lgh"<<endl;
	for(int i=0;i<ClrLgh.size();i++){
		if(ClrLgh.at(i)!="")ofsh<<"\t"<<ClrLgh.at(i)<<endl;
		else ofsh<<"\t[None]"<<endl;
	}
	ofsh<<"Sat"<<endl;
	for(int i=0;i<ClrSat.size();i++){
		if(ClrSat.at(i)!="")ofsh<<"\t"<<ClrSat.at(i)<<endl;
		else ofsh<<"\t[None]"<<endl;
	}
	ofsh<<"*/"<<endl;

	ofsh<<"#ifndef DXCOLORSET_H"<<endl
		<<"#define DXCOLORSET_H 101"<<endl
		<<"#include "<<'"'<<"dxColor.hpp"<<'"'<<endl
		<<"namespace CLR{"<<endl;
	ofsc<<"#ifndef DXCOLORSET_CPP"<<endl
		<<"#define DXCOLORSET_CPP"<<endl
		<<"#include "<<'"'<<"dxColorSet.h"<<'"'<<endl
		<<"namespace CLR{"<<endl;

	ofsh<<"\textern dxRGB "<<"Black"<<";"<<endl;
	ofsc<<"\tdxRGB "<<"Black"<<"(dxHLS("<<0<<","<<0<<","<<0<<"));"<<endl;
	for(int lgh=0;lgh<ClrLgh.size();lgh++){
		ofsh<<"\textern dxRGB "<<ClrLgh.at(lgh)<<"Gray"<<";"<<endl;
		ofsc<<"\tdxRGB "<<ClrLgh.at(lgh)<<"Gray"<<"(dxHLS("<<0<<","<<32*lgh+32<<","<<0<<"));"<<endl;
	}
	ofsh<<"\textern dxRGB "<<"White"<<";"<<endl;
	ofsc<<"\tdxRGB "<<"White"<<"(dxHLS("<<0<<","<<255<<","<<0<<"));"<<endl;

	for(int hue=0;hue<ClrHue.size();hue++){
		for(int lgh=0;lgh<ClrLgh.size();lgh++){
			for(int sat=0;sat<ClrSat.size();sat++){
				ofsh<<"\textern dxRGB "<<ClrLgh.at(lgh)<<ClrSat.at(sat)<<ClrHue.at(hue)<<";"<<endl;
				ofsc<<"\tdxRGB "<<ClrLgh.at(lgh)<<ClrSat.at(sat)<<ClrHue.at(hue)<<"(dxHLS("<<30*hue<<","<<32*lgh+32<<","<<64*sat+127<<"));"<<endl;
			}
		}
	}
	ofsh<<"}"<<endl
		<<"#endif";
	ofsc<<"}"<<endl
		<<"#endif";
}