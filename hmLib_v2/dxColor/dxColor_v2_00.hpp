/*
////////////////////////////////
dxColor.hpp	v2.0
	v2.0
		dxRGB,dxHSV,dxHLSに再編
////////////////////////////////
dxRGB
	赤r,緑g,青bの三原色による、明度0-255での色表示
dxHSV
	色相hue,彩度sat,明度vluによる、色表示
	彩度によって白色->彩色
	明度によって黒色->明色
dxHLS
	色相hue,輝度lgt,彩度satによる、色表示
	明度によって白->黒
	彩度によって無色->彩色
*/


#ifndef INC_HMLIB_DXCOLOR
#define INC_HMLIB_DXCOLOR 200
#
#ifndef INC_HMLIB_HMCOLOR
#	include "hmLib_v2/hmColor.hpp"
#endif
#include <DxLib.h>
#pragma warning(disable:4250)
class dxColor:virtual public hmColor{
public:
	virtual int getClr()const=0;
	virtual void setClr(int _clr)=0;
};
class dxRGB:public dxColor,public hmRGB{
public:
	dxRGB():hmRGB(){return;}
	explicit dxRGB(int _clr):hmRGB(){setClr(_clr);}
	dxRGB(uchar _r,uchar _g,uchar _b):hmRGB(_r,_g,_b){return;}
	dxRGB(const hmColor& _clr):hmRGB(_clr){return;}
	int getClr()const{return GetColor(r,g,b);}
	void setClr(int _clr){
		int tmpr,tmpg,tmpb;
		GetColor2(_clr,&tmpr,&tmpg,&tmpb);
		r=tmpr;
		g=tmpg;
		b=tmpb;
	}
	dxRGB& operator=(const hmColor& _clr){
		set(_clr.getRGB());
		return *this;
	}
	bool operator==(const dxRGB& _rgb){
		if(r!=_rgb.r)return false;
		if(g!=_rgb.g)return false;
		if(b!=_rgb.b)return false;
		return true;
	}
	bool operator!=(const dxRGB& _rgb){
		if(r!=_rgb.r)return true;
		if(g!=_rgb.g)return true;
		if(b!=_rgb.b)return true;
		return false;
	}
};
class dxHSV:public dxColor,public hmHSV{
public:
	dxHSV():hmHSV(){return;}
	dxHSV(int _clr):hmHSV(){setClr(_clr);}
	dxHSV(int _hue,uchar _sat,uchar _vlu):hmHSV(_hue,_sat,_vlu){return;}
	dxHSV(const hmColor& _clr):hmHSV(_clr){return;}
	int getClr()const{return dxRGB(*this).getClr();}
	void setClr(int _clr){this->set(dxRGB(_clr));}
	dxHSV& operator=(const hmColor& _clr){
		set(_clr.getHSV());
		return *this;
	}
};
class dxHLS:public dxColor,public hmHLS{
public:
	dxHLS():hmHLS(){return;}
	dxHLS(int _clr):hmHLS(){setClr(_clr);}
	dxHLS(int _hue,uchar _lgt,uchar _sat):hmHLS(_hue,_lgt,_sat){return;}
	dxHLS(const hmColor& _clr):hmHLS(_clr){return;}
	int getClr()const{return dxRGB(*this).getClr();}
	void setClr(int _clr){this->set(dxRGB(_clr));}
	dxHLS& operator=(const hmColor& _clr){
		set(_clr.getHLS());
		return *this;
	}
};
#
#endif
