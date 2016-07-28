#ifndef HMLIB_COLOR_INC
#define HMLIB_COLOR_INC 100
#ifndef HMLIB_BYTEBASETYPE_INC
#	include "bytebase_type.hpp"
#endif
#ifndef HMLIB_OPERATORPATTERN_INC
#	include "operator_pattern.hpp"
#endif
#include <algorithm>
namespace hmLib{
	namespace color{
		class rgb_color;	//RGB型カラー	r:赤強度 g:緑強度 b:青強度 を指定
		class hsv_color;	//HSV型カラー	hue:色相 sat:彩度 vlu:強度 を指定
		class hls_color;	//HLS型カラー	hue:色相 lgt:彩度 sat:明度 を指定
		class rgba_color;	//RGBA型カラー	r:赤強度 g:緑強度 b:青強度 a:透過度 を指定
		class hsva_color;	//HSVA型カラー	hue:色相 sat:彩度 vlu:強度 a:透過度 を指定
		class hlsa_color;	//HLSA型カラー	hue:色相 lgt:彩度 sat:明度 a:透過度 を指定
		class rgb_color:
			public pattern::add_pattern<rgb_color>,
			public pattern::sub_pattern<rgb_color>,
			public pattern::multi_pattern<rgb_color>,
			public pattern::and_pattern<rgb_color>,
			public pattern::or_pattern<rgb_color>,
			public pattern::bidirectional_multi_pattern<rgb_color,type::float64>,
			public pattern::forward_div_pattern<rgb_color,type::float64>,
			public pattern::equal_pattern<rgb_color>{
		public:
			type::uint8 r,g,b;
		protected:
			virtual void forward_add(const rgb_color& My_){
				if(256-r>My_.r)r+=My_.r;
				else r=255;
				if(256-g>My_.g)g+=My_.g;
				else g=255;
				if(256-b>My_.b)b+=My_.b;
				else b=255;
			}
			virtual void forward_sub(const rgb_color& My_){
				if(r>My_.r)r-=My_.r;
				else r=0;
				if(g>My_.g)g-=My_.g;
				else g=0;
				if(b>My_.b)b-=My_.b;
				else b=0;
			}
			virtual void forward_multi(const rgb_color& My_){
				r=static_cast<type::uint8>(static_cast<type::uint32>(r)*static_cast<type::uint32>(My_.r)/256);
				g=static_cast<type::uint8>(static_cast<type::uint32>(g)*static_cast<type::uint32>(My_.g)/256);
				b=static_cast<type::uint8>(static_cast<type::uint32>(b)*static_cast<type::uint32>(My_.b)/256);
			}
			virtual void forward_and(const rgb_color& My_){
				r=std::min(r,My_.r);
				g=std::min(g,My_.g);
				b=std::min(b,My_.b);
			}
			virtual void forward_or(const rgb_color& My_){
				r=std::max(r,My_.r);
				g=std::max(g,My_.g);
				b=std::max(b,My_.b);
			}
			virtual void forward_multi(const type::float64& f_){
				type::sint32 tmpr,tmpg,tmpb;
				tmpr=static_cast<type::sint32>(r*f_);
				tmpg=static_cast<type::sint32>(g*f_);
				tmpb=static_cast<type::sint32>(b*f_);
				if(tmpr<0)r=0;
				else if(tmpr>255)r=255;
				else r=tmpr;
				if(tmpg<0)g=0;
				else if(tmpg>255)g=255;
				else g=tmpg;
				if(tmpb<0)b=0;
				else if(tmpb>255)b=255;
				else b=tmpb;
			}
			virtual void forward_div(const type::float64& f_){
				type::sint32 tmpr,tmpg,tmpb;
				tmpr=static_cast<type::sint32>(r/f_);
				tmpg=static_cast<type::sint32>(g/f_);
				tmpb=static_cast<type::sint32>(b/f_);
				if(tmpr<0)r=0;
				else if(tmpr>255)r=255;
				else r=tmpr;
				if(tmpg<0)g=0;
				else if(tmpg>255)g=255;
				else g=tmpg;
				if(tmpb<0)b=0;
				else if(tmpb>255)b=255;
				else b=tmpb;
			}
			virtual bool forward_equal(const rgb_color& My_)const{return ((r==My_.r)&&(g==My_.g)&&(b==My_.b));}
		public:
			rgb_color(){set(0,0,0);}
			rgb_color(type::uint8 _r,type::uint8 _g,type::uint8 _b){set(_r,_g,_b);}
			rgb_color(const rgb_color& _rgb){set(_rgb);}
			void set(type::uint8 _r,type::uint8 _g,type::uint8 _b){
				r=_r;
				g=_g;
				b=_b;
			}
			void set(const rgb_color& _rgb){
				r=_rgb.r;
				g=_rgb.g;
				b=_rgb.b;
			}
			const rgb_color& operator=(const rgb_color& _rgb){
				if(this!=&_rgb)set(_rgb);
				return *this;
			}
			inline operator hsv_color()const;
			inline operator hls_color()const;
		};
		class hsv_color{
		public:
			type::sint32 hue;
			type::uint8 sat,vlu;
		public:
			hsv_color(){set(0,0,0);}
			hsv_color(type::sint32 _hue,type::uint8 _sat,type::uint8 _vlu){set(_hue,_sat,_vlu);}
			hsv_color(const hsv_color& _hsv){set(_hsv);}
			void set(type::sint32 _hue,type::uint8 _sat,type::uint8 _vlu){
				hue=_hue;
				sat=_sat;
				vlu=_vlu;
			}
			void set(const hsv_color& _hsv){
				hue=_hsv.hue;
				sat=_hsv.sat;
				vlu=_hsv.vlu;
			}
			const hsv_color& operator=(const hsv_color& _hsv){
				if(this!=&_hsv)set(_hsv);
				return *this;
			}
			inline operator rgb_color()const;
			inline operator hls_color()const;
		};
		class hls_color{
		public:
			type::sint32 hue;
			type::uint8 sat,lgt;
		public:
			hls_color(){set(0,0,0);}
			hls_color(type::sint32 _hue,type::uint8 _lgt,type::uint8 _sat){set(_hue,_lgt,_sat);}
			hls_color(const hls_color& _hls){set(_hls);}
			void set(type::sint32 _hue,type::uint8 _lgt,type::uint8 _sat){
				hue=_hue;
				lgt=_lgt;
				sat=_sat;
			}
			void set(const hls_color& _hls){
				hue=_hls.hue;
				lgt=_hls.lgt;
				sat=_hls.sat;
			}
			const hls_color& operator=(const hls_color& _hls){
				if(this!=&_hls)set(_hls);
				return *this;
			}
			inline operator rgb_color()const;
			inline operator hsv_color()const;
		};
		namespace{
			inline void HXXToRGB(rgb_color& rgb,type::sint32 hue,type::uint8 minv,type::uint8 maxv){
				type::sint32 adjh=hue%360;
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
			inline void RGBToHXX(type::sint32& hue,type::uint8& minv,type::uint8& maxv,rgb_color rgb){
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
		}
		inline rgb_color::operator hsv_color()const{
			hsv_color tmp(0,0,0);

			type::uint8 maxv,minv;
			RGBToHXX(tmp.hue,minv,maxv,*this);
			if(maxv==0 && minv==0)return tmp;
			tmp.vlu=maxv;
			tmp.sat=(type::sint32)(maxv-minv)*255/maxv;

			return tmp;
		}
		inline rgb_color::operator hls_color()const{
			hls_color tmp(0,0,0);

			type::uint8 maxv,minv;
			RGBToHXX(tmp.hue,minv,maxv,*this);

			tmp.lgt=(type::sint32)(maxv+minv)/2;
			if(maxv==minv)tmp.sat=0;
			else{
				if(tmp.lgt>127)tmp.sat=(type::sint32)(maxv-minv)*255/(510-maxv-minv);
				else tmp.sat=(type::sint32)(maxv-minv)*255/((type::sint32)maxv+(type::sint32)minv);
			}
			return tmp;
		};
		inline hsv_color::operator rgb_color()const{
			rgb_color tmp(0,0,0);
			HXXToRGB(tmp,this->hue,this->vlu-((type::sint32)this->sat*this->vlu/255),this->vlu);
			return tmp;
		}
		inline hsv_color::operator hls_color()const{return static_cast<hls_color>(static_cast<rgb_color>(*this));}
		inline hls_color::operator rgb_color()const{
			rgb_color tmp(0,0,0);
			if(this->lgt>127){
				type::uint8 maxv,minv;
				minv=(type::sint32)this->lgt*2-(type::sint32)this->lgt*(255-this->sat)/255-(type::sint32)this->sat;
				maxv=(type::sint32)this->lgt*(255-this->sat)/255+(type::sint32)this->sat;
				HXXToRGB(tmp,this->hue,minv,maxv);
			}else{
				type::uint8 maxv,minv;
				minv=(type::sint32)this->lgt*2-(type::sint32)this->lgt*(255+this->sat)/255;
				maxv=(type::sint32)this->lgt*(255+this->sat)/255;
				HXXToRGB(tmp,this->hue,minv,maxv);
			}
			return tmp;
		};
		inline hls_color::operator hsv_color()const{return static_cast<hsv_color>(static_cast<rgb_color>(*this));}
		inline rgb_color clrRGB(type::uint8 r,type::uint8 g,type::uint8 b){return rgb_color(r,g,b);}
		inline hsv_color clrHSV(type::sint32 hue,type::uint8 sat,type::uint8 vlu){return hsv_color(hue,sat,vlu);}
		inline hls_color clrHLS(type::sint32 hue,type::uint8 lgt,type::uint8 sat){return hls_color(hue,lgt,sat);}
		class rgba_color:public rgb_color{
		public:
			type::uint8 a;
		public:
			rgba_color(){set(0,0,0,0xff);}
			rgba_color(type::uint8 _r,type::uint8 _g,type::uint8 _b,type::uint8 _a=0xff){set(_r,_g,_b,_a);}
			rgba_color(const rgb_color& _rgb,type::uint8 _a=0xff){set(_rgb,_a);}
			rgba_color(const rgba_color& _rgba){set(_rgba);}
			void set(type::uint8 _r,type::uint8 _g,type::uint8 _b,type::uint8 _a=0xff){
				rgb_color::set(_r,_g,_b);
				a=_a;
			}
			void set(const rgb_color& _rgb,type::uint8 _a=0xff){
				rgb_color::set(_rgb);
				a=_a;
			}
			void set(const rgba_color& _rgba){
				rgb_color::set(_rgba);
				a=_rgba.a;
			}
			const rgba_color& operator=(const rgba_color& _rgba){
				if(this!=&_rgba)set(_rgba);
				return *this;
			}
			inline operator hsva_color()const;
			inline operator hlsa_color()const;
		};
		class hsva_color:public hsv_color{
		public:
			type::uint8 a;
		public:
			hsva_color(){set(0,0,0,0xff);}
			hsva_color(type::sint32 _hue,type::uint8 _sat,type::uint8 _vlu,type::uint8 _a=0xff){set(_hue,_sat,_vlu,_a);}
			hsva_color(const hsv_color& _hsv,type::uint8 _a=0xff){set(_hsv,_a);}
			hsva_color(const hsv_color& _hsva){set(_hsva);}
			void set(type::sint32 _hue,type::uint8 _sat,type::uint8 _vlu,type::uint8 _a=0xff){
				hsv_color::set(_hue,_sat,_vlu);
				a=_a;
			}
			void set(const hsv_color& _hsv,type::uint8 _a=0xff){
				hsv_color::set(_hsv);
				a=_a;
			}
			void set(const hsva_color& _hsva){
				hsv_color::set(_hsva);
				a=_hsva.a;
			}
			const hsva_color& operator=(const hsva_color& _hsva){
				if(this!=&_hsva)set(_hsva);
				return *this;
			}
			inline operator rgba_color()const;
			inline operator hlsa_color()const;
		};
		class hlsa_color:public hls_color{
		public:
			type::uint8 a;
		public:
			hlsa_color(){set(0,0,0,0xff);}
			hlsa_color(type::sint32 _hue,type::uint8 _lgt,type::uint8 _sat,type::uint8 _a=0xff){set(_hue,_lgt,_sat,_a);}
			hlsa_color(const hls_color& _hls,type::uint8 _a=0xff){set(_hls,_a);}
			hlsa_color(const hlsa_color& _hlsa){set(_hlsa);}
			void set(type::sint32 _hue,type::uint8 _lgt,type::uint8 _sat,type::uint8 _a=0xff){
				hls_color::set(_hue,_lgt,_sat);
				a=_a;
			}
			void set(const hlsa_color& _hls,type::uint8 _a=0xff){
				hls_color::set(_hls);
				a=_a;
			}
			const hlsa_color& operator=(const hlsa_color& _hlsa){
				if(this!=&_hlsa)set(_hlsa);
				return *this;
			}
			inline operator rgba_color()const;
			inline operator hsva_color()const;
		};
		inline rgba_color::operator hsva_color()const{return hsva_color(static_cast<hsv_color>(*this),this->a);}
		inline rgba_color::operator hlsa_color()const{return hlsa_color(static_cast<hls_color>(*this),this->a);}
		inline hsva_color::operator rgba_color()const{return rgba_color(static_cast<rgb_color>(*this),this->a);}
		inline hsva_color::operator hlsa_color()const{return hlsa_color(static_cast<hls_color>(*this),this->a);}
		inline hlsa_color::operator rgba_color()const{return rgba_color(static_cast<rgb_color>(*this),this->a);}
		inline hlsa_color::operator hsva_color()const{return hsva_color(static_cast<hsv_color>(*this),this->a);}
		inline rgba_color clrRGBA(type::uint8 r,type::uint8 g,type::uint8 b,type::uint8 a){return rgba_color(r,g,b,a);}
		inline hsva_color clrHSVA(type::sint32 hue,type::uint8 sat,type::uint8 vlu,type::uint8 a){return hsva_color(hue,sat,vlu,a);}
		inline hlsa_color clrHLSA(type::sint32 hue,type::uint8 lgt,type::uint8 sat,type::uint8 a){return hlsa_color(hue,lgt,sat,a);}
	}
}
#endif
