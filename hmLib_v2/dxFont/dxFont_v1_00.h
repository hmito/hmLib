#ifndef INC_HMLIB_DXFONT
#define INC_HMLIB_DXFONT 100
#
#ifndef INC_HMLIB_HMPOS
#	include "hmLib_v2/hmPos.hpp"
#endif
#ifndef INC_HMLIB_HMCOLOR
#	include "hmLib_v2/hmColor.hpp"
#endif
#include <string>
#include <DxLib.h>
namespace hmLib{
	//cFontƒNƒ‰ƒX
	namespace dx{
		class cFont{
		public:
			enum Type{Normal=DX_FONTTYPE_NORMAL,Edge=DX_FONTTYPE_EDGE,Aalia=DX_FONTTYPE_ANTIALIASING,EdgeAalia=DX_FONTTYPE_ANTIALIASING_EDGE};
		private:
			std::string Name;
			int Handle;
			bool Vertical;
			int Size;
			int Thick;
			Type FType;
		private:
			cFont(int Size_,int Thick_,std::string Name_,bool Vertical_,Type FType_){
				Handle=-1;
				Name=Name_;
				Size=Size_;
				Thick=Thick_;
				Vertical=Vertical_;
				FType=FType_;
			}
		private://static
			static cFont Default;
			static cFont VDefault;
		public:
			cFont():Handle(-1){return;}
			~cFont(){del();}
			bool make(int Size_=16,int Thick_=6,std::string Name_="‚l‚r ƒSƒVƒbƒN",bool Vertical_=false,Type FType_=Normal){
				del();
				if(Vertical_){
					Vertical=true;
					Handle=CreateFontToHandle(("@"+Name_).c_str(),Size_,Thick_,FType_);
				}else{
					Vertical=false;
					Handle=CreateFontToHandle(Name_.c_str(),Size_,Thick_,FType_);
				}
				if(is_make()){
					Name=Name_;
					Size=Size_;
					Thick=Thick_;
					FType=FType_;
					return false;
				}else{
					Handle=-1;
					return true;
				}
			}
			bool is_make()const{return Handle>=0;}
			void del(){
				if(is_make()){
					DeleteFontToHandle(Handle);
					Handle=-1;
				}
			}
			std::string getName()const{
				if(is_make())return Name;
				else return Default.Name;
			}
			int getHandle()const{return Handle;}
			bool getVertical()const{return Vertical;}
			int getSize()const{
				if(is_make())return Size;
				else return Default.Size;
			}
			int getThick()const{
				if(is_make())return Thick;
				else return Default.Thick;
			}
			Type getType()const{
				if(is_make())return FType;
				else return Default.FType;
			}
			Pint getDrawStrSize(const std::string& Str_)const{
				Pint ans;
				if(getVertical()){
					ans.x=getSize();
					if(getHandle()){
						ans.y = GetDrawStringWidthToHandle(Str_.c_str(),Str_.size(),getHandle(),1);
					}else{
						ans.y = GetDrawStringWidth(Str_.c_str(),Str_.size(),1);
					}
				}else{
					ans.y=getSize();
					if(getHandle()){
						ans.x = GetDrawStringWidthToHandle(Str_.c_str(),Str_.size(),getHandle(),0);
					}else{
						ans.x = GetDrawStringWidth(Str_.c_str(),Str_.size(),0);
					}
				}
				return ans;
			}
		public://static
			static const cFont& getDefault(bool Vertical_){
				if(Vertical_)return VDefault;
				else return Default;
			}
			static void setDefault(int Size_=16,int Thick_=6,std::string Name_="‚l‚r ƒSƒVƒbƒN",Type FType_=Normal){
				Default.Name=Name_;
				VDefault.Name=Name_;
				ChangeFont(Name_.c_str());

				Default.Size=Size_;
				VDefault.Size=Size_;
				SetFontSize(Size_);

				Default.Thick=Thick_;
				VDefault.Thick=Thick_;
				SetFontThickness(Thick_);

				Default.FType=FType_;
				VDefault.FType=FType_;
				ChangeFontType(FType_);
			}
		};
/*
		inline int getDrawStringWidth(const char* CStr,int StrLen,const cFont& Font){
			if(Font.getHandle()>0)return GetDrawStringWidthToHandle(CStr,StrLen,Font.getHandle(),Font.getVertical());
			else return GetDrawStringWidth(CStr,StrLen,Font.getVertical());
		}
		inline int getDrawStringWidth(const std::string& Str,const cFont& Font){
			return getDrawStringWidth(Str.c_str(),Str.size(),Font);
		}
*/
	}
}
#
#endif
