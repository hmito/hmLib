#ifndef HMLIB_POSITION_INC
#define HMLIB_POSITION_INC 100
#include <cmath>
#include <iostream>
namespace hmLib{
	template<class T>
	struct di_position{
	public:
		T x;
		T y;
		di_position<T>(){
			x=0;
			y=0;
		}
		di_position<T>(T _x,T _y){
			x=_x;
			y=_y;
		}
		di_position<T>(const di_position<T>& _T){
			x=_T.x;
			y=_T.y;
		}
		template<class S>
		di_position<T>(S _x,S _y){
			x=(T)_x;
			y=(T)_y;
		}
		template<class S>
		di_position<T>(const di_position<S>& _S){
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
		bool operator!=(const di_position<T>& _T)const{
			if(x!=_T.x || y!=_T.y)return true;
			else return false;
		}
		bool operator==(const di_position<T>& _T)const{
			if(x!=_T.x || y!=_T.y)return false;
			else return true;
		}
		bool operator<(const di_position<T>& _T)const{
			if(x>=_T.x || y>=_T.y)return false;
			else return true;
		}
		bool operator>(const di_position<T>& _T)const{
			if(x<=_T.x || y<=_T.y)return false;
			else return true;
		}
		bool operator<=(const di_position<T>& _T)const{
			if(x>_T.x || y>_T.y)return false;
			else return true;
		}
		bool operator>=(const di_position<T>& _T)const{
			if(x<_T.x || y<_T.y)return false;
			else return true;
		}
		const di_position<T> operator=(const di_position<T>& _T){
			if(this!=&_T){
				x=_T.x;
				y=_T.y;
			}
			return *this;
		}
		template<class S>
		const di_position<T> operator=(const di_position<S>& _S){
			x=(T)_S.x;
			y=(T)_S.y;
			return *this;
		}
		const di_position<T> operator+=(const di_position<T>& _T){
			x+=_T.x;
			y+=_T.y;
			return *this;
		}
		template<class S>
		const di_position<T> operator+=(const di_position<S>& _S){
			x+=(T)_S.x;
			y+=(T)_S.y;
			return *this;
		}
		const di_position<T> operator-=(const di_position<T>& _T){
			x-=_T.x;
			y-=_T.y;
			return *this;
		}
		template<class S>
		const di_position<T> operator-=(const di_position<S>& _S){
			x-=(T)_S.x;
			y-=(T)_S.y;
			return *this;
		}
		di_position<T> operator+()const{return *this;}
		di_position<T> operator-()const{
			di_position<T> ans(*this);
			ans.x*=-1;
			ans.y*=-1;
			return ans;
		}
		di_position<T> operator+(const di_position<T>& _T)const{
			di_position<T> ans(*this);
			ans+=_T;
			return ans;
		}
		di_position<T> operator-(const di_position<T>& _T)const{
			di_position<T> ans(*this);
			ans-=_T;
			return ans;
		}
		di_position<T> operator*(const di_position<T>& _T)const{
			di_position<T> ans(*this);
			ans.x*=_T.x;
			ans.y*=_T.y;
			return ans;
		}
		di_position<T> operator/(const di_position<T>& _T)const{
			di_position<T> ans(*this);
			if(_T.x==0 || _T.y==0)throw "di_position<T>::operator/(const di_position<T>& _T)const  div with 0"; 
			ans.x/=_T.x;
			ans.y/=_T.y;
			return ans;
		}
		template<class S>
		operator S()const{
			di_position<S> ans(*this);
			return ans;
		}
		//friend関数群
	//	friend bool operator==(const di_position<T>& _T1,const di_position<T>& _T2);
		friend di_position<T> operator*(const di_position<T>& _T,const T& p){
			di_position<T> ans(_T);
			ans.x*=p;
			ans.y*=p;
			return ans;
		}
		friend di_position<T> operator*(const T& p,const di_position<T>& _T){
			di_position<T> ans(_T);
			ans.x*=p;
			ans.y*=p;
			return ans;
		}
		friend di_position<T> operator/(const di_position<T>& _T,const T& p){
			di_position<T> ans(_T);
			ans.x/=p;
			ans.y/=p;
			return ans;
		}
		template<class S,class U>
		friend di_position<U> operator+(const di_position<T>& _T,const di_position<S>& _S);
		template<class S,class U>
		friend di_position<U> operator-(const di_position<T>& _T,const di_position<S>& _S);
		template<class S,class U>
		friend di_position<U> operator*(const di_position<T>& _T,const di_position<S>& _S);
		template<class S,class U>
		friend di_position<U> operator/(const di_position<T>& _T,const di_position<S>& _S);
		template<class S,class U>
		friend di_position<U> operator*(const di_position<T>& _T,const S& _S);
		template<class S,class U>
		friend di_position<U> operator/(const di_position<T>& _T,const S& _S);
		template<class S,class U>
		friend di_position<U> operator*(const S& _S,const di_position<T>& _T);
		friend std::ostream& operator<<(std::ostream& out,const di_position<T>& pos){
			out<<'('<<pos.x<<','<<pos.y<<')';
			return out;
		}
	};
	typedef di_position<int> pint;
	typedef di_position<double> pdouble;
	inline pdouble operator+(const pint& i,const pdouble& d){return pdouble(i)+d;}
	inline pdouble operator-(const pint& i,const pdouble& d){return pdouble(i)-d;}
	inline pdouble operator*(const pint& i,const pdouble& d){return pdouble(i)*d;}
	inline pdouble operator/(const pint& i,const pdouble& d){return pdouble(i)/d;}
	inline pdouble operator*(const pint& i,const double& d){return pdouble(i)*d;}
	inline pdouble operator/(const pint& i,const double& d){return pdouble(i)/d;}
	inline pdouble operator*(const double& d,const pint& i){return pdouble(i)*d;}
	inline pdouble operator+(const pdouble& d,const pint& i){return d+pdouble(i);}
	inline pdouble operator-(const pdouble& d,const pint& i){return d-pdouble(i);}
	inline pdouble operator*(const pdouble& d,const pint& i){return d*pdouble(i);}
	inline pdouble operator/(const pdouble& d,const pint& i){return d/pdouble(i);}
	inline pdouble operator*(const pdouble& d,const int& i){return d*double(i);}
	inline pdouble operator/(const pdouble& d,const int& i){return d/double(i);}
	inline pdouble operator*(const int& i,const pdouble& d){return d*double(i);}
	//di_position関連関数群
	template<class T>
	inline di_position<T> di_positionX(const di_position<T>& p){return di_position<T>(p.x,0);}
	template<class T>
	inline di_position<T> di_positionY(const di_position<T>& p){return di_position<T>(0,p.y);}
	template<class T>
	inline di_position<T> di_positionX(const T& p){return di_position<T>(p,0);}
	template<class T>
	inline di_position<T> di_positionY(const T& p){return di_position<T>(0,p);}
	template<class T>
	inline di_position<T> di_positionMin(const di_position<T>& p1,const di_position<T>& p2){
		di_position<T> tmp=p1;
		if(tmp.x>p2.x)tmp.x=p2.x;
		if(tmp.y>p2.y)tmp.y=p2.y;
		return tmp;
	}
	template<class T>
	inline di_position<T> di_positionMax(const di_position<T>& p1,const di_position<T>& p2){
		di_position<T> tmp=p1;
		if(tmp.x<p2.x)tmp.x=p2.x;
		if(tmp.y<p2.y)tmp.y=p2.y;
		return tmp;
	}
	template<class T>
	inline di_position<T> di_positionMinMax(const di_position<T>& p1,const di_position<T>& p2){
		di_position<T> tmp=p1;
		if(tmp.x>p2.x)tmp.x=p2.x;
		if(tmp.y<p2.y)tmp.y=p2.y;
		return tmp;
	}
	template<class T>
	inline di_position<T> di_positionMaxMin(const di_position<T>& p1,const di_position<T>& p2){
		di_position<T> tmp=p1;
		if(tmp.x<p2.x)tmp.x=p2.x;
		if(tmp.y>p2.y)tmp.y=p2.y;
		return tmp;
	}
	inline pint pintX(const pint& p){return pint(p.x,0);}
	inline pint pintY(const pint& p){return pint(0,p.y);}
	inline pint pintX(const int& p){return pint(p,0);}
	inline pint pintY(const int& p){return pint(0,p);}
	inline pdouble pdoubleX(const pdouble& p){return pdouble(p.x,0.);}
	inline pdouble pdoubleY(const pdouble& p){return pdouble(0.,p.y);}
	inline pdouble pdoubleX(const double& p){return pdouble(p,0.);}
	inline pdouble pdoubleY(const double& p){return pdouble(0.,p);}
	inline pint pintMin(const pint& p1,const pint& p2){
		pint tmp=p1;
		if(tmp.x>p2.x)tmp.x=p2.x;
		if(tmp.y>p2.y)tmp.y=p2.y;
		return tmp;
	}
	inline pint pintMax(const pint& p1,const pint& p2){
		pint tmp=p1;
		if(tmp.x<p2.x)tmp.x=p2.x;
		if(tmp.y<p2.y)tmp.y=p2.y;
		return tmp;
	}
	inline pint pintMinMax(const pint& p1,const pint& p2){
		pint tmp=p1;
		if(tmp.x>p2.x)tmp.x=p2.x;
		if(tmp.y<p2.y)tmp.y=p2.y;
		return tmp;
	}
	inline pint pintMaxMin(const pint& p1,const pint& p2){
		pint tmp=p1;
		if(tmp.x<p2.x)tmp.x=p2.x;
		if(tmp.y>p2.y)tmp.y=p2.y;
		return tmp;
	}
	inline pdouble pdoubleMin(const pdouble& p1,const pdouble& p2){
		pdouble tmp=p1;
		if(tmp.x>p2.x)tmp.x=p2.x;
		if(tmp.y>p2.y)tmp.y=p2.y;
		return tmp;
	}
	inline pdouble pdoubleMax(const pdouble& p1,const pdouble& p2){
		pdouble tmp=p1;
		if(tmp.x<p2.x)tmp.x=p2.x;
		if(tmp.y<p2.y)tmp.y=p2.y;
		return tmp;
	}
	inline pdouble pdoubleMinMax(const pdouble& p1,const pdouble& p2){
		pdouble tmp=p1;
		if(tmp.x>p2.x)tmp.x=p2.x;
		if(tmp.y<p2.y)tmp.y=p2.y;
		return tmp;
	}
	inline pdouble pdoubleMaxMin(const pdouble& p1,const pdouble& p2){
		pdouble tmp=p1;
		if(tmp.x<p2.x)tmp.x=p2.x;
		if(tmp.y>p2.y)tmp.y=p2.y;
		return tmp;
	}
	template<class T>
	inline T abs2(const di_position<T>& p){return p.x*p.x+p.y*p.y;}
	template<class T>
	inline double abs(const di_position<T>& p){return sqrt(static_cast<double>(p.x*p.x+p.y*p.y));}
	template<class T>
	inline di_position<T> ortho(const di_position<T>& p){return di_position<T>(p.y,-p.x);}
	template<class T>
	inline di_position<T> rot(const di_position<T>& p,const double& angle){return di_position<T>(p.x*cos(angle)-p.y*sin(angle),p.x*sin(angle)+p.y*cos(angle));}
	template<class T>
	inline T inprd(const di_position<T>& p1,const di_position<T>& p2){return p1.x*p2.x+p1.y*p2.y;}
	template<class T>
	inline T area(const di_position<T>& p1,const di_position<T>& p2){return p1.x*p2.y-p2.x*p1.y;}
}
#endif
