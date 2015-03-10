#ifndef HMLIB_BODY_INC
#define HMLIB_BODY_INC 100
#
#ifndef HMLIB_RECTANGLE_INC
#	include"rectangle.hpp"
#endif
namespace hmLib{
	namespace body{
		template<class type>
		class itfBody{
		protected:
			typedef di_position<type> pos_type;
			typedef di_rectangle<type> rect_type;
		public://itfBody<type>
			virtual bool includePixel(const pos_type& Pos_)const=0;
			virtual bool includeLine(const pos_type& Pos1_,const pos_type& Pos2_)const=0;
			virtual bool includeBox(const pos_type& Pos_,const pos_type& Size_)const=0;
			virtual bool includeCircle(const pos_type& Pos_,const type& r_)const=0;
			virtual bool included(const itfBody<type>& Body_)const=0;
			virtual bool crossLine(const pos_type& Pos1_,const pos_type& Pos2_)const=0;
			virtual bool crossBox(const pos_type& Pos_,const pos_type& Size_)const=0;
			virtual bool crossCircle(const pos_type& Pos_,const type& r_)const=0;
			virtual bool crossed(const itfBody<type>& Body_)const=0;
		public:
			bool includeBox(const rect_type& Rec_)const{return includeBox(Rec_.pos1,Rec_.pos2-Rec_.pos1);}
			bool include(const itfBody<type>& Body_)const{return Body_.included(*this);}
			bool crossPixel(const pos_type& Pos_)const{return includePixel(Pos_);}
			bool crossBox(const rect_type& Rec_)const{return crossBox(Rec_.pos1,Rec_.pos2-Rec_.pos1);}
			bool cross(const itfBody<type>& Body_)const{return Body_.crossed(*this);}
		};
		template <class type>
		class itfPixel:public itfBody<type>{
			typedef typename itfBody<type>::pos_type pos_type;
			typedef typename itfBody<type>::rect_type rect_type;
			typedef itfPixel<type> my_type;
		protected://itfPixel
			inline virtual pos_type getPos()const=0;
		public:
			const my_type& operator=(const my_type& Body_){
				if(this!=&Body_){
					getPos()=Body_.getPos();
				}
				return *this;
			}
		public://itfBody<type>
			virtual bool includePixel(const pos_type& Pos_)const{return Pos_==getPos();}
			virtual bool includeLine(const pos_type& Pos1_,const pos_type& Pos2_)const{return Pos1_==getPos() && Pos2_==getPos();}
			virtual bool includeBox(const pos_type& Pos_,const pos_type& Size_)const{return false;}
			virtual bool includeCircle(const pos_type& Pos_,const type& r_)const{return false;}
			virtual bool included(const itfBody<type>& Body_)const{return includePixel(getPos());}
			virtual bool crossLine(const pos_type& Pos1_,const pos_type& Pos2_)const{
				pos_type tmpPos=(Pos1_-getPos())*(Pos2_-getPos());
				if(tmpPos.x>0 ||tmpPos.y>0)return false;
				return  area(Pos1_-getPos(),Pos2_-getPos())==0;
			}
			virtual bool crossBox(const pos_type& Pos_,const pos_type& Size_)const{return Pos_<=getPos() && getPos()<Pos_+Size_;}
			virtual bool crossCircle(const pos_type& Pos_,const type& r_)const{return abs2(getPos()-Pos_)<=r_;}
			virtual bool crossed(const itfBody<type>& Body_)const{return Body_.crossPixel(getPos());}
		};
		template <class type>
		class cPixel:public itfPixel<type>{
			typedef typename itfBody<type>::pos_type pos_type;
			typedef typename itfBody<type>::rect_type rect_type;
			typedef cPixel<type> my_type;
		public:
			pos_type Pos;
		protected://itfPixel
			inline virtual pos_type getPos()const{return Pos;}
		public:
			cPixel(){return;}
			cPixel(const pos_type& Pos_):Pos(Pos_){return;}
			cPixel(const my_type& Body_):Pos(Body_.Pos){return;}
			const my_type& operator=(const my_type& Body_){
				if(this!=&Body_){
					Pos=Body_.Pos;
				}
				return *this;
			}
			void set(const pos_type& Pos_){Pos=Pos_;}
		};
		template <class type> class cLine;
		template <class type>
		class itfLine:public itfBody<type>{
			typedef typename itfBody<type>::pos_type pos_type;
			typedef typename itfBody<type>::rect_type rect_type;
			typedef itfLine<type> my_type;
		protected://itfLine
			inline virtual pos_type getPos1()const=0;
			inline virtual pos_type getPos2()const=0;
		public://itfBody<type>
			virtual bool includePixel(const pos_type& Pos_)const{return cPixel<type>(Pos_).crossLine(getPos1(),getPos2());}
			virtual bool includeLine(const pos_type& Pos1_,const pos_type& Pos2_)const{return includePixel(Pos1_)&&includePixel(Pos2_);}
			virtual bool includeBox(const pos_type& Pos_,const pos_type& Size_)const{return false;}
			virtual bool includeCircle(const pos_type& Pos_,const type& r_)const{return false;}
			virtual bool included(const itfBody<type>& Body_)const{return Body_.includeLine(getPos1(),getPos2());}
			virtual bool crossLine(const pos_type& Pos1_,const pos_type& Pos2_)const{
				pos_type LPos1=Pos1_-getPos1(),LPos2=Pos2_-getPos2(),LPos=getPos2()-getPos1();
				//ベクトル条件
				type t1=area(LPos,LPos1);
				type t2=area(LPos,LPos2);
				type t=area(LPos2,LPos1);

				//Pos1とPos1_,Pos2_が一直線上に存在
				if(t==0)return true;
				//ベクトル条件
				return (t1+t2)/t >= 1;
			}
			inline virtual bool crossBox(const pos_type& Pos_,const pos_type& Size_)const;
			virtual bool crossCircle(const pos_type& Pos_,const type& r_)const{
				type t=area(getPos1()-getPos2(),getPos2()-Pos_);
				type s=2*abs2(getPos1()-getPos2());
				if(t>=s)return abs2(getPos1()-Pos_)<=r_*r_;
				else if(t<=0)return abs2(getPos2()-Pos_)<=r_*r_;
				return (area(getPos1()-getPos2(),Pos_)+area(getPos1(),getPos2()))*(area(getPos1()-getPos2(),Pos_)+area(getPos1(),getPos2()))<=r_*r_*abs2(getPos1()-getPos2());
			}
			virtual bool crossed(const itfBody<type>& Body_)const{return Body_.crossLine(getPos1(),getPos2());}
		};
		template <class type>
		class cLine:public itfLine<type>{
			typedef typename itfBody<type>::pos_type pos_type;
			typedef typename itfBody<type>::rect_type rect_type;
			typedef cLine<type> my_type;
		public:
			pos_type Pos1;
			pos_type Pos2;
		protected://itfLine
			inline virtual pos_type getPos1()const{return Pos1;}
			inline virtual pos_type getPos2()const{return Pos2;}
		public:
			cLine(){return;}
			cLine(const pos_type& Pos1_,const pos_type& Pos2_):Pos1(Pos1_),Pos2(Pos2_){return;}
			cLine(const my_type& Body_):Pos1(Body_.Pos1),Pos2(Body_.Pos2){return;}
			const my_type& operator=(const my_type& Body_){
				if(this!=&Body_){
					Pos1=Body_.Pos1;
					Pos2=Body_.Pos2;
				}
				return *this;
			}
			void set(const pos_type& Pos1_,const pos_type& Pos2_){
				Pos1=Pos1_;
				Pos2=Pos2_;
			}
		};
		template<class type>
		inline bool itfLine<type>::crossBox(const pos_type& Pos_,const pos_type& Size_)const{
			if(getPos1().x<Pos_.x){
				if(getPos1().y<Pos_.y){
					return cLine<type>(Pos_,Pos_+di_positionY(Size_)).crossLine(getPos1(),getPos2()) || cLine<type>(Pos_,Pos_+di_positionX(Size_)).crossLine(getPos1(),getPos2());
				}else if(getPos1().y>=Pos_.y+Size_.y){
					return cLine<type>(Pos_,Pos_+di_positionY(Size_)).crossLine(getPos1(),getPos2()) || cLine<type>(Pos_+di_positionY(Size_),Pos_+Size_).crossLine(getPos1(),getPos2()) ;
				}else{
					return cLine<type>(Pos_,Pos_+di_positionY(Size_)).crossLine(getPos1(),getPos2());
				}
			}else if(getPos1().x>=Pos_.x+Size_.x){
				if(getPos1().y<Pos_.y){
					return cLine<type>(Pos_+di_positionX(Size_),Pos_+Size_).crossLine(getPos1(),getPos2()) || cLine<type>(Pos_,Pos_+di_positionX(Size_)).crossLine(getPos1(),getPos2());
				}else if(getPos1().y>=Pos_.y+Size_.y){
					return cLine<type>(Pos_+di_positionX(Size_),Pos_+Size_).crossLine(getPos1(),getPos2()) || cLine<type>(Pos_+di_positionY(Size_),Pos_+Size_).crossLine(getPos1(),getPos2()) ;
				}else{
					return cLine<type>(Pos_+di_positionX(Size_),Pos_+Size_).crossLine(getPos1(),getPos2());
				}
			}else{
				if(getPos1().y<Pos_.y){
					return cLine<type>(Pos_,Pos_+di_positionX(Size_)).crossLine(getPos1(),getPos2());
				}else if(getPos1().y>=Pos_.y+Size_.y){
					return cLine<type>(Pos_+di_positionY(Size_),Pos_+Size_).crossLine(getPos1(),getPos2());
				}else{
					return true;
				}
			}
			bool flag= ( area(Pos_,getPos1()-getPos2()) >= area(getPos1(),getPos2()));
			if(flag != ( area(Pos_+Size_,getPos1()-getPos2()) >= area(getPos1(),getPos2())) )return true;
			if(flag != ( area(Pos_+di_positionX(Size_),getPos1()-getPos2()) >= area(getPos1(),getPos2())) )return true;
			if(flag != ( area(Pos_+di_positionY(Size_),getPos1()-getPos2()) >= area(getPos1(),getPos2())) )return true;
			return false;
		}
		template <class type>
		class itfBox:public itfBody<type>{
			typedef typename itfBody<type>::pos_type pos_type;
			typedef typename itfBody<type>::rect_type rect_type;
			typedef itfBox<type> my_type;
		protected://itfBox
			inline virtual pos_type getPos()const=0;
			inline virtual pos_type getSize()const=0;
		public://itfBody<type>
			virtual bool includePixel(const pos_type& Pos_)const{return cPixel<type>(Pos_).crossBox(getPos(),getSize());}
			virtual bool includeLine(const pos_type& Pos1_,const pos_type& Pos2_)const{return includePixel(Pos1_)&includePixel(Pos2_);}
			virtual bool includeBox(const pos_type& Pos_,const pos_type& Size_)const{return getPos()<=Pos_ && Pos_+Size_<=getPos()+getSize();}
			virtual bool includeCircle(const pos_type& Pos_,const type& r_)const{return getPos()+pos_type(r_,r_)<=Pos_ && Pos_<pos_type(r_,r_)+getPos()+getSize();}
			virtual bool included(const itfBody<type>& Body_)const{return Body_.includeBox(getPos(),getSize());}
			virtual bool crossLine(const pos_type& Pos1_,const pos_type& Pos2_)const{return cLine<type>(Pos1_,Pos2_).crossBox(getPos(),getSize());}
			virtual bool crossBox(const pos_type& Pos_,const pos_type& Size_)const{return getPos()<Pos_+getSize() && Pos_<getPos()+getSize();}
			virtual bool crossCircle(const pos_type& Pos_,const type& r_)const{
				if(Pos_.x<getPos().x){
					if(Pos_.y<getPos().y){
						return abs2(getPos()-Pos_)<r_*r_;
					}else if(Pos_.y>=getPos().y+getSize().x){
						return abs2(getPos()+di_positionY(Pos_)-Pos_)<r_*r_;
					}else{
						return getPos().x-Pos_.x<r_;
					}
				}else if(Pos_.x>=getPos().x+getSize().x){
					if(Pos_.y<getPos().y){
						return abs2(getPos()+di_positionX(getSize())-Pos_)<r_*r_;
					}else if(Pos_.y>=getPos().y+getSize().x){
						return abs2(getPos()+getSize()-Pos_)<r_*r_;
					}else{
						return Pos_.x-getPos().x-getSize().x<r_;
					}
				}else{
					if(Pos_.y<getPos().y){
						return getPos().y-Pos_.y<r_;
					}else if(Pos_.y>=getPos().y+getSize().x){
						return Pos_.y-getPos().y-getSize().y<r_;
					}else{
						return true;
					}
				}
			}
			virtual bool crossed(const itfBody<type>& Body_)const{return Body_.crossBox(getPos(),getSize());}
		};
		template <class type>
		class cBox:public itfBox<type>{
			typedef typename itfBody<type>::pos_type pos_type;
			typedef typename itfBody<type>::rect_type rect_type;
			typedef cBox<type> my_type;
		public:
			pos_type Pos;
			pos_type Size;
		protected://itfBox
			inline virtual pos_type getPos()const{return Pos;}
			inline virtual pos_type getSize()const{return Size;}
		public:
			cBox(){return;}
			cBox(const pos_type& Pos_,const pos_type& Size_):Pos(Pos_),Size(Size_){return;}
			cBox(const my_type& Body_):Pos(Body_.Pos),Size(Body_.Size){return;}
			const my_type& operator=(const my_type& Body_){
				if(this!=&Body_){
					Pos=Body_.Pos;
					Size=Body_.Size;
				}
				return *this;
			}
			void set(const pos_type& Pos_,const pos_type& Size_){
				Pos=Pos_;
				Size=Size_;
			}
		};
		template <class type>
		class itfTriangle:public itfBody<type>{
			typedef typename itfBody<type>::pos_type pos_type;
			typedef typename itfBody<type>::rect_type rect_type;
			typedef itfTriangle<type> my_type;
		protected://itfLine
			inline virtual pos_type getPos1()const=0;
			inline virtual pos_type getPos2()const=0;
			inline virtual pos_type getPos3()const=0;
		public://itfBody<type>
			virtual bool includePixel(const pos_type& Pos_)const{
				pos_type LPos1=getPos2()-getPos1(),LPos2=getPos3()-getPos1(),LPos=Pos_-getPos1();
				//ベクトル条件
				type t1=area(LPos,LPos1);
				type t2=area(LPos,LPos2);
				type t=area(LPos2,LPos1);
				//Pos_とPos2,Pos3が一直線上に存在
				if(t==0)return true;
				return t1*t>=0 && t2*t>=0 && (t1+t2)/t<=1;
			}
			virtual bool includeLine(const pos_type& Pos1_,const pos_type& Pos2_)const{return includePixel(Pos1_)&&includePixel(Pos2_);}
			virtual bool includeBox(const pos_type& Pos_,const pos_type& Size_)const{return includePixel(Pos_)&&includePixel(Pos_+Size_)&&includePixel(Pos_+di_positionX(Size_))&&includePixel(Pos_+di_positionY(Size_));}
			virtual bool includeCircle(const pos_type& Pos_,const type& r_)const{return includePixel(Pos_) && (!cLine<type>(getPos1(),getPos2()).crossCircle(Pos_,r_)) && (!cLine<type>(getPos2(),getPos3()).crossCircle(Pos_,r_)) && (!cLine<type>(getPos3(),getPos1()).crossCircle(Pos_,r_));}
			virtual bool included(const itfBody<type>& Body_)const{return Body_.includeLine(getPos1(),getPos2()) && Body_.includeLine(getPos2(),getPos3()) && Body_.includeLine(getPos3(),getPos1());}
			virtual bool crossLine(const pos_type& Pos1_,const pos_type& Pos2_)const{
				if(includeLine(Pos1_,Pos2_))return true;
				cLine<type> LB(Pos1_,Pos2_);
				return LB.crossLine(getPos1(),getPos2()) || LB.crossLine(getPos2(),getPos3()) || LB.crossLine(getPos3(),getPos1());
			}
			virtual bool crossBox(const pos_type& Pos_,const pos_type& Size_)const{
				if(includeBox(Pos_,Size_))return true;
				cBox<type> BB(Pos_,Size_);
				return BB.crossLine(getPos1(),getPos2()) || BB.crossLine(getPos2(),getPos3()) || BB.crossLine(getPos3(),getPos1());
			}
			virtual bool crossCircle(const pos_type& Pos_,const type& r_)const{return includePixel(Pos_) || cLine<type>(getPos1(),getPos2()).crossCircle(Pos_,r_) || cLine<type>(getPos2(),getPos3()).crossCircle(Pos_,r_) || cLine<type>(getPos3(),getPos1()).crossCircle(Pos_,r_);}
			virtual bool crossed(const itfBody<type>& Body_)const{
				if(include(Body_))return true;
				return Body_.crossLine(getPos1(),getPos2()) || Body_.crossLine(getPos2(),getPos3()) || Body_.crossLine(getPos3(),getPos1());
			}
		};
		template <class type>
		class cTriangle:public itfTriangle<type>{
			typedef typename itfBody<type>::pos_type pos_type;
			typedef typename itfBody<type>::rect_type rect_type;
			typedef cTriangle<type> my_type;
		public:
			pos_type Pos1;
			pos_type Pos2;
			pos_type Pos3;
		protected://itfLine
			inline virtual pos_type getPos1()const{return Pos1;}
			inline virtual pos_type getPos2()const{return Pos2;}
			inline virtual pos_type getPos3()const{return Pos3;}
		public:
			cTriangle(){return;}
			cTriangle(const pos_type& Pos1_,const pos_type& Pos2_,const pos_type& Pos3_):Pos1(Pos1_),Pos2(Pos2_),Pos3(Pos3_){return;}
			cTriangle(const my_type& Body_):Pos1(Body_.Pos1),Pos2(Body_.Pos2),Pos3(Body_.Pos3){return;}
			const my_type& operator=(const my_type& Body_){
				if(this!=&Body_){
					Pos1=Body_.Pos1;
					Pos2=Body_.Pos2;
					Pos3=Body_.Pos3;
				}
				return *this;
			}
			void set(const pos_type& Pos1_,const pos_type& Pos2_,const pos_type& Pos3_){
				Pos1=Pos1_;
				Pos2=Pos2_;
				Pos3=Pos3_;
			}
		};
		template <class type>
		class itfCircle:public itfBody<type>{
			typedef typename itfBody<type>::pos_type pos_type;
			typedef typename itfBody<type>::rect_type rect_type;
			typedef itfCircle<type> my_type;
		protected://itfCircle
			inline virtual pos_type getPos()const=0;
			inline virtual type getR()const=0;
		public://itfBody<type>
			virtual bool includePixel(const pos_type& Pos_)const{return abs2(getPos()-Pos_) <= getR()*getR();}
			virtual bool includeLine(const pos_type& Pos1_,const pos_type& Pos2_)const{return includePixel(Pos1_)&&includePixel(Pos2_);}
			virtual bool includeBox(const pos_type& Pos_,const pos_type& Size_)const{return includePixel(Pos_)&&includePixel(Pos_+di_positionX(Size_))&&includePixel(Pos_+di_positionY(Size_))&&includePixel(Pos_+Size_);}
			virtual bool includeCircle(const pos_type& Pos_,const type& r_)const{return abs(getPos()-Pos_)+r_<=getR();}
			virtual bool included(const itfBody<type>& Body_)const{return Body_.includeCircle(getPos(),getR());}
			virtual bool crossLine(const pos_type& Pos1_,const pos_type& Pos2_)const{return cLine<type>(Pos1_,Pos2_).crossCircle(getPos(),getR());}
			virtual bool crossBox(const pos_type& Pos_,const pos_type& Size_)const{return cBox<type>(Pos_,Size_).crossCircle(getPos(),getR());}
			virtual bool crossCircle(const pos_type& Pos_,const type& r_)const{return abs(getPos()-Pos_)<=getR()+r_;}
			virtual bool crossed(const itfBody<type>& Body_)const{return Body_.crossCircle(getPos(),getR());}
		};
		template <class type>
		class cCircle:public itfCircle<type>{
			typedef typename itfBody<type>::pos_type pos_type;
			typedef typename itfBody<type>::rect_type rect_type;
			typedef cCircle<type> my_type;
		public:
			pos_type Pos;
			type r;
		protected://itfCircle
			inline virtual pos_type getPos()const{return Pos;}
			inline virtual type getR()const{return r;}
		public:
			cCircle():r(0){return;}
			cCircle(const pos_type& Pos_,const type& r_):Pos(Pos_),r(r_){return;}
			cCircle(const my_type& Body_):Pos(Body_.Pos),r(Body_.r){return;}
			const my_type& operator=(const my_type& Body_){
				if(this!=&Body_){
					Pos=Body_.Pos;
					r=Body_.r;
				}
				return *this;
			}
			void set(const pos_type& Pos_,const type& r_){
				Pos=Pos_;
				r=r_;
			}
		};
		template <class type>
		class itfOval:public itfBody<type>{
			typedef typename itfBody<type>::pos_type pos_type;
			typedef typename itfBody<type>::rect_type rect_type;
			typedef itfOval<type> my_type;
		protected://itfCircle
			inline virtual pos_type getPos()const=0;
			inline virtual pos_type getRs()const=0;
		public://itfBody<type>
			virtual bool includePixel(const pos_type& Pos_)const{return cCircle<type>(pos_type(getRs().y*getPos().x,getRs().x*getPos().y),getRs().x*getRs().y).includePixel(pos_type(getRs().y*Pos_.x,getRs().x*Pos_.y));}
			virtual bool includeLine(const pos_type& Pos1_,const pos_type& Pos2_)const{return cCircle<type>(pos_type(getRs().y*getPos().x,getRs().x*getPos().y),getRs().x*getRs().y).includeLine(pos_type(getRs().y*Pos1_.x,getRs().x*Pos1_.y),pos_type(getRs().y*Pos2_.x,getRs().x*Pos2_.y));}
			virtual bool includeBox(const pos_type& Pos_,const pos_type& Size_)const{return cCircle<type>(pos_type(getRs().y*getPos().x,getRs().x*getPos().y),getRs().x*getRs().y).includeBox(pos_type(getRs().y*Pos_.x,getRs().x*Pos_.y),pos_type(getRs().y*Size_.x,getRs().x*Size_.y));}
			virtual bool includeCircle(const pos_type& Pos_,const type& r_)const{return includeBox(Pos_-pos_type(r_,r_),2*pos_type(r_,r_));}
			virtual bool included(const itfBody<type>& Body_)const{return Body_.includeBox(getPos()-getRs(),getPos()+getRs());}
			virtual bool crossLine(const pos_type& Pos1_,const pos_type& Pos2_)const{return cCircle<type>(pos_type(getRs().y*getPos().x,getRs().x*getPos().y),getRs().x*getRs().y).crossLine(pos_type(getRs().y*Pos1_.x,getRs().x*Pos1_.y),pos_type(getRs().y*Pos2_.x,getRs().x*Pos2_.y));}
			virtual bool crossBox(const pos_type& Pos_,const pos_type& Size_)const{return cCircle<type>(pos_type(getRs().y*getPos().x,getRs().x*getPos().y),getRs().x*getRs().y).crossBox(pos_type(getRs().y*Pos_.x,getRs().x*Pos_.y),pos_type(getRs().y*Size_.x,getRs().x*Size_.y));}
			virtual bool crossCircle(const pos_type& Pos_,const type& r_)const{return crossBox(Pos_-pos_type(r_,r_),2*pos_type(r_,r_));}
			virtual bool crossed(const itfBody<type>& Body_)const{return Body_.crossBox(getPos()-getRs(),2*getRs());}
		};
		template <class type>
		class cOval:public itfOval<type>{
			typedef typename itfBody<type>::pos_type pos_type;
			typedef typename itfBody<type>::rect_type rect_type;
			typedef cOval<type> my_type;
		public:
			pos_type Pos;
			pos_type Rs;
		protected://itfCircle
			inline virtual pos_type getPos()const{return Pos;}
			inline virtual pos_type getRs()const{return Rs;}
		public:
			cOval():Rs(0,0){return;}
			cOval(const pos_type& Pos_,const pos_type& Rs_):Pos(Pos_),Rs(Rs_){return;}
			cOval(const my_type& Body_):Pos(Body_.Pos),Rs(Body_.Rs){return;}
			const my_type& operator=(const my_type& Body_){
				if(this!=&Body_){
					Pos=Body_.Pos;
					Rs=Body_.Rs;
				}
				return *this;
			}
			void set(const pos_type& Pos_,const pos_type& Rs_){
				Pos=Pos_;
				Rs=Rs_;
			}
		};
	}
}
#
#endif
