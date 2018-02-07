#ifndef HMLIB_COLOR_INC
#define HMLIB_COLOR_INC 100
#include <algorithm>
#include <type_traits>
namespace hmLib{
	namespace color{
		class rgb_color;	//RGB型カラー	r:赤強度 g:緑強度 b:青強度 を指定
		class hsv_color;	//HSV型カラー	hue:色相 sat:彩度 vlu:強度 を指定
		class hls_color;	//HLS型カラー	hue:色相 lgt:彩度 sat:明度 を指定
		class rgba_color;	//RGBA型カラー	r:赤強度 g:緑強度 b:青強度 a:透過度 を指定
		class hsva_color;	//HSVA型カラー	hue:色相 sat:彩度 vlu:強度 a:透過度 を指定
		class hlsa_color;	//HLSA型カラー	hue:色相 lgt:彩度 sat:明度 a:透過度 を指定
		class rgb_color{
		public:
			unsigned char r,g,b;
		public:
			rgb_color& operator+=(const rgb_color& Other) {
				if(256 - r>Other.r)r += Other.r;
				else r = 255;
				if(256 - g>Other.g)g += Other.g;
				else g = 255;
				if(256 - b>Other.b)b += Other.b;
				else b = 255;
				return *this;
			}
			friend rgb_color operator+(const rgb_color& v1, const rgb_color& v2) {
				rgb_color v(v1);
				v += v2;
				return v;
			}
			rgb_color& operator-=(const rgb_color& Other) {
				if(r>Other.r)r -= Other.r;
				else r = 0;
				if(g>Other.g)g -= Other.g;
				else g = 0;
				if(b>Other.b)b -= Other.b;
				else b = 0;
				return *this;
			}
			friend rgb_color operator-(const rgb_color& v1, const rgb_color& v2) {
				rgb_color v(v1);
				v -= v2;
				return v;
			}
			rgb_color& operator*=(const rgb_color& Other) {			
				r = static_cast<unsigned char>(static_cast<std::uint32_t>(r)*static_cast<std::uint32_t>(Other.r) / 256);
				g = static_cast<unsigned char>(static_cast<std::uint32_t>(g)*static_cast<std::uint32_t>(Other.g) / 256);
				b = static_cast<unsigned char>(static_cast<std::uint32_t>(b)*static_cast<std::uint32_t>(Other.b) / 256);
				return *this;
			}
			friend rgb_color operator*(const rgb_color& v1, const rgb_color& v2) {
				rgb_color v(v1);
				v *= v2;
				return v;
			}
			rgb_color& operator&=(const rgb_color& Other) {
				r = std::min(r, Other.r);
				g = std::min(g, Other.g);
				b = std::min(b, Other.b);
				return *this;
			}
			friend rgb_color operator&(const rgb_color& v1, const rgb_color& v2) {
				rgb_color v(v1);
				v &= v2;
				return v;
			}
			rgb_color& operator|=(const rgb_color& Other) {
				r = std::max(r, Other.r);
				g = std::max(g, Other.g);
				b = std::max(b, Other.b);
				return *this;
			}
			friend rgb_color operator|(const rgb_color& v1, const rgb_color& v2) {
				rgb_color v(v1);
				v |= v2;
				return v;
			}
			rgb_color& operator*=(double val) {
				std::int32_t tmpr, tmpg, tmpb;
				tmpr = static_cast<std::int32_t>(r*val);
				tmpg = static_cast<std::int32_t>(g*val);
				tmpb = static_cast<std::int32_t>(b*val);
				if(tmpr<0)r = 0;
				else if(tmpr>255)r = 255;
				else r = tmpr;
				if(tmpg<0)g = 0;
				else if(tmpg>255)g = 255;
				else g = tmpg;
				if(tmpb<0)b = 0;
				else if(tmpb>255)b = 255;
				else b = tmpb;
				return *this;
			}
			friend rgb_color operator*(const rgb_color& v1, double val) {
				rgb_color v(v1);
				v *= val;
				return v;
			}
			friend rgb_color operator*(double val, const rgb_color& v1) {
				rgb_color v(v1);
				v *= val;
				return v;
			}
			rgb_color& operator/=(double val) {
				std::int32_t tmpr, tmpg, tmpb;
				tmpr = static_cast<std::int32_t>(r/val);
				tmpg = static_cast<std::int32_t>(g/val);
				tmpb = static_cast<std::int32_t>(b/val);
				if(tmpr<0)r = 0;
				else if(tmpr>255)r = 255;
				else r = tmpr;
				if(tmpg<0)g = 0;
				else if(tmpg>255)g = 255;
				else g = tmpg;
				if(tmpb<0)b = 0;
				else if(tmpb>255)b = 255;
				else b = tmpb;
				return *this;
			}
			friend rgb_color operator/(const rgb_color& v1, double val) {
				rgb_color v(v1);
				v /= val;
				return v;
			}
			friend bool operator==(const rgb_color& v1, const rgb_color& v2) {
				return (v1.r == v2.r && v1.g == v2.g && v1.b == v2.b);
			}
			friend bool operator!=(const rgb_color& v1, const rgb_color& v2) {
				return !(v1.r == v2.r && v1.g == v2.g && v1.b == v2.b);
			}
		public:
			rgb_color(){set(0,0,0);}
			rgb_color(unsigned char _r,unsigned char _g,unsigned char _b){set(_r,_g,_b);}
			rgb_color(const rgb_color& _rgb){set(_rgb);}
			void set(unsigned char _r,unsigned char _g,unsigned char _b){
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
			std::int32_t hue;
			unsigned char sat,vlu;
		public:
			hsv_color(){set(0,0,0);}
			hsv_color(std::int32_t _hue,unsigned char _sat,unsigned char _vlu){set(_hue,_sat,_vlu);}
			hsv_color(const hsv_color& _hsv){set(_hsv);}
			void set(std::int32_t _hue,unsigned char _sat,unsigned char _vlu){
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
			std::int32_t hue;
			unsigned char sat,lgt;
		public:
			hls_color(){set(0,0,0);}
			hls_color(std::int32_t _hue,unsigned char _lgt,unsigned char _sat){set(_hue,_lgt,_sat);}
			hls_color(const hls_color& _hls){set(_hls);}
			void set(std::int32_t _hue,unsigned char _lgt,unsigned char _sat){
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
		namespace detail{
			inline void HXXToRGB(rgb_color& rgb,std::int32_t hue,unsigned char minv,unsigned char maxv){
				std::int32_t adjh=hue%360;
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
			inline void RGBToHXX(std::int32_t& hue,unsigned char& minv,unsigned char& maxv,rgb_color rgb){
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

			unsigned char maxv,minv;
			detail::RGBToHXX(tmp.hue,minv,maxv,*this);
			if(maxv==0 && minv==0)return tmp;
			tmp.vlu=maxv;
			tmp.sat=(std::int32_t)(maxv-minv)*255/maxv;

			return tmp;
		}
		inline rgb_color::operator hls_color()const{
			hls_color tmp(0,0,0);

			unsigned char maxv,minv;
			detail::RGBToHXX(tmp.hue,minv,maxv,*this);

			tmp.lgt=(std::int32_t)(maxv+minv)/2;
			if(maxv==minv)tmp.sat=0;
			else{
				if(tmp.lgt>127)tmp.sat=(std::int32_t)(maxv-minv)*255/(510-maxv-minv);
				else tmp.sat=(std::int32_t)(maxv-minv)*255/((std::int32_t)maxv+(std::int32_t)minv);
			}
			return tmp;
		};
		inline hsv_color::operator rgb_color()const{
			rgb_color tmp(0,0,0);
			detail::HXXToRGB(tmp,this->hue,this->vlu-((std::int32_t)this->sat*this->vlu/255),this->vlu);
			return tmp;
		}
		inline hsv_color::operator hls_color()const{return static_cast<hls_color>(static_cast<rgb_color>(*this));}
		inline hls_color::operator rgb_color()const{
			rgb_color tmp(0,0,0);
			if(this->lgt>127){
				unsigned char maxv,minv;
				minv=(std::int32_t)this->lgt*2-(std::int32_t)this->lgt*(255-this->sat)/255-(std::int32_t)this->sat;
				maxv=(std::int32_t)this->lgt*(255-this->sat)/255+(std::int32_t)this->sat;
				detail::HXXToRGB(tmp,this->hue,minv,maxv);
			}else{
				unsigned char maxv,minv;
				minv=(std::int32_t)this->lgt*2-(std::int32_t)this->lgt*(255+this->sat)/255;
				maxv=(std::int32_t)this->lgt*(255+this->sat)/255;
				detail::HXXToRGB(tmp,this->hue,minv,maxv);
			}
			return tmp;
		};
		inline hls_color::operator hsv_color()const{return static_cast<hsv_color>(static_cast<rgb_color>(*this));}
		inline rgb_color clrRGB(unsigned char r,unsigned char g,unsigned char b){return rgb_color(r,g,b);}
		inline hsv_color clrHSV(std::int32_t hue,unsigned char sat,unsigned char vlu){return hsv_color(hue,sat,vlu);}
		inline hls_color clrHLS(std::int32_t hue,unsigned char lgt,unsigned char sat){return hls_color(hue,lgt,sat);}
		class rgba_color:public rgb_color{
		public:
			unsigned char a;
		public:
			rgba_color(){set(0,0,0,0xff);}
			rgba_color(unsigned char _r,unsigned char _g,unsigned char _b,unsigned char _a=0xff){set(_r,_g,_b,_a);}
			rgba_color(const rgb_color& _rgb,unsigned char _a=0xff){set(_rgb,_a);}
			rgba_color(const rgba_color& _rgba){set(_rgba);}
			void set(unsigned char _r,unsigned char _g,unsigned char _b,unsigned char _a=0xff){
				rgb_color::set(_r,_g,_b);
				a=_a;
			}
			void set(const rgb_color& _rgb,unsigned char _a=0xff){
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
			unsigned char a;
		public:
			hsva_color(){set(0,0,0,0xff);}
			hsva_color(std::int32_t _hue,unsigned char _sat,unsigned char _vlu,unsigned char _a=0xff){set(_hue,_sat,_vlu,_a);}
			hsva_color(const hsv_color& _hsv,unsigned char _a=0xff){set(_hsv,_a);}
			hsva_color(const hsv_color& _hsva){set(_hsva);}
			void set(std::int32_t _hue,unsigned char _sat,unsigned char _vlu,unsigned char _a=0xff){
				hsv_color::set(_hue,_sat,_vlu);
				a=_a;
			}
			void set(const hsv_color& _hsv,unsigned char _a=0xff){
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
			unsigned char a;
		public:
			hlsa_color(){set(0,0,0,0xff);}
			hlsa_color(std::int32_t _hue,unsigned char _lgt,unsigned char _sat,unsigned char _a=0xff){set(_hue,_lgt,_sat,_a);}
			hlsa_color(const hls_color& _hls,unsigned char _a=0xff){set(_hls,_a);}
			hlsa_color(const hlsa_color& _hlsa){set(_hlsa);}
			void set(std::int32_t _hue,unsigned char _lgt,unsigned char _sat,unsigned char _a=0xff){
				hls_color::set(_hue,_lgt,_sat);
				a=_a;
			}
			void set(const hlsa_color& _hls,unsigned char _a=0xff){
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
		inline rgba_color clrRGBA(unsigned char r,unsigned char g,unsigned char b,unsigned char a){return rgba_color(r,g,b,a);}
		inline hsva_color clrHSVA(std::int32_t hue,unsigned char sat,unsigned char vlu,unsigned char a){return hsva_color(hue,sat,vlu,a);}
		inline hlsa_color clrHLSA(std::int32_t hue,unsigned char lgt,unsigned char sat,unsigned char a){return hlsa_color(hue,lgt,sat,a);}
	}
}
#endif
