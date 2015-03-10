#ifndef INC_HMLIB_ALIGN
#define INC_HMLIB_ALIGN 100
#
#ifndef INC_HMLIB_OPERATORPATTERN
#	include "hmLib_v2/operator_pattern.hpp"
#endif
namespace hmLib{
	class align_quad:
		public pattern::and_pattern<align_quad>,
		public pattern::or_pattern<align_quad>{
	private:
		unsigned char num;
	protected:
		virtual void forward_and(const align_quad& dir_){num&=dir_.num;}
		virtual void forward_or(const align_quad& dir_){num|=dir_.num;}
	public:
		align_quad():num(0){return;}
		explicit align_quad(unsigned char num_):num(num_){return;}
		align_quad(const align_quad& dir_):num(dir_.num){return;}
		const align_quad& operator=(const align_quad& dir_){
			if(this!=&dir_)num=dir_.num;
			return *this;
		}
		bool operator==(const align_quad& dir_)const{return dir_.num==num;}
		bool operator!=(const align_quad& dir_)const{return dir_.num!=num;}
		bool operator<=(const align_quad& dir_)const{return (num&dir_.num)==dir_.num;}
		bool operator>=(const align_quad& dir_)const{return (num&dir_.num)==num;}
		operator bool(){return num!=0;}
	};
	class align_octa:
		public pattern::and_pattern<align_octa>,
		public pattern::or_pattern<align_octa>{
	private:
		unsigned char num;
	protected:
		virtual void forward_and(const align_octa& dir_){num&=dir_.num;}
		virtual void forward_or(const align_octa& dir_){num|=dir_.num;}
	public:
		align_octa():num(0){return;}
		explicit align_octa(unsigned char num_):num(num_){return;}
		align_octa(const align_octa& dir_):num(dir_.num){return;}
		const align_octa& operator=(const align_octa& dir_){
			if(this!=&dir_)num=dir_.num;
			return *this;
		}
		bool operator==(const align_octa& dir_)const{return dir_.num==num;}
		bool operator!=(const align_octa& dir_)const{return dir_.num!=num;}
		bool operator<=(const align_octa& dir_)const{return (num&dir_.num)==dir_.num;}
		bool operator>=(const align_octa& dir_)const{return (num&dir_.num)==num;}
		operator bool(){return num!=0;}
	};
	namespace align{
		namespace quad{
			extern const align_quad center,left,right,top,bottom,all;
		}
		namespace octa{
			extern const align_octa center,left,right,top,bottom,left_top,rigt_top,left_bottom,right_bottom,all;
		}
	}
/*
	inline Pint PintRect(const Pint& pos,const Pint& size,align_quad dir_=aliq::center){
		Pint tmp=size/2;
		if(dir_&aliq::top)tmp.y=0;
		if(dir_&aliq::bottom)tmp.y=size.y;
		if(dir_&aliq::left)tmp.x=0;
		if(dir_&aliq::right)tmp.x=size.x;
		return pos+tmp;
	}
	inline Pint AliToPint(align_quad dir_){
		Pint tmp(0,0);
		if(dir_&aliq::top)--tmp.y;
		if(dir_&aliq::bottom)++tmp.y;
		if(dir_&aliq::left)--tmp.x;
		if(dir_&aliq::right)++tmp.x;
		return tmp;
	}
	inline align_quad PintToAli(Pint pos){
		align_quad Ali=aliq::center;
		if(pos.x>0)Ali|=aliq::right;
		if(pos.x<0)Ali|=aliq::left;
		if(pos.y>0)Ali|=aliq::bottom;
		if(pos.y<0)Ali|=aliq::top;
		return Ali;
	}
*/
}
#endif
