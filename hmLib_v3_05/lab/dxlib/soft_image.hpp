#ifndef HMLIB_SOFTIMAGE_INC
#define HMLIB_SOFTIMAGE_INC 100
#
namespace hmLib{
	namespace directx{
		class soft_image{
		private:
			struct data{
				unsigned char r;
				unsigned char g;
				unsigned char b;
				unsigned char a;
			};
		public:
			//画像走査用のイテレータ
			class iterator:
				public hmLib::basic_random_access_iterator<iterator,data>,
				public hmLib::basic_operator::bi_add<iterator,pint>,
				public hmLib::basic_operator::forward_sub<iterator,pint>{
			private:
				unsigned char* Ptr;
				pint Size;
			public:
				iterator():Ptr(0){return;}
				iterator(unsigned char* Ptr_,pint Size_):Ptr(Ptr_),Size(Size_){return;}
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
				const iterator& operator+=(const pint& num_){
					Ptr+=4*(num_.x+Size.x*num_.y);
					return *this;
				}
				const iterator& operator-=(const pint& num_){
					Ptr-=4*(num_.x+Size.x*num_.y);
					return *this;
				}
				bool operator==(const iterator& itr_)const{return Ptr==itr_.Ptr;}
				bool operator<(const iterator& itr_)const{return Ptr<itr_.Ptr;}
				data operator*(){return *reinterpret_cast<data*>(Ptr);}
				data operator[](const int& num_){return *reinterpret_cast<data*>(Ptr+4*num_);}
				data operator[](const pint& num_){return *reinterpret_cast<data*>(Ptr+4*(num_.x+Size.x*num_.y));}
				data* operator->(){return reinterpret_cast<data*>(Ptr);}
			};
		private:
			//管理用ハンドル
			int Handle;
		private: //Copy禁止
			soft_image(const soft_image& My_);
			const soft_image& operator=(const soft_image& My_);
		public:
			soft_image():Handle(-1){}
			soft_image(soft_image&& My_):Handle(-1){
			const soft_image& operator=(soft_image&& My_){
				if(this!=&My_){
					hmLib_assert(empty(),hmLib::exceptions::invalid_request,"This has already handle a soft_image.");
					std::swap(Handle,My_.Handle);
				}
				return *this;
			}
			~soft_image(){destroy();}
			void swap(soft_image& My_){std::swap(Handle,My_.Handle);}
			int get()const{return Handle;}
			bool empty()const{return Handle<=0;}
			bool create(const pint& Size_,bool Alpha_){
				hmLib_assert(empty(),hmLib::exceptions::invalid_request,"This has already handle a soft_image.");
	
				if(Alpha_)Handle=MakeARGB8ColorSoftImage(Size_.x,Size_.y);
				else Handle=MakeXRGB8ColorSoftImage(Size_.x,Size_.y);
				return empty();
			}
			bool create(const std::string& FileName_){
				hmLib_assert(empty(),hmLib::exceptions::invalid_request,"This has already handle a soft_image.");

				Handle=LoadSoftImage(FileName_.c_str());
				if(CheckPaletteSoftImage(Handle))release();
				return empty();
			}
			bool create(const std::string& FileName_,bool Alpha_){
				hmLib_assert(empty(),hmLib::exceptions::invalid_request,"This has already handle a soft_image.");

				soft_image TmpSoftImage;
				TmpSoftImage.create(FileName_);
				if(TmpSoftImage.is_alpha()==Alpha_)swap(TmpSoftImage);
				else create(TmpSoftImage,Alpha_);
				return empty();
			}
			bool create(void* _mem,const int& _memsize){
				hmLib_assert(empty(),hmLib::exceptions::invalid_request,"This has already handle a soft_image.");

				Handle=LoadSoftImageToMem(_mem,_memsize);
				if(CheckPaletteSoftImage(Handle)!=0)release();
				return empty();
			}
			bool create(void* _mem,const int& _memsize,bool Alpha_){
				hmLib_assert(empty(),hmLib::exceptions::invalid_request,"This has already handle a soft_image.");

				soft_image TmpSoftImage;
				TmpSoftImage.create(FileName_);
				if(TmpSoftImage.is_alpha()==Alpha_)swap(TmpSoftImage);
				else create(TmpSoftImage,Alpha_);
				return empty();
			}
			bool create(const image& Img_,bool Alpha_){
				cScreen Scr;
				Scr.load(Img_);
				return load(Scr,Alpha_);
			}
			bool create(const image& Img_,const pint& _pos,const pint& _size,bool Alpha_){
				cScreen Scr;
				Scr.load(Img_);
				return load(Scr,_pos,_size,Alpha_);
			}
			bool create(const screen_image& Scr_,bool Alpha_){
				return load(Scr_,pint(0,0),Scr_.size(),Alpha_);
			}
			bool create(const screen_image& Scr_,const pint& _pos,const pint& _size,bool Alpha_);
			bool create(const soft_image& SfImg_){
				return load(SfImg_,SfImg_.isAlpha());
			}
			bool create(const soft_image& SfImg_,bool Alpha_){
				return load(SfImg_,pint(0,0),SfImg_.size(),SfImg_.isAlpha());
			}
			bool create(const soft_image& SfImg_,const pint& _pos,const pint& _size,bool Alpha_){
				if(this==&SfImg_)return true;
				create(_size,Alpha_);
				BltSoftImage(_pos.x,_pos.y,_size.x,_size.y,SfImg_.getHandle(),0,0,Handle);
				return empty();
			}
			void destroy(){
				if(!empty();)DeleteSoftImage(Handle);
				Handle=-1;
			}
			int release(){
				int Tmp=Handle;
				Handle=-1;
				return Tmp;
			}
			void save(const std::string& _filename,cImage::FileType _ft)const;
			pint size()const{
				pint tmp;
				GetSoftImageSize(Handle,&(tmp.x),&(tmp.y));
				return tmp;
			}
			bool is_alpha()const{return  CheckAlphaSoftImage(Handle)!=0;}
			void fill(const hm::cRGBAClr& AClr_){
				FillSoftImage(Handle,AClr_.r,AClr_.g,AClr_.b,AClr_.a);
			}
			void fill_clr(const hm::cRGBClr& Clr_){
				iterator itr=begin(),enditr=end();
				for(;itr!=enditr;++itr){
					itr->r=Clr_.r;
					itr->g=Clr_.g;
					itr->b=Clr_.b;
				}
			}
			void fill_alpha(unsigned char Alpha_){
				iterator itr=begin(),enditr=end();
				for(;itr!=enditr;++itr){
					itr->a=Alpha_;
				}
			}
			void put_data(const pint& _pos,const hm::cRGBAClr& AClr_){
				iterator itr=begin()+_pos;
				itr->r=AClr_.r;
				itr->g=AClr_.g;
				itr->b=AClr_.b;
				itr->a=AClr_.a;
			}
			void put_clr(const pint& _pos,const hm::cRGBClr& Clr_){
				iterator itr=begin()+_pos;
				itr->r=Clr_.r;
				itr->g=Clr_.g;
				itr->b=Clr_.b;
			}
			void put_alpha(const pint& _pos,unsigned char _alpha){
				iterator itr=begin()+_pos;
				itr->a=_alpha;
			}
			hm::cRGBAClr get_data(const pint& _pos){
				iterator itr=begin()+_pos;
				return hm::cRGBAClr(itr->r,itr->g,itr->b,itr->a);
			}
			hm::cRGBClr get_clr(const pint& _pos){
				iterator itr=begin()+_pos;
				return hm::cRGBClr(itr->r,itr->g,itr->b);
			}
			unsigned char get_alpha(const pint& _pos){
				iterator itr=begin()+_pos;
				return itr->a;
			}
			iterator begin(){return iterator(static_cast<unsigned char*>(GetImageAddressSoftImage(Handle)),size());}
			iterator end(){return iterator(static_cast<unsigned char*>(GetImageAddressSoftImage(Handle)),size())+size();}
		};
	}
}
#
#endif
