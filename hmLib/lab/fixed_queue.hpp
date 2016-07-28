#ifndef HMLIB_FIXEDQUEUE_INC
#define HMLIB_FIXEDQUEUE_INC 100
/*=== fixed_deque ===
固定長の簡易配列

=== fixed_deque ===
v1_00/140329 hmIto
作成
*/
#ifndef HMLIB_FIXEDDEQUE_INC
#	include <hmLib/lab/fixed_deque.hpp>
#endif
namespace hmLib {
	template<typename T>
	class fixed_queue {
		typedef fixed_queue<T> my_type;
		typedef fixed_deque<T> base_type;
	public:
		typedef base_type::destructor destructor;
	private:
		static void default_destructor(T* Ptr) { delete[] Ptr; }
	public:
		typedef base_type::iterator iterator;
		typedef base_type::const_iterator const_iterator;
	private:
		base_type Base;
	public:
		//コンストラクタ
		fixed_queue() :Base() {
		}
		fixed_queue(unsigned int Size_)
			:Base(Size){
		}
		fixed_queue(T* BufBegin_, T* BufEnd_, destructor Destructor_)
			:Base(BufBegin_,BufEnd_,Destructor_) {
		}
		~fixed_queue() {
		}
	private:
		//コピー禁止
		fixed_queue(const my_type& My_);
		const my_type& operator=(const my_type& My_);
	public:
		//バッファを再確保
		void reset(unsigned int Size_) {
			Base.reset(Size_);
		}
		void reset(T* BufBegin_, T* BufEnd_, destructor Destructor_) {
			Base.reset(BufBegin_, BufEnd_, Destructor_);
		}
		//バッファが空か
		bool empty()const { return Base.empty(); }
		//バッファがいっぱいか
		bool full()const { return Base.full(); }
		//バッファサイズ
		unsigned int size()const {return Base.size();}
		//最大サイズ
		unsigned int maxsize()const { return Base.maxsize(); }
		//バッファの確保の有無
		operator bool()const { return static_cast<bool>(Base); }
		//スワップ
		void swap(my_type& My_) {Base.swap(My_.Base);}
		//クリア
		void clear() {Base.clear();}
	public:
		void pop() { Base.pop_front(); }
		void push(const T& val_) { Base.push_back(val_); }
	public:
		//先頭アドレスを取得
		T* data() { return Base.data(); }
		//先頭のデータを取得
		T& next() {return Base.front();}
		const T& next()const { return Base.front(); }
		//イテレータを取得
		iterator begin() { return Base.begin(); }
		iterator end() { return Base.end(); }
		const_iterator begin()const { return Base.begin(); }
		const_iterator end()const { return Base.end(); }
	};
}
#
#endif
