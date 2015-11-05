#ifndef HMLIB_ENUMERATOR_INC
#define HMLIB_ENUMERATOR_INC 100
#
/*
ターゲット
・iteratorクラスを非template関数で使いたい
・でも、any_iteratorでは、コピー等の操作が高コストすぎる
・begin, endが直接触りたいわけでなく、それを用いた操作が呼べればよい
ような場合に使うクラス。

目指すのは
・virtual関数でiteratorクラスを使用するためのラッパークラス
struct base{
public:
	template<typename input_iterator>
	bool func(input_iterator Begin, input_iterator End){
		input_enumerator<find_concept> Enum(Begin, End);
		return virtual_func(std::move(Enum));
	}
protected:
	virtual bool virtual_func(input_enumerator<find_concept> Enum)=0;
};
・機能拡張がconceptによって可能
・conceptが違っても、concept間の互換性があれば（アップキャスト可能なら）move可能
*/
namespace hmLib{
	namespace enumerators{
		template<typename T>
		struct default_concept{};
		template<typename T>
		struct find_concept{
		public:
			struct mixin_interface{
			public:
				virtual ~mixin_interface(){}
			public:
				virtual void find(T Val_) = 0;
				virtual void find_begin(T Val_) = 0;
				virtual void find_end(T Val_) = 0;
			};
		public:
			template<typename iterator_holder_>
			struct mixin :public mixin_interface{
			public:
				void find(iterator_holder_& Holder, T Val) override{
					Holder.itr = std::find(Holder.begin, Holder.end, Val);
				}
				void find_begin(iterator_holder_& Holder, T Val) override{
					Holder.begin = std::find(Holder.begin, Holder.end, Val);
				}
				void find_end(iterator_holder_& Holder, T Val) override{
					Holder.end = std::find(Holder.begin, Holder.end, Val);
				}
			};
		public:
			void find(T Val_){ ref.find(Val_); }
			void find_begin(T Val_){ ref.find_begin(Val_); }
			void find_end(T Val_){ ref.find_end(Val_); }
		};
	}
	template<typename T, typename Concept = enumerators::default_concept<T>>
	struct foward_enumeratort: public Concept{
	private:
		struct holder_interface : public Concept::mixin_interface{
		public:
			virtual const T& operator()(void) const = 0;
			virtual T& operator()(void) = 0;
			virtual operator bool()const = 0;
			virtual void operator++() = 0;
			virtual void reset() = 0;
		};
	private:
		holder_interface& ref;
	public:
		template<typename iterator_>
		struct holder :public  holder_interface, public Concept::mixin<holder<iterator_>> {
			iterator_ begin;
			iterator_ end;
			iterator_ itr;
		public:
			holder(iterator_ begin_, iterator_ end_)
				: begub(begin_)
				, end(end_)
				, itr(begin_){}
		public:
			const T& operator()(void) const override{ return *itr; }
			T& operator()(void) override{ return *itr; }
			operator bool()const override{ return itr != end; }
			void operator++() override{ ++itr; }
			void reset() override{ begin = itr; }
		};
	public:
		foward_enumeratort(holder_interface& ref_) :ref(ref_){}
	public:
		const T& operator()(void) const{ return ref(); }
		T& operator()(void){ return ref(); }
		operator bool()const{ return static_cast<bool>(ref()); }
		void operator++(){ ++ref; }
		void reset(){ ref.reset(); }
	};
}
#
#endif
