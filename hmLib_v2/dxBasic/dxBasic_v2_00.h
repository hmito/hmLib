#ifndef INC_HMLIB_DXBASIC
#define INC_HMLIB_DXBASIC 205
#
#include "hmLib_v2/DxLib.h"
#ifndef INC_HMLIB_DXCOLOR
#	include "hmLib_v2/dxColor.hpp"
#endif
#ifndef INC_HMLIB_HMRECT
#	include "hmLib_v2/hmRect.hpp"
#endif
#ifndef INC_HMLIB_HMALIGN
#	include "hmLib_v2/hmAlign.hpp"
#endif
#include <deque>
#include <list>
//namespace群
namespace KEY{
	enum KEYNAME{
		BACK=KEY_INPUT_BACK, 	// バックスペースキー
		TAB=KEY_INPUT_TAB, 	// タブキー
		ENTER=KEY_INPUT_RETURN, 	// エンターキー
		LSHIFT=KEY_INPUT_LSHIFT, 	// 左シフトキー
		RSHIFT=KEY_INPUT_RSHIFT,	// 右シフトキー
		LCTRL=KEY_INPUT_LCONTROL,	// 左コントロールキー
		RCTRL=KEY_INPUT_RCONTROL, 	// 右コントロールキー
		ESC=KEY_INPUT_ESCAPE, 	// エスケープキー
		SPACE=KEY_INPUT_SPACE, 	// スペースキー
		PGUP=KEY_INPUT_PGUP,	// ＰａｇｅＵＰキー
		PGDN=KEY_INPUT_PGDN, 	// ＰａｇｅＤｏｗｎキー
		END=KEY_INPUT_END, 	// エンドキー
		HOME=KEY_INPUT_HOME, 	// ホームキー
		LEFT=KEY_INPUT_LEFT, 	// 左キー
		UP=KEY_INPUT_UP, 	// 上キー
		RIGHT=KEY_INPUT_RIGHT, 	// 右キー
		DOWN=KEY_INPUT_DOWN,	// 下キー
		INS=KEY_INPUT_INSERT, 	// インサートキー
		DEL=KEY_INPUT_DELETE,	// デリートキー
		MINUS=KEY_INPUT_MINUS, 	// －キー
		INPUT=KEY_INPUT_YEN, 	// ￥キー
		PREVTRACK=KEY_INPUT_PREVTRACK, 	// ＾キー
		PERIOD=KEY_INPUT_PERIOD, 	// ．キー
		SLASH=KEY_INPUT_SLASH, 	// ／キー
		LALT=KEY_INPUT_LALT, 	// 左ＡＬＴキー
		RALT=KEY_INPUT_RALT, 	// 右ＡＬＴキー
		SCROLL=KEY_INPUT_SCROLL, 	// ScrollLockキー
		SEMICOLON=KEY_INPUT_SEMICOLON, 	// ；キー
		COLON=KEY_INPUT_COLON, 	// ：キー
		LBRACKET=KEY_INPUT_LBRACKET, 	// ［キー
		RBRACKET=KEY_INPUT_RBRACKET, 	// ］キー
		AT=KEY_INPUT_AT, 	// ＠キー
		BACKSLASH=KEY_INPUT_BACKSLASH, 	// ＼キー
		COMMA=KEY_INPUT_COMMA, 	// ，キー
		CAPLOCK=KEY_INPUT_CAPSLOCK, 	// CaspLockキー
		PAUSE=KEY_INPUT_PAUSE, 	// PauseBreakキー
		NUMPAD0=KEY_INPUT_NUMPAD0, 	// テンキー０
		NUMPAD1=KEY_INPUT_NUMPAD1, 	// テンキー１
		NUMPAD2=KEY_INPUT_NUMPAD2, 	// テンキー２
		NUMPAD3=KEY_INPUT_NUMPAD3, 	// テンキー３
		NUMPAD4=KEY_INPUT_NUMPAD4, 	// テンキー４
		NUMPAD5=KEY_INPUT_NUMPAD5, 	// テンキー５
		NUMPAD6=KEY_INPUT_NUMPAD6, 	// テンキー６
		NUMPAD7=KEY_INPUT_NUMPAD7, 	// テンキー７
		NUMPAD8=KEY_INPUT_NUMPAD8, 	// テンキー８
		NUMPAD9=KEY_INPUT_NUMPAD9, 	// テンキー９
		NUMPADMLT=KEY_INPUT_MULTIPLY, 	// テンキー＊キー
		NUMPADADD=KEY_INPUT_ADD, 	// テンキー＋キー
		NUMPADSUB=KEY_INPUT_SUBTRACT, 	// テンキー－キー
		NUMPADDOT=KEY_INPUT_DECIMAL, 	// テンキー．キー
		NUMPADDIV=KEY_INPUT_DIVIDE, 	// テンキー／キー
		NUMPADENTER=KEY_INPUT_NUMPADENTER, 	// テンキーのエンターキー
		F1=KEY_INPUT_F1, 	// Ｆ１キー
		F2=KEY_INPUT_F2, 	// Ｆ２キー
		F3=KEY_INPUT_F3, 	// Ｆ３キー
		F4=KEY_INPUT_F4, 	// Ｆ４キー
		F5=KEY_INPUT_F5, 	// Ｆ５キー
		F6=KEY_INPUT_F6, 	// Ｆ６キー
		F7=KEY_INPUT_F7, 	// Ｆ７キー
		F8=KEY_INPUT_F8, 	// Ｆ８キー
		F9=KEY_INPUT_F9, 	// Ｆ９キー
		F10=KEY_INPUT_F10, 	// Ｆ１０キー
		F11=KEY_INPUT_F11, 	// Ｆ１１キー
		F12=KEY_INPUT_F12, 	// Ｆ１２キー
		A=KEY_INPUT_A ,	// Ａキー
		B=KEY_INPUT_B ,	// Ｂキー
		C=KEY_INPUT_C ,	// Ｃキー
		D=KEY_INPUT_D ,	// Ｄキー
		E=KEY_INPUT_E ,	// Ｅキー
		F=KEY_INPUT_F ,	// Ｆキー
		G=KEY_INPUT_G ,	// Ｇキー
		H=KEY_INPUT_H ,	// Ｈキー
		I=KEY_INPUT_I ,	// Ｉキー
		J=KEY_INPUT_J ,	// Ｊキー
		K=KEY_INPUT_K ,	// Ｋキー
		L=KEY_INPUT_L ,	// Ｌキー
		M=KEY_INPUT_M ,	// Ｍキー
		N=KEY_INPUT_N ,	// Ｎキー
		O=KEY_INPUT_O ,	// Ｏキー
		P=KEY_INPUT_P ,	// Ｐキー
		Q=KEY_INPUT_Q ,	// Ｑキー
		R=KEY_INPUT_R ,	// Ｒキー
		S=KEY_INPUT_S ,	// Ｓキー
		T=KEY_INPUT_T ,	// Ｔキー
		U=KEY_INPUT_U ,	// Ｕキー
		V=KEY_INPUT_V ,	// Ｖキー
		W=KEY_INPUT_W ,	// Ｗキー
		X=KEY_INPUT_X ,	// Ｘキー
		Y=KEY_INPUT_Y ,	// Ｙキー
		Z=KEY_INPUT_Z ,	// Ｚキー
		NUM0=KEY_INPUT_0 ,	// ０キー
		NUM1=KEY_INPUT_1 ,	// １キー
		NUM2=KEY_INPUT_2 ,	// ２キー
		NUM3=KEY_INPUT_3 ,	// ３キー
		NUM4=KEY_INPUT_4 ,	// ４キー
		NUM5=KEY_INPUT_5 ,	// ５キー
		NUM6=KEY_INPUT_6 ,	// ６キー
		NUM7=KEY_INPUT_7, 	// ７キー
		NUM8=KEY_INPUT_8, 	// ８キー
		NUM9=KEY_INPUT_9, 	// ９キー
		SHIFT=256,			//両Shift
		CTRL=257,			//両CTRL
		ALT=258,			//両ALT
		MOSL=259,
		MOSR=260,
		MOSC=261
	};
}
namespace BLEND{
	enum BlendMode{Normal=DX_BLENDMODE_NOBLEND,Alpha=DX_BLENDMODE_ALPHA,Add=DX_BLENDMODE_ADD,Sub=DX_BLENDMODE_SUB,Mula=DX_BLENDMODE_MULA,Invsrc=DX_BLENDMODE_INVSRC};
}
namespace FONTTYPE{
	enum FontTypeName{Normal=DX_FONTTYPE_NORMAL,Edge=DX_FONTTYPE_EDGE,Aalia=DX_FONTTYPE_ANTIALIASING,EdgeAalia=DX_FONTTYPE_ANTIALIASING_EDGE};
};
class dxDMode;
class dxDArea;
class dxFont;
class dxInput;
class basic_dxGraph;
class dxGrpah;
class dxGraphs;
class dxImGraph;
//dxnamespace
namespace dx{
//基本関数
	int ini(std::string _WindowName="dxBasic",int _ScreenX=800,int _ScreenY=600,int _ScreenClrBit=32,int _HighPreciseFlag=0,int _AlwaysRunFlag=1,int _WindowModeFlag=1,int _WindowExtendFlag=0);
	int work(int _DrawIntervalTime);
	int end();
//Screen関連
	int getScreenX();
	int getScreenY();
	Pint getScreenSize();
	int getScreenClrBit();
//Font関連
	std::string getFontName(int FontHandle=-1);
	int getFontSize(int FontHandle=-1);
	int getFontThick(int FontHandle=-1);
	FONTTYPE::FontTypeName getFontType();
	void setFont(const std::string& _FontName,const int& _Size,const int& _Thick,FONTTYPE::FontTypeName _Type=FONTTYPE::Normal);
	const dxFont& getStdFont();
	const dxFont& getStdVFont();
	int getDrawStringWidth(const std::string& str,int strlen,int FontHandle=-1,int VFlag=0);
//KeyGet関数
	int getKey(KEY::KEYNAME KeyName);
	int getNum(int i);
	int getKeyUp();
	int getKeyDown();
	int getKeyLeft();
	int getKeyRight();
	int getMosL();
	int getMosR();
	int getMosC();
	int getMosH();
	Pint getMosPos();
	Pint getMosPPos();
	Pint getMosLPos();
	Pint getMosRPos();
	Pint getMosCPos();
//Time関連
	double getFPS();
	int getFrameTime();
	int getFrameCnt();
//Input関連
	bool checkStdInp(const std::string& InpName);
	void setStdInp(const std::string& InpName);
	int checkStdInpSta();
	dxInput& getStdInp(const std::string& InpName);
//描画関連
	void setDrawInpCandiNum(uint minNo,uint maxNo);
//標準関数
	bool getKeyActive();
	void setKeyActive(bool _KeyActiveFlag);
	void resetKeyActive();
	class TmpKeyActive;
	Pint getGlobalStd();
	void setGlobalStd(const Pint& _gstd);
	void resetGlobalStd();
	class TmpGlobalStd;
	const dxDMode& getDrawMode();
	void setDrawMode(const dxDMode& _dmode);
	void resetDrawMode();
	class TmpDrawMode;
	const dxDArea& getDrawArea();
	void setDrawArea(const dxDArea& _darea);
	void resetDrawArea();
	class TmpDrawArea;
	basic_dxGraph getDrawScreen();
	void setDrawScreen(const basic_dxGraph& _dxgrp);
	void resetDrawScreen();
	class TmpDrawScreen;
	inline Pint GtoS(const Pint& pos);
	inline Pint StoG(const Pint& pos);
}
//dxDModeクラス
class dxDMode{
public:
	int Blend;
	BLEND::BlendMode bmode;
	dxRGB Bright;
	dxDMode():Bright(255,255,255){
		Blend=255;
		bmode=BLEND::Alpha;
	}
	dxDMode(int _Blend,BLEND::BlendMode _bmode=BLEND::Alpha,dxRGB _Bright=dxRGB(255,255,255)){
		Blend=_Blend;
		bmode=_bmode;
		Bright=_Bright;
	}
	dxDMode(dxRGB _Bright,int _Blend=255,BLEND::BlendMode _bmode=BLEND::Alpha){
		Blend=_Blend;
		bmode=_bmode;
		Bright=_Bright;
	}
	dxDMode operator+(const dxDMode& dmode){
		dxDMode tmp;
		tmp.Bright.r=((int)Bright.r*(int)dmode.Bright.r)/255;
		tmp.Bright.g=((int)Bright.g*(int)dmode.Bright.g)/255;
		tmp.Bright.b=((int)Bright.b*(int)dmode.Bright.b)/255;

		if(bmode==dmode.bmode){
			if(bmode==BLEND::Alpha){
				tmp.bmode=bmode;
				tmp.Blend=((int)Blend*(int)dmode.Blend)/255;
			}else{
				tmp.bmode=bmode;
				tmp.Blend=min((int)Blend+(int)dmode.Blend,255);
			}
		}else if(bmode==BLEND::Alpha){
			if(Blend==255){
				tmp.bmode=dmode.bmode;
				tmp.Blend=dmode.Blend;
			}else{
				tmp.bmode=bmode;
				tmp.Blend=Blend;
			}
		}else if(dmode.bmode==BLEND::Alpha){
			if(dmode.Blend==255){
				tmp.bmode=bmode;
				tmp.Blend=Blend;
			}else{
				tmp.bmode=dmode.bmode;
				tmp.Blend=dmode.Blend;
			}
		}
		return tmp;
	}
	dxDMode& operator+=(const dxDMode& dmode){
		*this=*this+dmode;
		return *this;
	}
};
//dxDAreaクラス
class dxDArea{
public:
	Pint pos1,pos2;
	bool activeF;
	dxDArea(){
		activeF=false;
	}
	dxDArea(const Rint& _rec){
		activeF=true;
		pos1=_rec.pos1;
		pos2=_rec.pos2;
	}
	void set(){activeF=false;}
	void set(const Rint& _rec){
		pos1=_rec.pos1;
		pos2=_rec.pos2;
		activeF=true;
	}
	Rint getRect()const{return Rint(pos1,pos2);}
	dxDArea operator&(const dxDArea& _darea)const{
		if(!activeF)return _darea;
		if(!_darea.activeF)return *this;
		return dxDArea(this->getRect()&_darea.getRect());
	}
};
//dxFontクラス
class dxFont{
private:
	std::string Name;
	int Handle;
	int VFlag;
	int Size;
	int Thick;
	FONTTYPE::FontTypeName Type;
public:
	dxFont(){
		Name="ＭＳ ゴシック";
		Handle=-1;
		VFlag=0;
		Size=16;
		Thick=6;
		Type=FONTTYPE::Normal;
	}
	bool make(int _Size=16,int _Thick=6,std::string _Name="ＭＳ ゴシック",int _VFlag=0,FONTTYPE::FontTypeName _Type=FONTTYPE::Normal){
		del();
		if(_VFlag!=0){
			VFlag=1;
			Handle=CreateFontToHandle(hmstd::fCharp("@%s",Name.c_str()),Size,Thick,Type);
		}else{
			VFlag=0;
			Handle=CreateFontToHandle(Name.c_str(),Size,Thick,Type);
		}
		if(Handle>0){
			Name=_Name;
			Size=_Size;
			Thick=_Thick;
			Type=_Type;
			return false;
		}else{
			Name=dx::getFontName();
			Size=dx::getFontSize();
			Thick=dx::getFontThick();
			Type=dx::getFontType();
			return true;
		}
	}
	~dxFont(){del();}
	void del(){
		if(Handle>=0){
			DeleteFontToHandle(Handle);
			Handle=-1;
		}
	}
	bool empty()const{return Handle<0;}
	std::string getName()const{
		if(Handle>=0)return Name;
		return dx::getFontName();
	}
	int getHandle()const{return Handle;}
	int getVFlag()const{
		if(Handle>=0)return VFlag;
		return 0;
	}
	int getSize()const{
		if(Handle>=0)return Size;
		return dx::getFontSize();
	}
	int getThick()const{
		if(Handle>=0)return Thick;
		return dx::getFontThick();
	}
	FONTTYPE::FontTypeName getType()const{
		if(Handle>=0)return Type;
		return dx::getFontType();
	}
	friend void dx::setFont(const std::string& _FontName,const int& _Size,const int& _Thick,FONTTYPE::FontTypeName _Type);
	friend int dx::getFontSize(int FontHandle);
	friend int dx::getFontThick(int FontHandle);
	friend FONTTYPE::FontTypeName dx::getFontType();
	friend std::string dx::getFontName(int FontHandle);
};
//dxInputクラス
class dxInput{
private:
	int Handle;
	static int activeHandle;
	static dxInput defaultInp;
public:
	dxInput(){Handle=-1;}
	~dxInput(){del();}
	void del(){
		if(Handle>0)DeleteKeyInput(Handle);
		Handle=-1;
	}
	void make(int MaxStrLength=256,int CancelValidFlag=0,int SingleCharOnlyFlag=0,int NumCharOnlyFlag=0,std::string IniStr=""){
		inactive();
		del();
		Handle=MakeKeyInput(MaxStrLength,CancelValidFlag,SingleCharOnlyFlag,NumCharOnlyFlag);
		SetKeyInputString(IniStr.c_str(),Handle);
	}
	void setStr(std::string IniStr){SetKeyInputString(IniStr.c_str(),Handle);}
	void active(){
		if(activeHandle==Handle){
			ReStartKeyInput(Handle);
		}else{
			SetActiveKeyInput(Handle);
			activeHandle=Handle;
		}
	}
	void inactive(){
		if(activeHandle==Handle)defaultInp.active();
//		if(activeHandle==Handle)SetActiveKeyInput(-1);
	}
	int check()const{
		//非入力中:0
		//入力途中:1
		//入力完了:2
		//入力終了:3
		if(activeHandle!=Handle)return 0;
		else{
			int ans=CheckKeyInput(Handle);
			if(ans<0)return -1;
			return ans+1;
		}
	}
	int getHandle()const{return Handle;}
	std::string getInpStr()const{
		if(GetIMEInputData()==0)return "";
		return hmstd::fStr("%s",GetIMEInputData()->InputString);
	}
	std::string getStr()const{
		GetKeyInputString(hmstd::getTmpStrAdress(),Handle);
		return std::string(hmstd::getTmpStrAdress());
	}
	int getNum()const{return GetKeyInputNumber(Handle);}
	double getDNum()const{return (double)GetKeyInputNumberToFloat(Handle);}
	friend int dx::ini(std::string _WindowName,int _ScreenX,int _ScreenY,int _ScreenClrBit,int _HighPreciseFlag,int _AlwaysRunFlag,int _WindowModeFlag,int _WindowExtendFlag);
	friend int dx::work(int _DrawIntervalTime);
};
//basic_dxGraphクラス
class basic_dxGraph{
private:
	int Handle;
public:
	basic_dxGraph(){Handle=-1;}
	basic_dxGraph(int _Handle){Handle=_Handle;}
/*
	basic_dxGraph(const std::string& _filename){
		Handle=-1;
		open(_filename);
	}
	basic_dxGraph(const Pint& _size){
		Handle=-1;
		make(_size);
	}
	basic_dxGraph(void* _mem,const int& _memsize){
		Handle=-1;
		load(_mem,_memsize);
	}
	basic_dxGraph(const dxImGraph& _imgra){
		Handle=-1;
		load(_imgra);
	}
*/
	basic_dxGraph(const basic_dxGraph& _grp){Handle=_grp.Handle;}
//	~basic_dxGraph(){del();}
	bool open(const std::string& _filename){
		del();
		Handle=LoadGraph(_filename.c_str());
		return fail();
	}
	bool make(const Pint& _size){
		del();
		Handle=MakeGraph(_size.x,_size.y);
		return fail();
	}
	bool make(const Pint& _pos,const Pint& _size){
		if(make(_size))return true;
		get(_pos);
		return false;
	}
	bool load(void* _mem,const int& _memsize){
		del();
		Handle=CreateGraphFromMem(_mem,_memsize);
		return fail();
	}
	bool load(const dxImGraph& _imgra);
	const basic_dxGraph& operator=(const basic_dxGraph& _grp){
		if(this!=&_grp)Handle=_grp.Handle;
		return *this;
	}
	void save(const std::string& _filename){
		Pint size=getSize();
		SetDrawScreen(Handle);
		SaveDrawScreen(0,0,size.x,size.y,_filename.c_str());
		SetDrawScreen(DX_SCREEN_BACK);
	}
	void copy(const basic_dxGraph& _gra);
	void get(const Pint& _pos){
		Pint size=getSize();
		GetDrawScreenGraph(_pos.x,_pos.y,_pos.x+size.x,_pos.y+size.y,Handle);
	}
	Pint getSize()const{
		Pint tmp;
		GetGraphSize(Handle,&(tmp.x),&(tmp.y));
		return tmp;
	}
	int getHandle()const{return Handle;}
	void del(){
		if(Handle>0)DeleteGraph(Handle);
		Handle=-1;
	}
	bool fail()const{return (Handle<=0);}
	void inactive(){Handle=-1;}
};
//basic_dxGraphsクラス
class dxGraph:public basic_dxGraph{
public:
	dxGraph():basic_dxGraph(){return;}
	dxGraph(const basic_dxGraph& _grp){throw "dxGraph::dxGraph(const basic_dxGraph& _grp) Unable to use this function";}
	dxGraph(const dxGraph& _grp){throw "dxGraph::dxGraph(const dxGraph& _grp) Unable to use this function";}
	~dxGraph(){del();}
	const dxGraph& operator=(const basic_dxGraph& _grp){throw "const dxGraph& dxGraph::operator=(const dxGraph& _grp) Unable to use this function";}
	const dxGraph& operator=(const dxGraph& _grp){throw "const dxGraph& Graph::operator=(const dxGraph& _grp) Unable to use this function";}
	void inactive(){throw "void Graph::inactive() Unable to use this function";}
};
//dxImGraphクラス
class dxImGraph{
private:
	int Handle;
	bool AlphaFlag;
	bool PaletFlag;
public:
	dxImGraph(){Handle=-1;}
	dxImGraph(const dxImGraph& _imgra){throw "dxImGraph::dxImGraph(const dxImGraph& _imgra) Unable to use this function";}
	~dxImGraph(){del();}
	bool open(const std::string& _filename){
		del();
		Handle=LoadSoftImage(_filename.c_str());
		AlphaFlag=(CheckAlphaSoftImage(Handle)!=0);
		PaletFlag=(CheckPaletteSoftImage(Handle)!=0);
		return fail();
	}
	bool open(const std::string& _filename,bool _AlphaFlag,bool _PaletFlag=false){
		dxImGraph tmpim;
		tmpim.open(_filename);
		make(tmpim.getSize(),_AlphaFlag,_PaletFlag);
		copy(Pint(0,0),tmpim.getSize(),tmpim,Pint(0,0));
		return fail();
	}
	bool make(const Pint& _size,bool _AlphaFlag=false,bool _PaletFlag=false){
		del();
		if(_AlphaFlag){
			Handle=MakeARGB8ColorSoftImage(_size.x,_size.y);
			AlphaFlag=true;
			PaletFlag=false;
		}else if(_PaletFlag){
			Handle=MakePAL8ColorSoftImage(_size.x,_size.y);
			AlphaFlag=false;
			PaletFlag=true;
		}else{
			Handle=MakeXRGB8ColorSoftImage(_size.x,_size.y);
			AlphaFlag=false;
			PaletFlag=false;
		}
		return fail();
	}
	bool load(void* _mem,const int& _memsize){
		del();
		Handle=LoadSoftImageToMem(_mem,_memsize);
		AlphaFlag=(CheckAlphaSoftImage(Handle)!=0);
		PaletFlag=(CheckPaletteSoftImage(Handle)!=0);
		return fail();
	}
	bool load(void* _mem,const int& _memsize,bool _AlphaFlag,bool _PaletFlag=false){
		dxImGraph tmpim;
		tmpim.load(_mem,_memsize);
		make(tmpim.getSize(),_AlphaFlag,_PaletFlag);
		copy(Pint(0,0),tmpim.getSize(),tmpim,Pint(0,0));
		return fail();
	}
	bool load(const basic_dxGraph& _grph,bool _AlphaFlag=false,bool _PaletFlag=false);
	void copy(const dxImGraph& _imgrp){
		if(this==&_imgrp)return;
		Pint size=_imgrp.getSize();
		make(size,_imgrp.AlphaFlag,_imgrp.PaletFlag);
		copy(Pint(0,0),size,_imgrp,Pint(0,0));
	}
	void copy(const Pint& _pos,const Pint& _size,const dxImGraph& _imgrp,const Pint _orgpos){
		if(this==&_imgrp)return;
		BltSoftImage(_orgpos.x,_orgpos.y,_size.x,_size.y,_imgrp.getHandle(),_pos.x,_pos.y,Handle);
	}
	const dxImGraph& operator=(const dxImGraph& _imgrp){throw "const dxImGraph& dxImGraph::operator=(const dxImGraph& _imgrp) Unable to use this function";}
	void save(const std::string& _filename){
		Pint size=getSize();
		dxGraph tmpGrp;
		tmpGrp.load(*this);
		SetDrawScreen(tmpGrp.getHandle());
		SaveDrawScreen(0,0,size.x,size.y,_filename.c_str());
		SetDrawScreen(DX_SCREEN_BACK);
	}
	void del(){
		if(Handle>0)DeleteSoftImage(Handle);
		Handle=-1;
	}
	bool fail(){return (Handle<=0);}
	Pint getSize()const{
		Pint tmp;
		GetSoftImageSize(Handle,&(tmp.x),&(tmp.y));
		return tmp;
	}
	int getHandle()const{return Handle;}
	bool getAlphaFlag(){return AlphaFlag;}
	bool getPaletFlag(){return PaletFlag;}
	void fill(const dxRGB& _dxclr,uchar _alpha){
		Pint size=getSize();
		for(int x=0;x<size.x;x++){
			for(int y=0;y<size.y;y++){
				put(Pint(x,y),_dxclr,_alpha);
			}
		}
	}
	void fillClr(const dxRGB& _dxclr){
		Pint size=getSize();
		for(int x=0;x<size.x;x++){
			for(int y=0;y<size.y;y++){
				putClr(Pint(x,y),_dxclr);
			}
		}
	}
	void fillAlpha(uchar _alpha){
		Pint size=getSize();
		for(int x=0;x<size.x;x++){
			for(int y=0;y<size.y;y++){
				putAlpha(Pint(x,y),_alpha);
			}
		}
	}
	void put(const Pint& _pos,const dxRGB& _dxclr,uchar _alpha){
		DrawPixelSoftImage(Handle,_pos.x,_pos.y,_dxclr.r,_dxclr.g,_dxclr.b,(int)_alpha);
	}
	void putClr(const Pint& _pos,const dxRGB& _dxclr){
		uchar a;
		dxRGB dxclr;
		get(_pos,dxclr,a);
		put(_pos,_dxclr,a);
	}
	void putAlpha(const Pint& _pos,uchar _alpha){
		put(_pos,getClr(_pos),_alpha);
	}
	void setTrans(const dxRGB& _orgdxclr,uchar _alpha){
		Pint size=getSize();
		for(int x=0;x<size.x;x++){
			for(int y=0;y<size.y;y++){
				if(getClr(Pint(x,y))!=_orgdxclr)continue;
				put(Pint(x,y),_orgdxclr,_alpha);
			}
		}
	}
	void rotClr(const Pint& _pos,int addhue){
		dxHLS hls(getClr(_pos));
		hls.hue=(hls.hue+addhue)%360;
		putClr(_pos,dxRGB(hls));
	}
	void rotClr(int addhue){
		Pint size=getSize();
		for(int x=0;x<size.x;x++){
			for(int y=0;y<size.y;y++){
				rotClr(Pint(x,y),addhue);
			}
		}
	}
	void get(const Pint& _pos,dxRGB& _rgb,uchar& _alpha){
		int r,g,b,a;
		GetPixelSoftImage(Handle,_pos.x,_pos.y,&r,&g,&b,&a);
		_rgb.set(itouc(r),itouc(g),itouc(b));
		_alpha=itouc(a);
	}
	dxRGB getClr(const Pint& _pos){
		uchar alpha;
		dxRGB clr;
		get(_pos,clr,alpha);
		return clr;
	}
	uchar getAlpha(const Pint& _pos){
		uchar alpha;
		dxRGB clr;
		get(_pos,clr,alpha);
		return alpha;
	}
};
//dxGraphsクラス
class dxGraphs{
private:
	basic_dxGraph mstrgrp;
	std::deque<basic_dxGraph> grps;
public:
	~dxGraphs(){clear();}
	int open(const std::string& _filename,int xsize,int ysize){
		clear();
		dxGraph grp;
		grp.open(_filename);
		Pint tmpp=grp.getSize();
		int tmpi=(tmpp.x/xsize)*(tmpp.y/ysize);

		if(tmpi){
			int* tmpis;
			tmpis=new int[tmpi];

			LoadDivGraph(_filename.c_str(),tmpi,tmpp.x/xsize,tmpp.y/ysize,xsize,ysize,tmpis);
			for(int i=0;i<tmpi;i++)grps.push_back(basic_dxGraph(tmpis[i]));

			delete[] tmpis;
		}

		return tmpi;
	}
	int load(basic_dxGraph& _grp,int xsize,int ysize){
		clear();
		mstrgrp=_grp;
		_grp.inactive();
		Pint tmpp=mstrgrp.getSize();
		int tmpi=(tmpp.x/xsize)*(tmpp.y/ysize);

		if(tmpi){
			for(int y=0;y<tmpp.y/ysize;y++){
				for(int x=0;x<tmpp.x/xsize;x++){
					int tmp=DerivationGraph(x*xsize,y*ysize,xsize,ysize,mstrgrp.getHandle());
					if(tmp>0)grps.push_back(basic_dxGraph(tmp));
				}
			}
		}else{
			_grp=mstrgrp;
			mstrgrp.inactive();
		}

		return tmpi;
	}
	int load(dxImGraph& _imgrp,int xsize,int ysize){
		dxGraph tmp;
		tmp.load(_imgrp);
		return load(tmp,xsize,ysize);
	}
	uint size(){return grps.size();}
	basic_dxGraph& at(int i){
		if(i<0 || i>=(int)grps.size())throw "basic_dxGraph& dxGraphs::at(int i) out of range";
		return grps[i];
	}
	basic_dxGraph& operator[](int i){
		if(i<0 || i>=(int)grps.size())throw "basic_dxGraph& dxGraphs::operator[](int i) out of range";
		return grps[i];
	}
	basic_dxGraph& back(){return grps.back();}
	basic_dxGraph& front(){return grps.front();}
	void push_back(basic_dxGraph& grp){
		grps.push_back(grp);
		grp.inactive();
	}
	void push_front(basic_dxGraph& grp){
		grps.push_front(grp);
		grp.inactive();
	}
	void push_back(dxImGraph& imgrp){
		dxGraph tmp;
		tmp.load(imgrp);
		push_back(tmp);
	}
	void push_front(dxImGraph& imgrp){
		dxGraph tmp;
		tmp.load(imgrp);
		push_front(tmp);
	}
	void pop_back(){
		grps.back().del();
		grps.pop_back();
	}
	void pop_front(){
		grps.front().del();
		grps.pop_front();
	}
	void clear(){
		for(int i=0;i<(int)grps.size();i++)grps[i].del();
		grps.clear();
		mstrgrp.del();
	}
	bool add(uint pos,basic_dxGraph& grp){
		if(pos>grps.size())return true;
		grps.insert(grps.begin()+pos,grp);
		grp.inactive();
		return false;
	}
	bool add(uint pos,dxImGraph& imgrp){
		dxGraph tmp;
		tmp.load(imgrp);
		return add(pos,tmp);
	}
	bool del(uint pos){
		if(pos>=grps.size())return true;
		grps.at(pos).del();
		grps.erase(grps.begin()+pos);
		return false;
	}
};
namespace dx{
	//DxClr関数群
	void drawBox(const Pint& pos1,const Pint& pos2,const dxColor& clr,int FillFlag=1,dxDMode dmode=dxDMode());
	void drawTriangle(const Pint& pos1,const Pint& pos2,const Pint& pos3,const dxColor& clr,int FillFlag=1,dxDMode dmode=dxDMode());
	void drawCircle(const Pint& pos,const int& r,const dxColor& clr,int FillFlag=1,dxDMode dmode=dxDMode());
	void drawOval(const Pint& pos,const Pint& r,const dxColor& clr,int FillFlag=1,dxDMode dmode=dxDMode());
	void drawPixel(const Pint& pos,const dxColor& clr,dxDMode dmode=dxDMode());
	void drawLine(const Pint& pos1,const Pint& pos2,const dxColor& clr,int Thick=1,dxDMode dmode=dxDMode());
	int drawStr(const Pint& pos,const std::string Str,const dxColor& sclr,dxDMode dmode=dxDMode(),int VFlag=0,int FontHandle=-1,const dxColor& seclr=dxRGB(0,0,0));
	int drawStr(const Pint& pos,const std::string Str,const dxColor& sclr,const dxFont& font,dxDMode dmode=dxDMode(),const dxColor& seclr=dxRGB(0,0,0));
	int drawStrP(const Pint& pos,const Pint& size,const std::string Str,const dxColor& sclr,ALI::ALIGN DrawPos=ALI::center,dxDMode dmode=dxDMode(),int VFlag=0,int FontHandle=-1,int FontSize=dx::getFontSize(),const dxColor& seclr=dxRGB(0,0,0));
	int drawStrP(const Pint& pos,const Pint& size,const std::string Str,const dxColor& sclr,const dxFont& font,ALI::ALIGN DrawPos=ALI::center,dxDMode dmode=dxDMode(),const dxColor& seclr=dxRGB(0,0,0));
	int drawInpStr(const Pint& pos,const dxInput& inp,const dxColor& sclr,const dxColor& imeclr=dxRGB(0x00,0x00,0x8b),dxDMode dmode=dxDMode(),int VFlag=0,int FontHandle=-1,int FontSize=dx::getFontSize(),const dxColor& seclr=dxRGB(0,0,0));
	int drawInpStr(const Pint& pos,const dxInput& inp,const dxColor& sclr,const dxColor& imeclr,const dxFont& font,dxDMode dmode=dxDMode(),const dxColor& seclr=dxRGB(0,0,0));
	int drawInpStrP(const Pint& pos,const Pint& size,const dxInput& inp,const dxColor& sclr,const dxColor& imeclr=dxRGB(0x00,0x00,0x8b),ALI::ALIGN DrawPos=ALI::left,dxDMode dmode=dxDMode(),int VFlag=0,int FontHandle=-1,int FontSize=dx::getFontSize(),const dxColor& seclr=dxRGB(0,0,0));
	int drawInpStrP(const Pint& pos,const Pint& size,const dxInput& inp,const dxColor& sclr,const dxColor& imeclr,const dxFont& font,ALI::ALIGN DrawPos=ALI::left,dxDMode dmode=dxDMode(),const dxColor& seclr=dxRGB(0,0,0));
	void drawGraph(const Pint& pos,const basic_dxGraph& grp,int TransFlag=1,int TurnFlag=0,dxDMode dmode=dxDMode());
	void drawGraph(const Pint& pos,const basic_dxGraph& grp,const Pint& size,int TransFlag=1,dxDMode dmode=dxDMode());
	void drawGraph(const Pint& pos,const basic_dxGraph& grp,const Pint& grppos,const Pint& grpsize,int TransFlag=1,int TurnFlag=0,dxDMode dmode=dxDMode());
	void drawGraph(const Pint& pos,const basic_dxGraph& grp,const double& rate,const double& angle,int TransFlag=1,int TurnFlag=0,dxDMode dmode=dxDMode());
	void drawGraph(const Pint& pos,const basic_dxGraph& grp,const double& rate,const double& angle,const Pint& cpos,int TransFlag=1,int TurnFlag=0,dxDMode dmode=dxDMode());
	void drawImGraph(const Pint& pos,const dxImGraph& imgrp,dxDMode dmode=dxDMode());
}
namespace dx{
	class TmpKeyActive{
		bool tmp;
	public:
		TmpKeyActive(const bool keyactive){
			tmp=getKeyActive();
			setKeyActive(keyactive);
		}
		~TmpKeyActive(){
			setKeyActive(tmp);
		}
	};
	class TmpDrawMode{
		dxDMode dmode;
	public:
		TmpDrawMode(const dxDMode& _dmode){
			dmode=getDrawMode();
			setDrawMode(_dmode);
		}
		~TmpDrawMode(){
			setDrawMode(dmode);
		}
	};
	class TmpDrawArea{
		dxDArea darea;
	public:
		TmpDrawArea(const dxDArea& _darea){
			darea=getDrawArea();
			setDrawArea(_darea);
		}
		~TmpDrawArea(){
			setDrawArea(darea);
		}
	};
	class TmpGlobalStd{
		Pint gstd;
	public:
		TmpGlobalStd(const Pint& _gstd){
			gstd=getGlobalStd();
			setGlobalStd(_gstd);
		}
		~TmpGlobalStd(){
			setGlobalStd(gstd);
		}
	};
	class TmpDrawScreen{
		basic_dxGraph dgrp;
	public:
		TmpDrawScreen(const basic_dxGraph& _dgrp){
			dgrp=getDrawScreen();
			setDrawScreen(_dgrp);
		}
		~TmpDrawScreen(){
			setDrawScreen(dgrp);
		}
	};
}
#
#endif
