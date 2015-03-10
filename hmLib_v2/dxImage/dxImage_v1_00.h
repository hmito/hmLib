#ifndef INC_HMLIB_DXIMAGE
#define INC_HMLIB_DXIMAGE 100
#
#ifndef INC_HMLIB_HMPOS
#	include "hmLib_v2/hmPos.hpp"
#endif
#ifndef INC_HMLIB_HMCOLOR
#	include "hmLib_v2/hmColor.hpp"
#endif
#ifndef INC_HMLIB_BASICITERATOR
#	include "hmLib_v2/basic_iterator.hpp"
#endif
#ifndef INC_HMLIB_HMTYPE
#	include "hmLib_v2/hmType.hpp"
#endif
#include "hmLib_v2/DxLib.h"
namespace hmLib{
	namespace dx{
		class cImage;
		class cScreen;
		class cSoftImage;
		//描画不可能画像クラス
		class cImage{
		public:
			enum FileType{bmp,png,jpeg};
		protected:
			explicit cImage(int _Handle){Handle=_Handle;}
		protected:
			int Handle;
			virtual void makeHandle(const Pint& _size){
				Handle=MakeGraph(_size.x,_size.y);
			}
			virtual void loadFileHandle(const char* _name){
				Handle=LoadGraph(_name);
			}
			virtual void loadMemHandle(void* _mem,const int& _memsize){
				Handle=CreateGraphFromMem(_mem,_memsize);
			}
		public:
			cImage(){Handle=-1;}
			cImage(const cImage& _grp){throw "cImage::cImage(const cImage& _grp) Unable to use this function";}
			const cImage& operator=(const cImage& _grp){throw "const cImage& cImage::operator=(const cImage& _grp) Unable to use this function";}
			~cImage(){release();}
			void release(){
				if(Handle>0)DeleteGraph(Handle);
				Handle=-1;
			}
			bool create(const Pint& _size){
				release();
				makeHandle(_size);
				return empty();
			}
			bool load(const std::string& _filename){
				release();
				loadFileHandle(_filename.c_str());
				return empty();
			}
			bool load(void* _mem,const int& _memsize){
				release();
				loadMemHandle(_mem,_memsize);
				return empty();
			}
			bool load(const cImage& Img_);
			bool load(const cImage& Img_,const Pint& _pos,const Pint& _size);
			bool load(const cScreen& Scr_);
			bool load(const cScreen& Scr_,const Pint& _pos,const Pint& _size);
			bool load(const cSoftImage& SfImg_);
			bool load(const cSoftImage& SfImg_,const Pint& _pos,const Pint& _size);
			virtual void save(const std::string& _filename,cImage::FileType _ft)const;
			bool empty()const{return (Handle<=0);}
			Pint size()const{
				Pint tmp;
				GetGraphSize(Handle,&(tmp.x),&(tmp.y));
				return tmp;
			}
			int getHandle()const{return Handle;}
			void swap(cImage& Img_){
				int tmp=Handle;
				Handle=Img_.Handle;
				Img_.Handle=tmp;
			}
		};
		//描画可能画像クラス
		class cScreen:public cImage{
		protected:
			explicit cScreen(int _Handle):cImage(_Handle){return;}
		protected:
			virtual void makeHandle(const Pint& _size){
				SetDrawValidAlphaChannelGraphCreateFlag(TRUE);
				SetDrawValidGraphCreateFlag( TRUE );
				Handle=MakeGraph(_size.x,_size.y);
				SetDrawValidGraphCreateFlag( FALSE );
				SetDrawValidAlphaChannelGraphCreateFlag(FALSE);
			}
			virtual void loadFileHandle(const char* _name){
				SetDrawValidAlphaChannelGraphCreateFlag(TRUE);
				SetDrawValidGraphCreateFlag( TRUE );
				Handle=LoadGraph(_name);
				SetDrawValidGraphCreateFlag( FALSE );
				SetDrawValidAlphaChannelGraphCreateFlag(FALSE);
			}
			virtual void loadMemHandle(void* _mem,const int& _memsize){
				SetDrawValidAlphaChannelGraphCreateFlag(TRUE);
				SetDrawValidGraphCreateFlag( TRUE );
				Handle=CreateGraphFromMem(_mem,_memsize);
				SetDrawValidGraphCreateFlag( FALSE );
				SetDrawValidAlphaChannelGraphCreateFlag(FALSE);
			}
		public:
			cScreen():cImage(){return;}
			cScreen(const cScreen& _grp){throw "cScreen::cScreen(const cScreen& _grp) Unable to use this function";}
			const cScreen& operator=(const cScreen& _grp){throw "const cScreen& cScreen::operator=(const cScreen& _grp) Unable to use this function";}
			~cScreen(){release();}
			virtual void save(const std::string& _filename,cImage::FileType _ft)const;
		public:
			static const cScreen DefaultFrontScreen;
			static const cScreen DefaultBackScreen;
		private://Active管理
			static int ActiveHandle;
			static bool active(int Handle_){
				if(ActiveHandle!=Handle_){
					ActiveHandle=Handle_;
					SetDrawScreen(ActiveHandle);
					return false;
				}else{ 
					return true;
				}
			}
		public://Active管理
			static bool active(const cScreen& Scr_){return active(Scr_.getHandle());}
			class scoped_active{
			private:
				int Handle;
			public:
				scoped_active(const cScreen& Scr_):Handle(ActiveHandle){active(Scr_);}
				~scoped_active(){active(Handle);}
				void inactive(){active(Handle);}
			};
		};
		//仮想画像クラス
		class cSoftImage{
		private:
			struct data{
				type::uint8 r;
				type::uint8 g;
				type::uint8 b;
				type::uint8 a;
			};
		public:
			class iterator:
				public hmLib::basic_random_access_iterator<iterator,data>,
				public hmLib::basic_operator::bi_add<iterator,Pint>,
				public hmLib::basic_operator::forward_sub<iterator,Pint>{
			private:
				type::uint8* Ptr;
				Pint Size;
			public:
				iterator():Ptr(0){return;}
				iterator(type::uint8* Ptr_,Pint Size_):Ptr(Ptr_),Size(Size_){return;}
				const iterator& operator++(){
					Ptr+=4;
					return *this;
				}
				const iterator& operator--(){
					Ptr-=4;
					return *this;
				}
				const iterator& operator+=(const int& num_){
					Ptr+=4*num_;
					return *this;
				}
				const iterator& operator-=(const int& num_){
					Ptr-=4*num_;
					return *this;
				}
				const iterator& operator+=(const Pint& num_){
					Ptr+=4*(num_.x+Size.x*num_.y);
					return *this;
				}
				const iterator& operator-=(const Pint& num_){
					Ptr-=4*(num_.x+Size.x*num_.y);
					return *this;
				}
				bool operator==(const iterator& itr_)const{return Ptr==itr_.Ptr;}
				bool operator<(const iterator& itr_)const{return Ptr<itr_.Ptr;}
				data operator*(){return *reinterpret_cast<data*>(Ptr);}
				data operator[](const int& num_){return *reinterpret_cast<data*>(Ptr+4*num_);}
				data operator[](const Pint& num_){return *reinterpret_cast<data*>(Ptr+4*(num_.x+Size.x*num_.y));}
				data* operator->(){return reinterpret_cast<data*>(Ptr);}
			};
		private:
			int Handle;
		public:
			cSoftImage(){Handle=-1;}
			cSoftImage(const cSoftImage& _imgra){throw "cSoftImage::cSoftImage(const cSoftImage& _imgra) Unable to use this function";}
			~cSoftImage(){release();}
			void release(){
				if(Handle>0)DeleteSoftImage(Handle);
				Handle=-1;
			}
			bool create(const Pint& _size,bool Alpha_){
				release();
				if(Alpha_)Handle=MakeARGB8ColorSoftImage(_size.x,_size.y);
				else Handle=MakeXRGB8ColorSoftImage(_size.x,_size.y);
				return empty();
			}
			bool load(const std::string& _filename){
				release();
				Handle=LoadSoftImage(_filename.c_str());
				//Palette付の画像の場合は、破棄
				if(CheckPaletteSoftImage(Handle))release();
				return empty();
			}
			bool load(const std::string& _filename,bool Alpha_){
				cSoftImage tmpim;
				tmpim.load(_filename);
				if(tmpim.isAlpha()==Alpha_)swap(tmpim);
				else load(tmpim,Alpha_);
				return empty();
			}
			bool load(void* _mem,const int& _memsize){
				release();
				Handle=LoadSoftImageToMem(_mem,_memsize);
				if(CheckPaletteSoftImage(Handle)!=0)release();
				return empty();
			}
			bool load(void* _mem,const int& _memsize,bool Alpha_){
				cSoftImage tmpim;
				tmpim.load(_mem,_memsize);
				if(tmpim.isAlpha()==Alpha_)swap(tmpim);
				else load(tmpim,Alpha_);
				return empty();
			}
			bool load(const cImage& Img_,bool Alpha_){
				cScreen Scr;
				Scr.load(Img_);
				return load(Scr,Alpha_);
			}
			bool load(const cImage& Img_,const Pint& _pos,const Pint& _size,bool Alpha_){
				cScreen Scr;
				Scr.load(Img_);
				return load(Scr,_pos,_size,Alpha_);
			}
			bool load(const cScreen& Scr_,bool Alpha_){
				return load(Scr_,Pint(0,0),Scr_.size(),Alpha_);
			}
			bool load(const cScreen& Scr_,const Pint& _pos,const Pint& _size,bool Alpha_);
			bool load(const cSoftImage& SfImg_){
				return load(SfImg_,SfImg_.isAlpha());
			}
			bool load(const cSoftImage& SfImg_,bool Alpha_){
				return load(SfImg_,Pint(0,0),SfImg_.size(),SfImg_.isAlpha());
			}
			bool load(const cSoftImage& SfImg_,const Pint& _pos,const Pint& _size,bool Alpha_){
				if(this==&SfImg_)return true;
				create(_size,Alpha_);
				BltSoftImage(_pos.x,_pos.y,_size.x,_size.y,SfImg_.getHandle(),0,0,Handle);
				return empty();
			}
			virtual void save(const std::string& _filename,cImage::FileType _ft)const;
			const cSoftImage& operator=(const cSoftImage& _imgrp){throw "const cSoftImage& cSoftImage::operator=(const cSoftImage& _imgrp) Unable to use this function";}
			bool empty()const{return (Handle<=0);}
			Pint size()const{
				Pint tmp;
				GetSoftImageSize(Handle,&(tmp.x),&(tmp.y));
				return tmp;
			}
			int getHandle()const{return Handle;}
			bool isAlpha()const{return  CheckAlphaSoftImage(Handle)!=0;}
			void fill(const hm::cRGBAClr& AClr_){
				FillSoftImage(Handle,AClr_.r,AClr_.g,AClr_.b,AClr_.a);
			}
			void fillClr(const hm::cRGBClr& Clr_){
				iterator itr=begin(),enditr=end();
				for(;itr!=enditr;++itr){
					itr->r=Clr_.r;
					itr->g=Clr_.g;
					itr->b=Clr_.b;
				}
			}
			void fillAlpha(type::uint8 Alpha_){
				iterator itr=begin(),enditr=end();
				for(;itr!=enditr;++itr){
					itr->a=Alpha_;
				}
			}
			void put(const Pint& _pos,const hm::cRGBAClr& AClr_){
				iterator itr=begin()+_pos;
				itr->r=AClr_.r;
				itr->g=AClr_.g;
				itr->b=AClr_.b;
				itr->a=AClr_.a;
			}
			void putClr(const Pint& _pos,const hm::cRGBClr& Clr_){
				iterator itr=begin()+_pos;
				itr->r=Clr_.r;
				itr->g=Clr_.g;
				itr->b=Clr_.b;
			}
			void putAlpha(const Pint& _pos,type::uint8 _alpha){
				iterator itr=begin()+_pos;
				itr->a=_alpha;
			}
			hm::cRGBAClr get(const Pint& _pos){
				iterator itr=begin()+_pos;
				return hm::cRGBAClr(itr->r,itr->g,itr->b,itr->a);
			}
			hm::cRGBClr getClr(const Pint& _pos){
				iterator itr=begin()+_pos;
				return hm::cRGBClr(itr->r,itr->g,itr->b);
			}
			type::uint8 getAlpha(const Pint& _pos){
				iterator itr=begin()+_pos;
				return itr->a;
			}
			iterator begin(){return iterator(static_cast<type::uint8*>(GetImageAddressSoftImage(Handle)),size());}
			iterator end(){return iterator(static_cast<type::uint8*>(GetImageAddressSoftImage(Handle)),size())+size();}
			void swap(cSoftImage& SfImg_){
				int tmp=Handle;
				Handle=SfImg_.Handle;
				SfImg_.Handle=tmp;
			}
		};
	}
}
#
#endif
