#ifndef INC_HMLIB_DXIMAGE_CPP
#define INC_HMLIB_DXIMAGE_CPP 100
#
#ifndef INC_HMLIB_DXIMAGE
#	include "hmLib_v2/dxImage.hpp"
#endif
#ifndef INC_HMLIB_DXDRAW
#	include "hmLib_v2/dxDrawCtrl.hpp"
#endif
namespace hmLib{
	namespace dx{
		const cScreen cScreen::DefaultFrontScreen(DX_SCREEN_FRONT);
		const cScreen cScreen::DefaultBackScreen(DX_SCREEN_BACK);
		int cScreen::ActiveHandle=0;
		const cScreen& MainScreen(cScreen::DefaultBackScreen);
		//cImageä÷êîåQ
		bool cImage::load(const cImage& Img_){
			return load(Img_,Pint(0,0),Img_.size());
		}
		bool cImage::load(const cImage& Img_,const Pint& _pos,const Pint& _size){
			cScreen Scr;
			Scr.load(Img_);
			return load(Scr,_pos,_size);
		}
		bool cImage::load(const cScreen& Scr_){
			return load(Scr_,Pint(0,0),Scr_.size());
		}
		bool cImage::load(const cScreen& Scr_,const Pint& _pos,const Pint& _size){
			create(_size);
			cScreen::scoped_active TmpScr(Scr_);
			cArea Ar;
			cBlend Bl;
			cBright Br;
			cArea::scoped_active TmpArea(Ar);
			cBlend::scoped_active TmpBlend(Bl);
			cBright::scoped_active TmpBright(Br);
			GetDrawScreenGraph(_pos.x,_pos.y,_pos.x+_size.x,_pos.y+_size.y,Handle);
			return empty();
		}
		bool cImage::load(const cSoftImage& SfImg_){
			release();
			Handle=CreateGraphFromSoftImage(SfImg_.getHandle());
			return empty();
		}
		bool cImage::load(const cSoftImage& SfImg_,const Pint& _pos,const Pint& _size){
			cScreen Scr;
			Scr.load(SfImg_);
			return load(Scr,_pos,_size);
		}		
		bool cSoftImage::load(const cScreen& Scr_,const Pint& _pos,const Pint& _size,bool Alpha_){
			create(_size,Alpha_);
			cScreen::scoped_active TmpScr(Scr_);
			cArea Ar;
			cBlend Bl;
			cBright Br;
			cArea::scoped_active TmpArea(Ar);
			cBlend::scoped_active TmpBlend(Bl);
			cBright::scoped_active TmpBright(Br);
			GetDrawScreenSoftImage(_pos.x,_pos.y,_pos.x+_size.x,_pos.y+_size.y,Handle);
			return empty();
		}
		void cImage::save(const std::string& _filename,cImage::FileType _ft)const{
			cScreen Scr;
			Scr.load(*this);
			Scr.save(_filename,_ft);
		}
		void cScreen::save(const std::string& _filename,cImage::FileType _ft)const{
			cScreen::scoped_active TmpScr(*this);
			cArea Ar;
			cBlend Bl;
			cBright Br;
			cArea::scoped_active TmpArea(Ar);
			cBlend::scoped_active TmpBlend(Bl);
			cBright::scoped_active TmpBright(Br);
			Pint Size=size();
			if(_ft==cImage::bmp)SaveDrawScreenToBMP(0,0,Size.x,Size.y,_filename.c_str());
			else if(_ft==cImage::jpeg)SaveDrawScreenToJPEG(0,0,Size.x,Size.y,_filename.c_str());
			else if(_ft==cImage::png)SaveDrawScreenToPNG(0,0,Size.x,Size.y,_filename.c_str());
		}
		void cSoftImage::save(const std::string& _filename,cImage::FileType _ft)const{
			cScreen Scr;
			Scr.load(*this);
			Scr.save(_filename,_ft);
		}
	}
}
#
#endif
