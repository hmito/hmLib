#ifndef INC_HMPOS
#define INC_HMPOS 301
#include <cmath>
#ifndef INC_HMBASIC
#	include "hmBasic.hpp"
#endif
template<class T>
class hmPoint{
public:
	T x;
	T y;
	hmPoint<T>(){
		x=0;
		y=0;
	}
	hmPoint<T>(T _x,T _y){
		x=_x;
		y=_y;
	}
	hmPoint<T>(const hmPoint<T>& _T){
		x=_T.x;
		y=_T.y;
	}
	template<class S>
	hmPoint<T>(S _x,S _y){
		x=(T)_x;
		y=(T)_y;
	}
	template<class S>
	hmPoint<T>(const hmPoint<S>& _S){
		x=(T)_S.x;
		y=(T)_S.y;
	}
	void set(T _x,T _y){
		x=_x;
		y=_y;
	}
//	void Set(T _x,T _y){set(_x,_y);}
	template<class S>
	void set(S _x,S _y){
		x=(T)_x;
		y=(T)_y;
	}
//	template<class S>
//	void Set(S _x,S _y){set(_x,_y);}
	bool operator!=(const hmPoint<T>& _T)const{
		if(x!=_T.x || y!=_T.y)return true;
		else return false;
	}
	bool operator==(const hmPoint<T>& _T)const{
		if(x!=_T.x || y!=_T.y)return false;
		else return true;
	}
	bool operator<(const hmPoint<T>& _T)const{
		if(x>=_T.x || y>=_T.y)return false;
		else return true;
	}
	bool operator>(const hmPoint<T>& _T)const{
		if(x<=_T.x || y<=_T.y)return false;
		else return true;
	}
	bool operator<=(const hmPoint<T>& _T)const{
		if(x>_T.x || y>_T.y)return false;
		else return true;
	}
	bool operator>=(const hmPoint<T>& _T)const{
		if(x<_T.x || y<_T.y)return false;
		else return true;
	}
	const hmPoint<T> operator=(const hmPoint<T>& _T){
		if(this!=&_T){
			x=_T.x;
			y=_T.y;
		}
		return *this;
	}
	template<class S>
	const hmPoint<T> operator=(const hmPoint<S>& _S){
		x=(T)_S.x;
		y=(T)_S.y;
		return *this;
	}

	const hmPoint<T> operator+=(const hmPoint<T>& _T){
		x+=_T.x;
		y+=_T.y;
		return *this;
	}
	template<class S>
	const hmPoint<T> operator+=(const hmPoint<S>& _S){
		x+=(T)_S.x;
		y+=(T)_S.y;
		return *this;
	}
	const hmPoint<T> operator-=(const hmPoint<T>& _T){
		x-=_T.x;
		y-=_T.y;
		return *this;
	}
	template<class S>
	const hmPoint<T> operator-=(const hmPoint<S>& _S){
		x-=(T)_S.x;
		y-=(T)_S.y;
		return *this;
	}
	hmPoint<T> operator+()const{return *this;}
	hmPoint<T> operator-()const{
		hmPoint<T> ans(*this);
		ans.x*=-1;
		ans.y*=-1;
		return ans;
	}
	hmPoint<T> operator+(const hmPoint<T>& _T)const{
		hmPoint<T> ans(*this);
		ans+=_T;
		return ans;
	}
	hmPoint<T> operator-(const hmPoint<T>& _T)const{
		hmPoint<T> ans(*this);
		ans-=_T;
		return ans;
	}
	hmPoint<T> operator*(const hmPoint<T>& _T)const{
		hmPoint<T> ans(*this);
		ans.x*=_T.x;
		ans.y*=_T.y;
		return ans;
	}
	hmPoint<T> operator/(const hmPoint<T>& _T)const{
		hmPoint<T> ans(*this);
		if(_T.x==0 || _T.y==0)throw "hmPoint<T>::operator/(const hmPoint<T>& _T)const  div with 0"; 
		ans.x/=_T.x;
		ans.y/=_T.y;
		return ans;
	}
	template<class S>
	operator S()const{
		hmPoint<S> ans(*this);
		return ans;
	}
	//friendä÷êîåQ
//	friend bool operator==(const hmPoint<T>& _T1,const hmPoint<T>& _T2);
	template<class T>
	friend inline hmPoint<T> operator*(const hmPoint<T>& _T,const T& p);
	template<class T>
	friend inline hmPoint<T> operator/(const hmPoint<T>& _T,const T& p);
	template<class T>
	friend inline hmPoint<T> operator*(const T& p,const hmPoint<T>& _T);
	template<class T,class S,class U>
	friend hmPoint<U> operator+(const hmPoint<T>& _T,const hmPoint<S>& _S);
	template<class T,class S,class U>
	friend hmPoint<U> operator-(const hmPoint<T>& _T,const hmPoint<S>& _S);
	template<class T,class S,class U>
	friend hmPoint<U> operator*(const hmPoint<T>& _T,const hmPoint<S>& _S);
	template<class T,class S,class U>
	friend hmPoint<U> operator/(const hmPoint<T>& _T,const hmPoint<S>& _S);
	template<class T,class S,class U>
	friend hmPoint<U> operator*(const hmPoint<T>& _T,const S& _S);
	template<class T,class S,class U>
	friend hmPoint<U> operator/(const hmPoint<T>& _T,const S& _S);
	template<class T,class S,class U>
	friend hmPoint<U> operator*(const S& _S,const hmPoint<T>& _T);
};
template<class T>
inline hmPoint<T> operator*(const hmPoint<T>& _T,const T& p){
	hmPoint<T> ans(_T);
	ans.x*=p;
	ans.y*=p;
	return ans;
}
template<class T>
inline hmPoint<T> operator*(const T& p,const hmPoint<T>& _T){
	hmPoint<T> ans(_T);
	ans.x*=p;
	ans.y*=p;
	return ans;
}
template<class T>
inline hmPoint<T> operator/(const hmPoint<T>& _T,const T& p){
	hmPoint<T> ans(_T);
	ans.x/=p;
	ans.y/=p;
	return ans;
}
typedef hmPoint<int> Pint;
typedef hmPoint<double> Pdouble;
inline Pdouble operator+(const Pint& i,const Pdouble& d){return Pdouble(i)+d;}
inline Pdouble operator-(const Pint& i,const Pdouble& d){return Pdouble(i)-d;}
inline Pdouble operator*(const Pint& i,const Pdouble& d){return Pdouble(i)*d;}
inline Pdouble operator/(const Pint& i,const Pdouble& d){return Pdouble(i)/d;}
inline Pdouble operator*(const Pint& i,const double& d){return Pdouble(i)*d;}
inline Pdouble operator/(const Pint& i,const double& d){return Pdouble(i)/d;}
inline Pdouble operator*(const double& d,const Pint& i){return Pdouble(i)*d;}
inline Pdouble operator+(const Pdouble& d,const Pint& i){return d+Pdouble(i);}
inline Pdouble operator-(const Pdouble& d,const Pint& i){return d-Pdouble(i);}
inline Pdouble operator*(const Pdouble& d,const Pint& i){return d*Pdouble(i);}
inline Pdouble operator/(const Pdouble& d,const Pint& i){return d/Pdouble(i);}
inline Pdouble operator*(const Pdouble& d,const int& i){return d*double(i);}
inline Pdouble operator/(const Pdouble& d,const int& i){return d/double(i);}
inline Pdouble operator*(const int& i,const Pdouble& d){return d*double(i);}
//Posä÷òAä÷êîåQ
template<class T>
inline hmPoint<T> hmPointX(const hmPoint<T>& p){return hmPoint<T>(p.x,0);}
template<class T>
inline hmPoint<T> hmPointY(const hmPoint<T>& p){return hmPoint<T>(0,p.y);}
template<class T>
inline hmPoint<T> hmPointX(const T& p){return hmPoint<T>(p,0);}
template<class T>
inline hmPoint<T> hmPointY(const T& p){return hmPoint<T>(0,p);}
inline Pint PintX(const Pint& p){return Pint(p.x,0);}
inline Pint PintY(const Pint& p){return Pint(0,p.y);}
inline Pint PintX(const int& p){return Pint(p,0);}
inline Pint PintY(const int& p){return Pint(0,p);}
inline Pdouble PdoubleX(const Pdouble& p){return Pdouble(p.x,0.);}
inline Pdouble PdoubleY(const Pdouble& p){return Pdouble(0.,p.y);}
inline Pdouble PdoubleX(const double& p){return Pdouble(p,0.);}
inline Pdouble PdoubleY(const double& p){return Pdouble(0.,p);}
template<class T>
inline T abs2(const hmPoint<T>& p){return p.x*p.x+p.y*p.y;}
template<class T>
inline double abs(const hmPoint<T>& p){return sqrt(static_cast<double>(p.x*p.x+p.y*p.y));}
template<class T>
inline hmPoint<T> ortho(const hmPoint<T>& p){return hmPoint<T>(p.y,-p.x);}
template<class T>
inline hmPoint<T> rot(const hmPoint<T>& p,const double& angle){return hmPoint<T>(p.x*cos(angle)-p.y*sin(angle),p.x*sin(angle)+p.y*cos(angle));}
template<class T>
inline T inprd(const hmPoint<T>& p1,const hmPoint<T>& p2){return p1.x*p2.x+p1.y*p2.y;}
template<class T>
inline T area(const hmPoint<T>& p1,const hmPoint<T>& p2){return p1.x*p2.y-p2.x*p1.y;}
template<class T>
inline double getAngle(const hmPoint<T>& relpos,const hmPoint<T>& stdangle){return hm::sel(relpos.y>0,1.,-1.)*acos(static_cast<double>(inprd(relpos,stdangle))/abs(relpos)/abs(stdangle));}
template<class T>
inline hmPoint<T> hmPointMin(const hmPoint<T>& p1,const hmPoint<T>& p2){
	hmPoint<T> tmp=p1;
	if(tmp.x>p2.x)tmp.x=p2.x;
	if(tmp.y>p2.y)tmp.y=p2.y;
	return tmp;
}
template<class T>
inline hmPoint<T> hmPointMax(const hmPoint<T>& p1,const hmPoint<T>& p2){
	hmPoint<T> tmp=p1;
	if(tmp.x<p2.x)tmp.x=p2.x;
	if(tmp.y<p2.y)tmp.y=p2.y;
	return tmp;
}
template<class T>
inline hmPoint<T> hmPointMinMax(const hmPoint<T>& p1,const hmPoint<T>& p2){
	hmPoint<T> tmp=p1;
	if(tmp.x>p2.x)tmp.x=p2.x;
	if(tmp.y<p2.y)tmp.y=p2.y;
	return tmp;
}
template<class T>
inline hmPoint<T> hmPointMaxMin(const hmPoint<T>& p1,const hmPoint<T>& p2){
	hmPoint<T> tmp=p1;
	if(tmp.x<p2.x)tmp.x=p2.x;
	if(tmp.y>p2.y)tmp.y=p2.y;
	return tmp;
}
inline Pint PintMin(const Pint& p1,const Pint& p2){
	Pint tmp=p1;
	if(tmp.x>p2.x)tmp.x=p2.x;
	if(tmp.y>p2.y)tmp.y=p2.y;
	return tmp;
}
inline Pint PintMax(const Pint& p1,const Pint& p2){
	Pint tmp=p1;
	if(tmp.x<p2.x)tmp.x=p2.x;
	if(tmp.y<p2.y)tmp.y=p2.y;
	return tmp;
}
inline Pint PintMinMax(const Pint& p1,const Pint& p2){
	Pint tmp=p1;
	if(tmp.x>p2.x)tmp.x=p2.x;
	if(tmp.y<p2.y)tmp.y=p2.y;
	return tmp;
}
inline Pint PintMaxMin(const Pint& p1,const Pint& p2){
	Pint tmp=p1;
	if(tmp.x<p2.x)tmp.x=p2.x;
	if(tmp.y>p2.y)tmp.y=p2.y;
	return tmp;
}
inline Pdouble PdoubleMin(const Pdouble& p1,const Pdouble& p2){
	Pdouble tmp=p1;
	if(tmp.x>p2.x)tmp.x=p2.x;
	if(tmp.y>p2.y)tmp.y=p2.y;
	return tmp;
}
inline Pdouble PdoubleMax(const Pdouble& p1,const Pdouble& p2){
	Pdouble tmp=p1;
	if(tmp.x<p2.x)tmp.x=p2.x;
	if(tmp.y<p2.y)tmp.y=p2.y;
	return tmp;
}
inline Pdouble PdoubleMinMax(const Pdouble& p1,const Pdouble& p2){
	Pdouble tmp=p1;
	if(tmp.x>p2.x)tmp.x=p2.x;
	if(tmp.y<p2.y)tmp.y=p2.y;
	return tmp;
}
inline Pdouble PdoubleMaxMin(const Pdouble& p1,const Pdouble& p2){
	Pdouble tmp=p1;
	if(tmp.x<p2.x)tmp.x=p2.x;
	if(tmp.y>p2.y)tmp.y=p2.y;
	return tmp;
}
#endif
