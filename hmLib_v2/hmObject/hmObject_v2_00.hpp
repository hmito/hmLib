/*
////////////////////////////////////
hmObject.hpp	v2.0
	v2.0
		template class化
		位置後指定型追加	
////////////////////////////////////
class type
	class name=[a][b][c]
	[a]
		basic_		template class
		(nothing)	int type of template class
	[b]
		hmoP		位置先指定型
		hmo			位置後指定型
	[c]
		Null		非実体Object 通常不使用
		Pixel		点
		Line		直線
		Box			長方形
		Triangle	三角形
		Circle		円
		Oval		楕円

	example
		1.basic_hmoPLine<Type> 位置先指定型直線のtype型class
		2.hmoOval	位置後指定型楕円のint型class

naming rule
	t...	比較対照の	ex.tpos,tobj
	pos		特定地点
	size	長方形サイズ
	relpos	相対距離
	obj		object class
	r		半径

function
	basic_hmObject function
		.pIn		_tposがObject内ならtrue
		.lIn		_tpos,_trelposの直線がObject内に存在すればtrue
		.bIn		_tpos,_tsizeの長方形がObject内に存在すればtrue
		.oIn		_tposに存在する_tobjがObject内に存在すればtrue
		.thisIn		_tposに存在する_tobj内にObjectが存在すればtrue
		.oInOut		非接触なら0,_tposに存在する_tobjがObject内に存在すれば1,_tposに存在する_tobj内にObjectが存在すれば2,交差していれば3
		.getArea	存在基準(_pos)から見た、存在長方形の最小点min、最大点maxを返す
		.set		後型再設定関数
	basic_hmDirObject function
		.pDir		_tposから見たObjectの位置
		.lDir		_tpos,_trelposの直線から見たObjectの位置
		.bDir		_tpos,_tsizeの長方形から見たObjectの位置
		.oDir		_tposに存在する_tobjから見たObjectの位置
*/
#ifndef INC_HMLIB_HMOBJECT
#define INC_HMLIB_HMOBJECT 200
#ifndef INC_HMLIB_HMRECT
#	include "hmLib_v2/hmRect.hpp"
#endif
#include <algorithm>
namespace hmo{
	template<class Type>
	inline int GetBoxArea(const hmPoint<Type>& pos,const hmPoint<Type>& boxpos,const hmPoint<Type>& boxsize){
		//
		// 00 01 02
		//    __
		// 10|11|12
		//   |__|
		// 20 21 22
		//
		if(pos.x<boxpos.x){
			if(pos.y<boxpos.y)return 0;
			else if(pos.y>boxpos.y+boxsize.y-1)return 20;
			else return 10;
		}else if(pos.x>boxpos.x+boxsize.x-1){
			if(pos.y<boxpos.y)return 2;
			else if(pos.y>boxpos.y+boxsize.y-1)return 22;
			else return 12;
		}else{
			if(pos.y<boxpos.y)return 1;
			else if(pos.y>boxpos.y+boxsize.y-1)return 21;
			else return 11;
		}
	}
	template<class Type>
	inline int GetTriangleArea(const hmPoint<Type>& pos,const hmPoint<Type>& tripos1,const hmPoint<Type>& tripos2,const hmPoint<Type>& tripos3){
		//       |6/
		//       |/
		//      p1
		// 4   / |
		//    /  |   2
		//   / 0 |
		//__p2___p3____
		//5/     | 3
		///   1  |
		hmPoint<Type> a=tripos2-tripos1;
		hmPoint<Type> b=tripos3-tripos1;
		hmPoint<Type> c=pos-tripos1;

		if(area(a,b)==0){
			if(a.x*b.x<=0 && a.y*b.y<=0)return -1;
			else if(abs2(a)<abs2(b))return -2;
			else return -4;
		}

		double s=static_cast<double>(area(c,a))/static_cast<double>(area(b,a));
		double t=static_cast<double>(area(c,b))/static_cast<double>(area(a,b));

		if(s>=0.){
			if(t>=0.){
				if(s+t<=1.)return 0;
				else return 1;
			}else{
				if(s+t<=1.)return 2;
				else return 3;
			}
		}else{
			if(t>=0.){
				if(s+t<=1.)return 4;
				else return 5;
			}else return 6;
		}
	}
}
//hmObject
template <class Type>
class basic_hmObject{
public:
	virtual void getArea(hmPoint<Type>& _min,hmPoint<Type>& _max)const=0;
	virtual hmRect<Type> getArea(){
		hmRect<Type> tmp;
		getArea(tmp.pos1,tmp.pos2);
		return tmp;
	}
	virtual bool pIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos)const=0;
	virtual bool lIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _trelpos)const=0;
	virtual bool bIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _tsize)const=0;
	virtual bool oIn(const hmPoint<Type>& _pos,const basic_hmObject<Type>& _tobj,const hmPoint<Type>& _tpos)const{
		return _tobj.thisIn(_tpos,*this,_pos);
	}
	virtual Type oInout(const hmPoint<Type>& _pos,const basic_hmObject<Type>& _tobj,const hmPoint<Type>& _tpos)const{
		//非接触:0
		//含み:1
		//含まれ:2
		//交叉:3
		Type ans=0;
		if(oIn(_pos,_tobj,_tpos))ans+=1;
		if(thisIn(_pos,_tobj,_tpos))ans+=2;
		return ans;
	}
	virtual bool thisIn(const hmPoint<Type>& _pos,const basic_hmObject<Type>& _tobj,const hmPoint<Type>& _tpos)const=0;
};
template <class Type>
class basic_hmDirObject{
public:
	virtual hmPoint<Type> pDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos)const=0;
	virtual hmPoint<Type> lDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _trelpos)const=0;
	virtual hmPoint<Type> bDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _tsize)const=0;
	virtual hmPoint<Type> oDir(const hmPoint<Type>& _pos,const basic_hmDirObject<Type>& _tobj,const hmPoint<Type>& _tpos)const=0;
};
template <class Type>
class basic_hmoNull:public virtual basic_hmObject<Type>,public basic_hmDirObject<Type>{
protected:
	void copy(const basic_hmoNull<Type>& _hmo){return;}
public:
	basic_hmoNull<Type>(){return;}
	basic_hmoNull<Type>(const basic_hmoNull<Type>& _hmo){return;}
	basic_hmoNull<Type>& operator=(const basic_hmoNull<Type>& _hmo){return *this;}
	void set(){return;}
	virtual void getArea(hmPoint<Type>& _min,hmPoint<Type>& _max)const{
		_min=hmPoint<Type>(0,0);
		_max=hmPoint<Type>(0,0);
	}
	virtual bool pIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos)const{return false;}
	virtual bool lIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _trelpos)const{return false;}
	virtual bool bIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _tsize)const{return false;}
	virtual bool thisIn(const hmPoint<Type>& _pos,const basic_hmObject<Type>& _tobj,const hmPoint<Type>& _tpos)const{return false;}
	virtual hmPoint<Type> pDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos)const{return hmPoint<Type>(0,0);}
	virtual hmPoint<Type> lDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _trelpos)const{return hmPoint<Type>(0,0);}
	virtual hmPoint<Type> bDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _tsize)const{return hmPoint<Type>(0,0);}
	virtual hmPoint<Type> oDir(const hmPoint<Type>& _pos,const basic_hmDirObject<Type>& _tobj,const hmPoint<Type>& _tpos)const{return hmPoint<Type>(0,0);}
};
template <class Type>
class basic_hmoPixel:public virtual basic_hmObject<Type>,public basic_hmDirObject<Type>{
protected:
	void copy(const basic_hmoPixel<Type>& _hmo){return;}
public:
	basic_hmoPixel<Type>(){return;}
	basic_hmoPixel<Type>(const basic_hmoPixel<Type>& _hmo){copy(_hmo);}
	basic_hmoPixel<Type>& operator=(const basic_hmoPixel<Type>& _hmo){
		if(this!=&_hmo)copy(_hmo);
		return *this;
	}
	void set(){return;}
	virtual void getArea(hmPoint<Type>& _min,hmPoint<Type>& _max)const{
		_min=Pint();
		_max=Pint();
	}
	virtual bool pIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos)const{return _pos==_tpos;}
	virtual bool lIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _trelpos)const{return (_trelpos==hmPoint<Type>(0,0) && _pos==_tpos);}
	virtual bool bIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _tsize)const{return (_tsize==hmPoint<Type>(1,1) && _pos==_tpos);}
	virtual bool thisIn(const hmPoint<Type>& _pos,const basic_hmObject<Type>& _tobj,const hmPoint<Type>& _tpos)const{return _tobj.pIn(_tpos,_pos);}
	virtual hmPoint<Type> pDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos)const{return _tpos-_pos;}
	virtual hmPoint<Type> lDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _trelpos)const{
		hmPoint<Type> tmp1=_tpos-_pos;
		hmPoint<Type> tmp2=_tpos+_trelpos-_pos;
		double s=(double)(abs2(tmp2)-inprd(tmp1,tmp2))/(abs2(tmp1)+abs2(tmp2)-2*inprd(tmp1,tmp2));
		if(s<=0)return tmp2;
		else if(s>=1.)return tmp1;
		else return (hmPoint<Type>)(tmp1*s+tmp2*(1.-s));
	}
	virtual hmPoint<Type> bDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _tsize)const{
		hmPoint<Type> _tpos2=_tpos+_tsize-hmPoint<Type>(1,1);
		if(_pos.x<_tpos.x){
			if(_pos.y<_tpos.y)return _tpos-_pos;
			else if(_pos.y>_tpos2.y)return hmPointX(_tpos)+hmPointY(_tpos2)-_pos;
			else return hmPointX(_tpos.x-_pos.x);
		}else if(_pos.x>_tpos2.x){
			if(_pos.y<_tpos.y)return hmPointY(_tpos)+hmPointX(_tpos2)-_pos;
			else if(_pos.y>_tpos2.y)return _tpos2-_pos;
			else return hmPointX(_tpos2.x-_pos.x);
		}else{
			if(_pos.y<_tpos.y)return hmPointY(_tpos.y-_pos.y);
			else if(_pos.y>_tpos2.y)return hmPointY(_tpos2.y-_pos.y);
			else return hmPoint<Type>(0,0);
		}
		return hmPoint<Type>(0,0);
	}
	virtual hmPoint<Type> oDir(const hmPoint<Type>& _pos,const basic_hmDirObject<Type>& _tobj,const hmPoint<Type>& _tpos)const{
		return _tobj.pDir(_tpos,_pos)*-1;
	}
};
template <class Type>
class basic_hmoLine:public virtual basic_hmObject<Type>,public basic_hmDirObject<Type>{
protected:
	void copy(const basic_hmoLine<Type>& _hmo){relpos=_hmo.relpos;}
public:
	hmPoint<Type> relpos;
	basic_hmoLine<Type>(){set(hmPoint<Type>(0,0));}
	basic_hmoLine<Type>(const hmPoint<Type>& _relpos){set(_relpos);}
	basic_hmoLine<Type>(const basic_hmoLine<Type>& _hmo){copy(_hmo);}
	basic_hmoLine<Type>& operator=(const basic_hmoLine<Type>& _hmo){
		if(this!=&_hmo)copy(_hmo);
		return *this;
	}
	void set(const hmPoint<Type>& _relpos){relpos=_relpos;}
	virtual void getArea(hmPoint<Type>& _min,hmPoint<Type>& _max)const{
		_min=hmPointMin(relpos,hmPoint<Type>());
		_max=hmPointMax(relpos,hmPoint<Type>());
	}
	virtual bool pIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos)const{
		if((_pos-_tpos).x*(_pos+relpos-_tpos).x<=0 && area(_pos-_tpos,_pos+relpos-_tpos)==0)return true;
		else return false;
	}
	virtual bool lIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _trelpos)const{
		hmPoint<Type> c=relpos,a=_tpos-_pos,b=_tpos+_trelpos-_pos;
		//pos1が_tpos1,_tpos2直線上
		if(area(a,b)==0){
			//pos1が_tpos1,_tpos2上
			if(a.x*b.x<=0&&a.y*b.y<=0)return true;
			a-=c;
			b-=c;
			//pos1が_tpos1,_tpos2直線上で、pos2が_tpos1,_tpos2上
			if(area(a,b)==0 && a.x*b.x<=0 && a.y*b.y<=0)return true;
		}else{
			double s=(double)area(c,a)/area(b,a);
			double t=(double)area(c,b)/area(a,b);
			if(s>=0. && t>=0. && s+t>1.)return true;
		}
		return false;
	}
	virtual bool bIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _tsize)const{
		hmPoint<Type> _tpos2=_tpos+_tsize-hmPoint<Type>(1,1);

		int ans1=hmo::GetBoxArea(_pos,_tpos,_tsize);
		int ans2=hmo::GetBoxArea(_pos+relpos,_tpos,_tsize);

		if(ans1==ans2)return false;

		if(ans1%10==ans2%10){
			if(ans1%10==1)return true;
			else return false;
		}
		if(ans1/10==ans2/10){
			if(ans1/10==1)return true;
			else return false;
		}

		hmPoint<Type> tmp;
		if(ans1/10==0 && ans2%10==0 || ans2/10==0 && ans1%10==0){
			tmp=pDir(_pos,_tpos);
			if(tmp.x>=0&&tmp.y>=0)return false;
		}
		if(ans1/10==0 && ans2%10==2 || ans2/10==0 && ans1%10==2){
			tmp=pDir(_pos,hmPoint<Type>(_tpos2.x,_tpos.y));
			if(tmp.x<=0&&tmp.y>=0)return false;
		}
		if(ans1/10==2 && ans2%10==0 || ans2/10==2 && ans1%10==0){
			tmp=pDir(_pos,hmPoint<Type>(_tpos.x,_tpos2.y));
			if(tmp.x>=0&&tmp.y<=0)return false;
		}
		if(ans1/10==2 && ans2%10==2 || ans2/10==2 && ans1%10==2){
			tmp=pDir(_pos,_tpos2);
			if(tmp.x<=0&&tmp.y<=0)return false;
		}
		return true;
	}
	virtual bool thisIn(const hmPoint<Type>& _pos,const basic_hmObject<Type>& _tobj,const hmPoint<Type>& _tpos)const{return _tobj.lIn(_tpos,_pos,relpos);}
	virtual hmPoint<Type> pDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos)const{return basic_hmoPixel<Type>().lDir(_tpos,_pos,relpos)*-1.;}
	virtual hmPoint<Type> lDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _trelpos)const{
		hmPoint<Type> tmpP,ans;
		hmPoint<Type> c=relpos,a=_tpos-_pos,b=_tpos+_trelpos-_pos-relpos;

		if(area(a,b)==0){
			if(a.x*b.x<=0&&a.y*b.y<=0)return hmPoint<Type>(0,0);
			a-=c;
			b-=c;
			if(area(a,b)==0 && a.x*b.x<=0 && a.y*b.y<=0)return hmPoint<Type>(0,0);
		}else{
			Type s=area(c,a)/area(b,a);
			Type t=area(c,b)/area(a,b);
			if(s>=0. && t>=0. && s+t>1.)return hmPoint<Type>(0,0);
		}

		ans=basic_hmoPixel<Type>().lDir(_pos,_tpos,_trelpos);
		tmpP=basic_hmoPixel<Type>().lDir(_pos+relpos,_tpos,_trelpos);
		if(abs2(ans)>abs2(tmpP))ans=tmpP;
		tmpP=basic_hmoPixel<Type>().lDir(_tpos,_pos,relpos);
		if(abs2(ans)>abs2(tmpP))ans=tmpP*-1;
		tmpP=basic_hmoPixel<Type>().lDir(_tpos+_trelpos,_pos,relpos);
		if(abs2(ans)>abs2(tmpP))ans=tmpP*-1;
		return ans;
	}
	virtual hmPoint<Type> bDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _tsize)const{
		hmPoint<Type> _tpos2=_tpos+_tsize-hmPoint<Type>(1,1);

		int ans1=hmo::GetBoxArea(_pos,_tpos,_tsize);
		int ans2=hmo::GetBoxArea(_pos+relpos,_tpos,_tsize);

		if(ans1==ans2){
			if(ans1==0)return pDir(_pos,_tpos);
			if(ans1==2)return pDir(_pos,hmPoint<Type>(_tpos2.x,_tpos.y));
			if(ans1==20)return pDir(_pos,hmPoint<Type>(_tpos.x,_tpos2.y));
			if(ans1==22)return pDir(_pos,_tpos2);
			if(ans1==1)return hmPointY(_tpos.y-max(_pos.y,_pos.y+relpos.y));
			if(ans1==10)return hmPointX(_tpos.x-max(_pos.x,_pos.x+relpos.x));
			if(ans1==12)return hmPointX(_tpos2.x-min(_pos.x,_pos.x+relpos.x));
			if(ans1==21)return hmPointY(_tpos2.y-min(_pos.y,_pos.y+relpos.y));
		}

		if(ans1%10==ans2%10){
			if(ans1%10==0)return lDir(_pos,_tpos,hmPointY(_tsize.y-1));
			else if(ans1%10==2)return lDir(_pos,hmPoint<Type>(_tpos2.x,_tpos.y),hmPointY(_tsize.y-1));
			else return hmPoint<Type>(0,0);
		}
		if(ans1/10==ans2/10){
			if(ans1/10==0)return lDir(_pos,_tpos,hmPointX(_tsize.x-1));
			else if(ans1/10==2)return lDir(_pos,hmPoint<Type>(_tpos.x,_tpos2.y),hmPointX(_tsize.x-1));
			else return hmPoint<Type>(0,0);
		}

		hmPoint<Type> tmp;
		if(ans1/10==0 && ans2%10==0 || ans2/10==0 && ans1%10==0){
			tmp=pDir(_pos,_tpos);
			if(tmp.x>=0&&tmp.y>=0)return tmp;
		}
		if(ans1/10==0 && ans2%10==2 || ans2/10==0 && ans1%10==2){
			tmp=pDir(_pos,hmPoint<Type>(_tpos2.x,_tpos.y));
			if(tmp.x<=0&&tmp.y>=0)return tmp;
		}
		if(ans1/10==2 && ans2%10==0 || ans2/10==2 && ans1%10==0){
			tmp=pDir(_pos,hmPoint<Type>(_tpos.x,_tpos2.y));
			if(tmp.x>=0&&tmp.y<=0)return tmp;
		}
		if(ans1/10==2 && ans2%10==2 || ans2/10==2 && ans1%10==2){
			tmp=pDir(_pos,_tpos2);
			if(tmp.x<=0&&tmp.y<=0)return tmp;
		}
		return hmPoint<Type>(0,0);
	}
	virtual hmPoint<Type> oDir(const hmPoint<Type>& _pos,const basic_hmDirObject<Type>& _tobj,const hmPoint<Type>& _tpos)const{
		return _tobj.lDir(_tpos,_pos,relpos)*-1.;
	}
};
template <class Type>
class basic_hmoBox:public virtual basic_hmObject<Type>,public basic_hmDirObject<Type>{
protected:
	void copy(const basic_hmoBox<Type>& _hmo){size=_hmo.size;}
public:
	hmPoint<Type> size;
	basic_hmoBox<Type>(){set(hmPoint<Type>(0,0));}
	basic_hmoBox<Type>(const hmPoint<Type>& _size){set(_size);}
	basic_hmoBox<Type>(const basic_hmoBox<Type>& _hmo){copy(_hmo);}
	basic_hmoBox<Type>& operator=(const basic_hmoBox<Type>& _hmo){
		if(this!=&_hmo)copy(_hmo);
		return *this;
	}
	void set(const hmPoint<Type>& _size){size=_size;}
	virtual void getArea(hmPoint<Type>& _min,hmPoint<Type>& _max)const{
		_min=Pint();
		_max=size;
	}
	virtual bool pIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos)const{
		if(_pos<=_tpos && _tpos<_pos+size)return true;
		else return false;
	}
	virtual bool lIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _trelpos)const{
		if(pIn(_pos,_tpos) || pIn(_pos,_tpos+_trelpos))return true;
		return basic_hmoLine<Type>(_trelpos).bIn(_tpos,_pos,size);
	}
	virtual bool bIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _tsize)const{
		if(_tpos<_pos && _tpos+_tsize>_pos+size)return false;
		if(_tpos<=_pos+size-hmPoint<Type>(1,1) && _pos<=_tpos+_tsize-hmPoint<Type>(1,1))return true;
		return false;
	}
	virtual bool thisIn(const hmPoint<Type>& _pos,const basic_hmObject<Type>& _tbasic_hmobj,const hmPoint<Type>& _tpos)const{
		return _tbasic_hmobj.bIn(_tpos,_pos,size);
	}
	virtual hmPoint<Type> pDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos)const{
		return basic_hmoPixel<Type>().bDir(_tpos,_pos,size)*-1.;
	}
	virtual hmPoint<Type> lDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _trelpos)const{
		return basic_hmoLine<Type>(_trelpos).bDir(_tpos,_pos,size)*-1.;
	}
	virtual hmPoint<Type> bDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _tsize)const{
		hmPoint<Type> pos2=_pos+size-hmPoint<Type>(1.,1.);
		hmPoint<Type> tpos2=_tpos+_tsize-hmPoint<Type>(1.,1.);

		if(pos2.x<_tpos.x){
			if(pos2.y<_tpos.y)return _tpos-pos2;
			if(_pos.y>tpos2.y)return hmPoint<Type>(_tpos.x-pos2.x,tpos2.y-_pos.y);
			return hmPointX(_tpos.x-pos2.x);
		}
		if(_pos.x>tpos2.x){
			if(pos2.y<_tpos.y)return hmPoint<Type>(tpos2.x-_pos.x,_tpos.y-pos2.y);
			if(_pos.y>tpos2.y)return tpos2-_pos;
			return hmPointX(tpos2.x-_pos.x);	
		}
		if(pos2.y<_tpos.y)return hmPointY(_tpos.y-pos2.y);
		if(_pos.y>tpos2.y)return hmPointY(tpos2.y-_pos.y);
		return hmPoint<Type>(0,0);
	}
	virtual hmPoint<Type> oDir(const hmPoint<Type>& _pos,const basic_hmDirObject<Type>& _tbasic_hmo,const hmPoint<Type>& _tpos)const{
		return _tbasic_hmo.bDir(_tpos,_pos,size)*-1.;
	}
};
template <class Type>
class basic_hmoTriangle:public virtual basic_hmObject<Type>,public basic_hmDirObject<Type>{
protected:
	void copy(const basic_hmoTriangle<Type>& _hmo){
		relpos1=_hmo.relpos1;
		relpos2=_hmo.relpos2;
	}
public:
	hmPoint<Type> relpos1,relpos2;
	basic_hmoTriangle<Type>(){set(hmPoint<Type>(),hmPoint<Type>());}
	basic_hmoTriangle<Type>(const hmPoint<Type>& _relpos1,const hmPoint<Type>& _relpos2){set(_relpos1,_relpos2);}
	basic_hmoTriangle<Type>(const basic_hmoTriangle<Type>& _hmo){copy(_hmo);}
	basic_hmoTriangle<Type>& operator=(const basic_hmoTriangle<Type>& _hmo){
		if(this!=&_hmo)copy(_hmo);
		return *this;
	}
	void set(const hmPoint<Type>& _relpos1,const hmPoint<Type>& _relpos2){
		relpos1=_relpos1;
		relpos2=_relpos2;
	}
	virtual void getArea(hmPoint<Type>& _min,hmPoint<Type>& _max)const{
		_min=hmPointMin(hmPoint<Type>(),hmPointMin(relpos1,relpos2));
		_max=hmPointMax(hmPoint<Type>(),hmPointMax(relpos1,relpos2));
	}
	virtual bool pIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos)const{
		Type tmp=hmo::GetTriangleArea(_tpos,_pos,_pos+relpos1,_pos+relpos2);

		if(tmp<0){
			if(tmp==-1)return basic_hmoLine<Type>(relpos2-relpos1).pIn(_pos+relpos1,_tpos);
			if(tmp==-2)return basic_hmoLine<Type>(relpos2).pIn(_pos,_tpos);
			if(tmp==-4)return basic_hmoLine<Type>(relpos1).pIn(_pos,_tpos);
		}

		if(tmp==0)return true;
		else return false;
	}
	virtual bool lIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _trelpos)const{
		int tmp1=hmo::GetTriangleArea(_tpos,_pos,_pos+relpos1,_pos+relpos2);
		int tmp2=hmo::GetTriangleArea(_tpos+_trelpos,_pos,_pos+relpos1,_pos+relpos2);

		if(tmp1<0){
			if(tmp1==-1)return basic_hmoLine<Type>(relpos2-relpos1).lIn(_pos+relpos1,_tpos,_trelpos);
			if(tmp1==-2)return basic_hmoLine<Type>(relpos2).lIn(_pos,_tpos,_trelpos);
			if(tmp1==-4)return basic_hmoLine<Type>(relpos1).lIn(_pos,_tpos,_trelpos);
		}


		//どちらかが0
		if(tmp1==0||tmp2==0)return true;

		//1エリアに偏って配置
		if(tmp1==tmp2)return false;
		if((tmp1&tmp2)==1)return false;
		if((tmp1&tmp2)==2)return false;
		if((tmp1&tmp2)==4)return false;


		if(tmp1==1 || tmp2==1)return basic_hmoLine<Type>(relpos2-relpos1).lIn(_pos+relpos1,_tpos,_trelpos);
		if(tmp1==2 || tmp2==2)return basic_hmoLine<Type>(relpos2).lIn(_pos,_tpos,_trelpos);
		if(tmp1==4 || tmp2==4)return basic_hmoLine<Type>(relpos1).lIn(_pos,_tpos,_trelpos);

		return true;
	}
	virtual bool bIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _tsize)const{
		hmPoint<Type> _tpos2=_tpos+_tsize-hmPoint<Type>(1,1);
		basic_hmoBox<Type> tmpOB(_tsize);
		//Boxの中
		if(tmpOB.pIn(_tpos,_pos)&&tmpOB.pIn(_tpos,_pos+relpos1)&&tmpOB.pIn(_tpos,_pos+relpos2))return false;
		//Boxが衝突または中
		if(lIn(_pos,_tpos,hmPointY(_tsize.y-1))||lIn(_pos,hmPoint<Type>(_tpos.x,_tpos2.y),hmPointX(_tsize.x-1))||lIn(_pos,hmPoint<Type>(_tpos2.x,_tpos.y),hmPointY(_tsize.y-1))||lIn(_pos,_tpos,hmPointX(_tsize.x-1)))
			return true;
		else return false;
	}
	virtual bool thisIn(const hmPoint<Type>& _pos,const basic_hmObject<Type>& _tobj,const hmPoint<Type>& _tpos)const{
		if(_tobj.lIn(_tpos,_pos,relpos1)||_tobj.lIn(_tpos,_pos,relpos2)||_tobj.lIn(_tpos,_pos+relpos1,relpos2-relpos1))return true;
		else return false;
	}
	virtual hmPoint<Type> pDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos)const{
		int tmp=hmo::GetTriangleArea(_tpos,_pos,_pos+relpos1,_pos+relpos2);

		if(tmp<0){
			if(tmp==-1)return basic_hmoLine<Type>(relpos2-relpos1).pDir(_pos+relpos1,_tpos);
			if(tmp==-2)return basic_hmoLine<Type>(relpos2).pDir(_pos,_tpos);
			if(tmp==-4)return basic_hmoLine<Type>(relpos1).pDir(_pos,_tpos);
		}else if(tmp==0){
			return hmPoint<Type>(0,0);
		}


		if(tmp==6){
			if(inprd(relpos1,relpos2)>=0)return _tpos-_pos;
			hmPoint<Type> tmp1=basic_hmoLine<Type>(relpos2).pDir(_pos,_tpos);
			hmPoint<Type> tmp2=basic_hmoLine<Type>(relpos1).pDir(_pos,_tpos);
			if(abs2(tmp1)>abs2(tmp2))return tmp2;
			else return tmp1;
		}
		if(tmp==5){
			if(inprd(-relpos1,relpos2-relpos1)>=0)return _tpos-_pos-relpos1;
			hmPoint<Type> tmp1=basic_hmoLine<Type>(relpos2-relpos1).pDir(_pos+relpos1,_tpos);
			hmPoint<Type> tmp2=basic_hmoLine<Type>(-relpos1).pDir(_pos+relpos1,_tpos);
			if(abs2(tmp1)>abs2(tmp2))return tmp2;
			else return tmp1;
		}
		if(tmp==3){
			if(inprd(-relpos2,relpos1-relpos2)>=0)return _tpos-_pos-relpos2;
			hmPoint<Type> tmp1=basic_hmoLine<Type>(-relpos2).pDir(_pos+relpos2,_tpos);
			hmPoint<Type> tmp2=basic_hmoLine<Type>(relpos1-relpos2).pDir(_pos+relpos2,_tpos);
			if(abs2(tmp1)>abs2(tmp2))return tmp2;
			else return tmp1;
		}
		if(tmp==1)return basic_hmoLine<Type>(relpos2-relpos1).pDir(_pos+relpos1,_tpos);
		if(tmp==2)return basic_hmoLine<Type>(-relpos2).pDir(_pos+relpos2,_tpos);
		if(tmp==4)return basic_hmoLine<Type>(relpos1).pDir(_pos,_tpos);

		return hmPoint<Type>(0,0);
	}
	virtual hmPoint<Type> lDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _trelpos)const{
		int tmp1=hmo::GetTriangleArea(_tpos,_pos,_pos+relpos1,_pos+relpos2);
		int tmp2=hmo::GetTriangleArea(_tpos+_trelpos,_pos,_pos+relpos1,_pos+relpos2);

		if(tmp1<0){
			if(tmp1==-1)return basic_hmoLine<Type>(relpos2-relpos1).lDir(_pos+relpos1,_tpos,_trelpos);
			if(tmp1==-2)return basic_hmoLine<Type>(relpos2).lDir(_pos,_tpos,_trelpos);
			if(tmp1==-4)return basic_hmoLine<Type>(relpos1).lDir(_pos,_tpos,_trelpos);
		}

		if(tmp1==0||tmp2==0)return hmPoint<Type>(0,0);

		if((tmp1&tmp2)==1)return basic_hmoLine<Type>(relpos2-relpos1).lDir(_pos+relpos1,_tpos,_trelpos);
		if((tmp1&tmp2)==2)return basic_hmoLine<Type>(relpos2).lDir(_pos,_tpos,_trelpos);
		if((tmp1&tmp2)==4)return basic_hmoLine<Type>(relpos1).lDir(_pos,_tpos,_trelpos);

		if(tmp1==tmp2){
			if(tmp1==6){
				if(inprd(relpos1,relpos2)>=0)return basic_hmoPixel<Type>().lDir(_pos,_tpos,_trelpos);
				hmPoint<Type> tmpP1=basic_hmoLine<Type>(relpos2).lDir(_pos,_tpos,_trelpos);
				hmPoint<Type> tmpP2=basic_hmoLine<Type>(relpos1).lDir(_pos,_tpos,_trelpos);
				if(abs2(tmpP1)>abs2(tmpP2))return tmpP2;
				else return tmpP1;
			}
			if(tmp1==5){
				if(inprd(relpos1,relpos1-relpos2)>=0)return basic_hmoPixel<Type>().lDir(_pos+relpos1,_tpos,_trelpos);
				hmPoint<Type> tmpP1=basic_hmoLine<Type>(relpos2-relpos1).lDir(_pos+relpos1,_tpos,_trelpos);
				hmPoint<Type> tmpP2=basic_hmoLine<Type>(-relpos1).lDir(_pos+relpos1,_tpos,_trelpos);
				if(abs2(tmpP1)>abs2(tmpP2))return tmpP2;
				else return tmpP1;
			}
			if(tmp1==3){
				if(inprd(relpos2,relpos2-relpos1)>=0)return basic_hmoPixel<Type>().lDir(_pos+relpos2,_tpos,_trelpos);
				hmPoint<Type> tmpP1=basic_hmoLine<Type>(-relpos2).lDir(_pos+relpos2,_tpos,_trelpos);
				hmPoint<Type> tmpP2=basic_hmoLine<Type>(relpos1-relpos2).lDir(_pos+relpos2,_tpos,_trelpos);
				if(abs2(tmpP1)>abs2(tmpP2))return tmpP2;
				else return tmpP1;
			}
		}

		if(tmp1+tmp2==7){
			hmPoint<Type> tmpP1=basic_hmoLine<Type>(relpos1).lDir(_pos,_tpos,_trelpos);
			hmPoint<Type> tmpP2=basic_hmoLine<Type>(relpos2).lDir(_pos,_tpos,_trelpos);
			hmPoint<Type> tmpP3=basic_hmoLine<Type>(relpos2-relpos1).lDir(_pos+relpos1,_tpos,_trelpos);
			if(abs2(tmpP1)>abs2(tmpP2)){
				if(abs2(tmpP2)>abs2(tmpP3))return tmpP3;
				else return tmpP2;
			}else{
				if(abs2(tmpP1)>abs2(tmpP3))return tmpP3;
				else return tmpP1;
			}
		}else if(tmp1+tmp2==6){
			hmPoint<Type> tmpP1=basic_hmoLine<Type>(relpos1).lDir(_pos,_tpos,_trelpos);
			hmPoint<Type> tmpP2=basic_hmoLine<Type>(relpos2).lDir(_pos,_tpos,_trelpos);
			if(abs2(tmpP1)>abs2(tmpP2))return tmpP2;
			else return tmpP1;
		}else if(tmp1+tmp2==5){
			hmPoint<Type> tmpP1=basic_hmoLine<Type>(-relpos1).lDir(_pos+relpos1,_tpos,_trelpos);
			hmPoint<Type> tmpP2=basic_hmoLine<Type>(relpos2-relpos1).lDir(_pos+relpos1,_tpos,_trelpos);
			if(abs2(tmpP1)>abs2(tmpP2))return tmpP2;
			else return tmpP1;
		}else if(tmp1+tmp2==3){
			hmPoint<Type> tmpP1=basic_hmoLine<Type>(-relpos2).lDir(_pos+relpos2,_tpos,_trelpos);
			hmPoint<Type> tmpP2=basic_hmoLine<Type>(relpos1-relpos2).lDir(_pos+relpos2,_tpos,_trelpos);
			if(abs2(tmpP1)>abs2(tmpP2))return tmpP2;
			else return tmpP1;
		}
		return hmPoint<Type>(0,0);
	}
	virtual hmPoint<Type> bDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _tsize)const{
		hmPoint<Type> _tpos2=_tpos+_tsize-hmPoint<Type>(1.,1.);
		if(abs2(pDir(_pos,_tpos))+abs2(pDir(_pos,_tpos2))+abs2(pDir(_pos,hmPoint<Type>(_tpos.x,_tpos2.y)))+abs2(pDir(_pos,hmPoint<Type>(_tpos2.x,_tpos.y)))==0)
			return hmPoint<Type>(0,0);
		hmPoint<Type> tmp1,tmp2,tmp3;
		tmp1=basic_hmoBox<Type>(_tsize).lDir(_tpos,_pos,relpos1);
		tmp2=basic_hmoBox<Type>(_tsize).lDir(_tpos,_pos+relpos1,relpos2-relpos1);
		tmp3=basic_hmoBox<Type>(_tsize).lDir(_tpos,_pos+relpos2,-relpos2);

		if(abs2(tmp1)<abs2(tmp2)){
			if(abs2(tmp1)<abs2(tmp3))return tmp1*-1.;
			else return tmp3*-1.;
		}else{
			if(abs2(tmp2)<abs2(tmp3))return tmp2*-1.;
			else return tmp3*-1.;
		}
	}
	virtual hmPoint<Type> oDir(const hmPoint<Type>& _pos,const basic_hmDirObject<Type>& _tobj,const hmPoint<Type>& _tpos)const{
		if(abs2(_tobj.pDir(_tpos,_pos))+abs2(_tobj.pDir(_tpos,_pos+relpos1))+abs2(_tobj.pDir(_tpos,_pos+relpos2))==0)
			return hmPoint<Type>(0,0);
		hmPoint<Type> tmp1,tmp2,tmp3;
		tmp1=_tobj.lDir(_tpos,_pos,relpos1);
		tmp2=_tobj.lDir(_tpos,_pos+relpos1,relpos2-relpos1);
		tmp3=_tobj.lDir(_tpos,_pos+relpos2,-relpos2);

		if(abs2(tmp1)<abs2(tmp2)){
			if(abs2(tmp1)<abs2(tmp3))return tmp1*-1.;
			else return tmp3*-1.;
		}else{
			if(abs2(tmp2)<abs2(tmp3))return tmp2*-1.;
			else return tmp3*-1.;
		}
	}
};
template <class Type>
class basic_hmoCircle:public virtual basic_hmObject<Type>,public basic_hmDirObject<Type>{
protected:
	void copy(const basic_hmoCircle<Type>& _hmo){r=_hmo.r;}
public:
	Type r;
	basic_hmoCircle<Type>(){set(0);}
	basic_hmoCircle<Type>(const Type& _r){set(_r);}
	basic_hmoCircle<Type>(const basic_hmoCircle<Type>& _hmo){copy(_hmo);}
	basic_hmoCircle<Type>& operator=(const basic_hmoCircle<Type>& _hmo){
		if(this!=&_hmo)copy(_hmo);
		return *this;
	}
	void set(const Type& _r){r=_r;}
	virtual void getArea(hmPoint<Type>& _min,hmPoint<Type>& _max)const{
		_min=-hmPoint<Type>(r,r);
		_max=hmPoint<Type>(r,r);
	}
	virtual bool pIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos)const{
		if(abs2(_pos-_tpos)<=r*r)return true;
		else return false;
	}
	virtual bool lIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _trelpos)const{
		if(abs2(basic_hmoLine<Type>(_trelpos).pDir(_tpos,_pos))<=r*r)return true;
		else return false;
	}
	virtual bool bIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _tsize)const{
		if(lIn(_pos,_tpos,hmPointX(_tsize.x-1)) || lIn(_pos,_tpos,hmPointY(_tsize.y-1)) || lIn(_pos, _tpos+hmPointX(_tsize.x-1),hmPointY(_tsize.y-1)) || lIn(_pos, _tpos+hmPointY(_tsize.y-1),hmPointX(_tsize.x-1)))return true;
		else return false;
	}
	virtual bool thisIn(const hmPoint<Type>& _pos,const basic_hmObject<Type>& _tobj,const hmPoint<Type>& _tpos)const{
		for(Type i=0;i<16;i++){
			if(_tobj.pIn(_tpos,_pos+hmPoint<Type>(r*cos((Type)i/8.*pi()),r*sin((Type)i/8.*pi()))))return true;
		}
		return false;
	}
	virtual hmPoint<Type> pDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos)const{
		hmPoint<Type> tmpP=_tpos-_pos;
		if(abs2(tmpP)<r*r)return hmPoint<Type>(0,0);
		return tmpP*(1.-r/abs(tmpP));
	}
	virtual hmPoint<Type> lDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _trelpos)const{
		hmPoint<Type> tmpP=basic_hmoPixel<Type>().lDir(_pos,_tpos,_trelpos);
		if(abs2(tmpP)<r*r)return hmPoint<Type>(0,0);
		return tmpP*(1.-r/abs(tmpP));
	}
	virtual hmPoint<Type> bDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _tsize)const{
		hmPoint<Type> tmpP=basic_hmoPixel<Type>().bDir(_pos,_tpos,_tsize);
		if(abs2(tmpP)<r*r)return hmPoint<Type>(0,0);
		return tmpP*(1.-r/abs(tmpP));
	}
	virtual hmPoint<Type> oDir(const hmPoint<Type>& _pos,const basic_hmDirObject<Type>& _tobj,const hmPoint<Type>& _tpos)const{
		if(abs2(_tobj.pDir(_tpos,_pos))==0)return hmPoint<Type>(0,0);
		hmPoint<Type> tmpP=basic_hmoPixel<Type>().oDir(_pos,_tobj,_tpos);
		if(abs2(tmpP)<r*r)return hmPoint<Type>(0,0);
		return tmpP*(1.-r/abs(tmpP));
	}
};
template <class Type>
class basic_hmoOval:public virtual basic_hmObject<Type>{
protected:
	void copy(const basic_hmoOval<Type>& _hmo){r=_hmo.r;}
public:
	hmPoint<Type> r;
	basic_hmoOval<Type>(){set(hmPoint<Type>(0,0));}
	basic_hmoOval<Type>(const hmPoint<Type>& _r){set(_r);}
	basic_hmoOval<Type>(const basic_hmoOval<Type>& _hmo){copy(_hmo);}
	basic_hmoOval<Type>& operator=(const basic_hmoOval<Type>& _hmo){
		if(this!=&_hmo)copy(_hmo);
		return *this;
	}
	void set(const hmPoint<Type>& _r){r=_r;}
	virtual void getArea(hmPoint<Type>& _min,hmPoint<Type>& _max)const{
		_min=-r;
		_max=r;
	}
	virtual bool pIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos)const{
		hmPoint<double> _adp((double)_pos.x/r.x,(double)_pos.y/r.y);
		hmPoint<double> _tadp((double)_tpos.x/r.x,(double)_tpos.y/r.y);
		if(abs2(_adp-_tadp)<=1.)return true;
		else return false;
	}
	virtual bool lIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _trelpos)const{
		hmPoint<double> _adp((double)_pos.x/r.x,(double)_pos.y/r.y);
		hmPoint<double> _tadp((double)_tpos.x/r.x,(double)_tpos.y/r.y);
		hmPoint<double> _tadr((double)_trelpos.x/r.x,(double)_trelpos.y/r.y);
		if(abs2(basic_hmoLine<double>(_tadr).pDir(_tadp,_adp))<=1.)return true;
		else return false;
	}
	virtual bool bIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _tsize)const{
		if(lIn(_pos,_tpos,hmPointX(_tsize.x-1)) || lIn(_pos,_tpos,hmPointY(_tsize.y-1)) || lIn(_pos, _tpos+hmPointX(_tsize.x-1),hmPointY(_tsize.y-1)) || lIn(_pos, _tpos+hmPointY(_tsize.y-1),hmPointX(_tsize.x-1)))return true;
		else return false;
	}
	virtual bool thisIn(const hmPoint<Type>& _pos,const basic_hmObject<Type>& _tobj,const hmPoint<Type>& _tpos)const{
		for(Type i=0;i<16;i++){
			if(_tobj.pIn(_tpos,_pos+hmPoint<Type>(r.x*cos((Type)i/8.*pi()),r.y*sin((Type)i/8.*pi()))))return true;
		}
		return false;
	}
};
template <class Type>
class basic_hmoPNull:public virtual basic_hmObject<Type>,public basic_hmDirObject<Type>{
protected:
	void copy(const basic_hmoPixel<Type>& _hmo){return;}
public:
	hmPoint<Type> pos;
	basic_hmoPNull<Type>(){set(hmPoint<Type>(0,0));}
	basic_hmoPNull<Type>(const hmPoint<Type>& _pos){set(_pos);}
	basic_hmoPNull<Type>(const basic_hmoPNull<Type>& _hmo){copy(_hmo);}
	basic_hmoPNull<Type>& operator=(const basic_hmoPNull<Type>& _hmo){
		if(this==&_hmo)copy(_hmo);
		return *this;
	}
	void set(const hmPoint<Type>& _pos){pos=_pos;}
	virtual void getArea(hmPoint<Type>& _min,hmPoint<Type>& _max)const{
		_min=pos;
		_max=pos;
	}
	virtual bool pIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos)const{return false;}
	virtual bool lIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _trelpos)const{return false;}
	virtual bool bIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _tsize)const{return false;}
	virtual bool thisIn(const hmPoint<Type>& _pos,const basic_hmObject<Type>& _tobj,const hmPoint<Type>& _tpos)const{return false;}
	virtual hmPoint<Type> pDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos)const{return hmPoint<Type>(0,0);}
	virtual hmPoint<Type> lDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _trelpos)const{return hmPoint<Type>(0,0);}
	virtual hmPoint<Type> bDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _tsize)const{return hmPoint<Type>(0,0);}
	virtual hmPoint<Type> oDir(const hmPoint<Type>& _pos,const basic_hmDirObject<Type>& _tobj,const hmPoint<Type>& _tpos)const{return hmPoint<Type>(0,0);}

};
template <class Type>
class basic_hmoPPixel:public virtual basic_hmObject<Type>,public basic_hmDirObject<Type>{
protected:
	void copy(const basic_hmoPPixel<Type>& _hmo){pos=_hmo.pos;}
public:
	hmPoint<Type> pos;
	basic_hmoPPixel<Type>(){set(hmPoint<Type>(0,0));}
	basic_hmoPPixel<Type>(const hmPoint<Type>& _pos){set(_pos);}
	basic_hmoPPixel<Type>(const basic_hmoPPixel& _hmo){copy(_hmo);}
	basic_hmoPPixel<Type>& operator=(const basic_hmoPPixel<Type>& _hmo){
		if(this!=&_hmo)copy(_hmo);
		return *this;
	}
	void set(const hmPoint<Type>& _pos){pos=_pos;}
	virtual void getArea(hmPoint<Type>& _min,hmPoint<Type>& _max)const{
		_min=pos;
		_max=pos;
	}
	virtual bool pIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos)const{return basic_hmoPixel<Type>().pIn(_pos+pos,_tpos);}
	virtual bool lIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _trelpos)const{return basic_hmoPixel<Type>().lIn(_pos+pos,_tpos,_trelpos);}
	virtual bool bIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _tsize)const{return basic_hmoPixel<Type>().bIn(_pos+pos,_tpos,_tsize);}
	virtual bool thisIn(const hmPoint<Type>& _pos,const basic_hmObject<Type>& _tobj,const hmPoint<Type>& _tpos)const{return basic_hmoPixel<Type>().thisIn(_pos+pos,_tobj,_tpos);}
	virtual hmPoint<Type> pDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos)const{return basic_hmoPixel<Type>().pDir(_pos+pos,_tpos);}
	virtual hmPoint<Type> lDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _trelpos)const{return basic_hmoPixel<Type>().lDir(_pos+pos,_tpos,_trelpos);}
	virtual hmPoint<Type> bDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _tsize)const{return basic_hmoPixel<Type>().bDir(_pos+pos,_tpos,_tsize);}
	virtual hmPoint<Type> oDir(const hmPoint<Type>& _pos,const basic_hmDirObject<Type>& _tobj,const hmPoint<Type>& _tpos)const{return basic_hmoPixel<Type>().oDir(_pos+pos,_tobj,_tpos);}
};
template <class Type>
class basic_hmoPLine:public virtual basic_hmObject<Type>,public basic_hmDirObject<Type>{
protected:
	void copy(const basic_hmoPLine<Type>& _hmo){
		pos1=_hmo.pos1;
		pos2=_hmo.pos2;
	}
public:
	hmPoint<Type> pos1,pos2;
	basic_hmoPLine<Type>(){set(hmPoint<Type>(0,0),hmPoint<Type>(0,0));}
	basic_hmoPLine<Type>(const hmPoint<Type>& _pos1,const hmPoint<Type>& _pos2){set(_pos1,_pos2);}
	basic_hmoPLine<Type>(const basic_hmoPLine<Type>& _hmo){copy(_hmo);}
	basic_hmoPLine<Type>& operator=(const basic_hmoPLine<Type>& _hmo){
		if(this!=&_hmo)copy(_hmo);
		return *this;
	}
	void set(const hmPoint<Type>& _pos1,const hmPoint<Type>& _pos2){
		pos1=_pos1;
		pos2=_pos2;
	}
	virtual void getArea(hmPoint<Type>& _min,hmPoint<Type>& _max)const{
		_min=hmPointMin(pos1,pos2);
		_max=hmPointMax(pos1,pos2);
	}
	virtual bool pIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos)const{return basic_hmoLine<Type>(pos2-pos1).pIn(_pos+pos1,_tpos);}
	virtual bool lIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _trelpos)const{return basic_hmoLine<Type>(pos2-pos1).lIn(_pos+pos1,_tpos,_trelpos);}
	virtual bool bIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _tsize)const{return basic_hmoLine<Type>(pos2-pos1).bIn(_pos+pos1,_tpos,_tsize);}
	virtual bool thisIn(const hmPoint<Type>& _pos,const basic_hmObject<Type>& _tobj,const hmPoint<Type>& _tpos)const{return basic_hmoLine<Type>(pos2-pos1).thisIn(_pos+pos1,_tobj,_tpos);}
	virtual hmPoint<Type> pDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos)const{return basic_hmoLine<Type>(pos2-pos1).pDir(_pos+pos1,_tpos);}
	virtual hmPoint<Type> lDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _trelpos)const{return basic_hmoLine<Type>(pos2-pos1).lDir(_pos+pos1,_tpos,_trelpos);}
	virtual hmPoint<Type> bDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _tsize)const{return basic_hmoLine<Type>(pos2-pos1).bDir(_pos+pos1,_tpos,_tsize);}
	virtual hmPoint<Type> oDir(const hmPoint<Type>& _pos,const basic_hmDirObject<Type>& _tobj,const hmPoint<Type>& _tpos)const{return basic_hmoLine<Type>(pos2-pos1).oDir(_pos+pos1,_tobj,_tpos);}
};
template <class Type>
class basic_hmoPBox:public virtual basic_hmObject<Type>,public basic_hmDirObject<Type>{
protected:
	void copy(const basic_hmoPBox<Type>& _hmo){
		pos1=_hmo.pos1;
		pos2=_hmo.pos2;
	}
public:
	hmPoint<Type> pos1,pos2;
	basic_hmoPBox<Type>(){set(hmPoint<Type>(0,0),hmPoint<Type>(0,0));}
	basic_hmoPBox<Type>(const hmPoint<Type>& _pos1,const hmPoint<Type>& _pos2){set(_pos1,_pos2);}
	basic_hmoPBox<Type>(const basic_hmoPBox<Type>& _hmo){copy(_hmo);}
	basic_hmoPBox<Type>& operator=(const basic_hmoPBox<Type>& _hmo){
		if(this!=&_hmo)copy(_hmo);
		return *this;
	}
	void set(const hmPoint<Type>& _pos1,const hmPoint<Type>& _pos2){
		pos1=_pos1;
		pos2=_pos2;
	}
	virtual void getArea(hmPoint<Type>& _min,hmPoint<Type>& _max)const{
		_min=hmPointMin(pos1,pos2);
		_max=hmPointMax(pos1,pos2);
	}
	virtual bool pIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos)const{return basic_hmoBox<Type>(Pint(abs(pos1.x-pos2.x),abs(pos1.y-pos2.y))).pIn(_pos+PintMin(pos1,pos2),_tpos);}
	virtual bool lIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _trelpos)const{return basic_hmoBox<Type>(Pint(abs(pos1.x-pos2.x),abs(pos1.y-pos2.y))).lIn(_pos+PintMin(pos1,pos2),_tpos,_trelpos);}
	virtual bool bIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _tsize)const{return basic_hmoBox<Type>(Pint(abs(pos1.x-pos2.x),abs(pos1.y-pos2.y))).bIn(_pos+PintMin(pos1,pos2),_tpos,_tsize);}
	virtual bool thisIn(const hmPoint<Type>& _pos,const basic_hmObject<Type>& _tobj,const hmPoint<Type>& _tpos)const{return basic_hmoBox<Type>(Pint(abs(pos1.x-pos2.x),abs(pos1.y-pos2.y))).thisIn(_pos+PintMin(pos1,pos2),_tobj,_tpos);}
	virtual hmPoint<Type> pDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos)const{return basic_hmoBox<Type>(Pint(abs(pos1.x-pos2.x),abs(pos1.y-pos2.y))).pDir(_pos+PintMin(pos1,pos2),_tpos);}
	virtual hmPoint<Type> lDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _trelpos)const{return basic_hmoBox<Type>(Pint(abs(pos1.x-pos2.x),abs(pos1.y-pos2.y))).lDir(_pos+PintMin(pos1,pos2),_tpos,_trelpos);}
	virtual hmPoint<Type> bDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _tsize)const{return basic_hmoBox<Type>(Pint(abs(pos1.x-pos2.x),abs(pos1.y-pos2.y))).bDir(_pos+PintMin(pos1,pos2),_tpos,_tsize);}
	virtual hmPoint<Type> oDir(const hmPoint<Type>& _pos,const basic_hmDirObject<Type>& _tobj,const hmPoint<Type>& _tpos)const{return basic_hmoBox<Type>(Pint(abs(pos1.x-pos2.x),abs(pos1.y-pos2.y))).oDir(_pos+PintMin(pos1,pos2),_tobj,_tpos);}
};
template <class Type>
class basic_hmoPTriangle:public virtual basic_hmObject<Type>,public basic_hmDirObject<Type>{
protected:
	void copy(const basic_hmoPTriangle<Type>& _hmo){
		pos1=_hmo.pos1;
		pos2=_hmo.pos2;
		pos3=_hmo.pos3;
	}
public:
	hmPoint<Type> pos1,pos2,pos3;
	basic_hmoPTriangle<Type>(){set(hmPoint<Type>(),hmPoint<Type>(),hmPoint<Type>());}
	basic_hmoPTriangle<Type>(const hmPoint<Type>& _pos1,const hmPoint<Type>& _pos2,const hmPoint<Type>& _pos3){set(_pos1,_pos2,_pos3);}
	basic_hmoPTriangle<Type>(const basic_hmoPTriangle<Type>& _hmo){copy(_hmo);}
	basic_hmoPTriangle<Type>& operator=(const basic_hmoPTriangle<Type>& _hmo){
		if(this!=&_hmo)copy(_hmo);
		return *this;
	}
	void set(const hmPoint<Type>& _pos1,const hmPoint<Type>& _pos2,const hmPoint<Type>& _pos3){
		pos1=_pos1;
		pos2=_pos2;
		pos3=_pos3;
	}
	virtual void getArea(hmPoint<Type>& _min,hmPoint<Type>& _max)const{
		_min=hmPointMin(pos1,hmPointMin(pos2,pos3));
		_max=hmPointMax(pos1,hmPointMax(pos2,pos3));
	}
	virtual bool pIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos)const{return basic_hmoTriangle<Type>(pos2-pos1,pos3-pos1).pIn(_pos+pos1,_tpos);}
	virtual bool lIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _trelpos)const{return basic_hmoTriangle<Type>(pos2-pos1,pos3-pos1).lIn(_pos+pos1,_tpos,_trelpos);}
	virtual bool bIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _tsize)const{return basic_hmoTriangle<Type>(pos2-pos1,pos3-pos1).bIn(_pos+pos1,_tpos,_tsize);}
	virtual bool thisIn(const hmPoint<Type>& _pos,const basic_hmObject<Type>& _tobj,const hmPoint<Type>& _tpos)const{return basic_hmoTriangle<Type>(pos2-pos1,pos3-pos1).thisIn(_pos+pos1,_tobj,_tpos);}
	virtual hmPoint<Type> pDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos)const{return basic_hmoTriangle<Type>(pos2-pos1,pos3-pos1).pDir(_pos+pos1,_tpos);}
	virtual hmPoint<Type> lDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _trelpos)const{return basic_hmoTriangle<Type>(pos2-pos1,pos3-pos1).lDir(_pos+pos1,_tpos,_trelpos);}
	virtual hmPoint<Type> bDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _tsize)const{return basic_hmoTriangle<Type>(pos2-pos1,pos3-pos1).bDir(_pos+pos1,_tpos,_tsize);}
	virtual hmPoint<Type> oDir(const hmPoint<Type>& _pos,const basic_hmDirObject<Type>& _tobj,const hmPoint<Type>& _tpos)const{return basic_hmoTriangle<Type>(pos2-pos1,pos3-pos1).oDir(_pos+pos1,_tobj,_tpos);}
};
template <class Type>
class basic_hmoPCircle:public virtual basic_hmObject<Type>,public basic_hmDirObject<Type>{
protected:
	void copy(const basic_hmoPCircle<Type>& _hmo){
		pos=_hmo.pos;
		r=_hmo.r;
	}
public:
	hmPoint<Type> pos;
	Type r;
	basic_hmoPCircle<Type>(){set(hmPoint<Type>(0,0),0);}
	basic_hmoPCircle<Type>(const hmPoint<Type>& _pos,const Type& _r){set(_pos,_r);}
	basic_hmoPCircle<Type>(const basic_hmoPCircle<Type>& _hmo){copy(_hmo);}
	basic_hmoPCircle<Type>& operator=(const basic_hmoPCircle<Type>& _hmo){
		if(this!=&_hmo)copy(_hmo);
		return *this;
	}
	void set(const hmPoint<Type>& _pos,const Type& _r){
		pos=_pos;
		r=_r;
	}
	virtual void getArea(hmPoint<Type>& _min,hmPoint<Type>& _max)const{
		_min=pos-hmPoint<Type>(r,r);
		_max=pos+hmPoint<Type>(r,r);
	}
	virtual bool pIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos)const{return basic_hmoCircle<Type>(r).pIn(_pos+pos,_tpos);}
	virtual bool lIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _trelpos)const{return basic_hmoCircle<Type>(r).lIn(_pos+pos,_tpos,_trelpos);}
	virtual bool bIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _tsize)const{return basic_hmoCircle<Type>(r).bIn(_pos+pos,_tpos,_tsize);}
	virtual bool thisIn(const hmPoint<Type>& _pos,const basic_hmObject<Type>& _tobj,const hmPoint<Type>& _tpos)const{return basic_hmoCircle<Type>(r).thisIn(_pos+pos,_tobj,_tpos);}
	virtual hmPoint<Type> pDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos)const{return basic_hmoCircle<Type>(r).pDir(_pos+pos,_tpos);}
	virtual hmPoint<Type> lDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _trelpos)const{return basic_hmoCircle<Type>(r).lDir(_pos+pos,_tpos,_trelpos);}
	virtual hmPoint<Type> bDir(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _tsize)const{return basic_hmoCircle<Type>(r).bDir(_pos+pos,_tpos,_tsize);}
	virtual hmPoint<Type> oDir(const hmPoint<Type>& _pos,const basic_hmDirObject<Type>& _tobj,const hmPoint<Type>& _tpos)const{return basic_hmoCircle<Type>(r).oDir(_pos+pos,_tobj,_tpos);}
};
template <class Type>
class basic_hmoPOval:public virtual basic_hmObject<Type>{
protected:
	void copy(const basic_hmoPOval<Type>& _hmo){
		pos=_hmo.pos;
		r=_hmo.r;
	}
public:
	hmPoint<Type> pos,r;
	basic_hmoPOval<Type>(){set(hmPoint<Type>(0,0),hmPoint<Type>(0,0));}
	basic_hmoPOval<Type>(const hmPoint<Type>& _pos,const hmPoint<Type>& _r){set(_pos,_r);}
	basic_hmoPOval<Type>(const basic_hmoPOval<Type>& _hmo){copy(_hmo);}
	basic_hmoPOval<Type>& operator=(const basic_hmoPOval<Type>& _hmo){
		if(this!=&_hmo)copy(_hmo);
		return *this;
	}
	void set(const hmPoint<Type>& _pos,const hmPoint<Type>& _r){
		pos=_pos;
		r=_r;
	}
	virtual void getArea(hmPoint<Type>& _min,hmPoint<Type>& _max)const{
		_min=pos-r;
		_max=pos+r;
	}
	virtual bool pIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos)const{return basic_hmoOval<Type>(r).pIn(_pos+pos,_tpos);}
	virtual bool lIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _trelpos)const{return basic_hmoOval<Type>(r).lIn(_pos+pos,_tpos,_trelpos);}
	virtual bool bIn(const hmPoint<Type>& _pos,const hmPoint<Type>& _tpos,const hmPoint<Type>& _tsize)const{return basic_hmoOval<Type>(r).bIn(_pos+pos,_tpos,_tsize);}
	virtual bool thisIn(const hmPoint<Type>& _pos,const basic_hmObject<Type>& _tobj,const hmPoint<Type>& _tpos)const{return basic_hmoOval<Type>(r).thisIn(_pos+pos,_tobj,_tpos);}
};
//typedef define
typedef basic_hmObject<int>		hmObject;
typedef basic_hmDirObject<int>	hmDirObject;
typedef basic_hmoNull<int>		hmoNull;
typedef basic_hmoPixel<int>		hmoPixel;
typedef basic_hmoLine<int>		hmoLine;
typedef basic_hmoBox<int>		hmoBox;
typedef basic_hmoTriangle<int>	hmoTriangle;
typedef basic_hmoCircle<int>	hmoCircle;
typedef basic_hmoOval<int>		hmoOval;
typedef basic_hmoPNull<int>		hmoPNull;
typedef basic_hmoPPixel<int>	hmoPPixel;
typedef basic_hmoPLine<int>		hmoPLine;
typedef basic_hmoPBox<int>		hmoPBox;
typedef basic_hmoPTriangle<int>	hmoPTriangle;
typedef basic_hmoPCircle<int>	hmoPCircle;
typedef basic_hmoPOval<int>		hmoPOval;
#endif//HMOBJECT_HPP
