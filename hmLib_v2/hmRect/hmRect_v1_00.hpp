#ifndef INC_HMLIB_HMRECT
#define INC_HMLIB_HMRECT 100
#ifndef INC_HMLIB_HMPOS
#	include "hmLib_v2/hmPos.hpp"
#endif
template<class T>
class hmRect{
protected:
	void copy(const hmRect<T>& _rect){ 
		pos1=_rect.pos1;
		pos2=_rect.pos2;
	}
public:
	hmPoint<T> pos1,pos2;
	hmRect(){return;}
	hmRect(hmPoint<T> _pos1,hmPoint<T> _pos2){
		pos1=_pos1;
		pos2=_pos2;
	}
	hmRect(const hmRect<T>& _rect){
		copy(_rect);
	}
	void set(hmPoint<T> _pos1,hmPoint<T> _pos2){
		pos1=_pos1;
		pos2=_pos2;
	}
	bool operator==(const hmRect<T>& _rect)const{
		if(pos1!=_rect.pos1)return false;
		if(pos2!=_rect.pos2)return false;
		return true;
	}
	bool operator!=(const hmRect<T>& _rect)const{
		if(pos1!=_rect.pos1)return true;
		if(pos2!=_rect.pos2)return true;
		return false;
	}
	hmRect<T>& operator=(const hmRect<T>& _rect){
		if(this!=&_rect)copy(_rect);
		return *this;
	}
	hmRect<T>& operator+=(const hmRect<T>& _rect){
		pos1+=_rect.pos1;
		pos2+=_rect.pos2;
		return *this;
	}
	hmRect<T>& operator-=(const hmRect<T>& _rect){
		pos1-=_rect.pos1;
		pos2-=_rect.pos2;
		return *this;
	}
	hmRect<T>& operator+=(const hmPoint<T>& _pos){
		pos1+=_pos;
		pos2+=_pos;
		return *this;
	}
	hmRect<T>& operator-=(const hmPoint<T>& _pos){
		pos1-=_pos;
		pos2-=_pos;
		return *this;
	}
	hmRect<T>& operator&=(const hmRect<T>& _rect){
		pos1=hmPointMax(pos1,_rect.pos1);
		pos2=hmPointMin(pos2,_rect.pos2);
		return *this;
	}
	hmRect<T>& operator|=(const hmRect<T>& _rect){
		pos1=hmPointMin(pos1,_rect.pos1);
		pos2=hmPointMax(pos2,_rect.pos2);
		return *this;
	}
	hmRect<T> operator+(const hmRect<T>& _rect)const{
		hmRect<T> rect(*this);
		rect.pos1+=_rect.pos1;
		rect.pos2+=_rect.pos2;
		return rect;
	}
	hmRect<T> operator-(const hmRect<T>& _rect)const{
		hmRect<T> rect(*this);
		rect.pos1-=_rect.pos1;
		rect.pos2-=_rect.pos2;
		return rect;
	}
	hmRect<T> operator+(const hmPoint<T>& _pos)const{
		hmRect<T> rect(*this);
		rect.pos1+=_pos;
		rect.pos2+=_pos;
		return rect;
	}
	hmRect<T> operator-(const hmPoint<T>& _pos)const{
		hmRect<T> rect(*this);
		rect.pos1-=_pos;
		rect.pos2-=_pos;
		return rect;
	}
	hmRect<T> operator&(const hmRect<T>& _rect)const{
		hmRect<T> rect(*this);
		rect&=_rect;
		return rect;
	}
	hmRect<T> operator|(const hmRect<T>& _rect)const{
		hmRect<T> rect(*this);
		rect|=_rect;
		return rect;
	}
	T area()const{return area(pos1,pos2);}
	T inprd()const{return inprd(pos1,pos2);}
	bool include(const hmPoint<T>& _pos)const{
		if(pos1<=_pos && pos2>_pos)return true;
		else return false;
	}
	bool include(const hmRect<T>& _rect)const{
		if(pos1<=_rect.pos1 && pos2>_rect.pos2)return true;
		else return false;
	}
	bool cross(const hmRect<T>& _rect)const{
		if(pos1<=_rect.pos2 && pos2>=_rect.pos1)return true;
		else return false;
	}
};
typedef hmRect<int> Rint;
typedef hmRect<int> Rdouble;
#endif
