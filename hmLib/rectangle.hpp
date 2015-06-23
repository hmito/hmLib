#ifndef HMLIB_RECTANGLE_INC
#define HMLIB_RECTANGLE_INC 100
#ifndef HMLIB_POSITION_INC
#	include <hmLib/position.hpp>
#endif
namespace hmLib{
	template<class T>
	class di_rectangle{
		typedef di_position<T> pos_type;
	protected:
		void copy(const di_rectangle<T>& _rect){ 
			pos1=_rect.pos1;
			pos2=_rect.pos2;
		}
	public:
		pos_type pos1,pos2;
		di_rectangle(){return;}
		di_rectangle(pos_type _pos1,pos_type _pos2){
			pos1=_pos1;
			pos2=_pos2;
		}
		di_rectangle(const di_rectangle<T>& _rect){
			copy(_rect);
		}
		void set(pos_type _pos1,pos_type _pos2){
			pos1=_pos1;
			pos2=_pos2;
		}
		bool operator==(const di_rectangle<T>& _rect)const{
			if(pos1!=_rect.pos1)return false;
			if(pos2!=_rect.pos2)return false;
			return true;
		}
		bool operator!=(const di_rectangle<T>& _rect)const{
			if(pos1!=_rect.pos1)return true;
			if(pos2!=_rect.pos2)return true;
			return false;
		}
		di_rectangle<T>& operator=(const di_rectangle<T>& _rect){
			if(this!=&_rect)copy(_rect);
			return *this;
		}
		di_rectangle<T>& operator+=(const di_rectangle<T>& _rect){
			pos1+=_rect.pos1;
			pos2+=_rect.pos2;
			return *this;
		}
		di_rectangle<T>& operator-=(const di_rectangle<T>& _rect){
			pos1-=_rect.pos1;
			pos2-=_rect.pos2;
			return *this;
		}
		di_rectangle<T>& operator+=(const pos_type& _pos){
			pos1+=_pos;
			pos2+=_pos;
			return *this;
		}
		di_rectangle<T>& operator-=(const pos_type& _pos){
			pos1-=_pos;
			pos2-=_pos;
			return *this;
		}
		di_rectangle<T>& operator&=(const di_rectangle<T>& _rect){
			pos1=di_positionMax(pos1,_rect.pos1);
			pos2=di_positionMin(pos2,_rect.pos2);
			return *this;
		}
		di_rectangle<T>& operator|=(const di_rectangle<T>& _rect){
			pos1=di_positionMin(pos1,_rect.pos1);
			pos2=di_positionMax(pos2,_rect.pos2);
			return *this;
		}
		di_rectangle<T> operator+(const di_rectangle<T>& _rect)const{
			di_rectangle<T> rect(*this);
			rect.pos1+=_rect.pos1;
			rect.pos2+=_rect.pos2;
			return rect;
		}
		di_rectangle<T> operator-(const di_rectangle<T>& _rect)const{
			di_rectangle<T> rect(*this);
			rect.pos1-=_rect.pos1;
			rect.pos2-=_rect.pos2;
			return rect;
		}
		di_rectangle<T> operator+(const pos_type& _pos)const{
			di_rectangle<T> rect(*this);
			rect.pos1+=_pos;
			rect.pos2+=_pos;
			return rect;
		}
		di_rectangle<T> operator-(const pos_type& _pos)const{
			di_rectangle<T> rect(*this);
			rect.pos1-=_pos;
			rect.pos2-=_pos;
			return rect;
		}
		di_rectangle<T> operator&(const di_rectangle<T>& _rect)const{
			di_rectangle<T> rect(*this);
			rect&=_rect;
			return rect;
		}
		di_rectangle<T> operator|(const di_rectangle<T>& _rect)const{
			di_rectangle<T> rect(*this);
			rect|=_rect;
			return rect;
		}
		T area()const{return area(pos1,pos2);}
		T inprd()const{return inprd(pos1,pos2);}
		bool check()const{return !(pos1<pos2);}
		bool include(const pos_type& _pos)const{
			if(pos1<=_pos && pos2>_pos)return true;
			else return false;
		}
		bool include(const di_rectangle<T>& _rect)const{
			if(pos1<=_rect.pos1 && pos2>_rect.pos2)return true;
			else return false;
		}
		bool cross(const di_rectangle<T>& _rect)const{
			if(pos1<=_rect.pos2 && pos2>=_rect.pos1)return true;
			else return false;
		}
	};
	typedef di_rectangle<int> Rint;
	typedef di_rectangle<double> Rdouble;
}
#endif
