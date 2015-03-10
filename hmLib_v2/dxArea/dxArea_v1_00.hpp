#ifndef INC_HMLIB_DXAREA
#define INC_HMLIB_DXAREA 100
#
#ifndef INC_HMLIB_DXOBJECT
#	include "hmLib_v2/dxObject.hpp"
#endif
#pragma warning(disable:4250)
class dxArea:public dxBasicObject{
protected:
	I_pFunc_rDXO DrawFunc;
public:
	Pint size;
private:
	void local_set(I_pFunc_rDXO _DrawFunc,const Pint& _size){
		DrawFunc=_DrawFunc;
		size=_size;
	}
	void local_copy(const dxArea& _area){
		DrawFunc=_area.DrawFunc;
		size=_area.size;
	}
protected:
	void copy(const dxArea& _area){
		local_copy(_area);
	}
public:
	dxArea(){local_set(0,Pint());}
	dxArea(I_pFunc_rDXO _DrawFunc,const Pint& _size){local_set(_DrawFunc,_size);}
	dxArea(const dxArea& _area){local_copy(_area);}
	void set(I_pFunc_rDXO _DrawFunc,const Pint& _size){local_set(_DrawFunc,_size);}
	dxArea& operator=(const dxArea& _area){
		if(this!=&_area)copy(_area);
		return *this;
	}
	virtual int draw(Pint _pos,dxDMode _mode=dxDMode(),bool _active=true){
//		dx::TmpGlobalStd gstd(_pos+dx::getGlobalStd());
		dx::TmpDrawArea DA(dxDArea(Rint(_pos,_pos+size))&dx::getDrawArea());
//		dx::TmpDrawMode DM(_mode+dx::getDrawMode());
//		dx::draw(_pos,dxoBox(size,dxHLS(0,128,128)),128);
		dxO dxo(_pos,_mode,_active,this);
		return DrawFunc(dxo);
	}
};
class basic_dxAreaType{
public:
	virtual int drawIn(dxO& _dxo,I_pFunc_rDXO _DrawFunc)=0;
	virtual Pint FtoA(const Pint& _fpos,const dxO& _dxo)const=0;
	virtual Pint AtoF(const Pint& _apos,const dxO& _dxo)const=0;
	virtual Pint GtoA(const Pint& _gpos,const dxO& _dxo)const{return FtoA(GtoF(_gpos,_dxo),_dxo);}
	virtual Pint AtoG(const Pint& _apos,const dxO& _dxo)const{return FtoG(AtoF(_apos,_dxo),_dxo);}
	virtual Pint GtoF(const Pint& _gpos,const dxO& _dxo)const{return _dxo.GtoD(_gpos);}
	virtual Pint FtoG(const Pint& _fpos,const dxO& _dxo)const{return _dxo.DtoG(_fpos);}
};
class dxNormAreaType:virtual public basic_dxAreaType{
protected:
	virtual int drawIn(dxO& _dxo,I_pFunc_rDXO _DrawFunc){return _DrawFunc(_dxo);}
public:
	virtual Pint FtoA(const Pint& _fpos,const dxO& _dxo)const{return _fpos;}
	virtual Pint AtoF(const Pint& _apos,const dxO& _dxo)const{return _apos;}
};
class basic_dxMoveAreaType:virtual public basic_dxAreaType{
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
	basic_dxMoveAreaType(Pint& _size):MoveAreaSize(_size){
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
class dxMoveAreaType:public basic_dxMoveAreaType{
protected:
	dxRGB& MoveAreaClr;
protected:
	virtual int drawIn(dxO& _dxo,I_pFunc_rDXO _DrawFunc){
		_dxo.movePos(-viewpos);
		int ans=_DrawFunc(_dxo);
		Rint rect=_dxo.getArea();
		adjDrawArea(rect);
		_dxo.movePos(viewpos);

		if(rect.pos1.x<viewpos.x){
			dxoBox tmpbox(Pint(10,MoveAreaSize.y),MoveAreaClr);
			if(tmpbox.pIn(Pint(0,0),_dxo.GtoD(dx::getMosPos())) && _dxo.checkActive()){
				_dxo.draw(Pint(0,0),tmpbox,196);
				_dxo.draw(Pint(0,MoveAreaSize.y/2),dxoTriangle(Pint(10,-7),Pint(10,7),MoveAreaClr),196);
				if(dx::getMosL()>0)viewpos.x-=10;
				else if(dx::getMosR()>0)viewpos.x=rect.pos1.x;
				else viewpos.x-=2;
			}else{
				_dxo.draw(Pint(0,0),tmpbox,128);
				_dxo.draw(Pint(0,MoveAreaSize.y/2),dxoTriangle(Pint(10,-7),Pint(10,7),MoveAreaClr),128);
			}
		}
		if(rect.pos1.y<viewpos.y){
			dxoBox tmpbox(Pint(MoveAreaSize.x,10),MoveAreaClr);
			if(tmpbox.pIn(Pint(0,0),_dxo.GtoD(dx::getMosPos())) && _dxo.checkActive()){
				_dxo.draw(Pint(0,0),tmpbox,196);
				_dxo.draw(Pint(MoveAreaSize.x/2,0),dxoTriangle(Pint(-7,10),Pint(7,10),MoveAreaClr),196);
				if(dx::getMosL()>0)viewpos.y-=10;
				else if(dx::getMosR()>0)viewpos.y=rect.pos1.y;
				else viewpos.y-=2;
			}else{
				_dxo.draw(Pint(0,0),tmpbox,128);
				_dxo.draw(Pint(MoveAreaSize.x/2,0),dxoTriangle(Pint(-7,10),Pint(7,10),MoveAreaClr),128);
			}
		}
		if(rect.pos2.x>viewpos.x+MoveAreaSize.x){
			dxoBox tmpbox(Pint(10,MoveAreaSize.y),MoveAreaClr);
			if(tmpbox.pIn(Pint(MoveAreaSize.x-10,0),_dxo.GtoD(dx::getMosPos())) && _dxo.checkActive()){
				_dxo.draw(Pint(MoveAreaSize.x-10,0),tmpbox,196);
				_dxo.draw(Pint(MoveAreaSize.x,MoveAreaSize.y/2),dxoTriangle(Pint(-10,-7),Pint(-10,7),MoveAreaClr),196);
				if(dx::getMosL()>0)viewpos.x+=10;
				else if(dx::getMosR()>0)viewpos.x=rect.pos2.x-MoveAreaSize.x;
				else viewpos.x+=2;
			}else{
				_dxo.draw(Pint(MoveAreaSize.x-10,0),tmpbox,128);
				_dxo.draw(Pint(MoveAreaSize.x,MoveAreaSize.y/2),dxoTriangle(Pint(-10,-7),Pint(-10,7),MoveAreaClr),128);
			}
		}
		if(rect.pos2.y>viewpos.y+MoveAreaSize.y){
			dxoBox tmpbox(Pint(MoveAreaSize.x,10),MoveAreaClr);
			if(tmpbox.pIn(Pint(0,MoveAreaSize.y-10),_dxo.GtoD(dx::getMosPos())) && _dxo.checkActive()){
				_dxo.draw(Pint(0,MoveAreaSize.y-10),tmpbox,196);
				_dxo.draw(Pint(MoveAreaSize.x/2,MoveAreaSize.y),dxoTriangle(Pint(-7,-10),Pint(7,-10),MoveAreaClr),196);
				if(dx::getMosL()>0)viewpos.y+=10;
				else if(dx::getMosR()>0)viewpos.y=rect.pos2.y-MoveAreaSize.y;
				else viewpos.y+=2;
			}else{
				_dxo.draw(Pint(0,MoveAreaSize.y-10),tmpbox,128);
				_dxo.draw(Pint(MoveAreaSize.x/2,MoveAreaSize.y),dxoTriangle(Pint(-7,-10),Pint(7,-10),MoveAreaClr),128);
			}
		}

		return 0;
	}
public:
	dxMoveAreaType(Pint& _size,dxRGB& _clr):basic_dxMoveAreaType(_size),MoveAreaClr(_clr){return;}
};
class dxScrlAreaType:public basic_dxMoveAreaType{
protected:
	dxRGB& MoveAreaClr;
protected:
	void adjDrawArea(Rint& _rect){
		basic_dxMoveAreaType::adjDrawArea(_rect);
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
	virtual int drawIn(dxO& _dxo,I_pFunc_rDXO _DrawFunc){
		if(MoveAreaSize.x<=0 || MoveAreaSize.y<=0)return 0;
		_dxo.movePos(-viewpos);
		int ans=_DrawFunc(_dxo);
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
			_dxo.draw(Pint(0,MoveAreaSize.y-10),dxoSlider(Pint(MoveAreaSize.x-10,10),tabsize,tabpos,Pint(rect.pos2.x-rect.pos1.x-MoveAreaSize.x,0),MoveAreaClr),196);
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
			_dxo.draw(Pint(MoveAreaSize.x-10,0),dxoSlider(Pint(10,MoveAreaSize.y-10),tabsize,tabpos,Pint(0,rect.pos2.y-rect.pos1.y-MoveAreaSize.y),MoveAreaClr),196);
//			dxo::Draw(dxoTab(pos+Pint(MoveAreaSize.x-10,0),Pint(10,MoveAreaSize.y-10),tabsize,tabpos,Pint(0,rect.pos2.y-rect.pos1.y-MoveAreaSize.y),Color,128,FrameTabBlend*AreaBlend/255));
			viewpos.y=tabpos.y+rect.pos1.y;
		}

		if(!(rect.pos2-rect.pos1<=MoveAreaSize)){
			_dxo.draw(MoveAreaSize-Pint(10,10),dxoButIO(Pint(10,10),"",MoveAreaClr,false),196);
//			dxo::Draw(dxoButIO(pos+MoveAreaSize-Pint(10,10),Pint(10,10),Color,1,"",0,POS::center,FrameTabBlend*AreaBlend/255,0));
		}
		return 0;
	}
public:
	dxScrlAreaType(Pint& _size,dxRGB& _clr):basic_dxMoveAreaType(_size),MoveAreaClr(_clr){return;}
};
class basic_dxFrame:public dxArea,virtual protected basic_dxAreaType{
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
	void local_copy(const basic_dxFrame& _frame){
//		status=1;
		Clr=_frame.Clr;
		Out=_frame.Out;
		In=_frame.In;
		Passive=_frame.Passive;
	}
protected:
	void copy(const basic_dxFrame& _frame){
		dxArea::copy(_frame);
		local_copy(_frame);
	}
public:
	basic_dxFrame():dxArea(){local_set(dxRGB(0,0,0),dxDMode(),dxDMode(),dxDMode());}
	basic_dxFrame(I_pFunc_rDXO _DrawFunc,const Pint& _size,const dxRGB& _Clr,
		const dxDMode& _Out=dxDMode(196),const dxDMode& _In=dxDMode(128),const dxDMode& _Passive=dxDMode(128))
		:dxArea(_DrawFunc,_size){local_set(_Clr,_Out,_In,_Passive);}
	basic_dxFrame(const basic_dxFrame& _frame):dxArea(_frame){local_copy(_frame);}
	void set(I_pFunc_rDXO _DrawFunc,const Pint& _size,const dxRGB& _Clr,
		const dxDMode& _Out=dxDMode(196),const dxDMode& _In=dxDMode(128),const dxDMode& _Passive=dxDMode(128)){
		dxArea::set(_DrawFunc,_size);
		local_set(_Clr,_Out,_In,_Passive);
	}
	basic_dxFrame& operator=(const basic_dxFrame& _fra){
		if(this!=&_fra)copy(_fra);
		return *this;
	}
	virtual int draw(Pint _pos,dxDMode _mode=dxDMode(),bool _active=true){
//		if(status<0)return 0;
//		else if(status==0)_mode+=Passive;
		if(!_active)_mode+=Passive;
		dx::drawBox(_pos,_pos+size,Clr,1,_mode+In);
		dx::drawBox(_pos,_pos+size,Clr,0,_mode+Out);
		dx::TmpDrawArea DA(dxDArea(Rint(_pos,_pos+size))&dx::getDrawArea());
//		dxO dxo(_pos,_mode,status>0,this);
		dxO dxo(_pos,_mode,_active,this);
		return drawIn(dxo,DrawFunc);
	}
};
class basic_dxWindow:public basic_dxFrame{
public:
	std::string name;
private:
	void local_set(const std::string& _name){
		name=_name;
	}
	void local_copy(const basic_dxWindow& _win){
		name=_win.name;
	}
protected:
	void copy(const basic_dxWindow& _win){
		basic_dxFrame::copy(_win);
		local_copy(_win);
	}
public:
	basic_dxWindow():basic_dxFrame(){local_set("");}
	basic_dxWindow(I_pFunc_rDXO _DrawFunc,const Pint& _size,const dxRGB& _Clr,const std::string& _name="",
		const dxDMode& _Out=dxDMode(196),const dxDMode& _In=dxDMode(128),const dxDMode& _Passive=dxDMode(128))
		:basic_dxFrame(_DrawFunc,_size,_Clr,_Out,_In,_Passive){
			local_set(_name);
	}
	basic_dxWindow(const basic_dxWindow& _win):basic_dxFrame(_win){local_copy(_win);}
	basic_dxWindow& operator=(const basic_dxWindow& _win){
		if(this!=&_win)copy(_win);
		return *this;
	}
	void set(I_pFunc_rDXO _DrawFunc,const Pint& _size,const dxRGB& _Clr,const std::string& _name="",
		const dxDMode& _Out=dxDMode(196),const dxDMode& _In=dxDMode(128),const dxDMode& _Passive=dxDMode(128)){
			basic_dxFrame::set(_DrawFunc,_size,_Clr,_Out,_In,_Passive);
			local_set(_name);
	}
	virtual int draw(Pint _pos,dxDMode _mode=dxDMode(),bool _active=true){
		if(!_active)_mode+=Passive;
		dx::drawBox(_pos,_pos+size,Clr,1,_mode+In);
		dx::drawBox(_pos,_pos+size,Clr,0,_mode+Out);
		dx::drawBox(_pos+Pint(0,dx::getFontSize()*-1),_pos+Pint(size.x,0),Clr,1,_mode+Out);
		dx::drawStr(_pos+Pint(2,dx::getFontSize()*-1),name,dxRGB(255,255,255),_mode+Out);
		dx::TmpDrawArea DA(dxDArea(Rint(_pos,_pos+size))&dx::getDrawArea());
		dxO dxo(_pos,_mode,_active,this);
		return drawIn(dxo,DrawFunc);
	}
};
class dxNormFrame:public basic_dxFrame,public dxNormAreaType{
protected:
public:
	dxNormFrame():basic_dxFrame(),dxNormAreaType(){return;}
	dxNormFrame(I_pFunc_rDXO _DrawFunc,const Pint& _size,const dxRGB& _Clr,
		dxDMode _Out=dxDMode(196),dxDMode _In=dxDMode(128),dxDMode _Passive=dxDMode(128))
		:basic_dxFrame(_DrawFunc,_size,_Clr,_Out,_In,_Passive),dxNormAreaType(){return;}
};
class dxMoveFrame:public basic_dxFrame,public dxMoveAreaType{
protected:
public:
	dxMoveFrame():basic_dxFrame(),dxMoveAreaType(basic_dxFrame::size,basic_dxFrame::Clr){return;}
	dxMoveFrame(I_pFunc_rDXO _DrawFunc,const Pint& _size,const dxRGB& _Clr,
		dxDMode _Out=dxDMode(196),dxDMode _In=dxDMode(128),dxDMode _Passive=dxDMode(128))
		:basic_dxFrame(_DrawFunc,_size,_Clr,_Out,_In,_Passive),dxMoveAreaType(basic_dxFrame::size,basic_dxFrame::Clr){return;}
};
class dxScrlFrame:public basic_dxFrame,public dxScrlAreaType{
protected:
public:
	dxScrlFrame():basic_dxFrame(),dxScrlAreaType(basic_dxFrame::size,basic_dxFrame::Clr){return;}
	dxScrlFrame(I_pFunc_rDXO _DrawFunc,const Pint& _size,const dxRGB& _Clr,
		dxDMode _Out=dxDMode(196),dxDMode _In=dxDMode(128),dxDMode _Passive=dxDMode(128))
		:basic_dxFrame(_DrawFunc,_size,_Clr,_Out,_In,_Passive),dxScrlAreaType(basic_dxFrame::size,basic_dxFrame::Clr){return;}
};
class dxNormWindow:public basic_dxWindow,public dxNormAreaType{
protected:
public:
	dxNormWindow():basic_dxWindow(),dxNormAreaType(){return;}
	dxNormWindow(I_pFunc_rDXO _DrawFunc,const Pint& _size,const dxRGB& _Clr,const std::string& _name="",
		dxDMode _Out=dxDMode(196),dxDMode _In=dxDMode(128),dxDMode _Passive=dxDMode(128))
		:basic_dxWindow(_DrawFunc,_size,_Clr,_name,_Out,_In,_Passive),dxNormAreaType(){return;}
};
class dxMoveWindow:public basic_dxWindow,public dxMoveAreaType{
protected:
public:
	dxMoveWindow():basic_dxWindow(),dxMoveAreaType(basic_dxWindow::size,basic_dxWindow::Clr){return;}
	dxMoveWindow(I_pFunc_rDXO _DrawFunc,const Pint& _size,const dxRGB& _Clr,const std::string& _name="",
		dxDMode _Out=dxDMode(196),dxDMode _In=dxDMode(128),dxDMode _Passive=dxDMode(128))
		:basic_dxWindow(_DrawFunc,_size,_Clr,_name,_Out,_In,_Passive),dxMoveAreaType(basic_dxWindow::size,basic_dxWindow::Clr){return;}
};
class dxScrlWindow:public basic_dxWindow,public dxScrlAreaType{
protected:
public:
	dxScrlWindow():basic_dxWindow(),dxScrlAreaType(basic_dxWindow::size,basic_dxWindow::Clr){return;}
	dxScrlWindow(I_pFunc_rDXO _DrawFunc,const Pint& _size,const dxRGB& _Clr,const std::string& _name="",
		dxDMode _Out=dxDMode(196),dxDMode _In=dxDMode(128),dxDMode _Passive=dxDMode(128))
		:basic_dxWindow(_DrawFunc,_size,_Clr,_name,_Out,_In,_Passive),dxScrlAreaType(basic_dxWindow::size,basic_dxWindow::Clr){return;}
};
#
#endif
