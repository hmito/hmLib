#ifndef INC_HMLIB_HMCOLOR
#define INC_HMLIB_HMCOLOR 100
#ifndef INC_HMLIB_HMBASIC
#	include "hmLib_v2/hmBasic.hpp"
#endif
const double G_ClrKR=0.298912;
const double G_ClrKG=0.586611;
const double G_ClrKB=0.114478;
class hmRGB;	//RGB型カラー
class hmHSV;	//HSV型カラー　色相、彩度、強度
class hmHLS;	//HLS型カラー　色相、明度、彩度
class hmColor{
public:
	inline virtual hmRGB getRGB()const=0;
	inline virtual hmHSV getHSV()const=0;
	inline virtual hmHLS getHLS()const=0;
};
class hmRGB:virtual public hmColor{
public:
	uchar r,g,b;
	hmRGB(){set(0,0,0);}
	hmRGB(uchar _r,uchar _g,uchar _b){set(_r,_g,_b);}
	hmRGB(const hmColor& _clr){set(_clr.getRGB());}
	inline hmRGB getRGB()const{return *this;}
	inline hmHSV getHSV()const;
	inline hmHLS getHLS()const;
	void set(uchar _r,uchar _g,uchar _b){
		r=_r;
		g=_g;
		b=_b;
	}
	inline void set(const hmRGB& _rgb){
		if(this==&_rgb)return;
		r=_rgb.r;
		g=_rgb.g;
		b=_rgb.b;
	}
	inline void set(const hmHSV& _hsv);
	inline void set(const hmHLS& _hls);
	const hmRGB& operator=(const hmColor& _clr){
		set(_clr.getRGB());
		return *this;
	}
	bool operator==(const hmRGB& _rgb){
		if(r!=_rgb.r)return false;
		if(g!=_rgb.g)return false;
		if(b!=_rgb.b)return false;
		return true;
	}
	bool operator!=(const hmRGB& _rgb){
		if(r!=_rgb.r)return true;
		if(g!=_rgb.g)return true;
		if(b!=_rgb.b)return true;
		return false;
	}
};
class hmHSV:virtual public hmColor{
public:
	int hue;
	uchar sat,vlu;
	hmHSV(){set(0,0,0);}
	hmHSV(int _hue,uchar _sat,uchar _vlu){set(_hue,_sat,_vlu);}
	hmHSV(const hmColor& _clr){set(_clr.getHSV());}
	inline hmRGB getRGB()const;
	inline hmHSV getHSV()const{return *this;}
	inline hmHLS getHLS()const;
	void set(int _hue,uchar _sat,uchar _vlu){
		hue=_hue;
		sat=_sat;
		vlu=_vlu;
	}
	inline void set(const hmHSV& _hsv){
		if(this==&_hsv)return;
		hue=_hsv.hue;
		sat=_hsv.sat;
		vlu=_hsv.vlu;
	}
	inline void set(const hmRGB& _rgb);
	inline void set(const hmHLS& _hls);
	const hmHSV& operator=(const hmColor& _clr){
		set(_clr.getHSV());
		return *this;
	}
};
class hmHLS:virtual public hmColor{
public:
	int hue;
	uchar sat,lgt;
	hmHLS(){set(0,0,0);}
	hmHLS(int _hue,uchar _lgt,uchar _sat){set(_hue,_lgt,_sat);}
	hmHLS(const hmColor& _clr){set(_clr.getHLS());}
	inline hmRGB getRGB()const;
	inline hmHSV getHSV()const;
	inline hmHLS getHLS()const{return *this;}
	void set(int _hue,uchar _lgt,uchar _sat){
		hue=_hue;
		lgt=_lgt;
		sat=_sat;
	}
	inline void set(const hmHLS& _hls){
		if(this==&_hls)return;
		hue=_hls.hue;
		lgt=_hls.lgt;
		sat=_hls.sat;
	}
	inline void set(const hmRGB& _rgb);
	inline void set(const hmHSV& _hsv);
	const hmHLS& operator=(const hmColor& _clr){
		set(_clr.getHLS());
		return *this;
	}
};
inline void hmRGB::set(const hmHSV& _hsv){set(_hsv.getRGB());}
inline void hmRGB::set(const hmHLS& _hls){set(_hls.getRGB());}
inline void hmHSV::set(const hmRGB& _rgb){set(_rgb.getHSV());}
inline void hmHSV::set(const hmHLS& _hls){set(_hls.getHSV());}
inline void hmHLS::set(const hmRGB& _rgb){set(_rgb.getHLS());}
inline void hmHLS::set(const hmHSV& _hsv){set(_hsv.getHLS());}
inline void HXXToRGB(hmRGB& rgb,int hue,uchar minv,uchar maxv){
	int adjh=hue%360;
	if(adjh>=0 && adjh<60){
		rgb.r=maxv;
		rgb.b=minv;
		rgb.g=minv+(maxv-minv)*adjh/60;
	}else if(adjh>=60 && adjh<120){
		rgb.g=maxv;
		rgb.b=minv;
		rgb.r=minv+(maxv-minv)*(120-adjh)/60;
	}else if(adjh>=120 && adjh<180){
		rgb.g=maxv;
		rgb.r=minv;
		rgb.b=minv+(maxv-minv)*(adjh-120)/60;
	}else if(adjh>=180 && adjh<240){
		rgb.b=maxv;
		rgb.r=minv;
		rgb.g=minv+(maxv-minv)*(240-adjh)/60;
	}else if(adjh>=240 && adjh<300){
		rgb.b=maxv;
		rgb.g=minv;
		rgb.r=minv+(maxv-minv)*(adjh-240)/60;
	}else{
		rgb.r=maxv;
		rgb.g=minv;
		rgb.b=minv+(maxv-minv)*(360-adjh)/60;
	}
}
inline void RGBToHXX(int& hue,uchar& minv,uchar& maxv,hmRGB rgb){
	if(rgb.r==rgb.g && rgb.g==rgb.b){
		hue=0;
		minv=rgb.r;
		maxv=rgb.r;
	}else if(rgb.r>=rgb.g && rgb.g>=rgb.b){
		hue=0+(rgb.g-rgb.b)*60/(rgb.r-rgb.b);
		minv=rgb.b;
		maxv=rgb.r;
	}else if(rgb.g>=rgb.r && rgb.r>=rgb.b){
		hue=60+(rgb.g-rgb.r)*60/(rgb.g-rgb.b);
		minv=rgb.b;
		maxv=rgb.g;
	}else if(rgb.g>=rgb.b && rgb.b>=rgb.r){
		hue=120+(rgb.b-rgb.r)*60/(rgb.g-rgb.r);
		minv=rgb.r;
		maxv=rgb.g;
	}else if(rgb.b>=rgb.g && rgb.g>=rgb.r){
		hue=180+(rgb.b-rgb.g)*60/(rgb.b-rgb.r);
		minv=rgb.r;
		maxv=rgb.b;
	}else if(rgb.b>=rgb.r && rgb.r>=rgb.g){
		hue=240+(rgb.r-rgb.g)*60/(rgb.b-rgb.g);
		minv=rgb.g;
		maxv=rgb.b;
	}else if(rgb.r>=rgb.b && rgb.b>=rgb.g){
		hue=300+(rgb.r-rgb.b)*60/(rgb.r-rgb.g);
		minv=rgb.g;
		maxv=rgb.r;
	}
}
inline hmHSV hmRGB::getHSV()const{
	hmHSV tmp(0,0,0);

	uchar maxv,minv;
	RGBToHXX(tmp.hue,minv,maxv,*this);
	if(maxv==0 && minv==0)return tmp;
	tmp.vlu=maxv;
	tmp.sat=(int)(maxv-minv)*255/maxv;

	return tmp;
}
inline hmHLS hmRGB::getHLS()const{
	hmHLS tmp(0,0,0);

	uchar maxv,minv;
	RGBToHXX(tmp.hue,minv,maxv,*this);

	tmp.lgt=(int)(maxv+minv)/2;
	if(maxv==minv)tmp.sat=0;
	else{
		if(tmp.lgt>127)tmp.sat=(int)(maxv-minv)*255/(510-maxv-minv);
		else tmp.sat=(int)(maxv-minv)*255/((int)maxv+(int)minv);
	}
	return tmp;
};
inline hmRGB hmHSV::getRGB()const{
	hmRGB tmp(0,0,0);
	HXXToRGB(tmp,this->hue,this->vlu-((int)this->sat*this->vlu/255),this->vlu);
	return tmp;
}
inline hmHLS hmHSV::getHLS()const{return hmHLS(this->getRGB());}
inline hmRGB hmHLS::getRGB()const{
	hmRGB tmp(0,0,0);
	if(this->lgt>127){
		uchar maxv,minv;
		minv=(int)this->lgt*2-(int)this->lgt*(255-this->sat)/255-(int)this->sat;
		maxv=(int)this->lgt*(255-this->sat)/255+(int)this->sat;
		HXXToRGB(tmp,this->hue,minv,maxv);
	}else{
		uchar maxv,minv;
		minv=(int)this->lgt*2-(int)this->lgt*(255+this->sat)/255;
		maxv=(int)this->lgt*(255+this->sat)/255;
		HXXToRGB(tmp,this->hue,minv,maxv);
	}
	return tmp;
};
inline hmHSV hmHLS::getHSV()const{return hmHSV(this->getRGB());}
#endif
