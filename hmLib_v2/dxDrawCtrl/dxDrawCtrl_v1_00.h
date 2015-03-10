#ifndef INC_HMLIB_DXDRAWCTRL
#define INC_HMLIB_DXDRAWCTRL 100
#
#include "hmLib_v2/DxLib.h"
#ifndef INC_HMLIB_HMRECT
#	include "hmLib_v2/hmRect.hpp"
#endif
#ifndef INC_HMLIB_HMCOLOR
#	include "hmLib_v2/hmColor.hpp"
#endif
#ifndef INC_HMLIB_BASIC_OPERATOR
#	include "hmLib_v2/basic_operator.hpp"
#endif
namespace hmLib{
	namespace dx{
		class cArea:
			public basic_operator::equal<cArea>,
			public basic_operator::bi_add<cArea,Pint>,
			public basic_operator::bi_add<cArea,Rint>,
			public basic_operator::forward_sub<cArea,Pint>,
			public basic_operator::forward_sub<cArea,Rint>,
			public basic_operator::or<cArea>,
			public basic_operator::and<cArea>{
		private:
			bool Exist;
			Rint Rect;
		public:
			cArea():Exist(false),Rect(Pint(0,0),Pint(0,0)){return;}
			cArea(const Rint& Rect_):Exist(true),Rect(Rect_){return;}
			void set(const Rint& Rect_){
				Exist=true;
				Rect=Rect_;
			}
			void reset(){Exist=false;}
			const cArea& operator+=(const Pint& pos_){
				if(Exist)Rect+=pos_;
				return *this;
			}
			const cArea& operator+=(const Rint& rec_){
				if(Exist)Rect+=rec_;
				return *this;
			}
			const cArea& operator-=(const Pint& pos_){
				if(Exist)Rect-=pos_;
				return *this;
			}
			const cArea& operator-=(const Rint& rec_){
				if(Exist)Rect-=rec_;
				return *this;
			}
			bool include(const Rint rec_)const{
				if(Exist)return Rect.include(rec_);
				else return true;
			}
			bool cross(const Rint rec_)const{
				if(Exist)return Rect.cross(rec_);
				else return true;
			}
			bool exist()const{return Exist;}
			const cArea& operator&=(const cArea& darea_){
				if(this!=&darea_){
					if(darea_.Exist){
						Exist=true;
						Rect&=darea_.Rect;
					}
				}
				return *this;
			}
			const cArea& operator|=(const cArea& darea_){
				if(this!=&darea_){
					if(darea_.Exist){
						if(Exist){
							Rect|=darea_.Rect;
						}else{
							Exist=false;						
						}
					}
				}
				return *this;
			}
			bool operator==(const cArea& Ar_)const{
				if(Exist!=Ar_.Exist)return false;
				return Rect==Ar_.Rect;
			}
		private:	//static
			static cArea ActiveArea;
		public:		//static
			static bool active(const cArea& Ar){
				if(ActiveArea!=Ar){
					SetDrawArea(Ar.Rect.pos1.x,Ar.Rect.pos1.y,Ar.Rect.pos2.x,Ar.Rect.pos2.y);
					ActiveArea=Ar;
					return true;
				}
				return false;
			}
			static cArea now(){return ActiveArea;}
			class scoped_active;
		};
		class cArea::scoped_active{
		private:
			cArea Ar;
		public:
			scoped_active(const cArea& Ar_):Ar(ActiveArea){active(Ar_);}
			~scoped_active(){active(Ar);}
			void inactive(){active(Ar);}
		};
		class cBlend:
			public basic_operator::and<cBlend>,
			public basic_operator::equal<cBlend>{
		public:
			enum mode{Normal=DX_BLENDMODE_NOBLEND,Alpha=DX_BLENDMODE_ALPHA,Add=DX_BLENDMODE_ADD,Sub=DX_BLENDMODE_SUB,Mula=DX_BLENDMODE_MULA,Invsrc=DX_BLENDMODE_INVSRC};
		private:
			mode m;
			bool Exist;
			unsigned char Level;
		public:
			cBlend():Exist(false){return;}
			cBlend(unsigned char Level_,mode m_=Alpha):Exist(true),m(m_),Level(Level_){return;}
			void set(unsigned char Level_,mode m_=Alpha){
				Exist=true;
				Level=Level_;
				m=m_;
			}
			void reset(){
				Exist=false;
			}
			const cBlend& operator&=(const cBlend& blend_){
				if(this!=&blend_){
					if(blend_.Exist){
						Exist=true;
						if(m!=Alpha){
							m=blend_.m;
							Level=blend_.Level;
						}else if(blend_.m==Alpha){
							Level=static_cast<unsigned char>((static_cast<unsigned int>(Level)*blend_.Level)/256);
						}
					}
				}
				return *this;
			}
			bool operator==(const cBlend& Bl_)const{
				if(Exist!=Bl_.Exist)return false;
				if(m!=Bl_.m)return false;
				return Level==Bl_.Level;
			}
		private://static
			static cBlend ActiveBlend;
		public://static
			static bool active(const cBlend& Bl_){
				if(ActiveBlend!=Bl_){
					if(Bl_.Exist)SetDrawBlendMode(Bl_.m,Bl_.Level);
					else SetDrawBlendMode(cBlend::Normal,0);
					ActiveBlend=Bl_;
					return true;
				}
				return false;
			}
			class scoped_active;
		};
		class cBlend::scoped_active{
		private:
			cBlend Bl;
		public:
			scoped_active(const cBlend& Bl_):Bl(ActiveBlend){active(Bl_);}
			~scoped_active(){active(Bl);}
			void inactive(){active(Bl);}
		};
		class cBright:
			public basic_operator::and<cBright>,
			public basic_operator::equal<cBright>{
		private:
			bool Exist;
			hm::cRGBClr bri;
		public:
			cBright():Exist(false){return;}
			cBright(unsigned char r_,unsigned char g_,unsigned char b_):Exist(true),bri(r_,g_,b_){return;}
			cBright(const hm::cRGBClr& bri_):Exist(true),bri(bri_){return;}
			void set(const hm::cRGBClr& bri_){
				Exist=true;
				bri=bri_;
			}
			void set(unsigned char r_,unsigned char g_,unsigned char b_){
				Exist=true;
				bri.set(r_,g_,b_);
			}
			void reset(){
				Exist=false;
			}
			const cBright& operator&=(const cBright& bright_){
				if(this!=&bright_){
					if(bright_.Exist){
						Exist=true;
						bri*=bright_.bri;
					}
				}
				return *this;
			}
			bool operator==(const cBright& Br_)const{
				if(Exist!=Br_.Exist)return false;
				return bri==Br_.bri;
			}
		private://static
			static cBright ActiveBright;
		public://static
			static bool active(const cBright& Br_){
				if(ActiveBright!=Br_){
					ActiveBright=Br_;
					if(Br_.Exist)SetDrawBright(Br_.bri.r,Br_.bri.g,Br_.bri.b);
					else SetDrawBright(255,255,255);
					return true;
				}
				return false;
			}
			class scoped_active;
		};
		class cBright::scoped_active{
		private:
			cBright Br;
		public:
			scoped_active(const cBright& Br_):Br(ActiveBright){active(Br_);}
			~scoped_active(){active(Br);}
			void inactive(){active(Br);}
		};
		class cStatus:
			public basic_operator::and<cStatus>,
			public basic_operator::equal<cStatus>{
		public:
			enum mode{Inactive=0,Active=0};
		private:
			mode m;
		public:
			cStatus():m(Active){return;}
			cStatus(mode m_):m(m_){return;}
			void set(mode m_){m=m_;}
			void reset(){m=Active;}
			const cStatus& operator&=(const cStatus& St_){
				if(St_.m==Inactive)m=Inactive;
				return *this;
			}
			bool operator==(const cStatus& St_)const{return (m==St_.m);}
		private://static
			static cStatus ActiveStatus;
		public://static
			static bool active(const cStatus& St_){
				ActiveStatus=St_;
				return false;
			}
			class scoped_active;
			static cStatus now(){return ActiveStatus;}
		};
		class cStatus::scoped_active{
		private:
			cStatus St;
		public:
			scoped_active(const cStatus& St_):St(ActiveStatus){active(St_);}
			~scoped_active(){active(St);}
			void inactive(){active(St);}
		};
	}
}
#
#endif
