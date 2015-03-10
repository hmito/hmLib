#ifndef INC_HMLIB_HMALIGN
#define INC_HMLIB_HMALIGN 101
#ifndef INC_HMLIB_HMPOS
#	include "hmLib_v2/hmPos.hpp"
#endif
namespace ALI{
	class ALIGN{
	private:
		uchar num;
	public:
		ALIGN(){num=0;}
		ALIGN(uchar _num){num=_num;}
		ALIGN(const ALIGN& _ali){num=_ali.num;}
		const ALIGN& operator=(const ALIGN& _ali){
			if(this!=&_ali)num=_ali.num;
			return *this;
		}
		const ALIGN& operator|=(const ALIGN& _ali){
			if(this!=&_ali)num|=_ali.num;
			return *this;
		}
		const ALIGN& operator&=(const ALIGN& _ali){
			if(this!=&_ali)num&=_ali.num;
			return *this;
		}
		ALIGN operator|(const ALIGN& _ali)const{
			ALIGN tmp(*this);
			return tmp|=_ali;
		}
		ALIGN operator&(const ALIGN& _ali)const{
			ALIGN tmp(*this);
			return tmp&=_ali;
		}
		bool operator==(const ALIGN& _ali)const{
			return _ali.num==num;
		}
		bool operator!=(const ALIGN& _ali)const{
			return _ali.num!=num;
		}
		bool operator<=(const ALIGN& _ali)const{
			return (num&_ali.num)==_ali.num;
		}
		bool operator>=(const ALIGN& _ali)const{
			return (num&_ali.num)==num;
		}
		operator bool(){return num!=0;}
	};
	extern const ALIGN center,left,right,top,bottom,all;
//	enum ALIGN{center=0,left=1,right=2,top=4,bottom=8,left_top=5,left_bottom=9,right_top=6,right_bottom=10};
}
inline Pint PintAli(const Pint& pos,const Pint& size,ALI::ALIGN ali=ALI::center){
	Pint tmp=size/2;
	if(ali&ALI::top)tmp.y=0;
	if(ali&ALI::bottom)tmp.y=size.y;
	if(ali&ALI::left)tmp.x=0;
	if(ali&ALI::right)tmp.x=size.x;
	return pos+tmp;
}
inline Pint PintVec(ALI::ALIGN ali){
	Pint tmp(0,0);
	if(ali&ALI::top)--tmp.y;
	if(ali&ALI::bottom)++tmp.y;
	if(ali&ALI::left)--tmp.x;
	if(ali&ALI::right)++tmp.x;
	return tmp;
}
inline ALI::ALIGN AliVec(Pint pos){
	ALI::ALIGN Ali=ALI::center;
	if(pos.x>0)Ali|=ALI::right;
	if(pos.x<0)Ali|=ALI::left;
	if(pos.y>0)Ali|=ALI::bottom;
	if(pos.y<0)Ali|=ALI::top;
	return Ali;
}
#endif
