#ifndef HMLIB_MEMORY_SHAREDARRAY_INC
#define HMLIB_MEMORY_SHAREDARRAY_INC 104
#
/*
=== memory::shared_array ===
v1_04/140719 hmIto
	iteratorを追加
v1_03/140415 hmIto
	unique_ptrのinclude忘れを修正
v1_02/140412 hmIto
	_shared_array_elementを無名名前空間へ移動
	allocatorのrebindを用いた正しい記載へ変更
v1_01/140408 hmIto
	shared_write_arrayにwrite関数を追加
	flushを同時に自動でやってくれる
	戻り値に書き込み済みのサイズが帰ってくる
	shared_read_arrayがpointerやunique_ptrから直接構築できるようにコンストラクタ,reset関数を追加
	内部では、shared_write_arrayを一時的に作成してから受け取っている
	shared_read_arrayにflowingを追加
	書き込み終了しているかの確認。忘れてた。]
	その他、全体的にコメント追加
*/
#include<memory>
#include<hmLib/lab/rvalue_reference.hpp>
#include"unique_ptr.hpp"
#include"default_deleter.hpp"
namespace hmLib {
	namespace {
		template<typename T, typename D>
		struct _shared_array_element {
		public:
			typedef T* pointer;
			typedef D deleter;
			typedef _shared_array_element<T, D> my_type;
			typedef rvalue_reference<my_type> rvalue_reference;
		private:
			pointer ptr;
			deleter dlt;
			unsigned int rcnt;
			unsigned int wcnt;
			unsigned int rsize;
			unsigned int wsize;
		public:
			_shared_array_element(pointer ptr_, unsigned int size_, deleter dlt_)
				: ptr(ptr_)
				, dlt(dlt_)
				, rcnt(0)
				, wcnt(0)
				, rsize(0)
				, wsize(0) {
				if(ptr) {
					wsize=size_;
					++wcnt;
				}
			}
			_shared_array_element(rvalue_reference ref_)
				: ptr(ref_.ref.ptr)
				, dlt(ref_.ref.dlt)
				, rcnt(ref_.ref.rcnt)
				, wcnt(ref_.ref.wcnt)
				, rsize(ref_.ref.rsize)
				, wsize(ref_.ref.wsize) {
				ref_.ref.ptr=0;
				ref_.ref.rcnt=0;
				ref_.ref.wcnt=0;
				ref_.ref.rsize=0;
				ref_.ref.wsize=0;
			}
			const _shared_array_element& operator=(rvalue_reference ref_) {
				if(ptr)dlt(ptr);
				prt=ref_.ref.ptr;
				dlt=ref_.ref.dlt;
				cnt=ref_.ref.cnt;
				ref_.ref.ptr=0;
				ref_.ref.rcnt=0;
				ref_.ref.wcnt=0;
				ref_.ref.rsize=0;
				ref_.ref.wsize=0;
			}
			~_shared_array_element() {
				if(ptr)dlt(ptr);
			}
			my_type* read_attach() {
				++rcnt;
				return this;
			}
			bool read_detach() {
				if(rcnt>0)--rcnt;
				return ptr!=0 && rcnt==0 && wcnt==0;
			}
			my_type* write_attach() {
				++wcnt;
				return this;
			}
			bool write_detach() {
				if(wcnt>0)--wcnt;
				if(wcnt==0)rsize=wsize;
				return ptr!=0 && rcnt==0 && wcnt==0;
			}
			pointer get() { return ptr; }
			const pointer get()const { return ptr; }
			deleter get_deleter()const { return dlt; }
			unsigned int get_read_count()const {
				return rcnt;
			}
			unsigned int get_write_count()const {
				return wcnt;
			}
			operator bool()const { ptr!=0; }
			unsigned int get_rsize()const {
				return rsize;
			}
			unsigned int get_wsize()const {
				return wsize;
			}
			bool resize(unsigned int size_) {
				if(rsize>size_ || wsize<size_)return true;
				wsize=size_;
				return false;
			}
			bool resize() {
				wsize=rsize;
				return false;
			}
			void flush() { rsize=wsize; }
			void flush(unsigned int pos_) {
				if(wsize>pos_)rsize=pos_+1;
				else rsize=wsize;
			}
			bool flowing()const { return rsize==wsize; }
		public:
			_shared_array_element()
				: ptr(0)
				, dlt()
				, rcnt(0)
				, wcnt(0)
				, rsize(0)
				, wsize(0){
			}
			_shared_array_element(const _shared_array_element& my_)
				: ptr(my_.ptr)
				, dlt(my_.dlt)
				, rcnt(my_.rcnt)
				, wcnt(my_.wcnt)
				, rsize(my_.rsize)
				, wsize(my_.wsize){
			}
			const _shared_array_element& operator=(const _shared_array_element& my_) {
				if(this!=&my_) {
					ptr=my_.ptr;
					dlt=my_.dlt;
					rcnt=my_.rcnt;
					wcnt=my_.wcnt;
					rsize=my_.rsize;
					wsize=my_.wsize;
				}
				return *this;
			}
		};
	}
	template<typename T, typename D=default_deleter<T>, typename Alloc=std::allocator<T> >
	class shared_read_array;

	//書き込み側の配列管理クラス
	template<typename T, typename D=default_deleter<T>, typename Alloc=std::allocator<T> >
	class shared_write_array {
		typedef shared_read_array<T, D, Alloc> read_type;
		friend class read_type;
	private:
		typedef shared_write_array<T, D, Alloc> my_type;
		typedef _shared_array_element<T, D> my_array_element;
		typedef rvalue_reference<my_type> rvalue_reference;
		typedef hmLib::rvalue_reference<unique_ptr<T[], D> > unique_ptr_rvalue_reference;
	public:
		typedef T* pointer;
		typedef T& reference;
		typedef D deleter;
		typedef typename Alloc::template rebind<my_array_element>::other allocator;
		typedef T* iterator;
		typedef const T* const_iterator;
	private:
		my_array_element* ptrh;
		allocator alloc;
	public:
		shared_write_array() :ptrh(0) {}
		shared_write_array(pointer ptr_, unsigned int size_) :ptrh(0) {
			construct(ptr_, size_, deleter());
		}
		shared_write_array(pointer ptr_, unsigned int size_, deleter dlt_) :ptrh(0) {
			construct(ptr_, size_, dlt_);
		}
		shared_write_array(const my_type& my_) :ptrh(0) {
			share(my_.ptrh);
		}
		const my_type& operator=(const my_type& my_) {
			if(ptrh!=my_.ptrh) {
				if(ptrh)destroy();
				share(my_.ptrh);
			}
			return *this;
		}
		explicit shared_write_array(rvalue_reference mptr_) :ptrh(0) {
			swap(mptr_.ref.ptrh);
		}
		const my_type& operator=(rvalue_reference mptr_) {
			if(this!=&(mptr_.ref)) {
				if(ptrh)destroy();
				swap(mptr_.ref.ptrh);
			}
			return *this;
		}
		shared_write_array(unique_ptr_rvalue_reference mptr_, unsigned int size_) :ptrh(0) {
			construct(mptr_.ref.get(), size_, mptr_.ref.get_deleter());
			mptr_.ref.release();
		}
		~shared_write_array() {
			if(ptrh)destroy();
		}
	public:
		//配列を管理しているか
		operator bool() const { return ptrh!=0; }
		//管理している配列の先頭アドレスを取得
		pointer get() {
			if(ptrh)return const_cast<T*>(ptrh->get());
			else return 0;
		}
		//管理している配列の先頭アドレスを取得
		const pointer get() const{
			if(ptrh)return const_cast<T*>(ptrh->get());
			else return 0;
		}
		//管理している配列のデリータを取得
		deleter get_deleter() const {
			if(ptrh)return deleter();
			return ptrh->get_deleter();
		}
		//配列アクセス
		reference operator[](unsigned int pos_) {
			return (ptrh->get())[pos_];
		}
		//配列アクセス
		const reference operator[](unsigned int pos_) const {
			return (ptrh->get())[pos_];
		}
		//指定位置にvalを書き込み、同時にflushする　戻り値で、現在のflush_sizeを返す
		unsigned int write(unsigned int pos_, const T& val_) {
			if(prth==0)return 0;
			(ptrh->get())[pos_]=val_;
			prth->flush(pos_);
			return prth->get_rsize();
		}
		//書込末尾にvalを書き込み、同時にflushする　戻り値で、現在のflush_sizeを返す
		unsigned int  write(const T& val_) {
			return write(flush_size(), val_);
		}
		//管理している配列ののサイズを返す
		unsigned int size() const {
			if(ptrh==0)return 0;
			return ptrh->get_wsize();
		}
		//書込済みサイズを返す
		unsigned int flush_size()const {
			if(ptrh==0)return 0;
			return ptrh->get_rsize();
		}
		//管理している配列のサイズをsize_まで減らす
		bool resize(unsigned int size_) {
			if(ptrh==0)return true;
			return ptrh->resize(size_);
		}
		//管理している配列のサイズを現在のflush済みサイズまで減らす
		bool resize() {
			if(ptrh==0)return true;
			return ptrh->resize();
		}
		//pos_までは書込済みにする
		void flush(unsigned int pos_) {
			if(ptrh)ptrh->flush(pos_);
		}
		//管理している配列末尾まで書込済みにする
		void flush() {
			if(ptrh)ptrh->flush();
		}
		//現在所有している配列を管理しているshared_write/read_arrayの数を返す
		unsigned int use_count()const {
			if(ptrh==0)return 0;
			return ptrh->get_read_count() + ptrh->get_write_count();
		}
		//管理する配列を与えなおす すでに管理している配列は開放する
		void reset(pointer ptr_=0, unsigned int size_=0) {
			if(ptrh)destroy();
			construct(ptr_, size_, deleter());
		}
		//管理する配列を与えなおす すでに管理している配列は開放する（デリータ付）
		void reset(pointer ptr_, unsigned int size_, deleter dlt_) {
			if(ptrh)destroy();
			construct(ptr_, size_, dlt_)
		}
		//管理する配列をunique_ptrから与えなおす すでに管理している配列は開放する
		void reset(unique_ptr_rvalue_reference mptr_, unsigned int size_) {
			if(ptrh)destroy();
			construct(mptr_.ref.get(), size_, mptr_.ref.get_deleter());
			mptr_.ref.release();
		}
		//管理している配列を交換する
		void swap(my_type& My_) {
			my_array_element* ptrh_=My_.ptrh;
			allocator alloc_=My_.alloc;
			My_.ptrh=ptrh;
			My_.alloc=alloc;
			ptrh=ptrh_;
			alloc=alloc_;
		}
		//管理している配列を開放する（配列のメモリ解放自体は、全ての管理者がいなくなった時に行われる）
		void release() {
			if(ptrh)destroy();
		}
		//iterator関数
		iterator begin() { return get(); }
		iterator end() { return get()+size(); }
		const_iterator begin()const { return get(); }
		const_iterator end()const { return get()+size(); }
	private://いずれの関数も、ptrhが存在している/空であることの確認はしていないので注意
		void construct(pointer ptr_, unsigned int size_, deleter dlt_) {
			if(ptr_==0)return;
			if(size_==0) {
				dlt_(ptr_);
				return;
			}
			ptrh=alloc.allocate(std::size_t(1));
			alloc.construct(ptrh, move(my_array_element(ptr_, size_, dlt_)));
		}
		void destroy() {
			if(ptrh->write_detach()) {
				alloc.destroy(ptrh);
				alloc.deallocate(ptrh, std::size_t(1));
			}
			ptrh=0;
		}
		void share(my_array_element* ptrh_) {
			if(ptrh_) {
				ptrh=ptrh_->write_attach();
			}
		}
		void swap(my_array_element*& ptrh_) {
			my_array_element* tmp_prth=ptrh_;
			ptrh_=ptrh;
			ptrh=tmp_prth;
		}
	};
	//読み込み側の配列管理クラス
	template<typename T, typename D, typename Alloc>
	class shared_read_array {
	private:
		typedef shared_read_array<T, D, Alloc> my_type;
		typedef shared_write_array<T, D, Alloc> write_type;
		typedef _shared_array_element<T, D> my_array_element;
		typedef rvalue_reference<my_type> my_rvalue_reference;
		typedef rvalue_reference<write_type> write_rvalue_reference;
		typedef hmLib::rvalue_reference<unique_ptr<T[], D> > unique_ptr_rvalue_reference;
	public:
		typedef const T* pointer;
		typedef const T& reference;
		typedef D deleter;
		typedef typename Alloc::template rebind<my_array_element>::other allocator;
		typedef const T* const_iterator;
	private:
		my_array_element* ptrh;
		allocator alloc;
	public:
		shared_read_array() :ptrh(0) {}
		shared_read_array(const my_type& my_) :ptrh(0) {
			share(my_.ptrh);
		}
		shared_read_array(pointer ptr_, unsigned int size_) :ptrh(0) {
			*this=hmLib::move(shared_write_array(mptr_, size_));
		}
		shared_read_array(pointer ptr_, unsigned int size_, deleter dlt_) :ptrh(0) {
			*this=hmLib::move(shared_write_array(mptr_, size_, dlt_));
		}
		const my_type& operator=(const my_type& my_) {
			if(ptrh!=my_.ptrh) {
				if(ptrh)destroy();
				share(my_.ptrh);
			}
			return *this;
		}
		explicit shared_read_array(my_rvalue_reference mptr_) :ptrh(0) {
			swap(mptr_.ref);
		}
		const my_type& operator=(my_rvalue_reference mptr_) {
			if(this!=&(mptr_.ref)) {
				if(ptrh)destroy();
				swap(mptr_.ref);
			}
			return *this;
		}
		shared_read_array(const write_type& my_) :ptrh(0) {
			share(my_.ptrh);
		}
		const my_type& operator=(const write_type& my_) {
			if(ptrh!=my_.ptrh) {
				if(ptrh)destroy();
				share(my_.ptrh);
			}
			return *this;
		}
		explicit shared_read_array(write_rvalue_reference mptr_) :ptrh(0) {
			swap(mptr_.ref.ptrh);
		}
		const my_type& operator=(write_rvalue_reference mptr_) {
			if(this!=&(mptr_.ref)) {
				if(ptrh)destroy();
				swap(mptr_.ref.ptrh);
			}
			return *this;
		}
		shared_read_array(unique_ptr_rvalue_reference mptr_, unsigned int size_) :ptrh(0) {
			*this=hmLib::move(shared_write_array(mptr_, size_));
		}
		~shared_read_array() {
			if(ptrh)destroy();
		}
	public:
		//配列を管理しているか
		operator bool() const{ return ptrh!=0; }
		//管理している配列の先頭アドレスを取得
		pointer get() const {
			if(ptrh==0)return 0;
			return const_cast<T*>(ptrh->get());
		}
		//管理している配列のデリータを取得
		deleter get_deleter() const {
			if(prth==0)return deleter();
			return ptrh->get_deleter();
		}
		//配列アクセス
		reference operator[](unsigned int pos)const { return (ptrh->get())[pos]; }
		//管理している配列の読込可能なサイズを返す
		unsigned int size() const {
			if(ptrh==0)return 0;
			return ptrh->get_rsize();
		}
		//管理している配列のサイズを返す
		unsigned int maxsize()const {
			if(ptrh==0)return 0;
			return ptrh->get_wsize();
		}
		//管理している配列が書き込み終了しているかを取得する
		bool flowing()const {
			if(ptrh==0)return 0;
			ptrh->flowing();
		}
		//現在所有している配列を管理しているshared_write/read_arrayの数を返す
		unsigned int use_count()const {
			if(ptrh==0)return 0;
			return ptrh->get_write_count() + ptrh->get_read_count();
		}
		//管理する配列を与えなおす すでに管理している配列は開放する
		void reset(pointer ptr_=0, unsigned int size_=0) {
			if(ptrh)destroy();
			*this=hmLib::move(shared_write_array(ptr_, size_));
		}
		//管理する配列を与えなおす すでに管理している配列は開放する（デリータ付）
		void reset(pointer ptr_, unsigned int size_, deleter dlt_) {
			if(ptrh)destroy();
			*this=hmLib::move(shared_write_array(ptr_, size_, dlt_));
		}
		//管理する配列をunique_ptrから与えなおす すでに管理している配列は開放する
		void reset(unique_ptr_rvalue_reference mptr_, unsigned int size_) {
			if(ptrh)destroy();
			*this=hmLib::move(shared_write_array(mptr_, size_));
		}
		//管理している配列を交換する
		void swap(my_type& My_) {
			my_array_element* ptrh_=My_.ptrh;
			allocator alloc_=My_.alloc;
			My_.ptrh=ptrh;
			My_.alloc=alloc;
			ptrh=ptrh_;
			alloc=alloc_;
		}
		//管理している配列を開放する（配列のメモリ解放自体は、全ての管理者がいなくなった時に行われる）
		void release() {
			if(ptrh)destroy();
		}
		//iterator関数
		const_iterator begin()const { return get(); }
		const_iterator end()const { return get()+size(); }
	private://いずれの関数も、ptrhが存在している/空であることの確認はしていないので注意
		void destroy() {
			if(ptrh->read_detach()) {
				alloc.destroy(ptrh);
				alloc.deallocate(ptrh, std::size_t(1));
			}
			ptrh=0;
		}
		void share(my_array_element* ptrh_) {
			if(ptrh_) {
				ptrh=ptrh_->read_attach();
			}
		}
		void swap(my_array_element*& ptrh_) {
			my_array_element* tmp_prth=ptrh_;
			ptrh_=ptrh;
			ptrh=tmp_prth;
		}
	};
}
#
#endif
