#ifndef HMLIB_ANYITERATOR_INC
#define HMLIB_ANYITERATOR_INC 101
#
/*===any_iterator.hpp===
様々なiteratorを受け取り、オリジナルと同様にふるまう事の出来る汎用iteratorクラス。
内部で値コピーによりiteratorデータを保持している。
速度面では、通常のiteratorには劣る（処理によるが、2～100倍ほど）
マルチスレッド対応

any_iterator:v1_01/130328 hmIto
	const_forward/const_bidirectional/const_random_access_any_iteratortを追加
	forward_any_iteratorの致命的なバグを修正
		static destruct関数をdestroyと書いて呼び出していた問題を修正
any_iterator:v1_00/130326 hmIto
	output/input/forward/bidirectional/random_access_any_iteratorを追加
*/
#include<memory>
#include<boost/iterator/iterator_facade.hpp>
#include<boost/pool/pool_alloc.hpp>
namespace hmLib{
	//input_any_iterator
	template<typename value_type,typename difference_type=int>
	struct input_any_iterator:public boost::iterator_facade<input_any_iterator<value_type,difference_type>,const value_type,boost::single_pass_traversal_tag,const value_type&,difference_type>{
	private:
		typedef input_any_iterator<value_type,difference_type> my_type;
	private:
		struct basic_iterator_adaptor{
		public:
			struct deleter{void operator()(basic_iterator_adaptor* ptr)const{ptr->destroy();}};
		public:
			virtual basic_iterator_adaptor* clone()const=0;
			virtual void destroy()=0;
			virtual bool equal(const basic_iterator_adaptor& itr)const=0;
			virtual void increment()=0;
			virtual const value_type& dereference()const=0;
		public:
			template<typename iterator_>
			static basic_iterator_adaptor* construct(iterator_ itr);
		};
		template<typename iterator_>
		struct iterator_adaptor:public basic_iterator_adaptor{
			typedef iterator_adaptor<iterator_> adaptor;
		private:
			iterator_ itr;
		public:
			basic_iterator_adaptor* clone()const override{return adaptor::construct(itr);}
			void destroy() override{adaptor::destruct(this);}
			bool equal(const basic_iterator_adaptor& itr_)const override{
				const adaptor* adp=static_cast<const adaptor*>(&itr_);
				if(adp==nullptr)return false;
				return itr == adp->itr;
			}
			void increment() override{++itr;}
			const value_type& dereference()const override{return *itr;}
		public:
			iterator_adaptor(iterator_ itr_):itr(itr_){}
		private:
			static boost::fast_pool_allocator<adaptor> allocator;
		public:
			static void destruct(adaptor* ptr){
				allocator.destroy(ptr);
				allocator.deallocate(ptr);
			}
			static basic_iterator_adaptor* construct(iterator_ titr_){
				adaptor* ans=allocator.allocate();
				allocator.construct(ans,adaptor(titr_));
				return ans;
			}
		};
	private:
		std::unique_ptr<basic_iterator_adaptor,typename basic_iterator_adaptor::deleter> ptr;
	public://for iterator_facade
		bool equal(const my_type& My_)const{
			if(ptr==nullptr || My_.ptr==nullptr)return false;
			return ptr->equal(*(My_.ptr));
		}
		void increment(){ptr->increment();}
		const value_type& dereference()const{return ptr->dereference();}
	public://for conctructor
		input_any_iterator():ptr(nullptr){}
		input_any_iterator(const my_type& My_):ptr(nullptr){if(My_.ptr)ptr.reset(My_.ptr->clone());}
		const my_type& operator=(const my_type& My_){
			if(this!=&My_)ptr.reset(My_.ptr->clone());
			return *this;
		}
		input_any_iterator(my_type&& My_):ptr(std::move(My_.ptr)){}
		const my_type& operator=(my_type&& My_){
			if(this!=&My_)ptr=std::move(My_.ptr);
			return *this;
		}
		template<typename iterator_>
		input_any_iterator(iterator_ itr_):ptr(basic_iterator_adaptor::construct(itr_)){}
		template<typename iterator_>
		const my_type& operator=(iterator_ itr_){
			ptr.reset(basic_iterator_adaptor::construct(itr));
			return *this;
		}
	};
	template<typename value_type,typename difference_type>
	template<typename iterator_>
	static typename input_any_iterator<value_type,difference_type>::basic_iterator_adaptor* input_any_iterator<value_type,difference_type>::basic_iterator_adaptor::construct(iterator_ itr_){return iterator_adaptor<iterator_>::construct(itr_);}
	template<typename value_type,typename difference_type>
	template<typename iterator_>
	boost::fast_pool_allocator<typename input_any_iterator<value_type,difference_type>::iterator_adaptor<iterator_>> input_any_iterator<value_type,difference_type>::iterator_adaptor<iterator_>::allocator;
	//forward_any_iterator
	template<typename value_type,typename difference_type=int>
	struct const_forward_any_iterator:public boost::iterator_facade<const_forward_any_iterator<value_type,difference_type>,const value_type,boost::forward_traversal_tag,const value_type&,difference_type>{
	private:
		typedef const_forward_any_iterator<value_type,difference_type> my_type;
	private:
		struct basic_iterator_adaptor{
		public:
			struct deleter{void operator()(basic_iterator_adaptor* ptr)const{ptr->destroy();}};
		public:
			virtual basic_iterator_adaptor* clone()const=0;
			virtual void destroy()=0;
			virtual bool equal(const basic_iterator_adaptor& itr)const=0;
			virtual void increment()=0;
			virtual const value_type& dereference()const=0;
		public:
			template<typename iterator_>
			static basic_iterator_adaptor* construct(iterator_ itr);
		};
		template<typename iterator_>
		struct iterator_adaptor:public basic_iterator_adaptor{
			typedef iterator_adaptor<iterator_> adaptor;
		private:
			iterator_ itr;
		public:
			basic_iterator_adaptor* clone()const override{return adaptor::construct(itr);}
			void destroy() override{adaptor::destruct(this);}
			bool equal(const basic_iterator_adaptor& itr_)const override{
				const adaptor* adp=static_cast<const adaptor*>(&itr_);
				if(adp==nullptr)return false;
				return itr == adp->itr;
			}
			void increment() override{++itr;}
			const value_type& dereference()const override{return *itr;}
		public:
			iterator_adaptor(iterator_ itr_):itr(itr_){}
		private:
			static boost::fast_pool_allocator<adaptor> allocator;
		public:
			static void destruct(adaptor* ptr){
				allocator.destroy(ptr);
				allocator.deallocate(ptr);
			}
			static basic_iterator_adaptor* construct(iterator_ titr_){
				adaptor* ans=allocator.allocate();
				allocator.construct(ans,adaptor(titr_));
				return ans;
			}
		};
	private:
		std::unique_ptr<basic_iterator_adaptor,typename basic_iterator_adaptor::deleter> ptr;
	public://for iterator_facade
		bool equal(const my_type& My_)const{
			if(ptr==nullptr || My_.ptr==nullptr)return false;
			return ptr->equal(*(My_.ptr));
		}
		void increment(){ptr->increment();}
		const value_type& dereference()const{return ptr->dereference();}
	public://for conctructor
		const_forward_any_iterator():ptr(nullptr){}
		const_forward_any_iterator(const my_type& My_):ptr(nullptr){if(My_.ptr)ptr.reset(My_.ptr->clone());}
		const my_type& operator=(const my_type& My_){
			if(this!=&My_)ptr.reset(My_.ptr->clone());
			return *this;
		}
		const_forward_any_iterator(my_type&& My_):ptr(std::move(My_.ptr)){}
		const my_type& operator=(my_type&& My_){
			if(this!=&My_)ptr=std::move(My_.ptr);
			return *this;
		}
		template<typename iterator_>
		const_forward_any_iterator(iterator_ itr_):ptr(basic_iterator_adaptor::construct(itr_)){}
		template<typename iterator_>
		const my_type& operator=(iterator_ itr_){
			ptr.reset(basic_iterator_adaptor::construct(itr));
			return *this;
		}
	};
	template<typename value_type,typename difference_type>
	template<typename iterator_>
	static typename const_forward_any_iterator<value_type,difference_type>::basic_iterator_adaptor* const_forward_any_iterator<value_type,difference_type>::basic_iterator_adaptor::construct(iterator_ itr_){return iterator_adaptor<iterator_>::construct(itr_);}
	template<typename value_type,typename difference_type>
	template<typename iterator_>
	boost::fast_pool_allocator<typename const_forward_any_iterator<value_type,difference_type>::iterator_adaptor<iterator_>> const_forward_any_iterator<value_type,difference_type>::iterator_adaptor<iterator_>::allocator;
	//bidirectional_any_iterator
	template<typename value_type,typename difference_type=int>
	struct const_bidirectional_any_iterator:public boost::iterator_facade<const_bidirectional_any_iterator<value_type,difference_type>,const value_type,boost::bidirectional_traversal_tag,const value_type&,difference_type>{
	private:
		typedef const_bidirectional_any_iterator<value_type,difference_type> my_type;
	private:
		struct basic_iterator_adaptor{
		public:
			struct deleter{void operator()(basic_iterator_adaptor* ptr)const{ptr->destroy();}};
		public:
			virtual basic_iterator_adaptor* clone()const=0;
			virtual void destroy()=0;
			virtual bool equal(const basic_iterator_adaptor& itr)const=0;
			virtual void increment()=0;
			virtual void decrement()=0;
			virtual const value_type& dereference()const=0;
		public:
			template<typename iterator_>
			static basic_iterator_adaptor* construct(iterator_ itr);
		};
		template<typename iterator_>
		struct iterator_adaptor:public basic_iterator_adaptor{
			typedef iterator_adaptor<iterator_> adaptor;
		private:
			iterator_ itr;
		public:
			basic_iterator_adaptor* clone()const override{return adaptor::construct(itr);}
			void destroy() override{adaptor::destruct(this);}
			bool equal(const basic_iterator_adaptor& itr_)const override{
				const adaptor* adp=static_cast<const adaptor*>(&itr_);
				if(adp==nullptr)return false;
				return itr == adp->itr;
			}
			void increment() override{++itr;}
			void decrement() override{--itr;}
			const value_type& dereference()const override{return *itr;}
		public:
			iterator_adaptor(iterator_ itr_):itr(itr_){}
		private:
			static boost::fast_pool_allocator<adaptor> allocator;
		public:
			static void destruct(adaptor* ptr){
				allocator.destroy(ptr);
				allocator.deallocate(ptr);
			}
			static basic_iterator_adaptor* construct(iterator_ titr_){
				adaptor* ans=allocator.allocate();
				allocator.construct(ans,adaptor(titr_));
				return ans;
			}
		};
	private:
		std::unique_ptr<basic_iterator_adaptor,typename basic_iterator_adaptor::deleter> ptr;
	public://for iterator_facade
		bool equal(const my_type& My_)const{
			if(ptr==nullptr || My_.ptr==nullptr)return false;
			return ptr->equal(*(My_.ptr));
		}
		void increment(){ptr->increment();}
		void decrement(){ptr->decrement();}
		const value_type& dereference()const{return ptr->dereference();}
	public://for conctructor
		const_bidirectional_any_iterator():ptr(nullptr){}
		const_bidirectional_any_iterator(const my_type& My_):ptr(nullptr){if(My_.ptr)ptr.reset(My_.ptr->clone());}
		const my_type& operator=(const my_type& My_){
			if(this!=&My_)ptr.reset(My_.ptr->clone());
			return *this;
		}
		const_bidirectional_any_iterator(my_type&& My_):ptr(std::move(My_.ptr)){}
		const my_type& operator=(my_type&& My_){
			if(this!=&My_)ptr=std::move(My_.ptr);
			return *this;
		}
		template<typename iterator_>
		const_bidirectional_any_iterator(iterator_ itr_):ptr(basic_iterator_adaptor::construct(itr_)){}
		template<typename iterator_>
		const my_type& operator=(iterator_ itr_){
			ptr.reset(basic_iterator_adaptor::construct(itr));
			return *this;
		}
	};
	template<typename value_type,typename difference_type>
	template<typename iterator_>
	static typename const_bidirectional_any_iterator<value_type,difference_type>::basic_iterator_adaptor* const_bidirectional_any_iterator<value_type,difference_type>::basic_iterator_adaptor::construct(iterator_ itr_){return iterator_adaptor<iterator_>::construct(itr_);}
	template<typename value_type,typename difference_type>
	template<typename iterator_>
	boost::fast_pool_allocator<typename const_bidirectional_any_iterator<value_type,difference_type>::iterator_adaptor<iterator_>> const_bidirectional_any_iterator<value_type,difference_type>::iterator_adaptor<iterator_>::allocator;
	//random_access_any_iterator
	template<typename value_type,typename difference_type=int>
	struct const_random_access_any_iterator:public boost::iterator_facade<const_random_access_any_iterator<value_type,difference_type>,const value_type,boost::random_access_traversal_tag,const value_type&,difference_type>{
	private:
		typedef const_random_access_any_iterator<value_type,difference_type> my_type;
	private:
		struct basic_iterator_adaptor{
		public:
			struct deleter{void operator()(basic_iterator_adaptor* ptr)const{ptr->destroy();}};
		public:
			virtual basic_iterator_adaptor* clone()const=0;
			virtual void destroy()=0;
			virtual bool equal(const basic_iterator_adaptor& itr)const=0;
			virtual difference_type distance_to(const basic_iterator_adaptor& itr)const=0;
			virtual void increment()=0;
			virtual void decrement()=0;
			virtual void advance(difference_type d)=0;
			virtual const value_type& dereference()const=0;
		public:
			template<typename iterator_>
			static basic_iterator_adaptor* construct(iterator_ itr);
		};
		template<typename iterator_>
		struct iterator_adaptor:public basic_iterator_adaptor{
			typedef iterator_adaptor<iterator_> adaptor;
		private:
			iterator_ itr;
		public:
			basic_iterator_adaptor* clone()const override{return construct(itr);}
			void destroy() override{destruct(this);}
			bool equal(const basic_iterator_adaptor& itr_)const override{
				const adaptor* adp=static_cast<const adaptor*>(&itr_);
				if(adp==nullptr)return false;
				return itr == adp->itr;
			}
			difference_type distance_to(const basic_iterator_adaptor& itr_)const override{
				const adaptor* adp=static_cast<const adaptor*>(&itr_);
				return adp->itr - itr;
			}
			void increment() override{++itr;}
			void decrement() override{--itr;}
			void advance(difference_type d) override{itr+=d;}
			const value_type& dereference()const override{return *itr;}
		public:
			iterator_adaptor(iterator_ itr_):itr(itr_){}
		private:
			static boost::fast_pool_allocator<adaptor> allocator;
		public:
			static void destruct(adaptor* ptr){
				allocator.destroy(ptr);
				allocator.deallocate(ptr);
			}
			static basic_iterator_adaptor* construct(iterator_ titr_){
				adaptor* ans=allocator.allocate();
				allocator.construct(ans,adaptor(titr_));
				return ans;
			}
		};
	private:
		std::unique_ptr<basic_iterator_adaptor,typename basic_iterator_adaptor::deleter> ptr;
	public://for iterator_facade
		bool equal(const my_type& My_)const{
			if(ptr==nullptr || My_.ptr==nullptr)return false;
			return ptr->equal(*(My_.ptr));
		}
		difference_type distance_to(const my_type& My_)const{return ptr->distance_to(*(My_.ptr));}
		void increment(){ptr->increment();}
		void decrement(){ptr->decrement();}
		void advance(difference_type d){ptr->advance(d);}
		const value_type& dereference()const{return ptr->dereference();}
	public://for conctructor
		const_random_access_any_iterator():ptr(nullptr){}
		const_random_access_any_iterator(const my_type& My_):ptr(nullptr){if(My_.ptr)ptr.reset(My_.ptr->clone());}
		const my_type& operator=(const my_type& My_){
			if(this!=&My_)ptr.reset(My_.ptr->clone());
			return *this;
		}
		const_random_access_any_iterator(my_type&& My_):ptr(std::move(My_.ptr)){}
		const my_type& operator=(my_type&& My_){
			if(this!=&My_)ptr=std::move(My_.ptr);
			return *this;
		}
		template<typename iterator_>
		const_random_access_any_iterator(iterator_ itr_):ptr(basic_iterator_adaptor::construct(itr_)){}
		template<typename iterator_>
		const my_type& operator=(iterator_ itr_){
			ptr.reset(basic_iterator_adaptor::construct(itr));
			return *this;
		}
	};
	template<typename value_type,typename difference_type>
	template<typename iterator_>
	static typename const_random_access_any_iterator<value_type,difference_type>::basic_iterator_adaptor* const_random_access_any_iterator<value_type,difference_type>::basic_iterator_adaptor::construct(iterator_ itr_){return iterator_adaptor<iterator_>::construct(itr_);}
	template<typename value_type,typename difference_type>
	template<typename iterator_>
	boost::fast_pool_allocator<typename const_random_access_any_iterator<value_type,difference_type>::iterator_adaptor<iterator_>> const_random_access_any_iterator<value_type,difference_type>::iterator_adaptor<iterator_>::allocator;
	//output_any_iterator
	template<typename value_type,typename difference_type=int>
	struct output_any_iterator:public boost::iterator_facade<output_any_iterator<value_type,difference_type>,value_type,boost::incrementable_traversal_tag,value_type&,difference_type>{
	private:
		typedef output_any_iterator<value_type,difference_type> my_type;
	private:
		struct basic_iterator_adaptor{
		public:
			struct deleter{void operator()(basic_iterator_adaptor* ptr)const{ptr->destroy();}};
		public:
			virtual basic_iterator_adaptor* clone()const=0;
			virtual void destroy()=0;
			virtual void increment()=0;
			virtual value_type& dereference()const=0;
		public:
			template<typename iterator_>
			static basic_iterator_adaptor* construct(iterator_ itr);
		};
		template<typename iterator_>
		struct iterator_adaptor:public basic_iterator_adaptor{
			typedef iterator_adaptor<iterator_> adaptor;
		private:
			iterator_ itr;
		public:
			basic_iterator_adaptor* clone()const override{return adaptor::construct(itr);}
			void destroy() override{adaptor::destruct(this);}
			void increment() override{++itr;}
			value_type& dereference()const override{return *itr;}
		public:
			iterator_adaptor(iterator_ itr_):itr(itr_){}
		private:
			static boost::fast_pool_allocator<adaptor> allocator;
		public:
			static void destruct(adaptor* ptr){
				allocator.destroy(ptr);
				allocator.deallocate(ptr);
			}
			static basic_iterator_adaptor* construct(iterator_ titr_){
				adaptor* ans=allocator.allocate();
				allocator.construct(ans,adaptor(titr_));
				return ans;
			}
		};
	private:
		std::unique_ptr<basic_iterator_adaptor,typename basic_iterator_adaptor::deleter> ptr;
	public://for iterator_facade
		void increment(){ptr->increment();}
		value_type& dereference()const{return ptr->dereference();}
	public://for conctructor
		output_any_iterator():ptr(nullptr){}
		output_any_iterator(const my_type& My_):ptr(nullptr){if(My_.ptr)ptr.reset(My_.ptr->clone());}
		const my_type& operator=(const my_type& My_){
			if(this!=&My_)ptr.reset(My_.ptr->clone());
			return *this;
		}
		output_any_iterator(my_type&& My_):ptr(std::move(My_.ptr)){}
		const my_type& operator=(my_type&& My_){
			if(this!=&My_)ptr=std::move(My_.ptr);
			return *this;
		}
		template<typename iterator_>
		output_any_iterator(iterator_ itr_):ptr(basic_iterator_adaptor::construct(itr_)){}
		template<typename iterator_>
		const my_type& operator=(iterator_ itr_){
			ptr.reset(basic_iterator_adaptor::construct(itr));
			return *this;
		}
	};
	template<typename value_type,typename difference_type>
	template<typename iterator_>
	static typename output_any_iterator<value_type,difference_type>::basic_iterator_adaptor* output_any_iterator<value_type,difference_type>::basic_iterator_adaptor::construct(iterator_ itr_){return iterator_adaptor<iterator_>::construct(itr_);}
	template<typename value_type,typename difference_type>
	template<typename iterator_>
	boost::fast_pool_allocator<typename output_any_iterator<value_type,difference_type>::iterator_adaptor<iterator_>> output_any_iterator<value_type,difference_type>::iterator_adaptor<iterator_>::allocator;
	//forward_any_iterator
	template<typename value_type,typename difference_type=int>
	struct forward_any_iterator:public boost::iterator_facade<forward_any_iterator<value_type,difference_type>,value_type,boost::forward_traversal_tag,value_type&,difference_type>{
	private:
		typedef forward_any_iterator<value_type,difference_type> my_type;
	private:
		struct basic_iterator_adaptor{
		public:
			struct deleter{void operator()(basic_iterator_adaptor* ptr)const{ptr->destroy();}};
		public:
			virtual basic_iterator_adaptor* clone()const=0;
			virtual void destroy()=0;
			virtual bool equal(const basic_iterator_adaptor& itr)const=0;
			virtual void increment()=0;
			virtual value_type& dereference()const=0;
		public:
			template<typename iterator_>
			static basic_iterator_adaptor* construct(iterator_ itr);
		};
		template<typename iterator_>
		struct iterator_adaptor:public basic_iterator_adaptor{
			typedef iterator_adaptor<iterator_> adaptor;
		private:
			iterator_ itr;
		public:
			basic_iterator_adaptor* clone()const override{return adaptor::construct(itr);}
			void destroy() override{adaptor::destruct(this);}
			bool equal(const basic_iterator_adaptor& itr_)const override{
				const adaptor* adp=static_cast<const adaptor*>(&itr_);
				if(adp==nullptr)return false;
				return itr == adp->itr;
			}
			void increment() override{++itr;}
			value_type& dereference()const override{return *itr;}
		public:
			iterator_adaptor(iterator_ itr_):itr(itr_){}
		private:
			static boost::fast_pool_allocator<adaptor> allocator;
		public:
			static void destruct(adaptor* ptr){
				allocator.destroy(ptr);
				allocator.deallocate(ptr);
			}
			static basic_iterator_adaptor* construct(iterator_ titr_){
				adaptor* ans=allocator.allocate();
				allocator.construct(ans,adaptor(titr_));
				return ans;
			}
		};
	private:
		std::unique_ptr<basic_iterator_adaptor,typename basic_iterator_adaptor::deleter> ptr;
	public://for iterator_facade
		bool equal(const my_type& My_)const{
			if(ptr==nullptr || My_.ptr==nullptr)return false;
			return ptr->equal(*(My_.ptr));
		}
		void increment(){ptr->increment();}
		value_type& dereference()const{return ptr->dereference();}
	public://for conctructor
		forward_any_iterator():ptr(nullptr){}
		forward_any_iterator(const my_type& My_):ptr(nullptr){if(My_.ptr)ptr.reset(My_.ptr->clone());}
		const my_type& operator=(const my_type& My_){
			if(this!=&My_)ptr.reset(My_.ptr->clone());
			return *this;
		}
		forward_any_iterator(my_type&& My_):ptr(std::move(My_.ptr)){}
		const my_type& operator=(my_type&& My_){
			if(this!=&My_)ptr=std::move(My_.ptr);
			return *this;
		}
		template<typename iterator_>
		forward_any_iterator(iterator_ itr_):ptr(basic_iterator_adaptor::construct(itr_)){}
		template<typename iterator_>
		const my_type& operator=(iterator_ itr_){
			ptr.reset(basic_iterator_adaptor::construct(itr));
			return *this;
		}
	};
	template<typename value_type,typename difference_type>
	template<typename iterator_>
	static typename forward_any_iterator<value_type,difference_type>::basic_iterator_adaptor* forward_any_iterator<value_type,difference_type>::basic_iterator_adaptor::construct(iterator_ itr_){return iterator_adaptor<iterator_>::construct(itr_);}
	template<typename value_type,typename difference_type>
	template<typename iterator_>
	boost::fast_pool_allocator<typename forward_any_iterator<value_type,difference_type>::iterator_adaptor<iterator_>> forward_any_iterator<value_type,difference_type>::iterator_adaptor<iterator_>::allocator;
	//bidirectional_any_iterator
	template<typename value_type,typename difference_type=int>
	struct bidirectional_any_iterator:public boost::iterator_facade<bidirectional_any_iterator<value_type,difference_type>,value_type,boost::bidirectional_traversal_tag,value_type&,difference_type>{
	private:
		typedef bidirectional_any_iterator<value_type,difference_type> my_type;
	private:
		struct basic_iterator_adaptor{
		public:
			struct deleter{void operator()(basic_iterator_adaptor* ptr)const{ptr->destroy();}};
		public:
			virtual basic_iterator_adaptor* clone()const=0;
			virtual void destroy()=0;
			virtual bool equal(const basic_iterator_adaptor& itr)const=0;
			virtual void increment()=0;
			virtual void decrement()=0;
			virtual value_type& dereference()const=0;
		public:
			template<typename iterator_>
			static basic_iterator_adaptor* construct(iterator_ itr);
		};
		template<typename iterator_>
		struct iterator_adaptor:public basic_iterator_adaptor{
			typedef iterator_adaptor<iterator_> adaptor;
		private:
			iterator_ itr;
		public:
			basic_iterator_adaptor* clone()const override{return adaptor::construct(itr);}
			void destroy() override{adaptor::destruct(this);}
			bool equal(const basic_iterator_adaptor& itr_)const override{
				const adaptor* adp=static_cast<const adaptor*>(&itr_);
				if(adp==nullptr)return false;
				return itr == adp->itr;
			}
			void increment() override{++itr;}
			void decrement() override{--itr;}
			value_type& dereference()const override{return *itr;}
		public:
			iterator_adaptor(iterator_ itr_):itr(itr_){}
		private:
			static boost::fast_pool_allocator<adaptor> allocator;
		public:
			static void destruct(adaptor* ptr){
				allocator.destroy(ptr);
				allocator.deallocate(ptr);
			}
			static basic_iterator_adaptor* construct(iterator_ titr_){
				adaptor* ans=allocator.allocate();
				allocator.construct(ans,adaptor(titr_));
				return ans;
			}
		};
	private:
		std::unique_ptr<basic_iterator_adaptor,typename basic_iterator_adaptor::deleter> ptr;
	public://for iterator_facade
		bool equal(const my_type& My_)const{
			if(ptr==nullptr || My_.ptr==nullptr)return false;
			return ptr->equal(*(My_.ptr));
		}
		void increment(){ptr->increment();}
		void decrement(){ptr->decrement();}
		value_type& dereference()const{return ptr->dereference();}
	public://for conctructor
		bidirectional_any_iterator():ptr(nullptr){}
		bidirectional_any_iterator(const my_type& My_):ptr(nullptr){if(My_.ptr)ptr.reset(My_.ptr->clone());}
		const my_type& operator=(const my_type& My_){
			if(this!=&My_)ptr.reset(My_.ptr->clone());
			return *this;
		}
		bidirectional_any_iterator(my_type&& My_):ptr(std::move(My_.ptr)){}
		const my_type& operator=(my_type&& My_){
			if(this!=&My_)ptr=std::move(My_.ptr);
			return *this;
		}
		template<typename iterator_>
		bidirectional_any_iterator(iterator_ itr_):ptr(basic_iterator_adaptor::construct(itr_)){}
		template<typename iterator_>
		const my_type& operator=(iterator_ itr_){
			ptr.reset(basic_iterator_adaptor::construct(itr));
			return *this;
		}
	};
	template<typename value_type,typename difference_type>
	template<typename iterator_>
	static typename bidirectional_any_iterator<value_type,difference_type>::basic_iterator_adaptor* bidirectional_any_iterator<value_type,difference_type>::basic_iterator_adaptor::construct(iterator_ itr_){return iterator_adaptor<iterator_>::construct(itr_);}
	template<typename value_type,typename difference_type>
	template<typename iterator_>
	boost::fast_pool_allocator<typename bidirectional_any_iterator<value_type,difference_type>::iterator_adaptor<iterator_>> bidirectional_any_iterator<value_type,difference_type>::iterator_adaptor<iterator_>::allocator;
	//random_access_any_iterator
	template<typename value_type,typename difference_type=int>
	struct random_access_any_iterator:public boost::iterator_facade<random_access_any_iterator<value_type,difference_type>,value_type,boost::random_access_traversal_tag,value_type&,difference_type>{
	private:
		typedef random_access_any_iterator<value_type,difference_type> my_type;
	private:
		struct basic_iterator_adaptor{
		public:
			struct deleter{void operator()(basic_iterator_adaptor* ptr)const{ptr->destroy();}};
		public:
			virtual basic_iterator_adaptor* clone()const=0;
			virtual void destroy()=0;
			virtual bool equal(const basic_iterator_adaptor& itr)const=0;
			virtual difference_type distance_to(const basic_iterator_adaptor& itr)const=0;
			virtual void increment()=0;
			virtual void decrement()=0;
			virtual void advance(difference_type d)=0;
			virtual value_type& dereference()const=0;
		public:
			template<typename iterator_>
			static basic_iterator_adaptor* construct(iterator_ itr);
		};
		template<typename iterator_>
		struct iterator_adaptor:public basic_iterator_adaptor{
			typedef iterator_adaptor<iterator_> adaptor;
		private:
			iterator_ itr;
		public:
			basic_iterator_adaptor* clone()const override{return construct(itr);}
			void destroy() override{destruct(this);}
			bool equal(const basic_iterator_adaptor& itr_)const override{
				const adaptor* adp=static_cast<const adaptor*>(&itr_);
				if(adp==nullptr)return false;
				return itr == adp->itr;
			}
			difference_type distance_to(const basic_iterator_adaptor& itr_)const override{
				const adaptor* adp=static_cast<const adaptor*>(&itr_);
				return adp->itr - itr;
			}
			void increment() override{++itr;}
			void decrement() override{--itr;}
			void advance(difference_type d) override{itr+=d;}
			value_type& dereference()const override{return *itr;}
		public:
			iterator_adaptor(iterator_ itr_):itr(itr_){}
		private:
			static boost::fast_pool_allocator<adaptor> allocator;
		public:
			static void destruct(adaptor* ptr){
				allocator.destroy(ptr);
				allocator.deallocate(ptr);
			}
			static basic_iterator_adaptor* construct(iterator_ titr_){
				adaptor* ans=allocator.allocate();
				allocator.construct(ans,adaptor(titr_));
				return ans;
			}
		};
	private:
		std::unique_ptr<basic_iterator_adaptor,typename basic_iterator_adaptor::deleter> ptr;
	public://for iterator_facade
		bool equal(const my_type& My_)const{
			if(ptr==nullptr || My_.ptr==nullptr)return false;
			return ptr->equal(*(My_.ptr));
		}
		difference_type distance_to(const my_type& My_)const{return ptr->distance_to(*(My_.ptr));}
		void increment(){ptr->increment();}
		void decrement(){ptr->decrement();}
		void advance(difference_type d){ptr->advance(d);}
		value_type& dereference()const{return ptr->dereference();}
	public://for conctructor
		random_access_any_iterator():ptr(nullptr){}
		random_access_any_iterator(const my_type& My_):ptr(nullptr){if(My_.ptr)ptr.reset(My_.ptr->clone());}
		const my_type& operator=(const my_type& My_){
			if(this!=&My_)ptr.reset(My_.ptr->clone());
			return *this;
		}
		random_access_any_iterator(my_type&& My_):ptr(std::move(My_.ptr)){}
		const my_type& operator=(my_type&& My_){
			if(this!=&My_)ptr=std::move(My_.ptr);
			return *this;
		}
		template<typename iterator_>
		random_access_any_iterator(iterator_ itr_):ptr(basic_iterator_adaptor::construct(itr_)){}
		template<typename iterator_>
		const my_type& operator=(iterator_ itr_){
			ptr.reset(basic_iterator_adaptor::construct(itr));
			return *this;
		}
	};
	template<typename value_type,typename difference_type>
	template<typename iterator_>
	static typename random_access_any_iterator<value_type,difference_type>::basic_iterator_adaptor* random_access_any_iterator<value_type,difference_type>::basic_iterator_adaptor::construct(iterator_ itr_){return iterator_adaptor<iterator_>::construct(itr_);}
	template<typename value_type,typename difference_type>
	template<typename iterator_>
	boost::fast_pool_allocator<typename random_access_any_iterator<value_type,difference_type>::iterator_adaptor<iterator_>> random_access_any_iterator<value_type,difference_type>::iterator_adaptor<iterator_>::allocator;
}
#
#endif
