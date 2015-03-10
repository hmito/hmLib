/*
	list型高速割当コンテナ
	位置を選ばない頻繁なinsert&eraseが必要な場合に有効
	listとの相違点は、
		newとdeleteのタイミングがinsert,eraseと異なる
			あらかじめまとめて確保&削除してもメモリを再利用
		chainの一部を別のchainに委ねることができない
			確保位置を配列単位で管理しているので、全て統合する以外に移管する方法がない
*/
#ifndef HMLIB_CHAIN_INC
#define HMLIB_CHAIN_INC 100
#
#include <vector>
#include <deque>
#include <algorithm>
#include <type_traits>
#include <sstream>
#include <boost/iterator/iterator_facade.hpp>
#include<boost/pool/pool_alloc.hpp>
//#include <basic_iterator.hpp>
namespace hmLib{
	template<class T>
	class chain{
	private:
		typedef chain<T> this_type;
	protected:
		typedef unsigned int size_type;
		class Datum{
		public:
			Datum* front;
			Datum* back;
			T t;
		public:
			Datum()
				:front(/*C++0x_nullptr/*/NULL/**/)
				,back(/*C++0x_nullptr/*/NULL/**/){
				return;
			}
			Datum(const T& _t,Datum* _front,Datum* _back)
				:t(_t)
				,front(/*C++0x_nullptr/*/NULL/**/)
				,back(/*C++0x_nullptr/*/NULL/**/){
				return;
			}
			Datum(const Datum& _dat)
				:t(_dat.t)
				,front(_dat.front)
				,back(_dat.back){
				return;
			}
			T* operator->(){return &t;}
			Datum* frontPtr(){return front;}
			Datum* backPtr(){return back;}
			const Datum* frontPtr()const{return front;}
			const Datum* backPtr()const{return back;}
			Datum& operator>>(Datum& My_){
				My_.back=this;
				front=&My_;
				return My_;
			}
			Datum& operator<<(Datum& My_){
				My_.front=this;
				back=&My_;
				return My_;
			}
		};
	public:
		struct iterator:public boost::iterator_facade<iterator,T,boost::bidirectional_traversal_tag>{
			friend class chain;
		private:
			Datum* dat;
		private:
			friend class boost::iterator_core_access;
			void increment(){dat=dat->front;}
			void decrement(){dat=dat->back;}
			T& dereference()const{return dat->t;}
			bool equal(const iterator& val_)const{return dat==val_.dat;}
		public:
			iterator():dat(0){return;}
			iterator(Datum* dat_):dat(dat_){return;}
			iterator(const iterator& itr_):dat(itr_.dat){return;}
		};
		struct reverse_iterator:public boost::iterator_facade<iterator,T,boost::bidirectional_traversal_tag>{
			friend class chain;
		private:
			Datum* dat;
		private:
			friend class boost::iterator_core_access;
			void increment(){dat=dat->back;}
			void decrement(){dat=dat->front;}
			T& dereference()const{return dat->t;}
			bool equal(const iterator& val_)const{return dat==val_.dat;}
		public:
			reverse_iterator():dat(0){return;}
			reverse_iterator(Datum* dat_):dat(dat_){return;}
			reverse_iterator(const reverse_iterator& itr_):dat(itr_.dat){return;}
		};
		struct const_iterator:public boost::iterator_facade<iterator,const T,boost::bidirectional_traversal_tag>{
			friend class chain;
		private:
			Datum* dat;
		private:
			friend class boost::iterator_core_access;
			void increment(){dat=dat->front;}
			void decrement(){dat=dat->back;}
			const T& dereference()const{return dat->t;}
			bool equal(const iterator& val_)const{return dat==val_.dat;}
		public:
			const_iterator():dat(0){return;}
			const_iterator(Datum* dat_):dat(dat_){return;}
			const_iterator(const iterator& itr_):dat(itr_.dat){return;}
		};
		struct const_reverse_iterator:public boost::iterator_facade<iterator,const T,boost::bidirectional_traversal_tag>{
			friend class chain;
		private:
			Datum* dat;
		private:
			friend class boost::iterator_core_access;
			void increment(){dat=dat->back;}
			void decrement(){dat=dat->front;}
			const T& dereference()const{return dat->t;}
			bool equal(const iterator& val_)const{return dat==val_.dat;}
		public:
			const_reverse_iterator():dat(0){return;}
			const_reverse_iterator(Datum* dat_):dat(dat_){return;}
			const_reverse_iterator(const reverse_iterator& itr_):dat(itr_.dat){return;}
		};
	private:
		typedef std::pair<size_type,Datum*> buf_type;
	protected:
		size_type BufSize;
		Datum Null;
//		boost::fast_pool_allocator<buf_type> Bufs;
		std::vector<buf_type> Bufs;
		std::deque<Datum*> Ptrs;
	protected://chain
		void pre_insert(size_type Size_){
			if(Ptrs.size()<Size_){
				buf_type Buf;
				Buf.first=max(BufSize/2,Size_-Ptrs.size());
				Buf.second=new Datum[Buf.first];
				Datum* Ptr=Buf.second;
				for(size_type i=0;i<Buf.first;++i)Ptrs.push_back(Ptr++);
				BufSize+=Buf.first;
			}
		}
		void pre_erase(size_type Size_){return;}
		void pre_reset(){
			for(auto itr=Bufs.begin();itr!=Bufs.end();++itr)delete[] itr->second;
			Bufs.clear();
			Ptrs.clear();
		}
		void copy(const this_type& My_){
			if(BufSize<My_.BufSize)pre_insert(My_.BufSize-BufSize);
			clear();
			insert(end(),My_.begin(),My_.end());
		}
	public:
		//constructor系
		chain():BufSize(0){
			Null.front=&Null;
			Null.back=&Null;
		}
		chain(const this_type& _ar){
			Null.front=&Null;
			Null.back=&Null;
			copy(_ar);
		}
		chain(size_type Size_,const T& Val_=T()):BufSize(0){
			Null.front=&Null;
			Null.back=&Null;
			assign(Size_,Val_);
		}
		template<class input_iterator>
		chain(input_iterator _start,input_iterator _end):BufSize(0){
			Null.front=&Null;
			Null.back=&Null;
			assign(_start,_end);
		}
		virtual ~chain(){pre_reset();}
		//operator系
		const chain& operator=(const this_type& My_){
			if(this!=&My_)copy(My_);
			return *this;
		}
		//list-iterator系
		iterator begin(){return iterator(Null.front);}
		iterator end(){return iterator(&Null);}
		reverse_iterator rbegin(){return reverse_iterator(Null.back);}
		reverse_iterator rend(){return reverse_iterator(&Null);}
		const_iterator begin()const{return const_iterator(Null.front);}
		const_iterator end()const{return const_iterator(&Null);}
		const_reverse_iterator rbegin()const{return const_reverse_iterator(Null.back);}
		const_reverse_iterator rend()const{return const_reverse_iterator(&Null);}
		//list-assign系
		void assign(size_type _num,const T& _val=T()){_assign_num(_num,_val);}
		template<class input_iterator>
		void assign(input_iterator _start,input_iterator _last){_assign_itr(_start,_last,std::is_integral<input_iterator>::type());}
	private:
		//list-assign系
		void _assign_num(size_type _num,const T& _val=T()){
			clear();
			insert(end(),_num,_val);
		}
		template<class input_iterator>
		void _assign_itr(input_iterator _start,input_iterator _last,std::false_type){
			clear();
			insert(end(),_start,_last);
		}
		template<class input_iterator>
		void _assign_itr(input_iterator _start,input_iterator _last,std::true_type){_assign_num(static_cast<size_type>(_start),static_cast<const T&>(_last));}
	public:
		void clear(){
			for(auto itr=begin();itr!=end();++itr)Ptrs.push_back(itr.dat);
			Null.front=&Null;
			Null.back=&Null;
		}
		void resize(size_type new_size_,const T& val_){
			if(new_size_>size())insert(end(),new_size_-size(),val_);
			else if(new_size_<size())erase(begin(),begin()+(size()-new_size_));
		}
		//list-size系
		bool empty()const{return Ptrs.size()==BufSize;}
		size_type size()const{return (BufSize-Ptrs.size());}
		size_type capacity()const{return BufSize;}
		//list-reference系
		T& front(){return Null.front->t;}
		T& back(){return Null.back->t;}
		const T& front()const{return Null.front->t;}
		const T& back()const{return Null.back->t;}
		//list-buf系
		void push_back(const T& _val){insert(end(),_val);}
		void push_front(const T& _val){insert(begin(),_val);}
		void pop_back(){erase(iterator(Null.back));}
		void pop_front(){erase(iterator(Null.front));}
		iterator insert(iterator itr,const T& _val){
			pre_insert(1);

			Datum* Front=itr.dat;
			Datum* Back=itr.dat->back;

			Datum* Pos=Ptrs.front();
			Ptrs.pop_front();

			Pos->t=_val;

			(*Back)>>(*Pos)>>(*Front);

//			pos->t=_val;
//			pos->front=itr.dat;
//			pos->back=itr.dat->back;
//			pos->back->front=pos;
//			pos->front->back=pos;

			return iterator(Pos);
		}
		void insert(iterator itr,size_type _num,const T& _val){_insert_num(itr,_num,_val);}
		template<class input_iterator>
		void insert(iterator itr,input_iterator _start,input_iterator _end){_insert_itr(itr,_start,_end,std::is_integral<input_iterator>::type());}
	private:
		void _insert_num(iterator itr,size_type _num,const T& _val){
			pre_insert(_num);

			Datum* Front=itr.dat;
			Datum* Back=itr.dat->back;

			for(size_type i=0;i<_num;++i){
				Datum* Pos=Ptrs.front();
				Ptrs.pop_front();

				Pos->t=_val;

				(*Back)>>(*Pos);
				Back=Pos;
			}

			(*Back)>>(*Front);
		}
		template<class input_iterator>
		void _insert_itr(iterator itr,input_iterator _start,input_iterator _end,std::true_type){
			_insert_num(itr,static_cast<size_type>(_start),static_cast<const T&>(_end));
		}
		template<class input_iterator>
		void _insert_itr(iterator itr,input_iterator _start,input_iterator _end,std::false_type){
			Datum* Front=itr.dat;
			Datum* Back=itr.dat->back;

			while(_start!=_end){
				pre_insert(1);

				Datum* Pos=Ptrs.front();
				Ptrs.pop_front();

				Pos->t=(*_start);

				(*Back)>>(*Pos);
				Back=Pos;
			}

			(*Back)>>(*Front);			
		}
	public:
		iterator erase(iterator itr){
			Datum* Back=itr.dat->back;
			Datum* Front=itr.dat->front;

			pre_erase(1);
			Ptrs.push_back(itr.dat);

			(*Back)>>(*Front);

			return iterator(Front);
		}
		iterator erase(iterator _start,iterator _end){
			Datum* Back=_start.dat->back;
			Datum* Front=_end.dat;

			for(;_start!=_end;++_start){
				pre_erase(1);
				Ptrs.push_back(_start.dat);
			}

			(*Back)>>(*Front);

			return iterator(Front);
		}
		//list-algorithm系関数群
		void reverse(){
			iterator itr=end();
			do{
				std::swap(itr.dat->front,itr.dat->back);
				--itr;
			}while(itr!=end());
		}
		void swap(chain& My_){
			std::swap(BufSize,My_.BufSize);
			std::swap(Null.front,My_.Null.front);
			std::swap(Null.back,My_.Null.back);
			std::swap(Bufs,My_.Bufs);
			std::swap(Ptrs,My_.Ptrs);
		}
		// itrの位置にxを接続
		void splice(iterator itr, this_type& My_){
			Bufs.insert(Bufs.end(),My_.Bufs.begin(),Bufs.end());
			My_.Bufs.clear();

			Ptrs.insert(Ptrs.end(),My_.Ptrs.begin(),Ptrs.end());
			My_.Ptrs.clear();

			BufSize+=My_.BufSize;
			My_.BufSize=0;

			Datum* Front=itr.dat;
			Datum* Back=itr.dat->back;
			(*Back)>>(*(My_.Null.front));
			(*(My_.Null.back))>>(*Front);
			My_.Null.front=0;
			My_.Null.back=0;
		}
		 // 値valueを持つ要素を削除する。
		void remove(const T& val_){
			Datum* Pos=0;

			for(iterator itr=begin();itr==end();++itr){
				if(*itr==val_){
					Pos=itr->back;
					Ptrs.push_back(&(*itr));
				}else if(Pos){
					(*Pos)>>(*itr);
					Pos=0;
				}
			}
		}
		// 条件付き削除
		template <class tPred>
		void remove_if(tPred Pred){
			Datum* Pos=0;

			for(iterator itr=begin();itr==end();++itr){
				if(Pred(*itr)){
					Pos=itr->back;
					Ptrs.push_back(&(*itr));
				}else if(Pos){
					(*Pos)>>(*itr);
					Pos=0;
				}
			}
		}
	};
}
namespace std{
	template<class T>
	void swap(hmLib::chain<T>& Val1_,hmLib::chain<T>& Val2_){
		Val1_.swap(Val2_);
	}
}
#
#endif
