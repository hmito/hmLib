#ifndef INC_HMLIB_DXBASIC_CPP
#define INC_HMLIB_DXBASIC_CPP 201
#
#ifndef INC_HMLIB_DXBASIC
#	include "hmLib_v2/dxBasic.hpp"
#endif
#ifndef INC_HMLIB_WINTIME
#	include "hmLib_v2/winTime.hpp"
#endif
namespace{
	Pint ScreenSize;
	int ScreenClrBit;
	int WindowModeFlag;
	int WindowExtendFlag;

	//NowSereas
	bool KeyActiveFlag;	//KeyActive
	Pint GlobalStd;		//描画基準となるスクリーン上のpos
	dxDMode NowMode;	//globalなモード
	dxDMode UseMode;	//実際に使っているモード dmode+NowMode
	void useDrawMode(const dxDMode& _dmode){
		if(UseMode.Blend!=_dmode.Blend || _dmode.bmode!=UseMode.bmode){
			SetDrawBlendMode(_dmode.bmode,_dmode.Blend);
			UseMode.Blend=_dmode.Blend;
			UseMode.bmode=_dmode.bmode;
		}
		if(UseMode.Bright!=_dmode.Bright){
			SetDrawBright(_dmode.Bright.r,_dmode.Bright.g,_dmode.Bright.b);
			UseMode.Bright=_dmode.Bright;
		}
	}
	dxDArea NowArea;
	dxBasicScreen NowScreen;

	//KeyStatus
	int KeySta[256];
	int OtherSta[6];	//Shift,Ctrl,Alt,MosL,MosR,MosC
	int MosHNum;
	Pint MosPos,MosPPos;
	Pint MosLPos,MosRPos,MosCPos;
	void changeKeySta(int& _sta,char _onoff){
		if(_sta>0){
			if(_onoff)_sta=2;
			else _sta=-1;
		}else{
			if(_onoff)_sta=1;
			else _sta=0;
		}
	}
	void setKey(){
		if(!GetActiveFlag())return;
		char tmp[256];
		GetHitKeyStateAll(tmp);
		for(int i=0;i<256;i++)changeKeySta(KeySta[i],tmp[i]);
		changeKeySta(OtherSta[0],(tmp[KEY_INPUT_LSHIFT]||tmp[KEY_INPUT_RSHIFT]));
		changeKeySta(OtherSta[1],(tmp[KEY_INPUT_LCONTROL]||tmp[KEY_INPUT_RCONTROL]));
		changeKeySta(OtherSta[2],(tmp[KEY_INPUT_LALT]||tmp[KEY_INPUT_RALT]));
		changeKeySta(OtherSta[3],(GetMouseInput() & MOUSE_HMLIB_INPUT_LEFT));
		changeKeySta(OtherSta[4],(GetMouseInput() & MOUSE_HMLIB_INPUT_RIGHT));
		changeKeySta(OtherSta[5],(GetMouseInput() & MOUSE_HMLIB_INPUT_MIDDLE));

		MosPPos=MosPos;
		GetMousePoint(&MosPos.x,&MosPos.y);
		if(dx::getKey(KEY::MOSL)==1)MosLPos=MosPos;
		if(dx::getKey(KEY::MOSR)==1)MosRPos=MosPos;
		if(dx::getKey(KEY::MOSC)==1)MosCPos=MosPos;
		MosHNum=GetMouseWheelRotVol();
	}
	int getKey2(int _sta1,int _sta2){
		if(_sta1==2 || _sta2==2)return 2;	//一方が2
		if(_sta1*_sta2==-1)return 2;			//一方が1,一方が-1
		if(_sta1==1 || _sta2==1)return 1;
		if(_sta1==-1|| _sta2==-1)return -1;
		return 0;
	}
	//TimeCtr
	double Fps;
	int IniTime;
	int PreTime;
	int Time;
	int Cnt;
	double IniLocalTime;

	uint DrawInpCandiMin=2;
	uint DrawInpCandiMax=2;

	//StdFont
	dxFont Font;
	dxFont VFont;
	//StdInput
	dxInput Input;
	std::string InputName;

	//変換描画関数
	bool DrawInputChangeFlag=false;

	Pint InputChange_pos;
	int InputChange_adjlen;

	int InputChange_CandidateNum;
	int InputChange_selno;
	std::deque<std::string> InputChange_CandidateList;

	int InputChange_FontHandle;
	int InputChange_FontSize;
	int InputChange_VFlag;

	dxDMode InputChange_dmode;
	dxRGB InputChange_sclr;
	dxRGB InputChange_seclr;
	dxRGB InputChange_imeclr;

	void ReservDrawInputChange(Pint pos,int adjlen,int CandidateNum,int selno,const char**& CandidateList,int FontHandle,int FontSize,int VFlag,dxDMode dmode,const dxColor& sclr,const dxColor& imeclr,const dxColor& seclr){
		InputChange_pos=pos+GlobalStd;
		InputChange_adjlen=adjlen;

		InputChange_CandidateNum=CandidateNum;
		InputChange_selno=selno;
		InputChange_CandidateList.clear();
		for(int i=0;i<CandidateNum;i++){InputChange_CandidateList.push_back(hmstd::fStr("%s",CandidateList[i]));}

		InputChange_FontHandle=FontHandle;
		InputChange_FontSize=FontSize;
		InputChange_VFlag=VFlag;

		InputChange_dmode=dmode;
		InputChange_sclr=sclr.getRGB();
		InputChange_seclr=seclr.getRGB();
		InputChange_imeclr=imeclr.getRGB();

		DrawInputChangeFlag=true;
	}
	void drawInputChange(){
		using namespace dx;
		if(!DrawInputChangeFlag)return;
		std::string tmpstr;
		std::string numstr;
		int strlen,numlen;

		if(!InputChange_VFlag){
			for(int j=InputChange_selno-(int)DrawInpCandiMin;j<=InputChange_selno+(int)DrawInpCandiMax; j++ ){
				if(j<0 || j>=InputChange_CandidateNum || j==InputChange_selno)continue;

				tmpstr=InputChange_CandidateList[j];
				numstr=hmstd::fStr("%d:",j+1);
				strlen=dx::getDrawStringWidth(tmpstr,tmpstr.size(),InputChange_FontHandle,InputChange_VFlag);
				numlen=dx::getDrawStringWidth(numstr,numstr.size(),InputChange_FontHandle,InputChange_VFlag);
				drawBox(InputChange_pos+Pint(InputChange_adjlen-numlen,InputChange_FontSize*(j-InputChange_selno)),InputChange_pos+Pint(InputChange_adjlen,InputChange_FontSize*(j-InputChange_selno))+Pint(strlen,InputChange_FontSize),InputChange_imeclr,1,InputChange_dmode);
				drawStr(InputChange_pos+Pint(InputChange_adjlen-numlen,InputChange_FontSize*(j-InputChange_selno)),numstr+tmpstr,InputChange_sclr,InputChange_dmode,InputChange_VFlag,InputChange_FontHandle,InputChange_seclr);
			}
		}else{
			for(int j=InputChange_selno-(int)DrawInpCandiMin;j<=InputChange_selno+(int)DrawInpCandiMax; j++ ){
				if(j<0 || j>=InputChange_CandidateNum || j==InputChange_selno)continue;

				tmpstr=InputChange_CandidateList[j];
				numstr=hmstd::fStr("%d:",j+1);
				strlen=dx::getDrawStringWidth(tmpstr,tmpstr.size(),InputChange_FontHandle,InputChange_VFlag);
				numlen=dx::getDrawStringWidth(numstr,numstr.size(),InputChange_FontHandle,InputChange_VFlag);
				drawBox(InputChange_pos+Pint(InputChange_FontSize*(j-InputChange_selno),InputChange_adjlen-numlen),InputChange_pos+Pint(InputChange_FontSize*(j-InputChange_selno),InputChange_adjlen)+Pint(InputChange_FontSize,strlen),InputChange_imeclr,1,InputChange_dmode);
				drawStr(InputChange_pos+Pint(InputChange_FontSize*(j-InputChange_selno),InputChange_adjlen-numlen),numstr+tmpstr,InputChange_sclr,InputChange_dmode,InputChange_VFlag,InputChange_FontHandle,InputChange_seclr);
			}
		}

		DrawInputChangeFlag=false;
	}
};

namespace dx{
	int ini(std::string _WindowName,int _ScreenX,int _ScreenY,int _ScreenClrBit,
		int _HighPreciseFlag,int _AlwaysRunFlag,int _WindowModeFlag,int _WindowExtendFlag){
		ScreenSize=Pint(_ScreenX,_ScreenY);
		ScreenClrBit=_ScreenClrBit;
		WindowModeFlag=_WindowModeFlag;
		WindowExtendFlag=_WindowExtendFlag;

		Fps=0.;
		PreTime=GetNowCount();
		IniTime=PreTime;
		IniLocalTime=win::getTime();
		Time=PreTime;
		Cnt=0;

		//Key関連変数初期化
		for(int i=0;i<256;i++)KeySta[i]=0;
		for(int i=0;i<6;i++)OtherSta[i]=0;

		//ログ出力を無効にする
		SetOutApplicationLogValidFlag(0);

		//二重起動を容認する
		SetDoubleStartValidFlag(1);

		//Activeでなくても動作する
		SetAlwaysRunFlag(_AlwaysRunFlag);

		//doubleをfloatレベルまで落として演算しない
		SetUseFPUPreserveFlag(_HighPreciseFlag) ;

		//解像度を変更する
		SetGraphMode(ScreenSize.x,ScreenSize.y,ScreenClrBit);

		//ウィンドウモード
		ChangeWindowMode(WindowModeFlag) ;

		//ウィンドウサイズ変更可能モード
		SetWindowSizeChangeEnableFlag(WindowExtendFlag);

		//ウィンドウ名を変更
		SetMainWindowText(_WindowName.c_str());

		//ウィンドウアイコン変更
		SetWindowIconID(1) ;
		
		//DxLib初期化
		if( DxLib_Init() == -1 )return 1;

		// 描画先画面を裏画面にする
		SetDrawScreen( DX_SCREEN_BACK ) ;

		//文字列初期化
		VFont.make(Font.getSize(),Font.getThick(),Font.getName(),1,Font.getType());

		//入力クラス初期化
		dxInput::defaultInp.make();
		Input.make();

		//DMode,DArea初期化
		resetGlobalStd();
		resetKeyActive();
		resetDrawMode();
		resetDrawArea();
		resetDrawScreen();

		return 0;
	}
	int work(int _DrawIntervalTime){
		//変換があれば描写
		drawInputChange();

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();

		//処理待ち時間(ms)
		WaitTimer(_DrawIntervalTime);

		// Windows システムからくる情報を処理する
		if( ProcessMessage() == -1 )return -1;

		//KeyのStatusセット
		setKey();

		//Time関連処理
		Cnt++;
		Time=GetNowCount();
		if(Cnt%10==0){
			Fps=10000./((double)(Time-PreTime));
			PreTime=Time;
		}

		//画面描画内容消去
		ClearDrawScreen() ;

//		if(dxRect::iniRectArea())throw "dx::work(int _DrawIntervalTime) Non deleted dxRect exists";

		//Input関連
		dxInput::defaultInp.setStr("");

		return 0;
	}
	int end(){
		DxLib_End();
		return 0;
	}
	//Screen関連
	int getScreenX(){return ScreenSize.x;}
	int getScreenY(){return ScreenSize.y;}
	Pint getScreenSize(){return ScreenSize;}
	int getScreenClrBit(){return ScreenClrBit;}
	//Font関連
	std::string getFontName(int FontHandle){
		if(FontHandle==-1)return Font.Name;

		GetFontStateToHandle(hmstd::getTmpStrAdress(),NULL,NULL,FontHandle);
		return hmstd::fStr("%s",hmstd::getTmpStrAdress());
	}
	int getFontSize(int FontHandle){
		if(FontHandle==-1)return Font.Size;

		int tmp;
		GetFontStateToHandle(NULL,&tmp,NULL,FontHandle);
		return tmp;
	}
	int getFontThick(int FontHandle){
		if(FontHandle==-1)return Font.Thick;

		int tmp;
		GetFontStateToHandle(NULL,NULL,&tmp,FontHandle);
		return tmp;
	}
	FONTTYPE::FontTypeName getFontType(){return Font.Type;}
	void setFont(const std::string& _FontName,const int& _Size,const int& _Thick,FONTTYPE::FontTypeName _Type){
		Font.Name=_FontName;
		ChangeFont(_FontName.c_str());
		Font.Size=_Size;
		SetFontSize(_Size);
		Font.Thick=_Thick;
		SetFontThickness(_Thick);
		Font.VFlag=0;
		Font.Type=_Type;
		ChangeFontType(_Type);

		VFont.make(_Size,_Thick,_FontName,1,_Type);
	}
	const dxFont& getStdFont(){return Font;}
	const dxFont& getStdVFont(){return VFont;}
	int getDrawStringWidth(const std::string& str,int strlen,int FontHandle,int VFlag){
		if(FontHandle<0)return GetDrawStringWidth(str.c_str(),strlen,VFlag);
		else return GetDrawStringWidthToHandle(str.c_str(),strlen,FontHandle,VFlag);
	}
	//Input関連
	bool checkStdInp(const std::string& _InputName){return (InputName==_InputName);}
	int checkStdInpSta(){return Input.check();}
	void setStdInp(const std::string& _InputName){InputName=_InputName;}
	dxInput& getStdInp(const std::string& _InputName){
		static dxInput Null;
		if(InputName==_InputName)return Input;
		else return Null;
	}
	//KeyGet関数
	int getKey(KEY::KEYNAME KeyName){
		if(!KeyActiveFlag)return 0;
		if(KeyName<256)return KeySta[KeyName];
		else return OtherSta[KeyName-256];
	}
	int getNum(int Num){
		if(!KeyActiveFlag)return 0;
		switch(Num){
			case 0:
				return getKey2(dx::getKey(KEY::NUM0),dx::getKey(KEY::NUMPAD0));
			case 1:
				return getKey2(dx::getKey(KEY::NUM1),dx::getKey(KEY::NUMPAD1));
			case 2:
				return getKey2(dx::getKey(KEY::NUM2),dx::getKey(KEY::NUMPAD2));
			case 3:
				return getKey2(dx::getKey(KEY::NUM3),dx::getKey(KEY::NUMPAD3));
			case 4:
				return getKey2(dx::getKey(KEY::NUM4),dx::getKey(KEY::NUMPAD4));
			case 5:
				return getKey2(dx::getKey(KEY::NUM5),dx::getKey(KEY::NUMPAD5));
			case 6:
				return getKey2(dx::getKey(KEY::NUM6),dx::getKey(KEY::NUMPAD6));
			case 7:
				return getKey2(dx::getKey(KEY::NUM7),dx::getKey(KEY::NUMPAD7));
			case 8:
				return getKey2(dx::getKey(KEY::NUM8),dx::getKey(KEY::NUMPAD8));
			case 9:
				return getKey2(dx::getKey(KEY::NUM9),dx::getKey(KEY::NUMPAD9));
			default:
				return 0;
		}
	}
	int getKeyUp(){return getKey(KEY::UP);}
	int getKeyDown(){return getKey(KEY::DOWN);}
	int getKeyLeft(){return getKey(KEY::LEFT);}
	int getKeyRight(){return getKey(KEY::RIGHT);}
	int getMosL(){return getKey(KEY::MOSL);}
	int getMosR(){return getKey(KEY::MOSR);}
	int getMosC(){return getKey(KEY::MOSC);}
	int getMosH(){
		if(!KeyActiveFlag)return 0;
		return MosHNum;
	}
	Pint getMosPos(){return StoG(MosPos);}
	Pint getMosPPos(){return StoG(MosPPos);}
	Pint getMosLPos(){return StoG(MosLPos);}
	Pint getMosRPos(){return StoG(MosRPos);}
	Pint getMosCPos(){return StoG(MosCPos);}
	//Time関連
	double getFPS(){return Fps;}
	int getFrameTime(){return Time-IniTime;}
	int getFrameCnt(){return Cnt;}
	//描画関連
	void setDrawInpCandiNum(uint minNo,uint maxNo){
		DrawInpCandiMin=minNo;
		DrawInpCandiMax=maxNo;
	}
	//基本関数
	bool getKeyActive(){return KeyActiveFlag;}
	void setKeyActive(bool _KeyActiveFlag){KeyActiveFlag=_KeyActiveFlag;}
	void resetKeyActive(){KeyActiveFlag=true;}
	const dxDMode& getDrawMode(){return NowMode;}
	void setDrawMode(const dxDMode& _dxdraw){NowMode+=_dxdraw;}
	void resetDrawMode(){NowMode=dxDMode();}
	const dxDArea& getDrawArea(){return NowArea;}
	void setDrawArea(const dxDArea& _darea){
		if(!_darea.activeF)resetDrawArea();
		else{
			NowArea.pos1=GtoS(_darea.pos1);
			NowArea.pos2=GtoS(_darea.pos2);
			SetDrawArea(NowArea.pos1.x,NowArea.pos1.y,NowArea.pos2.x,NowArea.pos2.y);
		}
	}
	void resetDrawArea(){
		SetDrawArea(0,0,dx::getScreenX(),dx::getScreenY());
		NowArea.activeF=false;
	}
	Pint getGlobalStd(){return GlobalStd;}
	void setGlobalStd(const Pint& _gstd){GlobalStd=_gstd;}
	void resetGlobalStd(){GlobalStd.set(0,0);}
	dxBasicScreen getDrawScreen(){return NowScreen;}
	void setDrawScreen(const dxBasicScreen& _dxgrp){
		NowScreen=_dxgrp;
		if(_dxgrp.getHandle()<=0)SetDrawScreen(DX_SCREEN_BACK);
		else SetDrawScreen(NowScreen.getHandle());
	}
	void resetDrawScreen(){
		SetDrawScreen(DX_SCREEN_BACK);
	}
	inline Pint GtoS(const Pint& pos){return pos+GlobalStd;}
	inline Pint StoG(const Pint& pos){return pos-GlobalStd;}
}
//Graph関連
void dxBasicGraph::copy(const dxBasicGraph& _gra){
	if(this==&_gra)return;

	Pint size=_gra.getSize();

	dxScreen tmp;
	tmp.make(size);
	dx::TmpDrawScreen tmpDS(tmp);
	dx::TmpDrawArea tmpDA(dxDArea(Rint(Pint(0,0),size)));
	dx::drawGraph(Pint(0,0),_gra,0);

	del();

	make(size);
	get(Pint(0,0));
}
bool dxBasicGraph::load(const dxImGraph& _imgra){
	del();
	Handle=CreateGraphFromSoftImage(_imgra.getHandle());
	return fail();
}
void dxBasicGraph::save(const std::string& _filename)const{
	Pint size=getSize();

	dxScreen tmp;
	tmp.make(size);
	dx::TmpDrawScreen tmpDS(tmp);
	dx::TmpDrawArea tmpDA(dxDArea(Rint(Pint(0,0),size)));
	dx::drawGraph(Pint(0,0),*this,0,0);

	SaveDrawScreen(0,0,size.x,size.y,_filename.c_str());
}
bool dxBasicScreen::load(const dxImGraph& _imgra){
	del();
	SetDrawValidGraphCreateFlag( TRUE );
	Handle=CreateGraphFromSoftImage(_imgra.getHandle());
	SetDrawValidGraphCreateFlag( FALSE );
	return fail();
}
void dxBasicScreen::copy(const dxBasicGraph& _grp){
	make(_grp.getSize());
	dx::TmpDrawScreen tmpDS(*this);
	dx::TmpDrawArea tmpDA(dxDArea(Rint(Pint(0,0),_grp.getSize())));
	dx::drawGraph(Pint(0,0),_grp,0);
}
void dxBasicScreen::save(const std::string& _filename)const{
	Pint size=getSize();
	dx::TmpDrawScreen tmpDS(*this);
	SaveDrawScreen(0,0,size.x,size.y,_filename.c_str());
}
bool dxImGraph::load(const dxBasicGraph& _grph,bool _AlphaFlag,bool _PaletFlag){
	dxScreen tmpScr;
	tmpScr.copy(_grph);

	Pint tmp=_grph.getSize();
	del();
	make(tmp,_AlphaFlag,_PaletFlag);
	dx::TmpDrawScreen tmpDS(tmpScr);
	for(int x=0;x<tmp.x;x++){
		for(int y=0;y<tmp.y;y++){
			put(Pint(x,y),dxRGB(GetPixel(x,y)),255);
		}
	}
	return fail();
}
//Input関連
int dxInput::activeHandle;
dxInput dxInput::defaultInp;
namespace dx{
	//dx独自関数群
	void drawBox(const Pint& pos1,const Pint& pos2,const dxColor& clr,int FillFlag,dxDMode dmode){
		useDrawMode(NowMode+dmode);
		DrawBox(pos1.x+GlobalStd.x,pos1.y+GlobalStd.y,pos2.x+GlobalStd.x,pos2.y+GlobalStd.y,clr.getClr(),FillFlag);
	}
	void drawTriangle(const Pint& pos1,const Pint& pos2,const Pint& pos3,const dxColor& clr,int FillFlag,dxDMode dmode){
		useDrawMode(NowMode+dmode);
		DrawTriangle(pos1.x+GlobalStd.x,pos1.y+GlobalStd.y,pos2.x+GlobalStd.x,pos2.y+GlobalStd.y,pos3.x+GlobalStd.x,pos3.y+GlobalStd.y,clr.getClr(),FillFlag);
	}
	void drawCircle(const Pint& pos,const int& r,const dxColor& clr,int FillFlag,dxDMode dmode){
		useDrawMode(NowMode+dmode);
		DrawCircle(pos.x+GlobalStd.x,pos.y+GlobalStd.y,r,clr.getClr(),FillFlag);
	}
	void drawOval(const Pint& pos,const Pint& r,const dxColor& clr,int FillFlag,dxDMode dmode){
		useDrawMode(NowMode+dmode);
		DrawOval(pos.x+GlobalStd.x,pos.y+GlobalStd.y,r.x,r.y,clr.getClr(),FillFlag);
	}
	void drawPixel(const Pint& pos,const dxColor& clr,dxDMode dmode){
		useDrawMode(NowMode+dmode);
		DrawPixel(pos.x+GlobalStd.x,pos.y+GlobalStd.y,clr.getClr());
	}
	void drawLine(const Pint& pos1,const Pint& pos2,const dxColor& clr,int Thick,dxDMode dmode){
		useDrawMode(NowMode+dmode);
		DrawLine(pos1.x+GlobalStd.x,pos1.y+GlobalStd.y,pos2.x+GlobalStd.x,pos2.y+GlobalStd.y,clr.getClr(),Thick);
	}
	int drawStr(const Pint& pos,const std::string Str,const dxColor& sclr,dxDMode dmode,int VFlag,int FontHandle,const dxColor& seclr){
		useDrawMode(NowMode+dmode);

		if(FontHandle==-1){
			if(VFlag)DrawVString(pos.x+GlobalStd.x,pos.y+GlobalStd.y,Str.c_str(),sclr.getClr(),seclr.getClr());
			else DrawString(pos.x+GlobalStd.x,pos.y+GlobalStd.y,Str.c_str(),sclr.getClr(),seclr.getClr());
		}else{
			if(VFlag)DrawVStringToHandle(pos.x+GlobalStd.x,pos.y+GlobalStd.y,Str.c_str(),sclr.getClr(),FontHandle,seclr.getClr());
			else DrawStringToHandle(pos.x+GlobalStd.x,pos.y+GlobalStd.y,Str.c_str(),sclr.getClr(),FontHandle,seclr.getClr());
		}

		if(FontHandle==-1)return GetDrawStringWidth(Str.c_str(),Str.size());
		else return GetDrawStringWidthToHandle(Str.c_str(),Str.size(),FontHandle);
	}
	int drawStr(const Pint& pos,const std::string Str,const dxColor& sclr,const dxFont& font,dxDMode dmode,const dxColor& seclr){
		return drawStr(pos,Str,sclr,dmode,font.getVFlag(),font.getHandle(),seclr);
	}
	int drawStrP(const Pint& pos,const Pint& size,const std::string Str,const dxColor& sclr,ALI::ALIGN DrawPos,dxDMode dmode,int VFlag,int FontHandle,int FontSize,const dxColor& seclr){
		useDrawMode(NowMode+dmode);

		int len;
		if(FontHandle==-1){
			len=GetDrawStringWidth(Str.c_str(),Str.size());

			if(VFlag){
				int xctr=size.x/2-dx::getFontSize()/2;
				int yctr=size.y/2-len/2;
				if(DrawPos&ALI::left)xctr=0;
				else if(DrawPos&ALI::right)xctr=size.x-len;
				if(DrawPos&ALI::top)yctr=0;
				else if(DrawPos&ALI::bottom)yctr=size.y-dx::getFontSize();
				DrawVString(pos.x+xctr+GlobalStd.x,pos.y+yctr+GlobalStd.y,Str.c_str(),sclr.getClr(),seclr.getClr());
			}else{
				int xctr=size.x/2-len/2;
				int yctr=size.y/2-dx::getFontSize()/2;
				if(DrawPos&ALI::left)xctr=0;
				else if(DrawPos&ALI::right)xctr=size.x-len;
				if(DrawPos&ALI::top)yctr=0;
				else if(DrawPos&ALI::bottom)yctr=size.y-dx::getFontSize();
				DrawString(pos.x+xctr+GlobalStd.x,pos.y+yctr+GlobalStd.y,Str.c_str(),sclr.getClr(),seclr.getClr());
			}
		}else{
			len=GetDrawStringWidthToHandle(Str.c_str(),Str.size(),FontHandle);
			if(FontSize==0)GetFontStateToHandle(NULL,&FontSize,NULL,FontHandle);

			if(VFlag){
				int xctr=size.x/2-FontSize/2;
				int yctr=size.y/2-len/2;
				if(DrawPos&ALI::left)xctr=0;
				else if(DrawPos&ALI::right)xctr=size.x-len;
				if(DrawPos&ALI::top)yctr=0;
				else if(DrawPos&ALI::bottom)yctr=size.y-FontSize;
				DrawVStringToHandle(pos.x+xctr+GlobalStd.x,pos.y+yctr+GlobalStd.y,Str.c_str(),sclr.getClr(),FontHandle,seclr.getClr());
			}else{
				int xctr=size.x/2-len/2;
				int yctr=size.y/2-FontSize/2;
				if(DrawPos&ALI::left)xctr=0;
				else if(DrawPos&ALI::right)xctr=size.x-len;
				if(DrawPos&ALI::top)yctr=0;
				else if(DrawPos&ALI::bottom)yctr=size.y-FontSize;
				DrawStringToHandle(pos.x+xctr+GlobalStd.x,pos.y+yctr+GlobalStd.y,Str.c_str(),sclr.getClr(),FontHandle,seclr.getClr());
			}
		}

		return len;
	}
	int drawStrP(const Pint& pos,const Pint& size,const std::string Str,const dxColor& sclr,const dxFont& font,ALI::ALIGN DrawPos,dxDMode dmode,const dxColor& seclr){
		return drawStrP(pos,size,Str,sclr,DrawPos,dmode,font.getVFlag(),font.getHandle(),font.getSize(),seclr);
	}
	int drawInpStr(const Pint& pos,const dxInput& inp,const dxColor& sclr,const dxColor& imeclr,dxDMode dmode,int VFlag,int FontHandle,int FontSize,const dxColor& seclr){
		useDrawMode(NowMode+dmode);

		IMEINPUTDATA* imep;
		imep=GetIMEInputData();

		std::string str=inp.getStr();
		int crp=GetKeyInputCursorPosition(inp.getHandle());
		int crplen=dx::getDrawStringWidth(str,crp,FontHandle,VFlag);
		int adjlen=0;

		if(inp.check()!=1){
			if(VFlag)drawStr(pos,str,sclr,dmode,VFlag,FontHandle,seclr);
			else drawStr(pos,str,sclr,dmode,VFlag,FontHandle,seclr);
		}else if(imep){
			std::string imestr=inp.getInpStr();
			std::string tmpstr=str.substr(0,crp);

			drawStr(pos,tmpstr,sclr,dmode,VFlag,FontHandle,seclr);

			adjlen=crplen;
			int addlen;

			for(int i=0;i< imep->ClauseNum ;i++){
				tmpstr=imestr.substr(imep->ClauseData[i].Position,imep->ClauseData[i].Length);
				addlen=dx::getDrawStringWidth(tmpstr,tmpstr.size(),FontHandle,VFlag);

				if(VFlag){			
					if(i==imep->SelectClause){
	//					drawLine(pos+Pint(FontSize+1,adjlen+1),pos+Pint(FontSize+1,adjlen+addlen),sclr,3,dmode);
						drawBox(pos+Pint(0,adjlen),pos+Pint(0,adjlen)+Pint(FontSize,addlen),sclr,1,dmode);
						drawStr(pos+Pint(0,adjlen),tmpstr,imeclr,dmode,VFlag,FontHandle,seclr);

						if(imep->CandidateNum){
							ReservDrawInputChange(pos,adjlen,imep->CandidateNum,imep->SelectCandidate,imep->CandidateList,FontHandle,FontSize,VFlag,dmode,sclr,imeclr,seclr);
	/*						int selno=imep->SelectCandidate;
							int numlen;
							int strlen;
							std::string numstr;

							for(int j=selno-(int)DrawInpCandiMin;j<=selno+(int)DrawInpCandiMax; j++ ){
								if(j<0 || j>=imep->CandidateNum || j==selno)continue;

								tmpstr=imep->CandidateList[j];
								numstr=hmstd::fStr("%d:",j+1);
								strlen=dx::getDrawStringWidth(tmpstr,tmpstr.size(),FontHandle,VFlag);
								numlen=dx::getDrawStringWidth(numstr,numstr.size(),FontHandle,VFlag);
								DrawBox(pos+Pint(FontSize*(j-selno),adjlen-numlen),pos+Pint(FontSize*(j-selno),adjlen)+Pint(FontSize,strlen),IMEColor,1,dmode);
								DrawStr(pos+Pint(FontSize*(j-selno),adjlen-numlen),numstr+tmpstr,sclr,dmode,VFlag,FontHandle,seclr);
							}
	*/
						}

					}else{
						drawStr(pos+PintY(adjlen),tmpstr,sclr,dmode,VFlag,FontHandle,seclr);
						drawLine(pos+Pint(FontSize+1,adjlen+1),pos+Pint(FontSize+1,adjlen+addlen),sclr,1,dmode);
					}
				}else{
					if(i==imep->SelectClause){
	//					DrawLine(pos+Pint(FontSize+1,adjlen+1),pos+Pint(FontSize+1,adjlen+addlen),sclr,3,dmode);
						drawBox(pos+Pint(adjlen,0),pos+Pint(adjlen,0)+Pint(addlen,FontSize),sclr,1,dmode);
						drawStr(pos+Pint(adjlen,0),tmpstr,imeclr,dmode,VFlag,FontHandle,seclr);

						if(imep->CandidateNum){
							ReservDrawInputChange(pos,adjlen,imep->CandidateNum,imep->SelectCandidate,imep->CandidateList,FontHandle,FontSize,VFlag,dmode,sclr,imeclr,seclr);
	/*						int selno=imep->SelectCandidate;
							int numlen;
							int strlen;
							std::string numstr;

							for(int j=selno-(int)DrawInpCandiMin;j<=selno+(int)DrawInpCandiMax; j++ ){
								if(j<0 || j>=imep->CandidateNum || j==selno)continue;

								tmpstr=imep->CandidateList[j];
								numstr=hmstd::fStr("%d:",j+1);
								strlen=dx::getDrawStringWidth(tmpstr,tmpstr.size(),FontHandle,VFlag);
								numlen=dx::getDrawStringWidth(numstr,numstr.size(),FontHandle,VFlag);
								DrawBox(pos+Pint(adjlen-numlen,FontSize*(j-selno)),pos+Pint(adjlen,FontSize*(j-selno))+Pint(strlen,FontSize),IMEColor,1,dmode);
								DrawStr(pos+Pint(adjlen-numlen,FontSize*(j-selno)),numstr+tmpstr,sclr,dmode,VFlag,FontHandle,seclr);
							}
	*/
						}

					}else{
						drawStr(pos+PintX(adjlen),tmpstr,sclr,dmode,VFlag,FontHandle,seclr);
						drawLine(pos+Pint(adjlen+1,FontSize+1),pos+Pint(adjlen+addlen,FontSize+1),sclr,1,dmode);
					}
				}

				adjlen+=addlen;
			}

			tmpstr=str.substr(crp,str.size()-crp);
			if(VFlag)drawStr(pos+PintY(adjlen),tmpstr,sclr,dmode,VFlag,FontHandle,seclr);
			else drawStr(pos+PintX(adjlen),tmpstr,sclr,dmode,VFlag,FontHandle,seclr);

			//カーソル表示
			crplen+=dx::getDrawStringWidth(imestr,imep->CursorPosition,FontHandle,VFlag);
			if(VFlag)drawLine(pos+Pint(-1,crplen-1),pos+Pint(FontSize+1,crplen-1),sclr,1,dmode);
			else drawLine(pos+Pint(crplen-1,-1),pos+Pint(crplen-1,FontSize+1),sclr,1,dmode);
		}else{
			if(VFlag){
				drawStr(pos,str,sclr,dmode,VFlag,FontHandle,seclr);
				drawLine(pos+Pint(-1,crplen-1),pos+Pint(FontSize+1,crplen-1),sclr,1,dmode);
			}else{
				drawStr(pos,str,sclr,dmode,VFlag,FontHandle,seclr);
				drawLine(pos+Pint(crplen-1,-1),pos+Pint(crplen-1,FontSize+1),sclr,1,dmode);
			}
		}

		return 0;
	}
	int drawInpStr(const Pint& pos,const dxInput& inp,const dxColor& sclr,const dxColor& imeclr,const dxFont& font,dxDMode dmode,const dxColor& seclr){
		return drawInpStr(pos,inp,sclr,imeclr,dmode,font.getVFlag(),font.getHandle(),font.getSize(),seclr);
	}
	int drawInpStrP(const Pint& pos,const Pint& size,const dxInput& inp,const dxColor& sclr,const dxColor& imeclr,ALI::ALIGN DrawPos,dxDMode dmode,int VFlag,int FontHandle,int FontSize,const dxColor& seclr){
		useDrawMode(NowMode+dmode);

		int len=dx::getDrawStringWidth(inp.getStr()+inp.getInpStr(),(inp.getStr()+inp.getInpStr()).size(),FontHandle,VFlag);

		int xctr;
		int yctr;
		if(VFlag){
			xctr=size.x/2-FontSize/2;
			yctr=size.y/2-len/2;
			if(DrawPos&ALI::left)xctr=0;
			else if(DrawPos&ALI::right)xctr=size.x-len;
			if(DrawPos&ALI::top)yctr=0;
			else if(DrawPos&ALI::bottom)yctr=size.y-FontSize;
		}else{
			xctr=size.x/2-len/2;
			yctr=size.y/2-FontSize/2;
			if(DrawPos&ALI::left)xctr=0;
			else if(DrawPos&ALI::right)xctr=size.x-len;
			if(DrawPos&ALI::top)yctr=0;
			else if(DrawPos&ALI::bottom)yctr=size.y-FontSize;
		}

		return drawInpStr(pos+Pint(xctr,yctr),inp,sclr,imeclr,dmode,VFlag,FontHandle,FontSize,seclr);
	}
	int drawInpStrP(const Pint& pos,const Pint& size,const dxInput& inp,const dxColor& sclr,const dxColor& IMEColor,const dxFont& font,ALI::ALIGN DrawPos,dxDMode dmode,const dxColor& seclr){
		return drawInpStrP(pos,size,inp,sclr,IMEColor,DrawPos,dmode,font.getVFlag(),font.getHandle(),font.getSize(),seclr);
	}
	void drawGraph(const Pint& pos,const dxBasicGraph& grp,int TransFlag,int TurnFlag,dxDMode dmode){
		useDrawMode(NowMode+dmode);
		if(TurnFlag)DrawTurnGraph(pos.x+GlobalStd.x,pos.y+GlobalStd.y,grp.getHandle(),TransFlag);
		else DrawGraph(pos.x+GlobalStd.x,pos.y+GlobalStd.y,grp.getHandle(),TransFlag);
	}
	void drawGraph(const Pint& pos,const dxBasicGraph& grp,const Pint& grppos,const Pint& grpsize,int TransFlag,int TurnFlag,dxDMode dmode){
		useDrawMode(NowMode+dmode);
		DrawRectGraph(pos.x+GlobalStd.x,pos.y+GlobalStd.y,grppos.x,grppos.y,grpsize.x,grpsize.y,grp.getHandle(),TransFlag,TurnFlag);
	}
	void drawGraph(const Pint& pos,const dxBasicGraph& grp,const Pint& size,int TransFlag,dxDMode dmode){
		useDrawMode(NowMode+dmode);
		if(grp.getSize()==size){
			DrawGraph(pos.x+GlobalStd.x,pos.y+GlobalStd.y,grp.getHandle(),TransFlag);
		}else DrawExtendGraph(pos.x+GlobalStd.x,pos.y+GlobalStd.y,pos.x+size.x+GlobalStd.x,pos.y+size.y+GlobalStd.y,grp.getHandle(),TransFlag);
	}
	void drawGraph(const Pint& pos,const dxBasicGraph& grp,const double& rate,const double& angle,int TransFlag,int TurnFlag,dxDMode dmode){
		useDrawMode(NowMode+dmode);
		DrawRotaGraph(pos.x+GlobalStd.x,pos.y+GlobalStd.y,rate,angle,grp.getHandle(),TransFlag,TurnFlag);
	}
	void drawGraph(const Pint& pos,const dxBasicGraph& grp,const double& rate,const double& angle,const Pint& cpos,int TransFlag,int TurnFlag,dxDMode dmode){
		useDrawMode(NowMode+dmode);
		DrawRotaGraph2(pos.x+GlobalStd.x,pos.y+GlobalStd.y,cpos.x,cpos.y,rate,angle,grp.getHandle(),TransFlag,TurnFlag);
	}
	void drawImGraph(const Pint& pos,const dxImGraph& imgrp,dxDMode dmode){
		useDrawMode(NowMode+dmode);
		DrawSoftImage(pos.x+GlobalStd.x,pos.y+GlobalStd.y,imgrp.getHandle());
	}
}
#
#endif
