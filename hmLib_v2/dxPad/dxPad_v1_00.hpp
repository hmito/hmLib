#ifndef INC_HMLIB_DXPAD
#define INC_HMLIB_DXPAD 100
#
#ifndef INC_HMLIB_DXBASIC
#	include "hmLib_v2/dxBasic.hpp"
#endif
class dxPad{
private:
	int ch;
	int ButSet2;
	int ButSet;
public:
	dxPad(int _ch){
		ch=_ch;
		ButSet=0;
		ButSet2=0;
	}
	~dxPad(){stopVib();}
	void work(){
		ButSet2=ButSet;
		ButSet=GetJoypadInputState(ch);
	}
	int getBut(int _no){
		if((ButSet&(1<<_no))!=0 && (ButSet2&(1<<_no))!=0)return 2;
		else if((ButSet&(1<<_no))!=0)return 1;
		else if((ButSet2&(1<<_no))!=0)return -1;
		else return 0;
	}
	//右下が+、左上が-
	Pint getAnlgL(){
		Pint tmp;
		GetJoypadAnalogInput(&(tmp.x),&(tmp.y),ch);
		return tmp;
	}
	//右下が+、左上が-
	Pint getAnlgR(){
		Pint tmp;
		GetJoypadAnalogInputRight(&(tmp.y),&(tmp.x),ch);
		return tmp;
	}
	void strtVib(int Power,int mSec){StartJoypadVibration(ch,Power,mSec);}
	void stopVib(){StopJoypadVibration(ch);}
	static void connect(){
		ReSetupJoypad();
	}
};
#
#endif
