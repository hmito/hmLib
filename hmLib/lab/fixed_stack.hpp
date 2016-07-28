#ifndef HMLIB_FIXEDSTACK_INC
#define HMLIB_FIXEDSTACK_INC 100
/*=== fixed_stack ===
固定長の簡易配列

=== fixed_stack ===
v1_00/140329 hmIto
作成
*/
#ifndef HMLIB_FIXEDVECTOR_INC
#	include<hmLib/lab/fixed_vector.hpp>
#endif
namespace hmLib {
	template<typename T>
	class fixed_stack {
		typedef fixed_stack<T> my_type;
		typedef fixed_vector<T> base_type;
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
		fixed_stack() :Base() {
		}
		fixed_stack(unsigned int Size_)
			:Base(Size) {
		}
		fixed_stack(T* BufBegin_, T* BufEnd_, destructor Destructor_)
			:Base(BufBegin_, BufEnd_, Destructor_) {
		}
		~fixed_stack() {
		}
	private:
		//コピー禁止
		fixed_stack(const my_type& My_);
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
		unsigned int size()const { return Base.size(); }
		//最大サイズ
		unsigned int maxsize()const { return Base.maxsize(); }
		//バッファの確保の有無
		operator bool()const { return static_cast<bool>(Base); }
		//スワップ
		void swap(my_type& My_) { Base.swap(My_.Base); }
		//クリア
		void clear() { Base.clear(); }
	public:
		void pop() { Base.pop_back(); }
		void push(const T& val_) { Base.push_back(val_); }
	public:
		//先頭アドレスを取得
		T* data() { return Base.data(); }
		//先頭のデータを取得
		T& next() { return Base.back(); }
		const T& next()const { return Base.back(); }
		//イテレータを取得
		iterator begin() { return Base.begin(); }
		iterator end() { return Base.end(); }
		const_iterator begin()const { return Base.begin(); }
		const_iterator end()const { return Base.end(); }
	};
}
#
#endif
