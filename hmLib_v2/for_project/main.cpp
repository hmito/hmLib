#include "hmLib.h"
#include <dxBasic.h>
#include <dxColorSet.h>
#include <hmLib.cpp>
using namespace std;
using namespace hmstd;
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow){

	dx::ini();
	while(!dx::work(20)){
		if(dx::getKey(KEY::ESC))break;
	}
	dx::end();

	return 0;
}