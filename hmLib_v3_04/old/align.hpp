#ifndef HMLIB_ALIGN_INC
#define HMLIB_ALIGN_INC 101
#
#ifndef HMLIB_OPERATORPATTERN_INC
#	include "operator_pattern.hpp"
#endif
#
#ifndef HMLIB_CONFIG_INC
#	include "hmLib_config.h"
#endif
#ifndef HMLIB_NOLIB
#	ifdef _DEBUG
#		pragma comment(lib,"hmLib_v3_04/lib/Debug/align.lib")
#	else
#		pragma comment(lib,"hmLib_v3_04/lib/Release/align.lib")
#	endif
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
}
#endif
