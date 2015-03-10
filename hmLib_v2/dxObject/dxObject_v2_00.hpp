#ifndef INC_HMLIB_DXOBJECT
#define INC_HMLIB_DXOBJECT 200
#
#ifndef INC_HMLIB_DXBASIC
#	include "hmLib_v2/dxBasic.hpp"
#endif
#ifndef INC_HMLIB_HMOBJECT
#	include "hmLib_v2/hmObject.hpp"
#endif
#include <deque>
#pragma warning(disable:4250)
//dxObject群
class dxO;
typedef int(*I_pFunc_rDXO)(dxO&);
class dxBasicObject{
public:
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode())=0;
};
class dxObject:virtual public dxBasicObject,public virtual hmObject{
};
class dxSubObject:public dxBasicObject{
protected:
	Rint area;
	void resetArea(){area.set(Pint(INT_MAX,INT_MAX),Pint(INT_MIN,INT_MIN));}
public:
	virtual Rint getArea(){return area;}
};
class dxO{
private:
	dxBasicObject* baseP;
	Pint pos;
	dxDMode dmode;
	bool active;
	Rint rect;
	dxDArea darea;
public:
	dxO(const Pint& _pos,bool _active,const dxDMode& _dmode,dxBasicObject* _baseP=0){
		pos=_pos;
		dmode=_dmode;
		active=_active;
		baseP=_baseP;
		rect.set(Pint(INT_MAX,INT_MAX),Pint(INT_MIN,INT_MIN));
		darea=dx::getDrawArea();
	}
	int draw(Pint _pos,dxObject& _obj,bool _active=true,dxDMode _mode=dxDMode()){
		Rint tmp=_obj.getArea();
		tmp+=Rint(_pos,_pos);
		rect|=tmp;
		if(darea.activeF){
			if(darea.getRect().cross(tmp)==false)return 0;
		}
		return _obj.draw(_pos+pos,_active&&active,_mode+dmode);
	}
	int draw(Pint _pos,dxSubObject& _obj,bool _active=true,dxDMode _mode=dxDMode()){
		_mode+=dmode;
		int ans=_obj.draw(_pos+pos,_active&&active,_mode+dmode);
		rect|=_obj.getArea()+Rint(_pos,_pos);
		return ans;
	}
	Rint getArea()const{return rect;}
	bool checkActive()const{return active;}
	void movePos(Pint _mpos){pos+=_mpos;}
	Pint GtoD(Pint _pos)const{return _pos-pos;}
	Pint DtoG(Pint _pos)const{return _pos+pos;}
	template<class Type>
	class Acs{
	private:
		Type* baseP;
	public:
		Acs<Type>(dxO& _dxo){
			baseP=dynamic_cast<Type*>(_dxo.baseP);
			if(baseP==NULL){
				throw "dxO::Acs::Acs<Type>(dxO& _dxo); fail to dynamic_cast<Type>";
			}
		}
		Type& operator()(){return *baseP;}
	};
};
class dxFnObject:public dxSubObject{
public:
	I_pFunc_rDXO DrawFunc;
private:
	void local_set(I_pFunc_rDXO _DrawFunc){
		DrawFunc=_DrawFunc;
	}
	void local_copy(const dxFnObject& _dxo){
		DrawFunc=_dxo.DrawFunc;
	}
protected:
	void copy(const dxFnObject& _dxo){
		local_copy(_dxo);
	}
public:
	dxFnObject(){local_set(0);}
	dxFnObject(I_pFunc_rDXO _DrawFunc){local_set(_DrawFunc);}
	dxFnObject(const dxFnObject& _dxo){local_copy(_dxo);}
	dxFnObject& operator=(const dxFnObject& _dxo){
		if(this!=&_dxo)copy(_dxo);
		return *this;
	}
	void set(I_pFunc_rDXO _DrawFunc){local_set(_DrawFunc);}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
		dxO dxo(_pos,_active,_mode);
		int ans=DrawFunc(dxo);
		area=dxo.getArea();
		return ans;
	}
};
class dxImObject:public dxSubObject{
protected:
	//Image描画関数
	virtual int imdraw(dxO& dxo)=0;
public:
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
		dxO dxo(_pos,_active,_mode);
		int ans=imdraw(dxo);
		area=dxo.getArea();
		return ans;
	}
};
namespace dx{
	inline int draw(Pint _pos,dxBasicObject& _obj,bool _active=true,dxDMode _mode=dxDMode()){return _obj.draw(_pos,_active,_mode);}
	inline int checkDraw(Pint _pos,dxObject& _obj){
		Pint omin,omax;
//		_obj.getArea(omin,omax);

		Pint spos=getDrawArea().pos1;
		Pint ssize=getDrawArea().pos2-spos;

		int tmp=0;
		if(hmoBox(ssize).bIn(spos,_pos+omin,omax-omin))tmp+=1;
		if(hmoBox(omax-omin).bIn(_pos+omin,spos,ssize))tmp+=2;

		if(tmp==1)return 1;
		else if(tmp>1)return 2;
		else return 0;
	}
}
class dxoNull:public dxObject,public hmoPixel{
private:
	void local_set(){return;}
	void local_copy(){return;}
protected:
	void copy(const dxoNull& _dxo){
		hmoPixel::copy(_dxo);
		local_copy();
	}
public:
	dxoNull():hmoPixel(){return;}
	dxoNull(const dxoNull& _dxo):hmoPixel(_dxo){copy(_dxo);}
	dxoNull& operator=(const dxoNull& _dxon){
		if(this!=&_dxon)copy(_dxon);
		return *this;
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){return 0;}
	void set(){
		hmoPixel::set();
		local_set();
	}
};
class dxoPixel:public dxObject,public hmoPixel{
public:
	dxRGB Color;
private:
	void local_set(const dxRGB& _Color){
		Color=_Color;
	}
	void local_copy(const dxoPixel& _dxo){
		Color=_dxo.Color;
	}
protected:
	void copy(const dxoPixel& _dxo){
		hmoPixel::copy(_dxo);
		local_copy(_dxo);
	}
public:
	dxoPixel():hmoPixel(){
		Color=dxRGB(255,255,255);
	}
	dxoPixel(const dxRGB& _Color):hmoPixel(){
		local_set(_Color);
	}
	dxoPixel(const dxoPixel& _dxo):hmoPixel(_dxo){local_copy(_dxo);}
	dxoPixel& operator=(const dxoPixel& _dxop){
		if(this!=&_dxop)copy(_dxop);
		return *this;
	}
	void set(const dxRGB& _Color){
		hmoPixel::set();
		local_set(_Color);
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
		dx::drawPixel(_pos,Color,_mode);
		return 0;
	}
};
class dxoLine : public dxObject,public hmoLine{
public:
	dxRGB Color;
	int Thick;
private:
	void local_set(const dxRGB& _Color,int _Thick){
		Color=_Color;
		Thick=_Thick;
	}
	void local_copy(const dxoLine& _dxo){
		Color=_dxo.Color;
		Thick=_dxo.Thick;
	}
protected:
	void copy(const dxoLine& _dxo){
		hmoLine::copy(_dxo);
		local_copy(_dxo);
	}
public:
	dxoLine():hmoLine(){local_set(dxRGB(255,255,255),1);}
	dxoLine(const Pint& _relpos,const dxRGB& _Color,int _Thick=1):hmoLine(_relpos){local_set(_Color,_Thick);}
	dxoLine(const dxoLine& _dxo):hmoLine(_dxo){copy(_dxo);}
	dxoLine& operator=(const dxoLine& _dxol){
		if(this!=&_dxol)copy(_dxol);
		return *this;
	}
	void set(const Pint& _relpos,const dxRGB& _Color,int _Thick=1){
		hmoLine::set(_relpos);
		local_set(_Color,_Thick);
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
		dx::drawLine(_pos,_pos+relpos,Color,Thick,_mode);
		return 0;
	}
};
class dxoBox : public virtual dxObject,public virtual hmoBox{
public:
	dxRGB Color;
	int FillFlag;
private:
	void local_set(const dxRGB& _Color,int _FillFlag=1){
		Color=_Color;
		FillFlag=_FillFlag;
	}
	void local_copy(const dxoBox& _dxo){
		Color=_dxo.Color;
		FillFlag=_dxo.FillFlag;
	}
protected:
	void copy(const dxoBox& _dxo){
		hmoBox::copy(_dxo);
		local_copy(_dxo);
	}
public:
	dxoBox():hmoBox(){
		Color=dxRGB(255,255,255);
		FillFlag=1;
	}
	dxoBox(const Pint& _size,const dxRGB& _Color,int _FillFlag=1):hmoBox(_size){local_set(_Color,_FillFlag);}
	dxoBox(const dxoBox& _dxo):hmoBox(_dxo){copy(_dxo);}
	dxoBox& operator=(const dxoBox& _dxo){
		if(this!=&_dxo)copy(_dxo);
		return *this;
	}
	void set(const Pint& _size,const dxRGB& _Color,int _FillFlag=1){
		hmoBox::set(_size);
		local_set(_Color,_FillFlag);
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
		dx::drawBox(_pos,_pos+size,Color,FillFlag,_mode);
		return 0;
	}
};
class dxoTriangle:public dxObject,public hmoTriangle{
public:
	dxRGB Color;
	int FillFlag;
private:
	void local_set(const dxRGB& _Color,const int& _FillFlag){
		Color=_Color;
		FillFlag=_FillFlag;
	}
	void local_copy(const dxoTriangle& _dxo){
		Color=_dxo.Color;
		FillFlag=_dxo.FillFlag;
	}
protected:
	void copy(const dxoTriangle& _dxo){
		hmoTriangle::copy(_dxo);
		local_copy(_dxo);
	}
public:
	dxoTriangle():hmoTriangle(){local_set(dxRGB(255,255,255),1);}
	dxoTriangle(const Pint& _relpos1,const Pint& _relpos2,const dxRGB& _Color,int _FillFlag=1):hmoTriangle(_relpos1,_relpos2){local_set(_Color,_FillFlag);}
	dxoTriangle(const dxoTriangle& _dxo):hmoTriangle(_dxo){local_copy(_dxo);}
	dxoTriangle& operator=(const dxoTriangle& _dxo){
		if(this!=&_dxo)copy(_dxo);
		return *this;
	}
	void set(const Pint& _relpos1,const Pint& _relpos2,const dxRGB& _Color,int _FillFlag=1){
		hmoTriangle::set(_relpos1,_relpos2);
		local_set(_Color,_FillFlag);
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
		dx::drawTriangle(_pos,_pos+relpos1,_pos+relpos2,Color,FillFlag,_mode);
		return 0;
	}
};
class dxoCircle:public dxObject,public hmoCircle{
public:
	dxRGB Color;
	int FillFlag;
private:
	void local_set(const dxRGB& _Color,const int& _FillFlag){
		Color=_Color;
		FillFlag=_FillFlag;
	}
	void local_copy(const dxoCircle& _dxo){
		Color=_dxo.Color;
		FillFlag=_dxo.FillFlag;
	}
protected:
	void copy(const dxoCircle& _dxo){
		hmoCircle::copy(_dxo);
		local_copy(_dxo);
	}
public:
	dxoCircle():hmoCircle(){local_set(dxRGB(255,255,255),1);}
	dxoCircle(const int& _r,const dxRGB& _Color,int _FillFlag=1):hmoCircle(_r){local_set(_Color,_FillFlag);}
	dxoCircle(const dxoCircle& _dxo):hmoCircle(_dxo){local_copy(_dxo);}
	dxoCircle& operator=(const dxoCircle& _dxo){
		if(this!=&_dxo)copy(_dxo);
		return *this;
	}
	void set(const int& _r,const dxRGB& _Color,int _FillFlag=1){
		hmoCircle::set(_r);
		local_set(_Color,_FillFlag);
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
		dx::drawCircle(_pos,r,Color,FillFlag,_mode);
		return 0;
	}
};
class dxoOval:public dxObject,public hmoOval{
public:
	dxRGB Color;
	int FillFlag;
private:
	void local_set(const dxRGB& _Color,const int& _FillFlag){
		Color=_Color;
		FillFlag=_FillFlag;
	}
	void local_copy(const dxoOval& _dxo){
		Color=_dxo.Color;
		FillFlag=_dxo.FillFlag;
	}
protected:
	void copy(const dxoOval& _dxo){
		hmoOval::copy(_dxo);
		local_copy(_dxo);
	}
public:
	dxoOval():hmoOval(){local_set(dxRGB(255,255,255),1);}
	dxoOval(const Pint& _r,const dxRGB& _Color,const int& _FillFlag):hmoOval(_r){local_set(_Color,_FillFlag);}
	dxoOval(const dxoOval& _dxo):hmoOval(_dxo){local_copy(_dxo);}
	dxoOval& operator=(const dxoOval& _dxo){
		if(this!=&_dxo)copy(_dxo);
		return *this;
	}
	void set(const Pint& _r,const dxRGB& _Color,const int& _FillFlag){
		hmoOval::set(_r);
		local_set(_Color,_FillFlag);
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
		dx::drawOval(_pos,r,Color,FillFlag,_mode);
		return 0;
	}
};
class dxoPNull:public dxObject,public hmoPPixel{
private:
	void local_set(){return;}
	void local_copy(){return;}
protected:
	void copy(const dxoPNull& _dxoP){
		hmoPPixel::copy(_dxoP);
		local_copy();
	}
public:
	dxoPNull():hmoPPixel(){return;}
	dxoPNull(const Pint _pos):hmoPPixel(_pos){return;}
	dxoPNull(const dxoPNull& _dxoP):hmoPPixel(_dxoP){copy(_dxoP);}
	dxoPNull& operator=(const dxoPNull& _dxoP){
		if(this!=&_dxoP)copy(_dxoP);
		return *this;
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){return 0;}
	void set(const Pint _pos){
		hmoPPixel::set(_pos);
		local_set();
	}
};
class dxoPPixel:public dxObject,public hmoPPixel{
public:
	dxRGB Color;
private:
	void local_set(const dxRGB& _Color){
		Color=_Color;
	}
	void local_copy(const dxoPPixel& _dxoP){
		Color=_dxoP.Color;
	}
protected:
	void copy(const dxoPPixel& _dxoP){
		hmoPPixel::copy(_dxoP);
		local_copy(_dxoP);
	}
public:
	dxoPPixel():hmoPPixel(){
		Color=dxRGB(255,255,255);
	}
	dxoPPixel(const Pint& _pos,const dxRGB& _Color):hmoPPixel(_pos){
		local_set(_Color);
	}
	dxoPPixel(const dxoPPixel& _dxoP):hmoPPixel(_dxoP){local_copy(_dxoP);}
	dxoPPixel& operator=(const dxoPPixel& _dxoPp){
		if(this!=&_dxoPp)copy(_dxoPp);
		return *this;
	}
	void set(const Pint& _pos,const dxRGB& _Color){
		hmoPPixel::set(_pos);
		local_set(_Color);
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
		dx::drawPixel(_pos+pos,Color,_mode);
		return 0;
	}
};
class dxoPLine : public dxObject,public hmoPLine{
public:
	dxRGB Color;
	int Thick;
private:
	void local_set(const dxRGB& _Color,int _Thick){
		Color=_Color;
		Thick=_Thick;
	}
	void local_copy(const dxoPLine& _dxoP){
		Color=_dxoP.Color;
		Thick=_dxoP.Thick;
	}
protected:
	void copy(const dxoPLine& _dxoP){
		hmoPLine::copy(_dxoP);
		local_copy(_dxoP);
	}
public:
	dxoPLine():hmoPLine(){local_set(dxRGB(255,255,255),1);}
	dxoPLine(const Pint& _pos1,const Pint& _pos2,const dxRGB& _Color,int _Thick=1):hmoPLine(_pos1,_pos2){local_set(_Color,_Thick);}
	dxoPLine(const dxoPLine& _dxoP):hmoPLine(_dxoP){copy(_dxoP);}
	dxoPLine& operator=(const dxoPLine& _dxoPl){
		if(this!=&_dxoPl)copy(_dxoPl);
		return *this;
	}
	void set(const Pint& _pos1,const Pint& _pos2,const dxRGB& _Color,int _Thick=1){
		hmoPLine::set(_pos1,_pos2);
		local_set(_Color,_Thick);
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
		dx::drawLine(_pos+pos1,_pos+pos2,Color,Thick,_mode);
		return 0;
	}
};
class dxoPBox : public virtual dxObject,public virtual hmoPBox{
public:
	dxRGB Color;
	int FillFlag;
private:
	void local_set(const dxRGB& _Color,int _FillFlag=1){
		Color=_Color;
		FillFlag=_FillFlag;
	}
	void local_copy(const dxoPBox& _dxoP){
		Color=_dxoP.Color;
		FillFlag=_dxoP.FillFlag;
	}
protected:
	void copy(const dxoPBox& _dxoP){
		hmoPBox::copy(_dxoP);
		local_copy(_dxoP);
	}
public:
	dxoPBox():hmoPBox(){
		Color=dxRGB(255,255,255);
		FillFlag=1;
	}
	dxoPBox(const Pint& _pos1,const Pint& _pos2,const dxRGB& _Color,int _FillFlag=1):hmoPBox(_pos1,_pos2){local_set(_Color,_FillFlag);}
	dxoPBox(const dxoPBox& _dxoP):hmoPBox(_dxoP){copy(_dxoP);}
	dxoPBox& operator=(const dxoPBox& _dxoP){
		if(this!=&_dxoP)copy(_dxoP);
		return *this;
	}
	void set(const Pint& _pos1,const Pint& _pos2,const dxRGB& _Color,int _FillFlag=1){
		hmoPBox::set(_pos1,_pos2);
		local_set(_Color,_FillFlag);
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
		dx::drawBox(_pos+pos1,_pos+pos2,Color,FillFlag,_mode);
		return 0;
	}
};
class dxoPTriangle:public dxObject,public hmoPTriangle{
public:
	dxRGB Color;
	int FillFlag;
private:
	void local_set(const dxRGB& _Color,const int& _FillFlag){
		Color=_Color;
		FillFlag=_FillFlag;
	}
	void local_copy(const dxoPTriangle& _dxoP){
		Color=_dxoP.Color;
		FillFlag=_dxoP.FillFlag;
	}
protected:
	void copy(const dxoPTriangle& _dxoP){
		hmoPTriangle::copy(_dxoP);
		local_copy(_dxoP);
	}
public:
	dxoPTriangle():hmoPTriangle(){local_set(dxRGB(255,255,255),1);}
	dxoPTriangle(const Pint& _pos1,const Pint& _pos2,const Pint& _pos3,const dxRGB& _Color,int _FillFlag=1):hmoPTriangle(_pos1,_pos2,_pos3){local_set(_Color,_FillFlag);}
	dxoPTriangle(const dxoPTriangle& _dxoP):hmoPTriangle(_dxoP){local_copy(_dxoP);}
	dxoPTriangle& operator=(const dxoPTriangle& _dxoP){
		if(this!=&_dxoP)copy(_dxoP);
		return *this;
	}
	void set(const Pint& _pos1,const Pint& _pos2,const Pint& _pos3,const dxRGB& _Color,int _FillFlag=1){
		hmoPTriangle::set(_pos1,_pos2,_pos3);
		local_set(_Color,_FillFlag);
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
		dx::drawTriangle(_pos+pos1,_pos+pos2,_pos+pos3,Color,FillFlag,_mode);
		return 0;
	}
};
class dxoPCircle:public dxObject,public hmoPCircle{
public:
	dxRGB Color;
	int FillFlag;
private:
	void local_set(const dxRGB& _Color,const int& _FillFlag){
		Color=_Color;
		FillFlag=_FillFlag;
	}
	void local_copy(const dxoPCircle& _dxoP){
		Color=_dxoP.Color;
		FillFlag=_dxoP.FillFlag;
	}
protected:
	void copy(const dxoPCircle& _dxoP){
		hmoPCircle::copy(_dxoP);
		local_copy(_dxoP);
	}
public:
	dxoPCircle():hmoPCircle(){local_set(dxRGB(255,255,255),1);}
	dxoPCircle(const Pint& _pos,const int& _r,const dxRGB& _Color,int _FillFlag=1):hmoPCircle(_pos,_r){local_set(_Color,_FillFlag);}
	dxoPCircle(const dxoPCircle& _dxoP):hmoPCircle(_dxoP){local_copy(_dxoP);}
	dxoPCircle& operator=(const dxoPCircle& _dxoP){
		if(this!=&_dxoP)copy(_dxoP);
		return *this;
	}
	void set(const Pint& _pos,const int& _r,const dxRGB& _Color,int _FillFlag=1){
		hmoPCircle::set(_pos,_r);
		local_set(_Color,_FillFlag);
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
		dx::drawCircle(_pos+pos,r,Color,FillFlag,_mode);
		return 0;
	}
};
class dxoPOval:public dxObject,public hmoPOval{
public:
	dxRGB Color;
	int FillFlag;
private:
	void local_set(const dxRGB& _Color,const int& _FillFlag){
		Color=_Color;
		FillFlag=_FillFlag;
	}
	void local_copy(const dxoPOval& _dxoP){
		Color=_dxoP.Color;
		FillFlag=_dxoP.FillFlag;
	}
protected:
	void copy(const dxoPOval& _dxoP){
		hmoPOval::copy(_dxoP);
		local_copy(_dxoP);
	}
public:
	dxoPOval():hmoPOval(){local_set(dxRGB(255,255,255),1);}
	dxoPOval(const Pint& _pos,const Pint& _r,const dxRGB& _Color,const int& _FillFlag):hmoPOval(_pos,_r){local_set(_Color,_FillFlag);}
	dxoPOval(const dxoPOval& _dxoP):hmoPOval(_dxoP){local_copy(_dxoP);}
	dxoPOval& operator=(const dxoPOval& _dxoP){
		if(this!=&_dxoP)copy(_dxoP);
		return *this;
	}
	void set(const Pint& _pos,const Pint& _r,const dxRGB& _Color,const int& _FillFlag){
		hmoPOval::set(_pos,_r);
		local_set(_Color,_FillFlag);
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
		dx::drawOval(_pos+pos,r,Color,FillFlag,_mode);
		return 0;
	}
}; 
class dxoStr : public dxObject,public hmoBox{
protected:
	int FontHandle;
	int FontSize;
	int VFlag;
public:
	std::string str;
	dxRGB Color,EColor;
private:
	void local_set(const std::string _str,const dxFont& _font,const dxRGB& _Color,const dxRGB& _EColor){
		FontHandle=_font.getHandle();
		FontSize=_font.getSize();
		VFlag=_font.getVFlag();
		str=_str;
		Color=_Color;
		EColor=_EColor;
	}
	void local_copy(const dxoStr& _dxo){
		str=_dxo.str;
		FontHandle=_dxo.FontHandle;
		FontSize=_dxo.FontSize;
		VFlag=_dxo.VFlag;
		Color=_dxo.Color;
		EColor=_dxo.EColor;
	}
protected:
	void copy(const dxoStr& _dxo){
		hmoBox::copy(_dxo);
		local_copy(_dxo);
	}
public:
	dxoStr():hmoBox(){local_set("",dx::getStdFont(),dxRGB(255,255,255),dxRGB(0,0,0));}
	dxoStr(const std::string _str,const dxRGB& _Color=dxRGB(255,255,255),const dxRGB& _EColor=dxRGB(0,0,0)):hmoBox(){local_set(_str,dx::getStdFont(),_Color,_EColor);}
	dxoStr(const std::string _str,const dxFont& _font,const dxRGB& _Color=dxRGB(255,255,255),const dxRGB& _EColor=dxRGB(0,0,0)):hmoBox(){local_set(_str,_font,_Color,_EColor);}
	dxoStr(const dxoStr& _dxo):dxBasicObject(_dxo),dxObject(_dxo),hmoBox(_dxo){copy(_dxo);}
	dxoStr& operator=(const dxoStr& _dxo){
		if(this!=&_dxo)copy(_dxo);
		return *this;
	}
	void set(const std::string _str,const dxRGB& _Color=dxRGB(255,255,255),const dxRGB& _EColor=dxRGB(0,0,0)){
		hmoBox::set(Pint());
		local_set(_str,dx::getStdFont(),_Color,_EColor);
	}
	void set(const std::string _str,const dxFont& _font,const dxRGB& _Color=dxRGB(255,255,255),const dxRGB& _EColor=dxRGB(0,0,0)){
		hmoBox::set(Pint());
		local_set(_str,_font,_Color,_EColor);
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
		return dx::drawStr(_pos,str,Color,_mode,VFlag,FontHandle,EColor);
	}
	virtual void setArea(){
		if(VFlag){
			if(FontHandle==-1){
				size.x=dx::getFontSize();
				size.y=dx::getDrawStringWidth(str,str.size(),-1,1);
			}else{
				size.x=FontSize;
				size.y=dx::getDrawStringWidth(str,str.size(),FontHandle,1);
			}
		}else{
			if(FontHandle==-1){
				size.x=dx::getDrawStringWidth(str,str.size());
				size.y=dx::getFontSize();
			}else{
				size.x=dx::getDrawStringWidth(str,str.size(),FontHandle);
				size.y=FontSize;
			}
		}
	}
	void setFont(const dxFont& _font){
		FontHandle=_font.getHandle();
		VFlag=_font.getVFlag();
	}
};
class dxoStrP : public dxObject,public hmoBox{
protected:
	int FontHandle;
	int VFlag;
	int FontSize;
public:
	std::string str;
	dxRGB SColor,SEColor;
	ALI::ALIGN Ali;
private:
	void local_set(const std::string _str,const dxFont& _font,const dxRGB& _SColor,ALI::ALIGN _Ali,const dxRGB& _SEColor){
		str=_str;
		FontHandle=_font.getHandle();
		VFlag=_font.getVFlag();
		FontSize=_font.getSize();
		SColor=_SColor;
		SEColor=_SEColor;
		Ali=_Ali;
	}
	void local_copy(const dxoStrP& _dxo){
		str=_dxo.str;
		FontHandle=_dxo.FontHandle;
		VFlag=_dxo.VFlag;
		FontSize=_dxo.FontSize;
		SColor=_dxo.SColor;
		SEColor=_dxo.SEColor;
		Ali=_dxo.Ali;
	}
protected:
	virtual  void copy(const dxoStrP& _dxo){
		hmoBox::copy(_dxo);
		local_copy(_dxo);
	}
public:
	dxoStrP():hmoBox(){local_set("",dx::getStdFont(),dxRGB(255,255,255),ALI::center,dxRGB(0,0,0));}
	dxoStrP(const Pint& _size,const std::string _str,const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0)):hmoBox(_size){
		local_set(_str,dx::getStdFont(),_SColor,_Ali,_SEColor);
	}
	dxoStrP(const Pint& _size,const std::string _str,const dxFont& _font,const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0)):hmoBox(_size){
		local_set(_str,_font,_SColor,_Ali,_SEColor);
	}
	dxoStrP(const dxoStrP& _dxo):hmoBox(_dxo){local_copy(_dxo);}
	dxoStrP& operator=(const dxoStrP& _dxo){
		if(this!=&_dxo)copy(_dxo);
		return *this;
	}
	void set(const Pint& _size,const std::string _str,const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0)){
		hmoBox::set(_size);
		local_set(_str,dx::getStdFont(),_SColor,_Ali,_SEColor);
	}
	void set(const Pint& _size,const std::string _str,const dxFont& _font,const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0)){
		hmoBox::set(_size);
		local_set(_str,_font,_SColor,_Ali,_SEColor);
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
		dx::drawStrP(_pos,size,str,SColor,Ali,_mode,VFlag,FontHandle,FontSize,SEColor);
		return 0;
	}
	void setFont(const dxFont& _font){
		FontHandle=_font.getHandle();
		VFlag=_font.getVFlag();
		FontSize=_font.getSize();
	}
};
class dxoBasicBut:public dxoStrP{
public:
	dxRGB ButColor;
	dxDMode NonSelM,DarkM;
private:
	void local_set(const dxRGB& _ButColor,const dxDMode& _NonSelM,const dxDMode& _DarkM){
		ButColor=_ButColor;
		NonSelM=_NonSelM;
		DarkM=_DarkM;
	}
	void local_copy(const dxoBasicBut& _dxo){
		ButColor=_dxo.ButColor;
		NonSelM=_dxo.NonSelM;
		DarkM=_dxo.DarkM;
	}
protected:
	virtual bool checkOnOff(const Pint& _pos){
		if(hmoBox::pIn(_pos,dx::getMosPos()) && dx::getMosL()>0)return true;
		else return false;
	}
	virtual int checkAns(const Pint& _pos){
		if(!hmoBox::pIn(_pos,dx::getMosPos()))return 0;
		return dx::getMosL();
	}
	void copy(const dxoBasicBut& _dxo){
		dxoStrP::copy(_dxo);
		local_copy(_dxo);
	}
public:
	dxoBasicBut():dxoStrP(){local_set(dxRGB(128,128,128),dxDMode(),dxDMode());}
	dxoBasicBut(const Pint& _size,const std::string _str,const dxRGB& _ButColor,const dxDMode& _NonSelM,const dxDMode& _DarkM,
		const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0))
		:dxoStrP(_size,_str,_SColor,_Ali,_SEColor){
			local_set(_ButColor,_NonSelM,_DarkM);
	}
	dxoBasicBut(const Pint& _size,const std::string _str,const dxRGB& _ButColor,const dxDMode& _NonSelM,const dxDMode& _DarkM,
		const dxFont& _font,const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0))
		:dxoStrP(_size,_str,_font,_SColor,_Ali,_SEColor){
			local_set(_ButColor,_NonSelM,_DarkM);
	}
	dxoBasicBut(const dxoBasicBut& _dxo):dxoStrP(_dxo){local_copy(_dxo);}
	dxoBasicBut& operator=(const dxoBasicBut& _dxo){
		if(this!=&_dxo)copy(_dxo);
		return *this;
	}
	void set(const Pint& _size,const std::string _str,const dxRGB& _ButColor,const dxDMode& _NonSelM,const dxDMode& _DarkM,
		const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0)){
			dxoStrP::set(_size,_str,_SColor,_Ali,_SEColor);
			local_set(_ButColor,_NonSelM,_DarkM);
	}
	void set(const Pint& _size,const std::string _str,const dxRGB& _ButColor,const dxDMode& _NonSelM,const dxDMode& _DarkM,
		const dxFont& _font,const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0)){
			dxoStrP::set(_size,_str,_font,_SColor,_Ali,_SEColor);
			local_set(_ButColor,_NonSelM,_DarkM);
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
		if(!hmoBox::pIn(_pos,dx::getMosPos()) || !_active)_mode+=NonSelM;

		if(checkOnOff(_pos) && _active){
			dx::drawBox(_pos,_pos+size-Pint(2,2),ButColor,1,_mode+DarkM);
			dx::drawBox(_pos+PintY(size.y-2),_pos+size,ButColor,1,_mode);
			dx::drawBox(_pos+PintX(size.x-2),_pos+size-PintY(2),ButColor,1,_mode);
			dxoStrP::draw(_pos,_active,_mode);
		}else{
			dx::drawBox(_pos,_pos+size-Pint(2,2),ButColor,1,_mode);
			dx::drawBox(_pos+PintY(size.y-2),_pos+size,ButColor,1,_mode+DarkM);
			dx::drawBox(_pos+PintX(size.x-2),_pos+size-PintY(2),ButColor,1,_mode+DarkM);
			dxoStrP::draw(_pos,_active,_mode);
		}

		if(_active)return checkAns(_pos);
		else return 0;
	}
};
class dxoBut:public dxoBasicBut{
private:
	void local_set(){return;}
	void local_copy(const dxoBut& _dxo){return;}
protected:
	void copy(const dxoBut& _dxo){
		dxoBasicBut::copy(_dxo);
		local_copy(_dxo);
	}
public:
	dxoBut():dxoBasicBut(){return;}
	dxoBut(const Pint& _size,const std::string _str,const dxRGB& _ButColor,
		const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0))
		:dxoBasicBut(_size,_str,_ButColor,dxDMode(196),dxDMode(dxRGB(128,128,128)),_SColor,_Ali,_SEColor){
			local_set();
	}
	dxoBut(const Pint& _size,const std::string _str,const dxRGB& _ButColor,
		const dxFont& _font,const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0))
		:dxoBasicBut(_size,_str,_ButColor,dxDMode(196),dxDMode(dxRGB(128,128,128)),_font,_SColor,_Ali,_SEColor){
			local_set();
	}
	dxoBut(const dxoBut& _dxo):dxoBasicBut(_dxo){local_copy(_dxo);}
	dxoBut& operator=(const dxoBut& _dxo){
		if(this!=&_dxo)copy(_dxo);
		return *this;
	}
	void set(const Pint& _size,const std::string _str,const dxRGB& _ButColor,
		const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0)){
			dxoBasicBut::set(_size,_str,_ButColor,dxDMode(196),dxDMode(dxRGB(128,128,128)),_SColor,_Ali,_SEColor);
			local_set();
	}
	void set(const Pint& _size,const std::string _str,const dxRGB& _ButColor,
		const dxFont& _font,const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0)){
			dxoBasicBut::set(_size,_str,_ButColor,dxDMode(196),dxDMode(dxRGB(128,128,128)),_font,_SColor,_Ali,_SEColor);
			local_set();
	}
};
class dxoButLR:public dxoBasicBut{
private:
	void local_set(){return;}
	void local_copy(const dxoButLR& _dxo){return;}
protected:
	virtual bool checkOnOff(const Pint& _pos){
		if(hmoBox::pIn(_pos,dx::getMosPos()) && (dx::getMosL()>0 || dx::getMosR()>0))return true;
		else return false;
	}
	virtual int checkAns(const Pint& _pos){
		if(!hmoBox::pIn(_pos,dx::getMosPos()))return 0;
		return dx::getMosL()+dx::getMosR()*10;
	}
	void copy(const dxoButLR& _dxo){
		dxoBasicBut::copy(_dxo);
		local_copy(_dxo);
	}
public:
	dxoButLR():dxoBasicBut(){return;}
	dxoButLR(const Pint& _size,const std::string _str,const dxRGB& _ButColor,
		const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0))
		:dxoBasicBut(_size,_str,_ButColor,dxDMode(196),dxDMode(dxRGB(128,128,128)),_SColor,_Ali,_SEColor){
			local_set();
	}
	dxoButLR(const Pint& _size,const std::string _str,const dxRGB& _ButColor,
		const dxFont& _font,const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0))
		:dxoBasicBut(_size,_str,_ButColor,dxDMode(196),dxDMode(dxRGB(128,128,128)),_font,_SColor,_Ali,_SEColor){
			local_set();
	}
	dxoButLR(const dxoButLR& _dxo):dxoBasicBut(_dxo){local_copy(_dxo);}
	dxoButLR& operator=(const dxoButLR& _dxo){
		if(this!=&_dxo)copy(_dxo);
		return *this;
	}
	void set(const Pint& _size,const std::string _str,const dxRGB& _ButColor,
		const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0)){
			dxoBasicBut::set(_size,_str,_ButColor,dxDMode(196),dxDMode(dxRGB(128,128,128)),_SColor,_Ali,_SEColor);
			local_set();
	}
	void set(const Pint& _size,const std::string _str,const dxRGB& _ButColor,
		const dxFont& _font,const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0)){
			dxoBasicBut::set(_size,_str,_ButColor,dxDMode(196),dxDMode(dxRGB(128,128,128)),_font,_SColor,_Ali,_SEColor);
			local_set();
	}
};
class dxoBasicButIO:public dxoBasicBut{
public:
	bool OnOff;
private:
	void local_set(bool _OnOff){
		OnOff=_OnOff;
	}
	void local_copy(const dxoBasicButIO& _dxo){
		OnOff=_dxo.OnOff;
	}
protected:
	virtual bool checkOnOff(const Pint& _pos){return OnOff;}
	void copy(const dxoBasicButIO& _dxo){
		dxoBasicBut::copy(_dxo);
		local_copy(_dxo);
	}
public:
	dxoBasicButIO():dxoBasicBut(){local_set(false);}
	dxoBasicButIO(const Pint& _size,const std::string _str,const dxRGB& _ButColor,const dxDMode& _NonSelM,const dxDMode& _DarkM,bool _OnOff,
		const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0))
		:dxoBasicBut(_size,_str,_ButColor,_NonSelM,_DarkM,_SColor,_Ali,_SEColor){
			local_set(_OnOff);
	}
	dxoBasicButIO(const Pint& _size,const std::string _str,const dxRGB& _ButColor,const dxDMode& _NonSelM,const dxDMode& _DarkM,bool _OnOff,
		const dxFont& _font,const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0))
		:dxoBasicBut(_size,_str,_ButColor,_NonSelM,_DarkM,_font,_SColor,_Ali,_SEColor){
			local_set(_OnOff);
	}
	dxoBasicButIO(const dxoBasicButIO& _dxo):dxoBasicBut(_dxo){local_copy(_dxo);}
	dxoBasicButIO& operator=(const dxoBasicButIO& _dxo){
		if(this!=&_dxo)copy(_dxo);
		return *this;
	}
	void set(const Pint& _size,const std::string _str,const dxRGB& _ButColor,const dxDMode& _NonSelM,const dxDMode& _DarkM,bool _OnOff,
		const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0)){
			dxoBasicBut::set(_size,_str,_ButColor,_NonSelM,_DarkM,_SColor,_Ali,_SEColor);
			local_set(_OnOff);
	}
	void set(const Pint& _size,const std::string _str,const dxRGB& _ButColor,const dxDMode& _NonSelM,const dxDMode& _DarkM,bool _OnOff,
		const dxFont& _font,const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0)){
			dxoBasicBut::set(_size,_str,_ButColor,_NonSelM,_DarkM,_font,_SColor,_Ali,_SEColor);
			local_set(_OnOff);
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
		if(!hmoBox::pIn(_pos,dx::getMosPos()) || !_active)_mode+=NonSelM;

		if(checkOnOff(_pos)){
			dx::drawBox(_pos,_pos+size,ButColor,0,_mode+DarkM);
			dx::drawBox(_pos+Pint(1,1),_pos+size-Pint(1,1),ButColor,1,_mode);
			dxoStrP::draw(_pos,_active,_mode);
		}else{
			dx::drawBox(_pos,_pos+size,ButColor,0,_mode);
			dx::drawBox(_pos+Pint(1,1),_pos+size-Pint(1,1),ButColor,1,_mode+DarkM);
			dxoStrP::draw(_pos,_active,_mode);
		}

		if(_active)return checkAns(_pos);
		else return 0;
	}
};
class dxoButIO:public dxoBasicButIO{
private:
	void local_set(){return;}
	void local_copy(const dxoButIO& _dxo){return;}
protected:
	void copy(const dxoButIO& _dxo){
		dxoBasicButIO::copy(_dxo);
		local_copy(_dxo);
	}
public:
	dxoButIO():dxoBasicButIO(){return;}
	dxoButIO(const Pint& _size,const std::string _str,const dxRGB& _ButColor,bool _OnOff,
		const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0))
		:dxoBasicButIO(_size,_str,_ButColor,dxDMode(196),dxDMode(dxRGB(128,128,128)),_OnOff,_SColor,_Ali,_SEColor){
			local_set();
	}
	dxoButIO(const Pint& _size,const std::string _str,const dxRGB& _ButColor,bool _OnOff,
		const dxFont& _font,const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0))
		:dxoBasicButIO(_size,_str,_ButColor,dxDMode(196),dxDMode(dxRGB(128,128,128)),_OnOff,_font,_SColor,_Ali,_SEColor){
			local_set();
	}
	dxoButIO(const dxoButIO& _dxo):dxoBasicButIO(_dxo){local_copy(_dxo);}
	dxoButIO& operator=(const dxoButIO& _dxo){
		if(this!=&_dxo)copy(_dxo);
		return *this;
	}
	void set(const Pint& _size,const std::string _str,const dxRGB& _ButColor,bool _OnOff,
		const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0)){
			dxoBasicButIO::set(_size,_str,_ButColor,dxDMode(196),dxDMode(dxRGB(128,128,128)),_OnOff,_SColor,_Ali,_SEColor);
			local_set();
	}
	void set(const Pint& _size,const std::string _str,const dxRGB& _ButColor,bool _OnOff,
		const dxFont& _font,const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0)){
			dxoBasicButIO::set(_size,_str,_ButColor,dxDMode(196),dxDMode(dxRGB(128,128,128)),_OnOff,_font,_SColor,_Ali,_SEColor);
			local_set();
	}
};
class dxoButLRIO:public dxoBasicButIO{
private:
	void local_set(){return;}
	void local_copy(const dxoButLRIO& _dxo){return;}
protected:
	virtual int checkAns(const Pint& _pos){
		if(!hmoBox::pIn(_pos,dx::getMosPos()))return 0;
		return dx::getMosL()+dx::getMosR()*10;
	}
	void copy(const dxoButLRIO& _dxo){
		dxoBasicButIO::copy(_dxo);
		local_copy(_dxo);
	}
public:
	dxoButLRIO():dxoBasicButIO(){return;}
	dxoButLRIO(const Pint& _size,const std::string _str,const dxRGB& _ButColor,bool _OnOff,
		const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0))
		:dxoBasicButIO(_size,_str,_ButColor,dxDMode(196),dxDMode(dxRGB(128,128,128)),_OnOff,_SColor,_Ali,_SEColor){
			local_set();
	}
	dxoButLRIO(const Pint& _size,const std::string _str,const dxRGB& _ButColor,bool _OnOff,
		const dxFont& _font,const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0))
		:dxoBasicButIO(_size,_str,_ButColor,dxDMode(196),dxDMode(dxRGB(128,128,128)),_OnOff,_font,_SColor,_Ali,_SEColor){
			local_set();
	}
	dxoButLRIO(const dxoButLRIO& _dxo):dxoBasicButIO(_dxo){local_copy(_dxo);}
	dxoButLRIO& operator=(const dxoButLRIO& _dxo){
		if(this!=&_dxo)copy(_dxo);
		return *this;
	}
	void set(const Pint& _size,const std::string _str,const dxRGB& _ButColor,bool _OnOff,
		const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0)){
			dxoBasicButIO::set(_size,_str,_ButColor,dxDMode(196),dxDMode(dxRGB(128,128,128)),_OnOff,_SColor,_Ali,_SEColor);
			local_set();
	}
	void set(const Pint& _size,const std::string _str,const dxRGB& _ButColor,bool _OnOff,
		const dxFont& _font,const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0)){
			dxoBasicButIO::set(_size,_str,_ButColor,dxDMode(196),dxDMode(dxRGB(128,128,128)),_OnOff,_font,_SColor,_Ali,_SEColor);
			local_set();
	}
};
class dxoRadio: public dxoStrP{
public:
	int r;
	dxRGB RadColor;
	bool OnOff;
private:
	void local_set(const int& _r,const dxRGB& _RadColor,bool _OnOff){
		r=_r;
		RadColor=_RadColor;
		OnOff=_OnOff;
	}
	void local_copy(const dxoRadio& _dxo){
		r=_dxo.r;
		RadColor=_dxo.RadColor;
		OnOff=_dxo.OnOff;
	}
protected:
	void copy(const dxoRadio& _dxo){
		dxoStrP::copy(_dxo);
		local_copy(_dxo);
	}
public:
	dxoRadio():dxoStrP(){return;}
	dxoRadio(const Pint& _size,const std::string _str,const int& _r,const dxRGB& _RadColor,bool _OnOff,
		const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0))
		:dxoStrP(_size,_str,_SColor,_Ali,_SEColor){
			local_set(_r,_RadColor,_OnOff);
	}
	dxoRadio(const Pint& _size,const std::string _str,const int& _r,const dxRGB& _RadColor,bool _OnOff,
		const dxFont& _font,const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0))
		:dxoStrP(_size,_str,_font,_SColor,_Ali,_SEColor){
			local_set(_r,_RadColor,_OnOff);
	}
	dxoRadio(const dxoRadio& _dxo):dxoStrP(_dxo){local_copy(_dxo);}
	dxoRadio& operator=(const dxoRadio& _dxo){
		if(this!=&_dxo)copy(_dxo);
		return *this;
	}
	void set(const Pint& _size,const std::string _str,const int& _r,const dxRGB& _RadColor,bool _OnOff,
		const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0)){
			dxoStrP::set(_size,_str,_SColor,_Ali,_SEColor);
			local_set(_r,_RadColor,_OnOff);
	}
	void set(const Pint& _size,const std::string _str,const int& _r,const dxRGB& _RadColor,bool _OnOff,
		const dxFont& _font,const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0)){
			dxoStrP::set(_size,_str,_font,_SColor,_Ali,_SEColor);
			local_set(_r,_RadColor,_OnOff);
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
		int len=dx::getDrawStringWidth(str,str.size(),FontHandle,VFlag)+r*2+2;
		int xctr;
		int yctr;

		if(VFlag){
			xctr=size.x/2-r;
			yctr=size.y/2-len/2;
		}else{
			xctr=size.x/2-len/2;
			yctr=size.y/2-r;
		}

		if(Ali&ALI::left)xctr=0;
		else if(Ali&ALI::right)xctr=size.x-len;
		if(Ali&ALI::top)yctr=0;
		else if(Ali&ALI::bottom)yctr=size.y-r*2;

		if(!hmoCircle(r).pIn(Pint(xctr+r,yctr+r)+_pos,dx::getMosPos()) || !_active)_mode+=dxDMode(196);

		dx::drawCircle(_pos+Pint(xctr+r,yctr+r),r,RadColor,1,_mode+dxDMode(dxRGB(128,128,128)));
		dx::drawCircle(_pos+Pint(xctr+r,yctr+r),r,RadColor,0,_mode);

		if(OnOff)dx::drawCircle(_pos+Pint(xctr+r,yctr+r),r/2,RadColor,1,_mode);

		if(VFlag){
			dx::drawStrP(_pos+Pint(xctr,yctr+r*2+2),Pint(r*2,0),str,SColor,ALI::top,_mode,VFlag,FontHandle,FontSize,SEColor);
		}else{
			dx::drawStrP(_pos+Pint(xctr+r*2+2,yctr),Pint(0,r*2),str,SColor,ALI::left,_mode,VFlag,FontHandle,FontSize,SEColor);
		}

		if(hmoCircle(r).pIn(Pint(xctr+r,yctr+r)+_pos,dx::getMosPos()) && _active)return dx::getKey(KEY::MOSL);
		else return 0;
	}
};
class dxoCheck: public dxoStrP{
public:
	int bsize;
	dxRGB CheColor;
	bool OnOff;
private:
	void local_set(const int& _bsize,const dxRGB& _CheColor,bool _OnOff){
		bsize=_bsize;
		CheColor=_CheColor;
		OnOff=_OnOff;
	}
	void local_copy(const dxoCheck& _dxo){
		bsize=_dxo.bsize;
		CheColor=_dxo.CheColor;
		OnOff=_dxo.OnOff;
	}
protected:
	void copy(const dxoCheck& _dxo){
		dxoStrP::copy(_dxo);
		local_copy(_dxo);
	}
public:
	dxoCheck():dxoStrP(){return;}
	dxoCheck(const Pint& _size,const std::string _str,const int& _bsize,const dxRGB& _CheColor,bool _OnOff,
		const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0))
		:dxoStrP(_size,_str,_SColor,_Ali,_SEColor){
			local_set(_bsize,_CheColor,_OnOff);
	}
	dxoCheck(const Pint& _size,const std::string _str,const int& _bsize,const dxRGB& _CheColor,bool _OnOff,
		const dxFont& _font,const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0))
		:dxoStrP(_size,_str,_font,_SColor,_Ali,_SEColor){
			local_set(_bsize,_CheColor,_OnOff);
	}
	dxoCheck(const dxoCheck& _dxo):dxoStrP(_dxo){local_copy(_dxo);}
	dxoCheck& operator=(const dxoCheck& _dxo){
		if(this!=&_dxo)copy(_dxo);
		return *this;
	}
	void set(const Pint& _size,const std::string _str,const int& _bsize,const dxRGB& _CheColor,bool _OnOff,
		const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0)){
			dxoStrP::set(_size,_str,_SColor,_Ali,_SEColor);
			local_set(_bsize,_CheColor,_OnOff);
	}
	void set(const Pint& _size,const std::string _str,const int& _bsize,const dxRGB& _CheColor,bool _OnOff,
		const dxFont& _font,const dxRGB& _SColor=dxRGB(255,255,255),ALI::ALIGN _Ali=ALI::center,const dxRGB& _SEColor=dxRGB(0,0,0)){
			dxoStrP::set(_size,_str,_font,_SColor,_Ali,_SEColor);
			local_set(_bsize,_CheColor,_OnOff);
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
		int len=dx::getDrawStringWidth(str,str.size(),FontHandle,VFlag)+bsize+2;
		int xctr;
		int yctr;

		if(VFlag){
			xctr=size.x/2-bsize/2;
			yctr=size.y/2-len/2;
		}else{
			xctr=size.x/2-len/2;
			yctr=size.y/2-bsize/2;
		}

		if(Ali&ALI::left)xctr=0;
		else if(Ali&ALI::right)xctr=size.x-len;
		if(Ali&ALI::top)yctr=0;
		else if(Ali&ALI::bottom)yctr=size.y-bsize;

		if(!hmoBox(Pint(bsize,bsize)).pIn(_pos+Pint(xctr,yctr),dx::getMosPos()) || !_active)_mode+=dxDMode(196);

		dx::drawBox(_pos+Pint(xctr,yctr),_pos+Pint(xctr+bsize,yctr+bsize),CheColor,1,_mode+dxDMode(dxRGB(128,128,128)));
		SetDrawBright(255,255,255);
		dx::drawBox(_pos+Pint(xctr,yctr),_pos+Pint(xctr+bsize,yctr+bsize),CheColor,0,_mode);

		if(OnOff){
			dx::drawLine(_pos+Pint(xctr+bsize/4,yctr+bsize/2),_pos+Pint(xctr+bsize/2,yctr+bsize*3/4),CheColor,2,_mode);
			dx::drawLine(_pos+Pint(xctr+bsize*3/4,yctr+bsize/4),_pos+Pint(xctr+bsize/2,yctr+bsize*3/4),CheColor,2,_mode);
		}

		if(VFlag){
			dx::drawStrP(_pos+Pint(xctr,yctr+bsize+2),Pint(bsize,len),str,SColor,ALI::top,_mode,VFlag,FontHandle,FontSize,SEColor);
		}else{
			dx::drawStrP(_pos+Pint(xctr+bsize+2,yctr),Pint(len,bsize),str,SColor,ALI::left,_mode,VFlag,FontHandle,FontSize,SEColor);
		}

		if(hmoBox(Pint(bsize,bsize)).pIn(_pos+Pint(xctr,yctr),dx::getMosPos()) && _active)return dx::getMosL();
		return 0;
	}
};
class dxoSlider:public dxObject,public hmoBox{
public:
	dxRGB SliColor;
	Pint tsize,datasize;
	Pint* datap;
private:
	void local_set(const Pint& _tsize,Pint& _data,const Pint& _datasize,const dxRGB& _SliColor){
		SliColor=_SliColor;
		tsize=_tsize;
		datasize=_datasize;
		datap=&_data;
	}
	void local_copy(const dxoSlider& _dxo){
		SliColor=_dxo.SliColor;
		tsize=_dxo.tsize;
		datasize=_dxo.datasize;
		datap=_dxo.datap;
	}
protected:
	void copy(const dxoSlider& _dxo){
		hmoBox::copy(_dxo);
		local_copy(_dxo);
	}
public:
	dxoSlider():hmoBox(){local_set(Pint(0,0),Pint(0,0),Pint(0,0),dxRGB(255,255,255));}
	dxoSlider(const Pint& _size,const Pint& _tsize,Pint& _data,const Pint& _datasize,const dxRGB& _SliColor)
		:hmoBox(_size){
			local_set(_tsize,_data,_datasize,_SliColor);
	}
	dxoSlider(const dxoSlider& _dxo):hmoBox(_dxo){local_copy(_dxo);}
	dxoSlider& operator=(const dxoSlider& _dxo){
		if(this!=&_dxo)copy(_dxo);
		return *this;
	}
	void set(const Pint& _size,const Pint& _tsize,Pint& _data,const Pint& _datasize,const dxRGB& _SliColor){
		hmoBox::set(_size);
		local_set(_tsize,_data,_datasize,_SliColor);
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
		if(!this->pIn(_pos,dx::getMosPos()) || !_active)_mode+=dxDMode(196);

		dx::drawBox(_pos,_pos+size,SliColor,1,_mode+dxDMode(dxRGB(128,128,128)));
		dx::drawBox(_pos,_pos+size,SliColor,0,_mode);

		Pint tpos;
		if(datasize.x!=0)tpos.x=(size.x-tsize.x)*(datap->x)/datasize.x;
		if(datasize.y!=0)tpos.y=(size.y-tsize.y)*(datap->y)/datasize.y;

		if(this->pIn(_pos,dx::getMosLPos()) && dx::getMosL()==2 && _active){
			tpos=(dx::getMosPos()-_pos-tsize/2);
			tpos=PintMax(tpos,Pint(0,0));
			tpos=PintMin(tpos,size-tsize);
			if(datasize.x!=0)datap->x=datasize.x*tpos.x/(size.x-tsize.x);
			if(datasize.y!=0)datap->y=datasize.y*tpos.y/(size.y-tsize.y);
		}
		dx::drawBox(_pos+tpos,_pos+tpos+tsize,SliColor,1,_mode);

		return 0;
	}
};
class dxoGraph : public virtual dxObject,public virtual hmoBox{
public:
	basic_dxGraph* grpp;
	bool TurnFlag;
	bool TransFlag;
private:
	void local_set(basic_dxGraph* _grpp,bool _TransFlag,bool _TurnFlag){
		grpp=_grpp;
		TransFlag=_TransFlag;
		TurnFlag=_TurnFlag;
	}
	void local_copy(const dxoGraph& _dxo){
		grpp=_dxo.grpp;
		TransFlag=_dxo.TransFlag;
		TurnFlag=_dxo.TurnFlag;
	}
protected:
	void copy(const dxoGraph& _dxo){
		hmoBox::copy(_dxo);
		local_copy(_dxo);
	}
public:
	dxoGraph():hmoBox(){local_set(0,false,false);}
	dxoGraph(basic_dxGraph& _grp,bool _TransFlag=true,bool _TurnFlag=0)
		:hmoBox(_grp.getSize()){
			local_set(&_grp,_TransFlag,_TurnFlag);
	}
	dxoGraph(basic_dxGraph& _grp,const Pint& _size,bool _TransFlag=true)
		:hmoBox(_size){
			local_set(&_grp,_TransFlag,false);
	}
	dxoGraph(const dxoGraph& _dxo):hmoBox(_dxo){local_copy(_dxo);}
	dxoGraph& operator=(const dxoGraph& _dxo){
		if(this!=&_dxo)copy(_dxo);
		return *this;
	}
	void set(dxGraph& _grp,bool _TransFlag=true,bool _TurnFlag=0){
		hmoBox::set(_grp.getSize());
		local_set(&_grp,_TransFlag,_TurnFlag);
	}
	void set(const Pint& _size,dxGraph& _grp,bool _TransFlag=true){
		hmoBox::set(_size);
		local_set(&_grp,_TransFlag,false);
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
		if(grpp==0)return -1;
		if(grpp->getHandle()<0)return -1;
		if(TurnFlag)dx::drawGraph(_pos,*grpp,TransFlag,TurnFlag,_mode);
		else dx::drawGraph(_pos,*grpp,size,TransFlag,_mode);
		return 0;
	}
};
class dxoRotGraph : public virtual dxObject,public virtual hmDirObject{
public:
	basic_dxGraph* grpp;
	Pint cpos;
	double rate,angle;
	bool TurnFlag;
	bool TransFlag;
private:
	void local_set(basic_dxGraph* _grpp,const Pint& _cpos,const double& _rate,const double& _angle,bool _TransFlag,bool _TurnFlag){
		grpp=_grpp;
		cpos=_cpos;
		rate=_rate;
		angle=_angle;
		TransFlag=_TransFlag;
		TurnFlag=_TurnFlag;
	}
	void local_copy(const dxoRotGraph& _dxo){
		grpp=_dxo.grpp;
		cpos=_dxo.cpos;
		rate=_dxo.rate;
		angle=_dxo.angle;
		TransFlag=_dxo.TransFlag;
		TurnFlag=_dxo.TurnFlag;
	}
	int getr()const{return static_cast<int>(abs(grpp->getSize()/2)*rate)+1;}
	Pint getAdj()const{return static_cast<Pint>(rot(static_cast<Pdouble>(grpp->getSize()/2-cpos)*rate,angle));}
protected:
	void copy(const dxoRotGraph& _dxo){local_copy(_dxo);}
public:
	dxoRotGraph(){local_set(0,Pint(),0.,0.,false,false);}
	dxoRotGraph(basic_dxGraph& _grp,const double& _rate,const double& _angle,bool _TransFlag=true,bool _TurnFlag=false){
			local_set(&_grp,_grp.getSize()/2,_rate,_angle,_TransFlag,_TurnFlag);
	}
	dxoRotGraph(basic_dxGraph& _grp,const double& _rate,const double& _angle,const Pint& _cpos,bool _TransFlag=true,bool _TurnFlag=false){
			local_set(&_grp,_cpos,_rate,_angle,_TransFlag,_TurnFlag);
	}
	dxoRotGraph(const dxoRotGraph& _dxo){local_copy(_dxo);}
	dxoRotGraph& operator=(const dxoRotGraph& _dxo){
		if(this!=&_dxo)copy(_dxo);
		return *this;
	}
	void set(basic_dxGraph& _grp,const double& _rate,const double& _angle,bool _TransFlag=true,bool _TurnFlag=false){
		local_set(&_grp,_grp.getSize()/2,_rate,_angle,_TransFlag,_TurnFlag);
	}
	void set(basic_dxGraph& _grp,const double& _rate,const double& _angle,const Pint& _cpos,bool _TransFlag=true,bool _TurnFlag=false){
		local_set(&_grp,_cpos,_rate,_angle,_TransFlag,_TurnFlag);
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
		if(grpp==0)return -1;
		if(grpp->getHandle()<0)return -1;
		dx::drawGraph(_pos,*grpp,rate,angle,cpos,TransFlag,TurnFlag,_mode);
		return 0;
	}
	virtual void getArea(Pint& _min,Pint& _max)const{
		hmoCircle(getr()).getArea(_min,_max);
		_min+=getAdj();
		_max+=getAdj();
	}
	virtual bool pIn(const Pint& _pos,const Pint& _tpos)const{return hmoCircle(getr()).pIn(_pos+getAdj(),_tpos);}
	virtual bool lIn(const Pint& _pos,const Pint& _tpos,const Pint& _trelpos)const{return hmoCircle(getr()).lIn(_pos+getAdj(),_tpos,_trelpos);}
	virtual bool bIn(const Pint& _pos,const Pint& _tpos,const Pint& _tsize)const{return hmoCircle(getr()).bIn(_pos+getAdj(),_tpos,_tsize);}
	virtual bool thisIn(const Pint& _pos,const hmObject& _tobj,const Pint& _tpos)const{return hmoCircle(getr()).thisIn(_pos+getAdj(),_tobj,_tpos);}
	virtual Pint pDir(const Pint& _pos,const Pint& _tpos)const{return hmoCircle(getr()).pDir(_pos+getAdj(),_tpos);}
	virtual Pint lDir(const Pint& _pos,const Pint& _tpos,const Pint& _trelpos)const{return hmoCircle(getr()).lDir(_pos+getAdj(),_tpos,_trelpos);}
	virtual Pint bDir(const Pint& _pos,const Pint& _tpos,const Pint& _tsize)const{return hmoCircle(getr()).bDir(_pos+getAdj(),_tpos,_tsize);}
	virtual Pint oDir(const Pint& _pos,const hmDirObject& _tobj,const Pint& _tpos)const{return hmoCircle(getr()).oDir(_pos+getAdj(),_tobj,_tpos);}
};
class dxoStdInpBox: public virtual dxoStrP{
private:
	std::string InpName;
	bool activeFlag;
	std::string prestr;
public:
	dxRGB InpColor,IMEColor;
	bool RemainStrFlag;
	bool ClickEndFlag;
	bool ESCCancelFlag;
	bool HalfOnlyFlag;
	bool NumOnlyFlag;
private:
	void local_set(const std::string& _InpName,const dxRGB& _InpColor,dxRGB _IMEColor,
		bool _RemainStrFlag,bool _ClickEndFlag,bool _ESCCancelFlag,bool _HalfOnlyFlag,bool _NumOnlyFlag){
			InpName=_InpName;
			InpColor=_InpColor;
			IMEColor=_IMEColor;
			RemainStrFlag=_RemainStrFlag;
			ClickEndFlag=_ClickEndFlag;
			ESCCancelFlag=_ESCCancelFlag;
			HalfOnlyFlag=_HalfOnlyFlag;
			NumOnlyFlag=_NumOnlyFlag;
			activeFlag=false;
			prestr="";
	}
	void local_copy(const dxoStdInpBox& _dxo){
			InpName=_dxo.InpName;
			InpColor=_dxo.InpColor;
			IMEColor=_dxo.IMEColor;
			RemainStrFlag=_dxo.RemainStrFlag;
			ClickEndFlag=_dxo.ClickEndFlag;
			ESCCancelFlag=_dxo.ESCCancelFlag;
			HalfOnlyFlag=_dxo.HalfOnlyFlag;
			NumOnlyFlag=_dxo.NumOnlyFlag;
			activeFlag=_dxo.activeFlag;
			prestr=_dxo.prestr;
	}
protected:
	void copy(const dxoStdInpBox& _dxo){
		dxoStrP::copy(_dxo);
		local_copy(_dxo);
	}
public:
	dxoStdInpBox():dxoStrP(){local_set("",dxRGB(255,255,255),dxRGB(0x00,0x00,0x8b),false,false,false,false,false);}
	dxoStdInpBox(const Pint& _size,const std::string& _InpName,const dxRGB& _InpColor,const dxRGB& _SColor=dxRGB(255,255,255),
		dxRGB _IMEColor=dxRGB(0x00,0x00,0x8b),bool _RemainStrFlag=false,bool _ClickEndFlag=false,bool _ESCCancelFlag=false,
		bool _HalfOnlyFlag=false,bool _NumOnlyFlag=false,ALI::ALIGN _Ali=ALI::left,dxRGB _SEColor=dxRGB(128,128,128))
		:dxoStrP(_size,"",_SColor,_Ali,_SEColor){
			local_set(_InpName,_InpColor,_IMEColor,_RemainStrFlag,_ClickEndFlag,_ESCCancelFlag,_HalfOnlyFlag,_NumOnlyFlag);
	}
	dxoStdInpBox(const Pint& _size,const std::string& _InpName,const dxRGB& _InpColor,const dxFont& _font,const dxRGB& _SColor=dxRGB(255,255,255),
		dxRGB _IMEColor=dxRGB(0x00,0x00,0x8b),bool _RemainStrFlag=false,bool _ClickEndFlag=false,bool _ESCCancelFlag=false,
		bool _HalfOnlyFlag=false,bool _NumOnlyFlag=false,ALI::ALIGN _Ali=ALI::left,dxRGB _SEColor=dxRGB(128,128,128))
		:dxoStrP(_size,"",_font,_SColor,_Ali,_SEColor){
			local_set(_InpName,_InpColor,_IMEColor,_RemainStrFlag,_ClickEndFlag,_ESCCancelFlag,_HalfOnlyFlag,_NumOnlyFlag);
	}
	dxoStdInpBox(const dxoStdInpBox& _dxo):dxoStrP(_dxo){local_copy(_dxo);}
	dxoStdInpBox& operator=(const dxoStdInpBox& _dxo){
		if(this!=&_dxo)copy(_dxo);
		return *this;
	}
	void set(const Pint& _size,const std::string& _InpName,const dxRGB& _InpColor,const dxRGB& _SColor,
		dxRGB _IMEColor=dxRGB(0x00,0x00,0x8b),bool _RemainStrFlag=false,bool _ClickEndFlag=false,bool _ESCCancelFlag=false,
		bool _HalfOnlyFlag=false,bool _NumOnlyFlag=false,ALI::ALIGN _Ali=ALI::left,dxRGB _SEColor=dxRGB(128,128,128)){
			dxoStrP::set(_size,"",_SColor,_Ali,_SEColor);
			local_set(_InpName,_InpColor,_IMEColor,_RemainStrFlag,_ClickEndFlag,_ESCCancelFlag,_HalfOnlyFlag,_NumOnlyFlag);
	}
	void set(const Pint& _size,const std::string& _InpName,const dxRGB& _InpColor,const dxFont& _font,const dxRGB& _SColor=dxRGB(255,255,255),
		dxRGB _IMEColor=dxRGB(0x00,0x00,0x8b),bool _RemainStrFlag=false,bool _ClickEndFlag=false,bool _ESCCancelFlag=false,
		bool _HalfOnlyFlag=false,bool _NumOnlyFlag=false,ALI::ALIGN _Ali=ALI::left,dxRGB _SEColor=dxRGB(128,128,128)){
			dxoStrP::set(_size,"",_font,_SColor,_Ali,_SEColor);
			local_set(_InpName,_InpColor,_IMEColor,_RemainStrFlag,_ClickEndFlag,_ESCCancelFlag,_HalfOnlyFlag,_NumOnlyFlag);
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
//		if(!this->pIn(_pos,dx::getMosPos())){
//			if(!dx::checkStdInp(InpName) || dx::checkStdInpSta()!=1)_mode+=dxDMode(196);
//		}
		Pint adjpos;
		if(VFlag)adjpos=PintY(5);
		else adjpos=PintX(5);

		//自身が選択されている
		if(dx::checkStdInp(InpName) && dx::checkStdInpSta()==1){
			dx::drawBox(_pos+Pint(1,1),_pos+size-Pint(1,1),InpColor,1,_mode+dxDMode(dxRGB(128,128,128)));
			dx::drawBox(_pos,_pos+size,InpColor,0,_mode);
			dx::drawInpStrP(_pos+adjpos,size-adjpos,dx::getStdInp(InpName),SColor,IMEColor,Ali,_mode,VFlag,FontHandle,FontSize,SEColor);

			str=dx::getStdInp(InpName).getStr();

			if(ClickEndFlag && dx::getKey(KEY::MOSL) && !this->pIn(_pos,dx::getMosPos())){
				dx::getStdInp(InpName).inactive();
				activeFlag=false;
				return 2;
			}

			activeFlag=true;
			return 1;
		}//他のinputが選択されている
		else{
			if(!this->pIn(_pos,dx::getMosPos()))_mode+=dxDMode(196);
			if(dx::checkStdInp(InpName) && dx::checkStdInpSta()==3)str=prestr;

			dx::drawBox(_pos,_pos+size,InpColor,0,_mode);
			dx::drawBox(_pos+Pint(1,1),_pos+size-Pint(1,1),InpColor,0,_mode+dxDMode(dxRGB(128,128,128)));
			dx::drawBox(_pos+Pint(2,2),_pos+size-Pint(2,2),InpColor,1,_mode);

			dx::drawStrP(_pos+adjpos,size-adjpos,str,SColor,Ali,_mode,VFlag,FontHandle,FontSize,SEColor);

			if(this->pIn(_pos,dx::getMosPos()) && dx::getKey(KEY::MOSL)==1 && _active){
				dx::setStdInp(InpName);
				dx::getStdInp(InpName).make(1024,ESCCancelFlag,HalfOnlyFlag,NumOnlyFlag);
				if(RemainStrFlag)dx::getStdInp(InpName).setStr(str);
				dx::getStdInp(InpName).active();
				prestr=str;
				activeFlag=true;
				return 1;
			}

			//前回まで選択されていた
			if(activeFlag){
				activeFlag=false;
				//アクティブに一度なっていた
				if(dx::checkStdInp(InpName)){
					int ans=dx::checkStdInpSta();
					dx::getStdInp(InpName).inactive();
					return ans;
				//初期状態だった
				}else return 2;
			}else return 0;
		}
	}
};
class dxoInpBox: public virtual dxoStrP{
private:
	std::string prestr;
public:
	dxInput* inpp;
	dxRGB InpColor,IMEColor;
	bool RemainStrFlag;
	bool ClickEndFlag;
private:
	void local_set(dxInput* _inpp,const dxRGB& _InpColor,const dxRGB& _IMEColor,bool _RemainStrFlag,bool _ClickEndFlag){
			inpp=_inpp;
			InpColor=_InpColor;
			IMEColor=_IMEColor;
			RemainStrFlag=_RemainStrFlag;
			ClickEndFlag=_ClickEndFlag;
			prestr="";
	}
	void local_copy(const dxoInpBox& _dxo){
			inpp=_dxo.inpp;
			InpColor=_dxo.InpColor;
			IMEColor=_dxo.IMEColor;
			RemainStrFlag=_dxo.RemainStrFlag;
			ClickEndFlag=_dxo.ClickEndFlag;
			prestr=_dxo.prestr;
	}
protected:
	void copy(const dxoInpBox& _dxo){
		dxoStrP::copy(_dxo);
		local_copy(_dxo);
	}
public:
	dxoInpBox():dxoStrP(){local_set(0,dxRGB(255,255,255),dxRGB(0x00,0x00,0x8b),false,false);}
	dxoInpBox(const Pint& _size,dxInput& _inp,const dxRGB& _InpColor,const dxRGB& _SColor=dxRGB(255,255,255),
		dxRGB _IMEColor=dxRGB(0x00,0x00,0x8b),bool _RemainStrFlag=false,bool _ClickEndFlag=false,
		ALI::ALIGN _Ali=ALI::left,dxRGB _SEColor=dxRGB(128,128,128))
		:dxoStrP(_size,"",_SColor,_Ali,_SEColor){
			local_set(&_inp,_InpColor,_IMEColor,_RemainStrFlag,_ClickEndFlag);
	}
	dxoInpBox(const Pint& _size,dxInput& _inp,const dxRGB& _InpColor,const dxFont& _font,const dxRGB& _SColor=dxRGB(255,255,255),
		dxRGB _IMEColor=dxRGB(0x00,0x00,0x8b),bool _RemainStrFlag=false,bool _ClickEndFlag=false,
		ALI::ALIGN _Ali=ALI::left,dxRGB _SEColor=dxRGB(128,128,128))
		:dxoStrP(_size,"",_font,_SColor,_Ali,_SEColor){
			local_set(&_inp,_InpColor,_IMEColor,_RemainStrFlag,_ClickEndFlag);
	}
	dxoInpBox(const dxoInpBox& _dxo):dxoStrP(_dxo){local_copy(_dxo);}
	dxoInpBox& operator=(const dxoInpBox& _dxo){
		if(this!=&_dxo)copy(_dxo);
		return *this;
	}
	void set(const Pint& _size,dxInput& _inp,const dxRGB& _InpColor,const dxRGB& _SColor,
		dxRGB _IMEColor=dxRGB(0x00,0x00,0x8b),bool _RemainStrFlag=false,bool _ClickEndFlag=false,
		ALI::ALIGN _Ali=ALI::left,dxRGB _SEColor=dxRGB(128,128,128)){
			dxoStrP::set(_size,"",_SColor,_Ali,_SEColor);
			local_set(&_inp,_InpColor,_IMEColor,_RemainStrFlag,_ClickEndFlag);
	}
	void set(const Pint& _size,dxInput& _inp,const dxRGB& _InpColor,const dxFont& _font,const dxRGB& _SColor=dxRGB(255,255,255),
		dxRGB _IMEColor=dxRGB(0x00,0x00,0x8b),bool _RemainStrFlag=false,bool _ClickEndFlag=false,
		ALI::ALIGN _Ali=ALI::left,dxRGB _SEColor=dxRGB(128,128,128)){
			dxoStrP::set(_size,"",_font,_SColor,_Ali,_SEColor);
			local_set(&_inp,_InpColor,_IMEColor,_RemainStrFlag,_ClickEndFlag);
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
		Pint adjpos;
		if(VFlag)adjpos=PintY(2);
		else adjpos=PintX(2);

		if(inpp->check()==1){
			dx::drawBox(_pos+Pint(1,1),_pos+size-Pint(1,1),InpColor,1,_mode+dxDMode(dxRGB(128,128,128)));
			dx::drawBox(_pos,_pos+size,InpColor,0,_mode);
			dx::drawInpStrP(_pos+adjpos,size-adjpos,*inpp,SColor,IMEColor,Ali,_mode,VFlag,FontHandle,FontSize,SEColor);

			if(ClickEndFlag && dx::getKey(KEY::MOSL) && !this->pIn(_pos,dx::getMosPos()))inpp->inactive();
		}else{
			if(!this->pIn(_pos,dx::getMosPos()))_mode+=dxDMode(196);
			if(inpp->check()==3)str=prestr;

			dx::drawBox(_pos,_pos+size,InpColor,0,_mode);
			dx::drawBox(_pos+Pint(1,1),_pos+size-Pint(1,1),InpColor,0,_mode+dxDMode(dxRGB(128,128,128)));
			dx::drawBox(_pos+Pint(2,2),_pos+size-Pint(2,2),InpColor,1,_mode);

			dx::drawStrP(_pos+adjpos,size-adjpos,inpp->getStr(),SColor,Ali,_mode,VFlag,FontHandle,FontSize,SEColor);

			if(this->pIn(_pos,dx::getMosPos()) && dx::getKey(KEY::MOSL)==1 && _active){
				if(!RemainStrFlag)inpp->setStr("");
				prestr=str;
				inpp->active();
			}
		}

		str=inpp->getStr();
		return inpp->check();
	}
};
//class dxoSpin
//class dxoImGraph
class dxoProgress:public dxObject,public hmoBox{
public:
	dxRGB PrgColor;
	int data,datasize;
	bool PrgVFlag;
private:
	void local_set(const int& _data,const int& _datasize,const dxRGB& _PrgColor,bool _PrgVFlag){
		PrgColor=_PrgColor;
		data=_data;
		datasize=_datasize;
		PrgVFlag=_PrgVFlag;
	}
	void local_copy(const dxoProgress& _dxo){
		PrgColor=_dxo.PrgColor;
		data=_dxo.data;
		datasize=_dxo.datasize;
		PrgVFlag=_dxo.PrgVFlag;
	}
protected:
	void copy(const dxoProgress& _dxo){
		hmoBox::copy(_dxo);
		local_copy(_dxo);
	}
public:
	dxoProgress():hmoBox(){local_set(0,0,dxRGB(255,255,255),false);}
	dxoProgress(const Pint& _size,const int& _data,const int& _datasize,const dxRGB& _PrgColor,bool _PrgVFlag=false)
		:hmoBox(_size){
			local_set(_data,_datasize,_PrgColor,_PrgVFlag);
	}
	dxoProgress(const dxoProgress& _dxo):hmoBox(_dxo){local_copy(_dxo);}
	dxoProgress& operator=(const dxoProgress& _dxo){
		if(this!=&_dxo)copy(_dxo);
		return *this;
	}
	void set(const Pint& _size,const int& _data,const int& _datasize,const dxRGB& _PrgColor,bool _PrgVFlag=false){
		hmoBox::set(_size);
		local_set(_data,_datasize,_PrgColor,_PrgVFlag);
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
		if(!this->pIn(_pos,dx::getMosPos()) || !_active)_mode+=dxDMode(196);

		dx::drawBox(_pos,_pos+size,PrgColor,1,_mode+dxDMode(dxRGB(128,128,128)));
		dx::drawBox(_pos,_pos+size,PrgColor,0,_mode);

		if(datasize>0 && data>0){
			if(PrgVFlag){
				dx::drawBox(_pos+size,_pos+Pint(0,size.y-size.y*data/datasize),PrgColor,1,_mode);
			}else{
				dx::drawBox(_pos,_pos+Pint(size.x*data/datasize,size.y),PrgColor,1,_mode);
			}
		}

		if(this->pIn(_pos,dx::getMosLPos()) && dx::getMosL()==2 && _active){
			if(PrgVFlag)return max(min(datasize+(_pos.y-dx::getMosPos().y)*datasize/size.y,datasize),0);
			else return max(min((dx::getMosPos().x-_pos.x)*datasize/size.x,datasize),0);
		}else return data;
	}
};
//*/
#
#endif
