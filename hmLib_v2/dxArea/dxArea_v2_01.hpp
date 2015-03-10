#ifndef INC_HMLIB_DXAREA
#define INC_HMLIB_DXAREA 201
#
#ifndef INC_HMLIB_DXOBJECT
#	include "hmLib_v2/dxObject.hpp"
#endif
#pragma warning(disable:4250)
template<class DrawFuncType>
class dxArea_:public dxBasicObject{
protected:
	DrawFuncType* pDrawFunc;
public:
	Pint size;
private:
	void local_set(DrawFuncType& _DrawFunc,const Pint& _size){
		pDrawFunc=&_DrawFunc;
		size=_size;
	}
	void local_copy(const dxArea_<DrawFuncType>& _area){
		pDrawFunc=_area.pDrawFunc;
		size=_area.size;
	}
protected:
	void copy(const dxArea_<DrawFuncType>& _area){
		local_copy(_area);
	}
public:
	dxArea_(){local_set(0,Pint());}
	dxArea_(DrawFuncType& _DrawFunc,const Pint& _size){local_set(_DrawFunc,_size);}
	dxArea_(const dxArea_<DrawFuncType>& _area){local_copy(_area);}
	void set(DrawFuncType& _DrawFunc,const Pint& _size){local_set(_DrawFunc,_size);}
	dxArea_<DrawFuncType>& operator=(const dxArea_<DrawFuncType>& _area){
		if(this!=&_area)copy(_area);
		return *this;
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
//		dx::TmpGlobalStd gstd(_pos+dx::getGlobalStd());
		dx::TmpDrawArea DA(dxDArea(Rint(_pos,_pos+size))&dx::getDrawArea());
//		dx::TmpDrawMode DM(_mode+dx::getDrawMode());
//		dx::draw(_pos,dxoBox(size,dxHLS(0,128,128)),128);
		dxO dxo(_pos,_active,_mode,this);
		return (*pDrawFunc)(dxo);
	}
};
template<class DrawFuncType>
class basic_dxAreaType_{
public:
	virtual int drawIn(dxO& _dxo,DrawFuncType* _pDrawFunc)=0;
	virtual Pint FtoA(const Pint& _fpos,const dxO& _dxo)const=0;
	virtual Pint AtoF(const Pint& _apos,const dxO& _dxo)const=0;
	virtual Pint GtoA(const Pint& _gpos,const dxO& _dxo)const{return FtoA(GtoF(_gpos,_dxo),_dxo);}
	virtual Pint AtoG(const Pint& _apos,const dxO& _dxo)const{return FtoG(AtoF(_apos,_dxo),_dxo);}
	virtual Pint GtoF(const Pint& _gpos,const dxO& _dxo)const{return _dxo.GtoD(_gpos);}
	virtual Pint FtoG(const Pint& _fpos,const dxO& _dxo)const{return _dxo.DtoG(_fpos);}
};
template<class DrawFuncType>
class dxNormAreaType_:virtual public basic_dxAreaType_<DrawFuncType>{
protected:
	virtual int drawIn(dxO& _dxo,DrawFuncType* _pDrawFunc){return (*_pDrawFunc)(_dxo);}
public:
	virtual Pint FtoA(const Pint& _fpos,const dxO& _dxo)const{return _fpos;}
	virtual Pint AtoF(const Pint& _apos,const dxO& _dxo)const{return _apos;}
};
template<class DrawFuncType>
class basic_dxMoveAreaType_:virtual public basic_dxAreaType_<DrawFuncType>{
protected:
	Rint moveable;
	bool moveableF[4];//LURD
	Pint& MoveAreaSize;
public:
	Pint viewpos;
protected:
	void adjDrawArea(Rint& _rect){
		//移動可能範囲による、タブ移動可能位置の修正
		if(moveableF[0])_rect.pos1.x=moveable.pos1.x;
		if(moveableF[1])_rect.pos1.y=moveable.pos1.y;
		if(moveableF[2])_rect.pos2.x=moveable.pos2.x;
		if(moveableF[3])_rect.pos2.y=moveable.pos2.y;
	}
public:
	basic_dxMoveAreaType_(Pint& _size):MoveAreaSize(_size){
		moveable.pos1=Pint(0,0);
		moveableF[0]=true;
		moveableF[1]=true;
		moveableF[2]=false;
		moveableF[3]=false;
	}
	virtual Pint FtoA(const Pint& _fpos,const dxO& _dxo)const{return _fpos+viewpos;}
	virtual Pint AtoF(const Pint& _apos,const dxO& _dxo)const{return _apos-viewpos;}
	void setMoveArea(const ALI::ALIGN& _ali,const Pint& _pos){
		if(_ali&ALI::top){
			moveableF[1]=true;
			moveable.pos1.y=_pos.y;
		}
		if(_ali&ALI::bottom){
			moveableF[3]=true;
			moveable.pos2.y=_pos.y;
		}
		if(_ali&ALI::left){
			moveableF[0]=true;
			moveable.pos1.x=_pos.x;
		}
		if(_ali&ALI::right){
			moveableF[2]=true;
			moveable.pos2.x=_pos.x;
		}
	}
	void setMoveArea(const ALI::ALIGN& _ali,const int& _pos){
		if(_ali&ALI::top){
			moveableF[1]=true;
			moveable.pos1.y=_pos;
		}
		if(_ali&ALI::bottom){
			moveableF[3]=true;
			moveable.pos2.y=_pos;
		}
		if(_ali&ALI::left){
			moveableF[0]=true;
			moveable.pos1.x=_pos;
		}
		if(_ali&ALI::right){
			moveableF[2]=true;
			moveable.pos2.x=_pos;
		}
	}
	void resetMoveArea(const ALI::ALIGN& _ali){
		if(_ali&ALI::top){
			moveableF[1]=false;
		}
		if(_ali&ALI::bottom){
			moveableF[3]=false;
		}
		if(_ali&ALI::left){
			moveableF[0]=false;
		}
		if(_ali&ALI::right){
			moveableF[2]=false;
		}
	}
	void setViewPos(const ALI::ALIGN& _ali,const Pint& _pos){
		viewpos=PintAli(_pos,MoveAreaSize,_ali);
	}
};
template<class DrawFuncType>
class dxMoveAreaType_:public basic_dxMoveAreaType_<DrawFuncType>{
protected:
	dxRGB& MoveAreaClr;
protected:
	virtual int drawIn(dxO& _dxo,DrawFuncType* _pDrawFunc){
		_dxo.movePos(-viewpos);
		int ans=(*_pDrawFunc)(_dxo);
		Rint rect=_dxo.getArea();
		adjDrawArea(rect);
		_dxo.movePos(viewpos);

		if(rect.pos1.x<viewpos.x){
			dxoBox tmpbox(Pint(10,MoveAreaSize.y),MoveAreaClr);
			if(tmpbox.pIn(Pint(0,0),_dxo.GtoD(dx::getMosPos())) && _dxo.checkActive()){
				_dxo.draw(Pint(0,0),tmpbox,true,196);
				_dxo.draw(Pint(0,MoveAreaSize.y/2),dxoTriangle(Pint(10,-7),Pint(10,7),MoveAreaClr),true,196);
				if(dx::getMosL()>0)viewpos.x-=10;
				else if(dx::getMosR()>0)viewpos.x=rect.pos1.x;
				else viewpos.x-=2;
			}else{
				_dxo.draw(Pint(0,0),tmpbox,true,128);
				_dxo.draw(Pint(0,MoveAreaSize.y/2),dxoTriangle(Pint(10,-7),Pint(10,7),MoveAreaClr),true,128);
			}
		}
		if(rect.pos1.y<viewpos.y){
			dxoBox tmpbox(Pint(MoveAreaSize.x,10),MoveAreaClr);
			if(tmpbox.pIn(Pint(0,0),_dxo.GtoD(dx::getMosPos())) && _dxo.checkActive()){
				_dxo.draw(Pint(0,0),tmpbox,true,196);
				_dxo.draw(Pint(MoveAreaSize.x/2,0),dxoTriangle(Pint(-7,10),Pint(7,10),MoveAreaClr),true,196);
				if(dx::getMosL()>0)viewpos.y-=10;
				else if(dx::getMosR()>0)viewpos.y=rect.pos1.y;
				else viewpos.y-=2;
			}else{
				_dxo.draw(Pint(0,0),tmpbox,true,128);
				_dxo.draw(Pint(MoveAreaSize.x/2,0),dxoTriangle(Pint(-7,10),Pint(7,10),MoveAreaClr),true,128);
			}
		}
		if(rect.pos2.x>viewpos.x+MoveAreaSize.x){
			dxoBox tmpbox(Pint(10,MoveAreaSize.y),MoveAreaClr);
			if(tmpbox.pIn(Pint(MoveAreaSize.x-10,0),_dxo.GtoD(dx::getMosPos())) && _dxo.checkActive()){
				_dxo.draw(Pint(MoveAreaSize.x-10,0),tmpbox,true,196);
				_dxo.draw(Pint(MoveAreaSize.x,MoveAreaSize.y/2),dxoTriangle(Pint(-10,-7),Pint(-10,7),MoveAreaClr),true,196);
				if(dx::getMosL()>0)viewpos.x+=10;
				else if(dx::getMosR()>0)viewpos.x=rect.pos2.x-MoveAreaSize.x;
				else viewpos.x+=2;
			}else{
				_dxo.draw(Pint(MoveAreaSize.x-10,0),tmpbox,true,128);
				_dxo.draw(Pint(MoveAreaSize.x,MoveAreaSize.y/2),dxoTriangle(Pint(-10,-7),Pint(-10,7),MoveAreaClr),true,128);
			}
		}
		if(rect.pos2.y>viewpos.y+MoveAreaSize.y){
			dxoBox tmpbox(Pint(MoveAreaSize.x,10),MoveAreaClr);
			if(tmpbox.pIn(Pint(0,MoveAreaSize.y-10),_dxo.GtoD(dx::getMosPos())) && _dxo.checkActive()){
				_dxo.draw(Pint(0,MoveAreaSize.y-10),tmpbox,true,196);
				_dxo.draw(Pint(MoveAreaSize.x/2,MoveAreaSize.y),dxoTriangle(Pint(-7,-10),Pint(7,-10),MoveAreaClr),true,196);
				if(dx::getMosL()>0)viewpos.y+=10;
				else if(dx::getMosR()>0)viewpos.y=rect.pos2.y-MoveAreaSize.y;
				else viewpos.y+=2;
			}else{
				_dxo.draw(Pint(0,MoveAreaSize.y-10),tmpbox,true,128);
				_dxo.draw(Pint(MoveAreaSize.x/2,MoveAreaSize.y),dxoTriangle(Pint(-7,-10),Pint(7,-10),MoveAreaClr),true,128);
			}
		}

		return 0;
	}
public:
	dxMoveAreaType_(Pint& _size,dxRGB& _clr):basic_dxMoveAreaType_<DrawFuncType>(_size),MoveAreaClr(_clr){return;}
};
template<class DrawFuncType>
class dxScrlAreaType_:public basic_dxMoveAreaType_<DrawFuncType>{
protected:
	dxRGB& MoveAreaClr;
protected:
	void adjDrawArea(Rint& _rect){
		basic_dxMoveAreaType_<DrawFuncType>::adjDrawArea(_rect);
		//現在地による補正
		_rect.pos1=PintMin(_rect.pos1,viewpos);
		Pint viewFlag(0,0);
		if(_rect.pos2.x<viewpos.x+MoveAreaSize.x-10){
			_rect.pos2.x=viewpos.x+MoveAreaSize.x-10;
			viewFlag.x=1;
		}
		if(_rect.pos2.y<viewpos.y+MoveAreaSize.y-10){
			_rect.pos2.y=viewpos.y+MoveAreaSize.y-10;
			viewFlag.y=1;
		}

		//タブが両方必要
		if(_rect.pos2-_rect.pos1>MoveAreaSize){
			if(viewFlag.x==0)_rect.pos2.x+=10;
			if(viewFlag.y==0)_rect.pos2.y+=10;
		}//タブがxのみ必要
		else if(_rect.pos2.x-_rect.pos1.x>MoveAreaSize.x){
			if(viewFlag.y==0)_rect.pos2.y+=10;
			if(_rect.pos2.y-_rect.pos1.y>MoveAreaSize.y && viewFlag.x==0)_rect.pos2.x+=10;
		}//タブがyのみ必要
		else if(_rect.pos2.y-_rect.pos1.y>MoveAreaSize.y){
			if(viewFlag.x==0)_rect.pos2.x+=10;
			if(_rect.pos2.x-_rect.pos1.x>MoveAreaSize.x && viewFlag.y==0)_rect.pos2.y+=10;
		}
	}
	virtual int drawIn(dxO& _dxo,DrawFuncType* _pDrawFunc){
		if(MoveAreaSize.x<=0 || MoveAreaSize.y<=0)return 0;
		_dxo.movePos(-viewpos);
		int ans=(*_pDrawFunc)(_dxo);
		Rint rect=_dxo.getArea();
		adjDrawArea(rect);
		_dxo.movePos(viewpos);

		if(rect.pos2.x-rect.pos1.x>MoveAreaSize.x){
			Pint tabpos;
			Pint tabsize;
			tabsize.x=MoveAreaSize.x*(MoveAreaSize.x-10)/(rect.pos2.x-rect.pos1.x);
			if(tabsize.x<5)tabsize.x=5;
			tabsize.y=10;
			tabpos.x=viewpos.x-rect.pos1.x;
			tabpos.y=0;
			_dxo.draw(Pint(0,MoveAreaSize.y-10),dxoSlider(Pint(MoveAreaSize.x-10,10),tabsize,tabpos,Pint(rect.pos2.x-rect.pos1.x-MoveAreaSize.x,0),MoveAreaClr),true,196);
//			dxo::Draw(dxoTab(pos+Pint(0,MoveAreaSize.y-10),Pint(MoveAreaSize.x-10,10),tabsize,tabpos,Pint(rect.pos2.x-rect.pos1.x-MoveAreaSize.x,0),Color,128,FrameTabBlend*AreaBlend/255));
			viewpos.x=tabpos.x+rect.pos1.x;
		}

		if(rect.pos2.y-rect.pos1.y>MoveAreaSize.y){
			Pint tabpos;
			Pint tabsize;
			tabsize.y=MoveAreaSize.y*(MoveAreaSize.y-10)/(rect.pos2.y-rect.pos1.y);
			if(tabsize.y<5)tabsize.y=5;
			tabsize.x=10;
			tabpos.y=viewpos.y-rect.pos1.y;
			tabpos.x=0;
			_dxo.draw(Pint(MoveAreaSize.x-10,0),dxoSlider(Pint(10,MoveAreaSize.y-10),tabsize,tabpos,Pint(0,rect.pos2.y-rect.pos1.y-MoveAreaSize.y),MoveAreaClr),true,196);
//			dxo::Draw(dxoTab(pos+Pint(MoveAreaSize.x-10,0),Pint(10,MoveAreaSize.y-10),tabsize,tabpos,Pint(0,rect.pos2.y-rect.pos1.y-MoveAreaSize.y),Color,128,FrameTabBlend*AreaBlend/255));
			viewpos.y=tabpos.y+rect.pos1.y;
		}

		if(!(rect.pos2-rect.pos1<=MoveAreaSize)){
			_dxo.draw(MoveAreaSize-Pint(10,10),dxoButIO(Pint(10,10),"",MoveAreaClr,false),true,196);
//			dxo::Draw(dxoButIO(pos+MoveAreaSize-Pint(10,10),Pint(10,10),Color,1,"",0,POS::center,FrameTabBlend*AreaBlend/255,0));
		}
		return 0;
	}
public:
	dxScrlAreaType_(Pint& _size,dxRGB& _clr):basic_dxMoveAreaType_<DrawFuncType>(_size),MoveAreaClr(_clr){return;}
};
template<class DrawFuncType>
class basic_dxFrame_:public dxArea_<DrawFuncType>,virtual protected basic_dxAreaType_<DrawFuncType>{
protected:
//	int status;	//-:inactive 0:passive +:active
	dxRGB Clr;
	dxDMode Out,In,Passive;
private:
	void local_set(const dxRGB& _Clr,const dxDMode& _Out,const dxDMode& _In,const dxDMode& _Passive){
//		status=1;
		Clr=_Clr;
		Out=_Out;
		In=_In;
		Passive=_Passive;
	}
	void local_copy(const basic_dxFrame_<DrawFuncType>& _frame){
//		status=1;
		Clr=_frame.Clr;
		Out=_frame.Out;
		In=_frame.In;
		Passive=_frame.Passive;
	}
protected:
	void copy(const basic_dxFrame_<DrawFuncType>& _frame){
		dxArea::copy(_frame);
		local_copy(_frame);
	}
public:
	basic_dxFrame_():dxArea_<DrawFuncType>(){local_set(dxRGB(0,0,0),dxDMode(),dxDMode(),dxDMode());}
	basic_dxFrame_(DrawFuncType& _DrawFunc,const Pint& _size,const dxRGB& _Clr,
		const dxDMode& _Out=dxDMode(196),const dxDMode& _In=dxDMode(128),const dxDMode& _Passive=dxDMode(128))
		:dxArea_<DrawFuncType>(_DrawFunc,_size){local_set(_Clr,_Out,_In,_Passive);}
	basic_dxFrame_(const basic_dxFrame_<DrawFuncType>& _frame):dxArea_<DrawFuncType>(_frame){local_copy(_frame);}
	void set(DrawFuncType& _DrawFunc,const Pint& _size,const dxRGB& _Clr,
		const dxDMode& _Out=dxDMode(196),const dxDMode& _In=dxDMode(128),const dxDMode& _Passive=dxDMode(128)){
		dxArea_<DrawFuncType>::set(_DrawFunc,_size);
		local_set(_Clr,_Out,_In,_Passive);
	}
	basic_dxFrame_& operator=(const basic_dxFrame_<DrawFuncType>& _fra){
		if(this!=&_fra)copy(_fra);
		return *this;
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
//		if(status<0)return 0;
//		else if(status==0)_mode+=Passive;
		if(!_active)_mode+=Passive;
		dx::drawBox(_pos,_pos+size,Clr,1,_mode+In);
		dx::drawBox(_pos,_pos+size,Clr,0,_mode+Out);
		dx::TmpDrawArea DA(dxDArea(Rint(_pos,_pos+size))&dx::getDrawArea());
//		dxO dxo(_pos,_mode,status>0,this);
		dxO dxo(_pos,_active,_mode,this);
		return drawIn(dxo,pDrawFunc);
	}
};
template<class DrawFuncType>
class basic_dxWindow_:public basic_dxFrame_<DrawFuncType>{
public:
	std::string name;
private:
	void local_set(const std::string& _name){
		name=_name;
	}
	void local_copy(const basic_dxWindow_<DrawFuncType>& _win){
		name=_win.name;
	}
protected:
	void copy(const basic_dxWindow_<DrawFuncType>& _win){
		basic_dxFrame_<DrawFuncType>::copy(_win);
		local_copy(_win);
	}
public:
	basic_dxWindow_():basic_dxFrame_<DrawFuncType>(){local_set("");}
	basic_dxWindow_(DrawFuncType& _DrawFunc,const Pint& _size,const dxRGB& _Clr,const std::string& _name="",
		const dxDMode& _Out=dxDMode(196),const dxDMode& _In=dxDMode(128),const dxDMode& _Passive=dxDMode(128))
		:basic_dxFrame_<DrawFuncType>(_DrawFunc,_size,_Clr,_Out,_In,_Passive){
			local_set(_name);
	}
	basic_dxWindow_(const basic_dxWindow_<DrawFuncType>& _win):basic_dxFrame_<DrawFuncType>(_win){local_copy(_win);}
	basic_dxWindow_<DrawFuncType>& operator=(const basic_dxWindow_<DrawFuncType>& _win){
		if(this!=&_win)copy(_win);
		return *this;
	}
	void set(DrawFuncType& _DrawFunc,const Pint& _size,const dxRGB& _Clr,const std::string& _name="",
		const dxDMode& _Out=dxDMode(196),const dxDMode& _In=dxDMode(128),const dxDMode& _Passive=dxDMode(128)){
			basic_dxFrame_<DrawFuncType>::set(_DrawFunc,_size,_Clr,_Out,_In,_Passive);
			local_set(_name);
	}
	virtual int draw(Pint _pos,bool _active=true,dxDMode _mode=dxDMode()){
		if(!_active)_mode+=Passive;
		dx::drawBox(_pos,_pos+size,Clr,1,_mode+In);
		dx::drawBox(_pos,_pos+size,Clr,0,_mode+Out);
		dx::drawBox(_pos+Pint(0,dx::getFontSize()*-1),_pos+Pint(size.x,0),Clr,1,_mode+Out);
		dx::drawStr(_pos+Pint(2,dx::getFontSize()*-1),name,dxRGB(255,255,255),_mode+Out);
		dx::TmpDrawArea DA(dxDArea(Rint(_pos,_pos+size))&dx::getDrawArea());
		dxO dxo(_pos,_active,_mode,this);
		return drawIn(dxo,pDrawFunc);
	}
};
template<class DrawFuncType>
class dxNormFrame_:public basic_dxFrame_<DrawFuncType>,public dxNormAreaType_<DrawFuncType>{
protected:
public:
	dxNormFrame_():basic_dxFrame_<DrawFuncType>(),dxNormAreaType_<DrawFuncType>(){return;}
	dxNormFrame_(DrawFuncType& _DrawFunc,const Pint& _size,const dxRGB& _Clr,
		dxDMode _Out=dxDMode(196),dxDMode _In=dxDMode(128),dxDMode _Passive=dxDMode(128))
		:basic_dxFrame_<DrawFuncType>(_DrawFunc,_size,_Clr,_Out,_In,_Passive),dxNormAreaType_<DrawFuncType>(){return;}
};
template<class DrawFuncType>
class dxMoveFrame_:public basic_dxFrame_<DrawFuncType>,public dxMoveAreaType_<DrawFuncType>{
protected:
public:
	dxMoveFrame_():basic_dxFrame_<DrawFuncType>(),dxMoveAreaType_<DrawFuncType>(basic_dxFrame_<DrawFuncType>::size,basic_dxFrame_<DrawFuncType>::Clr){return;}
	dxMoveFrame_(DrawFuncType& _DrawFunc,const Pint& _size,const dxRGB& _Clr,
		dxDMode _Out=dxDMode(196),dxDMode _In=dxDMode(128),dxDMode _Passive=dxDMode(128))
		:basic_dxFrame_<DrawFuncType>(_DrawFunc,_size,_Clr,_Out,_In,_Passive),dxMoveAreaType_<DrawFuncType>(basic_dxFrame_<DrawFuncType>::size,basic_dxFrame_<DrawFuncType>::Clr){return;}
};
template<class DrawFuncType>
class dxScrlFrame_:public basic_dxFrame_<DrawFuncType>,public dxScrlAreaType_<DrawFuncType>{
protected:
public:
	dxScrlFrame_():basic_dxFrame_<DrawFuncType>(),dxScrlAreaType_<DrawFuncType>(basic_dxFrame::size,basic_dxFrame::Clr){return;}
	dxScrlFrame_(DrawFuncType& _DrawFunc,const Pint& _size,const dxRGB& _Clr,
		dxDMode _Out=dxDMode(196),dxDMode _In=dxDMode(128),dxDMode _Passive=dxDMode(128))
		:basic_dxFrame_<DrawFuncType>(_DrawFunc,_size,_Clr,_Out,_In,_Passive),dxScrlAreaType_<DrawFuncType>(basic_dxFrame_<DrawFuncType>::size,basic_dxFrame_<DrawFuncType>::Clr){return;}
};
template <class DrawFuncType>
class dxNormWindow_:public basic_dxWindow_<DrawFuncType>,public dxNormAreaType_<DrawFuncType>{
protected:
public:
	dxNormWindow_():basic_dxWindow_<DrawFuncType>(),dxNormAreaType_<DrawFuncType>(){return;}
	dxNormWindow_(DrawFuncType& _DrawFunc,const Pint& _size,const dxRGB& _Clr,const std::string& _name="",
		dxDMode _Out=dxDMode(196),dxDMode _In=dxDMode(128),dxDMode _Passive=dxDMode(128))
		:basic_dxWindow_<DrawFuncType>(_DrawFunc,_size,_Clr,_name,_Out,_In,_Passive),dxNormAreaType_<DrawFuncType>(){return;}
};
template<class DrawFuncType>
class dxMoveWindow_:public basic_dxWindow_<DrawFuncType>,public dxMoveAreaType_<DrawFuncType>{
protected:
public:
	dxMoveWindow_():basic_dxWindow_<DrawFuncType>(),dxMoveAreaType_<DrawFuncType>(basic_dxWindow_<DrawFuncType>::size,basic_dxWindow_<DrawFuncType>::Clr){return;}
	dxMoveWindow_(DrawFuncType& _DrawFunc,const Pint& _size,const dxRGB& _Clr,const std::string& _name="",
		dxDMode _Out=dxDMode(196),dxDMode _In=dxDMode(128),dxDMode _Passive=dxDMode(128))
		:basic_dxWindow_<DrawFuncType>(_DrawFunc,_size,_Clr,_name,_Out,_In,_Passive),dxMoveAreaType_<DrawFuncType>(basic_dxWindow_<DrawFuncType>::size,basic_dxWindow_<DrawFuncType>::Clr){return;}
};
template<class DrawFuncType>
class dxScrlWindow_:public basic_dxWindow_<DrawFuncType>,public dxScrlAreaType_<DrawFuncType>{
protected:
public:
	dxScrlWindow_():basic_dxWindow_<DrawFuncType>(),dxScrlAreaType_<DrawFuncType>(basic_dxWindow_<DrawFuncType>::size,basic_dxWindow_<DrawFuncType>::Clr){return;}
	dxScrlWindow_(DrawFuncType& _DrawFunc,const Pint& _size,const dxRGB& _Clr,const std::string& _name="",
		dxDMode _Out=dxDMode(196),dxDMode _In=dxDMode(128),dxDMode _Passive=dxDMode(128))
		:basic_dxWindow_<DrawFuncType>(_DrawFunc,_size,_Clr,_name,_Out,_In,_Passive),dxScrlAreaType_<DrawFuncType>(basic_dxWindow_<DrawFuncType>::size,basic_dxWindow_<DrawFuncType>::Clr){return;}
};
typedef dxArea_<dxFn> dxArea;
typedef basic_dxAreaType_<dxFn> basic_dxAreaType;
typedef dxNormAreaType_<dxFn> dxNormAreaType;
typedef basic_dxMoveAreaType_<dxFn> basic_dxMoveAreaType;
typedef dxMoveAreaType_<dxFn> dxMoveAreaType;
typedef dxScrlAreaType_<dxFn> dxScrlAreaType;
typedef basic_dxFrame_<dxFn> basic_dxFrame;
typedef basic_dxWindow_<dxFn> basic_dxWindow;
typedef dxNormFrame_<dxFn> dxNormFrame;
typedef dxMoveFrame_<dxFn> dxMoveFrame;
typedef dxScrlFrame_<dxFn> dxScrlFrame;
typedef dxNormWindow_<dxFn> dxNormWindow;
typedef dxMoveWindow_<dxFn> dxMoveWindow;
typedef dxScrlWindow_<dxFn> dxScrlWindow;
#
#endif
