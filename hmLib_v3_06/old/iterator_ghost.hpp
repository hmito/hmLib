#ifndef HMLIB_ITERATORGHOST_INC
#define HMLIB_ITERATORGHOST_INC 100
#
#ifndef HMLIB_CHAIN_INC
#	include "chain.hpp"
#endif
#ifndef HMLIB_ITERATORPATTERN_INC
#	include "iterator_pattern.hpp"
#endif
namespace hmLib{
	namespace ghost{
		//input_iterator
		template<typename value_type_,typename difference_type_=int>
		struct input_iterator_ghost
			:public pattern::input_iterator_pattern<input_iterator_ghost<value_type_,difference_type_>,value_type_,difference_type_>{
		private:
			typedef input_iterator_ghost<value_type_,difference_type_> my_type;
		private:
			struct basic_iterator{
			public:
				void* ptr;
			public:
				virtual void increment()=0;
				virtual const value_type_& const_refer()const=0;
				virtual const value_type_* const_access()const=0;
				virtual bool forward_equal(const basic_iterator& val_)const=0;
				//自身のクローンを生成するコマンド
				virtual basic_iterator* clone()const=0;
				//自身を破壊するコマンド
				virtual void destruct()=0;
			};
			template<typename target_iterator_>
			struct target_adapter{
			private:
				struct iterator:public basic_iterator{
				private:
					typedef iterator my_type;
				private:
					target_iterator_ titr;
				public:
					void increment(){++titr;}
					const value_type_& const_refer()const /*C++0x_override/**/{return *titr;}
					const value_type_* const_access()const /*C++0x_override/**/{return &(*titr);}
					bool forward_equal(const basic_iterator& My_)const /*C++0x_override/**/{
//						const iterator* MyItr=dynamic_cast<const iterator*>(&My_);
//						if(MyItr==/*C++0x_nullptr/*/NULL/**/)return false;
//						else return (this->titr==MyItr->titr);
						return (*reinterpret_cast<target_iterator_*>(basic_iterator::ptr))==(*reinterpret_cast<target_iterator_*>(My_.basic_iterator::ptr));
					}
					basic_iterator* clone()const /*C++0x_override/**/{return create(titr);}
					void destruct()/*C++0x_override/**/{release(this);};
				public:
					iterator():titr(){
						basic_iterator::ptr=&titr;
					}
					iterator(target_iterator_ titr_):titr(titr_){
						basic_iterator::ptr=&titr;
					}
					iterator(const iterator& my_):titr(my_.titr){
						basic_iterator::ptr=&titr;
					}
					const iterator& operator=(const iterator& my_){
						if(this!=&my_){
							titr=my_.titr;
						}
						return *this;
					}
				};
			private:
				static chain<iterator> MyChain;
			public:
				static iterator* create(target_iterator_ titr_=target_iterator_()){
					MyChain.push_front(iterator(titr_));
					return &(MyChain.front());
				}
				static void release(iterator* itr_ptr_){
					for(auto itr=MyChain.begin();itr!=MyChain.end();++itr){
						if(&(*itr) == itr_ptr_){
							MyChain.erase(itr);
							break;
						}
					}
				}
			};
		private:
			basic_iterator* itr_ptr;
		private:
			template<class target_iterator_>
			basic_iterator* create(target_iterator_ titr_){return target_adapter<target_iterator_>::create(titr_);}
		protected:
			virtual void increment()/*C++0x_override/**/{itr_ptr->increment();}
			virtual const value_type_& const_refer()const /*C++0x_override/**/{return itr_ptr->const_refer();}
			virtual const value_type_* const_access()const /*C++0x_override/**/{return &(const_refer());}
			virtual bool forward_equal(const my_type& val_)const /*C++0x_override/**/{return itr_ptr->forward_equal(*(val_.itr_ptr));}
		public:
			input_iterator_ghost():itr_ptr(/*C++0x_nullptr/*/NULL/**/){return;}
			template<class target_iterator_>
			input_iterator_ghost(target_iterator_ titr_):itr_ptr(create(titr_)){return;}
			input_iterator_ghost(const my_type& my_):itr_ptr(/*C++0x_nullptr/*/NULL/**/){
				if(my_.itr_ptr!=/*C++0x_nullptr/*/NULL/**/)itr_ptr=my_.itr_ptr->clone();
			}
			~input_iterator_ghost(){if(itr_ptr!=/*C++0x_nullptr/*/NULL/**/)itr_ptr->destruct();}
			template<class target_iterator_>
			const my_type& operator=(target_iterator_ titr_){
				if(itr_ptr!=/*C++0x_nullptr/*/NULL/**/){
					itr_ptr->destruct();
					itr_ptr=/*C++0x_nullptr/*/NULL/**/;
				}
				itr_ptr=create(titr_);
				return *this;
			}
			const my_type& operator=(const my_type& my_){
				if(itr_ptr!=/*C++0x_nullptr/*/NULL/**/){
					itr_ptr->destruct();
					itr_ptr=/*C++0x_nullptr/*/NULL/**/;
				}
				if(my_.itr_ptr!=/*C++0x_nullptr/*/NULL/**/)itr_ptr=my_.itr_ptr->clone();
				return *this;
			}
		};
		template<typename value_type_,typename difference_type_>
		template<typename target_iterator_>
		chain<typename input_iterator_ghost<value_type_,difference_type_>::template target_adapter<target_iterator_>::iterator> input_iterator_ghost<value_type_,difference_type_>::target_adapter<target_iterator_>::MyChain;
		//output_iterator
		template<typename value_type_,typename difference_type_=int>
		struct output_iterator_ghost
			:public pattern::output_iterator_pattern<output_iterator_ghost<value_type_,difference_type_>,value_type_,difference_type_>{
		private:
			typedef output_iterator_ghost<value_type_,difference_type_> my_type;
		private:
			struct basic_iterator{
			public:
				void* ptr;
			public:
				virtual void increment()=0;
				virtual value_type_& refer()=0;
				virtual value_type_* access()=0;
				//自身のクローンを生成するコマンド
				virtual basic_iterator* clone()const=0;
				//自身を破壊するコマンド
				virtual void destruct()=0;
			};
			template<typename target_iterator_>
			struct target_adapter{
			private:
				struct iterator:public basic_iterator{
				private:
					typedef iterator my_type;
				private:
					target_iterator_ titr;
				public:
					void increment(){++titr;}
					value_type_& refer()/*C++0x_override/**/{return *titr;}
					value_type_* access()/*C++0x_override/**/{return &(*titr);}
					basic_iterator* clone()const /*C++0x_override/**/{return create(titr);}
					void destruct()/*C++0x_override/**/{release(this);};
				public:
					iterator():titr(){
						basic_iterator::ptr=&titr;
					}
					iterator(target_iterator_ titr_):titr(titr_){
						basic_iterator::ptr=&titr;
					}
					iterator(const iterator& my_):titr(my_.titr){
						basic_iterator::ptr=&titr;
					}
					const iterator& operator=(const iterator& my_){
						if(this!=&my_){
							titr=my_.titr;
						}
						return *this;
					}
				};
			private:
				static chain<iterator> MyChain;
			public:
				static iterator* create(target_iterator_ titr_=target_iterator_()){
					MyChain.push_front(iterator(titr_));
					return &(MyChain.front());
				}
				static void release(iterator* itr_ptr_){
					for(auto itr=MyChain.begin();itr!=MyChain.end();++itr){
						if(&(*itr) == itr_ptr_){
							MyChain.erase(itr);
							break;
						}
					}
				}
			};
		private:
			basic_iterator* itr_ptr;
		private:
			template<class target_iterator_>
			basic_iterator* create(target_iterator_ titr_){return target_adapter<target_iterator_>::create(titr_);}
		protected:
			virtual void increment()/*C++0x_override/**/{itr_ptr->do_increment();}
			virtual const value_type_& refer()const /*C++0x_override/**/{return itr_ptr->do_const_refer();}
			virtual const value_type_* access()const /*C++0x_override/**/{return &(refer());}
		public:
			output_iterator_ghost():itr_ptr(/*C++0x_nullptr/*/NULL/**/){return;}
			template<class target_iterator_>
			output_iterator_ghost(target_iterator_ titr_):itr_ptr(create(titr_)){return;}
			output_iterator_ghost(const my_type& my_):itr_ptr(/*C++0x_nullptr/*/NULL/**/){
				if(my_.itr_ptr!=/*C++0x_nullptr/*/NULL/**/)itr_ptr=my_.itr_ptr->clone();
			}
			~output_iterator_ghost(){if(itr_ptr!=/*C++0x_nullptr/*/NULL/**/)itr_ptr->destruct();}
			template<class target_iterator_>
			const my_type& operator=(target_iterator_ titr_){
				if(itr_ptr!=/*C++0x_nullptr/*/NULL/**/){
					itr_ptr->destruct();
					itr_ptr=/*C++0x_nullptr/*/NULL/**/;
				}
				itr_ptr=create(titr_);
				return *this;
			}
			const my_type& operator=(const my_type& my_){
				if(itr_ptr!=/*C++0x_nullptr/*/NULL/**/){
					itr_ptr->destruct();
					itr_ptr=/*C++0x_nullptr/*/NULL/**/;
				}
				if(my_.itr_ptr!=/*C++0x_nullptr/*/NULL/**/)itr_ptr=my_.itr_ptr->clone();
				return *this;
			}
		};
		template<typename value_type_,typename difference_type_>
		template<typename target_iterator_>
		chain<typename output_iterator_ghost<value_type_,difference_type_>::template target_adapter<target_iterator_>::iterator> output_iterator_ghost<value_type_,difference_type_>::target_adapter<target_iterator_>::MyChain;
		//forward_iterator
		template<typename value_type_,typename difference_type_=int>
		struct forward_iterator_ghost
			:public pattern::forward_iterator_pattern<forward_iterator_ghost<value_type_,difference_type_>,value_type_,difference_type_>{
		private:
			typedef forward_iterator_ghost<value_type_,difference_type_> my_type;
		private:
			struct basic_iterator{
			public:
				void* ptr;
			public:
				virtual void increment()=0;
				virtual value_type_& refer()=0;
				virtual value_type_* access()=0;
				virtual bool forward_equal(const basic_iterator& val_)const=0;
				//自身のクローンを生成するコマンド
				virtual basic_iterator* clone()const=0;
				//自身を破壊するコマンド
				virtual void destruct()=0;
			};
			template<typename target_iterator_>
			struct target_adapter{
			private:
				struct iterator:public basic_iterator{
				private:
					typedef iterator my_type;
				private:
					target_iterator_ titr;
				public:
					virtual void increment()/*C++0x_override/**/{++titr;}
					virtual const value_type_& refer()const /*C++0x_override/**/{return *titr;}
					virtual const value_type_* access()const /*C++0x_override/**/{return &(*titr);}
					virtual bool forward_equal(const basic_iterator& val_)const /*C++0x_override/**/{return (*reinterpret_cast<target_iterator_*>(basic_iterator::ptr))==(*reinterpret_cast<target_iterator_*>(val_.basic_iterator::ptr));}
					virtual basic_iterator* clone()const /*C++0x_override/**/{return create(titr);}
					virtual void destruct()/*C++0x_override/**/{release(this);};
				public:
					iterator():titr(){
						basic_iterator::ptr=&titr;
					}
					iterator(target_iterator_ titr_):titr(titr_){
						basic_iterator::ptr=&titr;
					}
					iterator(const iterator& my_):titr(my_.titr){
						basic_iterator::ptr=&titr;
					}
					const iterator& operator=(const iterator& my_){
						if(this!=&my_){
							titr=my_.titr;
						}
						return *this;
					}
				};
			private:
				static chain<iterator> MyChain;
			public:
				static iterator* create(target_iterator_ titr_=target_iterator_()){
					MyChain.push_front(iterator(titr_));
					return &(MyChain.front());
				}
				static void release(iterator* itr_ptr_){
					for(auto itr=MyChain.begin();itr!=MyChain.end();++itr){
						if(&(*itr) == itr_ptr_){
							MyChain.erase(itr);
							break;
						}
					}
				}
			};
		private:
			basic_iterator* itr_ptr;
		private:
			template<class target_iterator_>
			basic_iterator* create(target_iterator_ titr_){return target_adapter<target_iterator_>::create(titr_);}
		protected:
			virtual void increment()/*C++0x_override/**/{itr_ptr->increment();}
			virtual const value_type_& refer()const /*C++0x_override/**/{return itr_ptr->refer();}
			virtual const value_type_* access()const /*C++0x_override/**/{return &(refer());}
			virtual bool forward_equal(const my_type& val_)const /*C++0x_override/**/{return itr_ptr->forward_equal(*(val_.itr_ptr));}
		public:
			forward_iterator_ghost():itr_ptr(/*C++0x_nullptr/*/NULL/**/){return;}
			template<class target_iterator_>
			forward_iterator_ghost(target_iterator_ titr_):itr_ptr(create(titr_)){return;}
			forward_iterator_ghost(const my_type& my_):itr_ptr(/*C++0x_nullptr/*/NULL/**/){
				if(my_.itr_ptr!=/*C++0x_nullptr/*/NULL/**/)itr_ptr=my_.itr_ptr->clone();
			}
			~forward_iterator_ghost(){if(itr_ptr!=/*C++0x_nullptr/*/NULL/**/)itr_ptr->destruct();}
			template<class target_iterator_>
			const my_type& operator=(target_iterator_ titr_){
				if(itr_ptr!=/*C++0x_nullptr/*/NULL/**/){
					itr_ptr->destruct();
					itr_ptr=/*C++0x_nullptr/*/NULL/**/;
				}
				itr_ptr=create(titr_);
				return *this;
			}
			const my_type& operator=(const my_type& my_){
				if(itr_ptr!=/*C++0x_nullptr/*/NULL/**/){
					itr_ptr->destruct();
					itr_ptr=/*C++0x_nullptr/*/NULL/**/;
				}
				if(my_.itr_ptr!=/*C++0x_nullptr/*/NULL/**/)itr_ptr=my_.itr_ptr->clone();
				return *this;
			}
		};
		template<typename value_type_,typename difference_type_>
		template<typename target_iterator_>
		chain<typename forward_iterator_ghost<value_type_,difference_type_>::template target_adapter<target_iterator_>::iterator> forward_iterator_ghost<value_type_,difference_type_>::target_adapter<target_iterator_>::MyChain;
		//forward_const_iterator
		template<typename value_type_,typename difference_type_=int>
		struct forward_const_iterator_ghost
			:public pattern::forward_const_iterator_pattern<forward_const_iterator_ghost<value_type_,difference_type_>,value_type_,difference_type_>{
		private:
			typedef forward_const_iterator_ghost<value_type_,difference_type_> my_type;
		private:
			struct basic_iterator{
			public:
				void* ptr;
			public:
				virtual void increment()=0;
				virtual const value_type_& const_refer()const=0;
				virtual const value_type_* const_access()const=0;
				virtual bool forward_equal(const basic_iterator& val_)const=0;
				//自身のクローンを生成するコマンド
				virtual basic_iterator* clone()const=0;
				//自身を破壊するコマンド
				virtual void destruct()=0;
			};
			template<typename target_iterator_>
			struct target_adapter{
			private:
				struct iterator:public basic_iterator{
				private:
					typedef iterator my_type;
				private:
					mutable target_iterator_ titr;
				public:
					virtual void increment()/*C++0x_override/**/{++titr;}
					virtual const value_type_& const_refer()const /*C++0x_override/**/{return *titr;}
					virtual const value_type_* const_access()const /*C++0x_override/**/{return &(*titr);}
					virtual bool forward_equal(const basic_iterator& val_)const /*C++0x_override/**/{return (*reinterpret_cast<target_iterator_*>(basic_iterator::ptr))==(*reinterpret_cast<target_iterator_*>(val_.basic_iterator::ptr));}
					virtual basic_iterator* clone()const /*C++0x_override/**/{return create(titr);}
					virtual void destruct()/*C++0x_override/**/{release(this);};
				public:
					iterator():titr(){
						basic_iterator::ptr=&titr;
					}
					iterator(target_iterator_ titr_):titr(titr_){
						basic_iterator::ptr=&titr;
					}
					iterator(const iterator& my_):titr(my_.titr){
						basic_iterator::ptr=&titr;
					}
					const iterator& operator=(const iterator& my_){
						if(this!=&my_){
							titr=my_.titr;
						}
						return *this;
					}
				};
			private:
				static chain<iterator> MyChain;
			public:
				static iterator* create(target_iterator_ titr_=target_iterator_()){
					MyChain.push_front(iterator(titr_));
					return &(MyChain.front());
				}
				static void release(iterator* itr_ptr_){
					for(auto itr=MyChain.begin();itr!=MyChain.end();++itr){
						if(&(*itr) == itr_ptr_){
							MyChain.erase(itr);
							break;
						}
					}
				}
			};
		private:
			basic_iterator* itr_ptr;
		private:
			template<class target_iterator_>
			basic_iterator* create(target_iterator_ titr_){return target_adapter<target_iterator_>::create(titr_);}
		protected:
			virtual void increment()/*C++0x_override/**/{itr_ptr->increment();}
			virtual const value_type_& const_refer()const /*C++0x_override/**/{return itr_ptr->const_refer();}
			virtual const value_type_* const_access()const /*C++0x_override/**/{return &(const_refer());}
			virtual bool forward_const_equal(const my_type& val_)const /*C++0x_override/**/{return itr_ptr->forward_const_equal(*(val_.itr_ptr));}
		public:
			forward_const_iterator_ghost():itr_ptr(/*C++0x_nullptr/*/NULL/**/){return;}
			template<class target_iterator_>
			forward_const_iterator_ghost(target_iterator_ titr_):itr_ptr(create(titr_)){return;}
			forward_const_iterator_ghost(const my_type& my_):itr_ptr(/*C++0x_nullptr/*/NULL/**/){
				if(my_.itr_ptr!=/*C++0x_nullptr/*/NULL/**/)itr_ptr=my_.itr_ptr->clone();
			}
			~forward_const_iterator_ghost(){if(itr_ptr!=/*C++0x_nullptr/*/NULL/**/)itr_ptr->destruct();}
			template<class target_iterator_>
			const my_type& operator=(target_iterator_ titr_){
				if(itr_ptr!=/*C++0x_nullptr/*/NULL/**/){
					itr_ptr->destruct();
					itr_ptr=/*C++0x_nullptr/*/NULL/**/;
				}
				itr_ptr=create(titr_);
				return *this;
			}
			const my_type& operator=(const my_type& my_){
				if(itr_ptr!=/*C++0x_nullptr/*/NULL/**/){
					itr_ptr->destruct();
					itr_ptr=/*C++0x_nullptr/*/NULL/**/;
				}
				if(my_.itr_ptr!=/*C++0x_nullptr/*/NULL/**/)itr_ptr=my_.itr_ptr->clone();
				return *this;
			}
		};
		template<typename value_type_,typename difference_type_>
		template<typename target_iterator_>
		chain<typename forward_const_iterator_ghost<value_type_,difference_type_>::template target_adapter<target_iterator_>::iterator> forward_const_iterator_ghost<value_type_,difference_type_>::target_adapter<target_iterator_>::MyChain;
		//bidirectional_iterator
		template<typename value_type_,typename difference_type_=int>
		struct bidirectional_iterator_ghost
			:public pattern::bidirectional_iterator_pattern<bidirectional_iterator_ghost<value_type_,difference_type_>,value_type_,difference_type_>{
		private:
			typedef bidirectional_iterator_ghost<value_type_,difference_type_> my_type;
		private:
			struct basic_iterator{
			public:
				void* ptr;
			public:
				virtual void increment()=0;
				virtual void decrement()=0;
				virtual const value_type_& refer()const =0;
				virtual const value_type_* access()const =0;
				virtual bool forward_equal(const basic_iterator& val_)const=0;
				//自身のクローンを生成するコマンド
				virtual basic_iterator* clone()const=0;
				//自身を破壊するコマンド
				virtual void destruct()=0;
			};
			template<typename target_iterator_>
			struct target_adapter{
			private:
				struct iterator:public basic_iterator{
				private:
					typedef iterator my_type;
				private:
					target_iterator_ titr;
				public:
					virtual void increment()/*C++0x_override/**/{++titr;}
					virtual void decrement()/*C++0x_override/**/{--titr;}
					virtual const value_type_& refer()const /*C++0x_override/**/{return *titr;}
					virtual const value_type_* access()const /*C++0x_override/**/{return &(*titr);}
					virtual bool forward_equal(const basic_iterator& val_)const /*C++0x_override/**/{return (*reinterpret_cast<target_iterator_*>(basic_iterator::ptr))==(*reinterpret_cast<target_iterator_*>(val_.basic_iterator::ptr));}
					virtual basic_iterator* clone()const /*C++0x_override/**/{return create(titr);}
					virtual void destruct()/*C++0x_override/**/{release(this);};
				public:
					iterator():titr(){
						basic_iterator::ptr=&titr;
					}
					iterator(target_iterator_ titr_):titr(titr_){
						basic_iterator::ptr=&titr;
					}
					iterator(const iterator& my_):titr(my_.titr){
						basic_iterator::ptr=&titr;
					}
					const iterator& operator=(const iterator& my_){
						if(this!=&my_){
							titr=my_.titr;
						}
						return *this;
					}
				};
			private:
				static chain<iterator> MyChain;
			public:
				static iterator* create(target_iterator_ titr_=target_iterator_()){
					MyChain.push_front(iterator(titr_));
					return &(MyChain.front());
				}
				static void release(iterator* itr_ptr_){
					for(auto itr=MyChain.begin();itr!=MyChain.end();++itr){
						if(&(*itr) == itr_ptr_){
							MyChain.erase(itr);
							break;
						}
					}
				}
			};
		private:
			basic_iterator* itr_ptr;
		private:
			template<class target_iterator_>
			basic_iterator* create(target_iterator_ titr_){return target_adapter<target_iterator_>::create(titr_);}
		protected:
			virtual void increment()/*C++0x_override/**/{itr_ptr->increment();}
			virtual void decrement()/*C++0x_override/**/{itr_ptr->decrement();}
			virtual const value_type_& refer()const /*C++0x_override/**/{return itr_ptr->refer();}
			virtual const value_type_* access()const /*C++0x_override/**/{return &(refer());}
			virtual bool forward_equal(const my_type& val_)const /*C++0x_override/**/{return itr_ptr->forward_equal(*(val_.itr_ptr));}
		public:
			bidirectional_iterator_ghost():itr_ptr(/*C++0x_nullptr/*/NULL/**/){return;}
			template<class target_iterator_>
			bidirectional_iterator_ghost(target_iterator_ titr_):itr_ptr(create(titr_)){return;}
			bidirectional_iterator_ghost(const my_type& my_):itr_ptr(/*C++0x_nullptr/*/NULL/**/){
				if(my_.itr_ptr!=/*C++0x_nullptr/*/NULL/**/)itr_ptr=my_.itr_ptr->clone();
			}
			~bidirectional_iterator_ghost(){if(itr_ptr!=/*C++0x_nullptr/*/NULL/**/)itr_ptr->destruct();}
			template<class target_iterator_>
			const my_type& operator=(target_iterator_ titr_){
				if(itr_ptr!=/*C++0x_nullptr/*/NULL/**/){
					itr_ptr->destruct();
					itr_ptr=/*C++0x_nullptr/*/NULL/**/;
				}
				itr_ptr=create(titr_);
				return *this;
			}
			const my_type& operator=(const my_type& my_){
				if(itr_ptr!=/*C++0x_nullptr/*/NULL/**/){
					itr_ptr->destruct();
					itr_ptr=/*C++0x_nullptr/*/NULL/**/;
				}
				if(my_.itr_ptr!=/*C++0x_nullptr/*/NULL/**/)itr_ptr=my_.itr_ptr->clone();
				return *this;
			}
		};
		template<typename value_type_,typename difference_type_>
		template<typename target_iterator_>
		chain<typename bidirectional_iterator_ghost<value_type_,difference_type_>::template target_adapter<target_iterator_>::iterator> bidirectional_iterator_ghost<value_type_,difference_type_>::target_adapter<target_iterator_>::MyChain;
		//bidirectional_const_iterator
		template<typename value_type_,typename difference_type_=int>
		struct bidirectional_const_iterator_ghost
			:public pattern::bidirectional_const_iterator_pattern<bidirectional_const_iterator_ghost<value_type_,difference_type_>,value_type_,difference_type_>{
		private:
			typedef bidirectional_const_iterator_ghost<value_type_,difference_type_> my_type;
		private:
			struct basic_iterator{
			public:
				void* ptr;
			public:
				virtual void increment()=0;
				virtual void decrement()=0;
				virtual const value_type_& const_refer()const=0;
				virtual const value_type_* const_access()const=0;
				virtual bool forward_equal(const basic_iterator& val_)const=0;
				//自身のクローンを生成するコマンド
				virtual basic_iterator* clone()const=0;
				//自身を破壊するコマンド
				virtual void destruct()=0;
			};
			template<typename target_iterator_>
			struct target_adapter{
			private:
				struct iterator:public basic_iterator{
				private:
					typedef iterator my_type;
				private:
					mutable target_iterator_ titr;
				public:
					virtual void increment()/*C++0x_override/**/{++titr;}
					virtual void decrement()/*C++0x_override/**/{--titr;}
					virtual const value_type_& const_refer()const /*C++0x_override/**/{return *titr;}
					virtual const value_type_* const_access()const /*C++0x_override/**/{return &(*titr);}
					virtual bool forward_equal(const basic_iterator& val_)const /*C++0x_override/**/{return (*reinterpret_cast<target_iterator_*>(basic_iterator::ptr))==(*reinterpret_cast<target_iterator_*>(val_.basic_iterator::ptr));}
					virtual basic_iterator* clone()const /*C++0x_override/**/{return create(titr);}
					virtual void destruct()/*C++0x_override/**/{release(this);};
				public:
					iterator():titr(){
						basic_iterator::ptr=&titr;
					}
					iterator(target_iterator_ titr_):titr(titr_){
						basic_iterator::ptr=&titr;
					}
					iterator(const iterator& my_):titr(my_.titr){
						basic_iterator::ptr=&titr;
					}
					const iterator& operator=(const iterator& my_){
						if(this!=&my_){
							titr=my_.titr;
						}
						return *this;
					}
				};
			private:
				static chain<iterator> MyChain;
			public:
				static iterator* create(target_iterator_ titr_=target_iterator_()){
					MyChain.push_front(iterator(titr_));
					return &(MyChain.front());
				}
				static void release(iterator* itr_ptr_){
					for(auto itr=MyChain.begin();itr!=MyChain.end();++itr){
						if(&(*itr) == itr_ptr_){
							MyChain.erase(itr);
							break;
						}
					}
				}
			};
		private:
			basic_iterator* itr_ptr;
		private:
			template<class target_iterator_>
			basic_iterator* create(target_iterator_ titr_){return target_adapter<target_iterator_>::create(titr_);}
		protected:
			virtual void increment()/*C++0x_override/**/{itr_ptr->increment();}
			virtual void decrement()/*C++0x_override/**/{itr_ptr->decrement();}
			virtual const value_type_& const_refer()const /*C++0x_override/**/{return itr_ptr->const_refer();}
			virtual const value_type_* const_access()const /*C++0x_override/**/{return &(const_refer());}
			virtual bool bidirectional_const_equal(const my_type& val_)const /*C++0x_override/**/{return itr_ptr->bidirectional_const_equal(*(val_.itr_ptr));}
		public:
			bidirectional_const_iterator_ghost():itr_ptr(/*C++0x_nullptr/*/NULL/**/){return;}
			template<class target_iterator_>
			bidirectional_const_iterator_ghost(target_iterator_ titr_):itr_ptr(create(titr_)){return;}
			bidirectional_const_iterator_ghost(const my_type& my_):itr_ptr(/*C++0x_nullptr/*/NULL/**/){
				if(my_.itr_ptr!=/*C++0x_nullptr/*/NULL/**/)itr_ptr=my_.itr_ptr->clone();
			}
			~bidirectional_const_iterator_ghost(){if(itr_ptr!=/*C++0x_nullptr/*/NULL/**/)itr_ptr->destruct();}
			template<class target_iterator_>
			const my_type& operator=(target_iterator_ titr_){
				if(itr_ptr!=/*C++0x_nullptr/*/NULL/**/){
					itr_ptr->destruct();
					itr_ptr=/*C++0x_nullptr/*/NULL/**/;
				}
				itr_ptr=create(titr_);
				return *this;
			}
			const my_type& operator=(const my_type& my_){
				if(itr_ptr!=/*C++0x_nullptr/*/NULL/**/){
					itr_ptr->destruct();
					itr_ptr=/*C++0x_nullptr/*/NULL/**/;
				}
				if(my_.itr_ptr!=/*C++0x_nullptr/*/NULL/**/)itr_ptr=my_.itr_ptr->clone();
				return *this;
			}
		};
		template<typename value_type_,typename difference_type_>
		template<typename target_iterator_>
		chain<typename bidirectional_const_iterator_ghost<value_type_,difference_type_>::template target_adapter<target_iterator_>::iterator> bidirectional_const_iterator_ghost<value_type_,difference_type_>::target_adapter<target_iterator_>::MyChain;
		//random_access_iterator
		template<typename value_type_,typename difference_type_=int>
		struct random_access_iterator_ghost
			:public pattern::random_access_iterator_pattern<random_access_iterator_ghost<value_type_,difference_type_>,value_type_,difference_type_>{
		private:
			typedef random_access_iterator_ghost<value_type_,difference_type_> my_type;
		private:
			struct basic_iterator{
			public:
				void* ptr;
			public:
				virtual void increment()=0;
				virtual void decrement()=0;
				virtual void forward_add(const difference_type_& sub_)=0;
				virtual void forward_sub(const difference_type_& sub_)=0;
				virtual const value_type_& refer()const =0;
				virtual const value_type_& index(const difference_type_& index_)const =0;
				virtual const value_type_* access()const =0;
				virtual bool forward_equal(const basic_iterator& val_)const=0;
				virtual bool forward_less(const basic_iterator& val_)const=0;
				virtual bool forward_more(const basic_iterator& val_)const=0;
				virtual difference_type_ forward_binary_sub(const basic_iterator& val_)const=0;
				//自身のクローンを生成するコマンド
				virtual basic_iterator* clone()const=0;
				//自身を破壊するコマンド
				virtual void destruct()=0;
			};
			template<typename target_iterator_>
			struct target_adapter{
			private:
				struct iterator:public basic_iterator{
				private:
					typedef iterator my_type;
				private:
					target_iterator_ titr;
				public:
					virtual void increment()/*C++0x_override/**/{++titr;}
					virtual void decrement()/*C++0x_override/**/{--titr;}
					virtual void forward_add(const difference_type_& sub_)/*C++0x_override/**/{titr+=sub_;}
					virtual void forward_sub(const difference_type_& sub_)/*C++0x_override/**/{titr-=sub_;}
					virtual const value_type_& refer()const /*C++0x_override/**/{return *titr;}
					virtual const value_type_& index(const difference_type_& index_)const /*C++0x_override/**/{return titr[index_];}
					virtual const value_type_* access()const /*C++0x_override/**/{return &(*titr);}
					virtual bool forward_equal(const basic_iterator& val_)const /*C++0x_override/**/{return (*reinterpret_cast<target_iterator_*>(basic_iterator::ptr))==(*reinterpret_cast<target_iterator_*>(val_.basic_iterator::ptr));}
					virtual bool forward_less(const basic_iterator& val_)const /*C++0x_override/**/{return (*reinterpret_cast<target_iterator_*>(basic_iterator::ptr))<(*reinterpret_cast<target_iterator_*>(val_.basic_iterator::ptr));}
					virtual bool forward_more(const basic_iterator& val_)const /*C++0x_override/**/{return (*reinterpret_cast<target_iterator_*>(basic_iterator::ptr))>(*reinterpret_cast<target_iterator_*>(val_.basic_iterator::ptr));}
					virtual difference_type_ forward_binary_sub(const basic_iterator& val_)const /*C++0x_override/**/{return (*reinterpret_cast<target_iterator_*>(basic_iterator::ptr))-(*reinterpret_cast<target_iterator_*>(val_.basic_iterator::ptr));}
					virtual basic_iterator* clone()const /*C++0x_override/**/{return create(titr);}
					virtual void destruct()/*C++0x_override/**/{release(this);};
				public:
					iterator():titr(){
						basic_iterator::ptr=&titr;
					}
					iterator(target_iterator_ titr_):titr(titr_){
						basic_iterator::ptr=&titr;
					}
					iterator(const iterator& my_):titr(my_.titr){
						basic_iterator::ptr=&titr;
					}
					const iterator& operator=(const iterator& my_){
						if(this!=&my_){
							titr=my_.titr;
						}
						return *this;
					}
				};
			private:
				static chain<iterator> MyChain;
			public:
				static iterator* create(target_iterator_ titr_=target_iterator_()){
					MyChain.push_front(iterator(titr_));
					return &(MyChain.front());
				}
				static void release(iterator* itr_ptr_){
					for(auto itr=MyChain.begin();itr!=MyChain.end();++itr){
						if(&(*itr) == itr_ptr_){
							MyChain.erase(itr);
							break;
						}
					}
				}
			};
		private:
			basic_iterator* itr_ptr;
		private:
			template<class target_iterator_>
			basic_iterator* create(target_iterator_ titr_){return target_adapter<target_iterator_>::create(titr_);}
		protected:
			virtual void increment()/*C++0x_override/**/{itr_ptr->increment();}
			virtual void decrement()/*C++0x_override/**/{itr_ptr->decrement();}
			virtual void forward_add(const difference_type_& sub_)/*C++0x_override/**/{itr_ptr->forward_add(sub_);}
			virtual void forward_sub(const difference_type_& sub_)/*C++0x_override/**/{itr_ptr->forward_sub(sub_);}
			virtual const value_type_& refer()const /*C++0x_override/**/{return itr_ptr->refer();}
			virtual const value_type_& index(const difference_type_& index_)const /*C++0x_override/**/{return itr_ptr->index(index_);}
			virtual const value_type_* access()const /*C++0x_override/**/{return &(refer());}
			virtual bool forward_equal(const my_type& val_)const /*C++0x_override/**/{return itr_ptr->forward_equal(*(val_.itr_ptr));}
			virtual bool forward_less(const my_type& val_)const /*C++0x_override/**/{return itr_ptr->forward_less(*(val_.itr_ptr));}
			virtual bool forward_more(const my_type& val_)const /*C++0x_override/**/{return itr_ptr->forward_more(*(val_.itr_ptr));}
			virtual difference_type_ forward_binary_sub(const my_type& val_)const /*C++0x_override/**/{return itr_ptr->forward_binary_sub(*(val_.itr_ptr));}
		public:
			random_access_iterator_ghost():itr_ptr(/*C++0x_nullptr/*/NULL/**/){return;}
			template<class target_iterator_>
			random_access_iterator_ghost(target_iterator_ titr_):itr_ptr(create(titr_)){return;}
			random_access_iterator_ghost(const my_type& my_):itr_ptr(/*C++0x_nullptr/*/NULL/**/){
				if(my_.itr_ptr!=/*C++0x_nullptr/*/NULL/**/)itr_ptr=my_.itr_ptr->clone();
			}
			~random_access_iterator_ghost(){if(itr_ptr!=/*C++0x_nullptr/*/NULL/**/)itr_ptr->destruct();}
			template<class target_iterator_>
			const my_type& operator=(target_iterator_ titr_){
				if(itr_ptr!=/*C++0x_nullptr/*/NULL/**/){
					itr_ptr->destruct();
					itr_ptr=/*C++0x_nullptr/*/NULL/**/;
				}
				itr_ptr=create(titr_);
				return *this;
			}
			const my_type& operator=(const my_type& my_){
				if(itr_ptr!=/*C++0x_nullptr/*/NULL/**/){
					itr_ptr->destruct();
					itr_ptr=/*C++0x_nullptr/*/NULL/**/;
				}
				if(my_.itr_ptr!=/*C++0x_nullptr/*/NULL/**/)itr_ptr=my_.itr_ptr->clone();
				return *this;
			}
		};
		template<typename value_type_,typename difference_type_>
		template<typename target_iterator_>
		chain<typename random_access_iterator_ghost<value_type_,difference_type_>::template target_adapter<target_iterator_>::iterator> random_access_iterator_ghost<value_type_,difference_type_>::target_adapter<target_iterator_>::MyChain;
		//random_access_const_iterator
		template<typename value_type_,typename difference_type_=int>
		struct random_access_const_iterator_ghost
			:public pattern::random_access_const_iterator_pattern<random_access_const_iterator_ghost<value_type_,difference_type_>,value_type_,difference_type_>{
		private:
			typedef random_access_const_iterator_ghost<value_type_,difference_type_> my_type;
		private:
			struct basic_iterator{
			public:
				void* ptr;
			public:
				virtual void increment()=0;
				virtual void decrement()=0;
				virtual void forward_add(const difference_type_& sub_)=0;
				virtual void forward_sub(const difference_type_& sub_)=0;
				virtual const value_type_& const_refer()const=0;
				virtual const value_type_& const_index(const difference_type_& index_)const=0;
				virtual const value_type_* const_access()const=0;
				virtual bool forward_equal(const basic_iterator& val_)const=0;
				virtual bool forward_less(const basic_iterator& val_)const=0;
				virtual bool forward_more(const basic_iterator& val_)const=0;
				virtual difference_type_ forward_binary_sub(const basic_iterator& val_)const=0;
				//自身のクローンを生成するコマンド
				virtual basic_iterator* clone()const=0;
				//自身を破壊するコマンド
				virtual void destruct()=0;
			};
			template<typename target_iterator_>
			struct target_adapter{
			private:
				struct iterator:public basic_iterator{
				private:
					typedef iterator my_type;
				private:
					mutable target_iterator_ titr;
				public:
					virtual void increment()/*C++0x_override/**/{++titr;}
					virtual void decrement()/*C++0x_override/**/{--titr;}
					virtual void forward_add(const difference_type_& sub_)/*C++0x_override/**/{titr+=sub_;}
					virtual void forward_sub(const difference_type_& sub_)/*C++0x_override/**/{titr-=sub_;}
					virtual const value_type_& const_refer()const /*C++0x_override/**/{return *titr;}
					virtual const value_type_& const_index(const difference_type_& index_)const /*C++0x_override/**/{return titr[index_];}
					virtual const value_type_* const_access()const /*C++0x_override/**/{return &(*titr);}
					virtual bool forward_equal(const basic_iterator& val_)const /*C++0x_override/**/{return (*reinterpret_cast<target_iterator_*>(basic_iterator::ptr))==(*reinterpret_cast<target_iterator_*>(val_.basic_iterator::ptr));}
					virtual bool forward_less(const basic_iterator& val_)const /*C++0x_override/**/{return (*reinterpret_cast<target_iterator_*>(basic_iterator::ptr))<(*reinterpret_cast<target_iterator_*>(val_.basic_iterator::ptr));}
					virtual bool forward_more(const basic_iterator& val_)const /*C++0x_override/**/{return (*reinterpret_cast<target_iterator_*>(basic_iterator::ptr))>(*reinterpret_cast<target_iterator_*>(val_.basic_iterator::ptr));}
					virtual difference_type_ forward_binary_sub(const basic_iterator& val_)const /*C++0x_override/**/{return (*reinterpret_cast<target_iterator_*>(basic_iterator::ptr))-(*reinterpret_cast<target_iterator_*>(val_.basic_iterator::ptr));}
					virtual basic_iterator* clone()const /*C++0x_override/**/{return create(titr);}
					virtual void destruct()/*C++0x_override/**/{release(this);};
				public:
					iterator():titr(){
						basic_iterator::ptr=&titr;
					}
					iterator(target_iterator_ titr_):titr(titr_){
						basic_iterator::ptr=&titr;
					}
					iterator(const iterator& my_):titr(my_.titr){
						basic_iterator::ptr=&titr;
					}
					const iterator& operator=(const iterator& my_){
						if(this!=&my_){
							titr=my_.titr;
						}
						return *this;
					}
				};
			private:
				static chain<iterator> MyChain;
			public:
				static iterator* create(target_iterator_ titr_=target_iterator_()){
					MyChain.push_front(iterator(titr_));
					return &(MyChain.front());
				}
				static void release(iterator* itr_ptr_){
					for(auto itr=MyChain.begin();itr!=MyChain.end();++itr){
						if(&(*itr) == itr_ptr_){
							MyChain.erase(itr);
							break;
						}
					}
				}
			};
		private:
			basic_iterator* itr_ptr;
		private:
			template<class target_iterator_>
			basic_iterator* create(target_iterator_ titr_){return target_adapter<target_iterator_>::create(titr_);}
		protected:
			virtual void increment()/*C++0x_override/**/{itr_ptr->increment();}
			virtual void decrement()/*C++0x_override/**/{itr_ptr->decrement();}
			virtual void forward_add(const difference_type_& sub_)/*C++0x_override/**/{itr_ptr->forward_add(sub_);}
			virtual void forward_sub(const difference_type_& sub_)/*C++0x_override/**/{itr_ptr->forward_sub(sub_);}
			virtual const value_type_& const_refer()const /*C++0x_override/**/{return itr_ptr->const_refer();}
			virtual const value_type_& const_index(const difference_type_& index_)const /*C++0x_override/**/{return itr_ptr->const_index(index_);}
			virtual const value_type_* const_access()const /*C++0x_override/**/{return &(const_refer());}
			virtual bool forward_equal(const my_type& val_)const /*C++0x_override/**/{return itr_ptr->forward_equal(*(val_.itr_ptr));}
			virtual bool forward_less(const my_type& val_)const /*C++0x_override/**/{return itr_ptr->forward_less(*(val_.itr_ptr));}
			virtual bool forward_more(const my_type& val_)const /*C++0x_override/**/{return itr_ptr->forward_more(*(val_.itr_ptr));}
			virtual difference_type_ forward_binary_sub(const my_type& val_)const /*C++0x_override/**/{return itr_ptr->forward_binary_sub(*(val_.itr_ptr));}
		public:
			random_access_const_iterator_ghost():itr_ptr(/*C++0x_nullptr/*/NULL/**/){return;}
			template<class target_iterator_>
			random_access_const_iterator_ghost(target_iterator_ titr_):itr_ptr(create(titr_)){return;}
			random_access_const_iterator_ghost(const my_type& my_):itr_ptr(/*C++0x_nullptr/*/NULL/**/){
				if(my_.itr_ptr!=/*C++0x_nullptr/*/NULL/**/)itr_ptr=my_.itr_ptr->clone();
			}
			~random_access_const_iterator_ghost(){if(itr_ptr!=/*C++0x_nullptr/*/NULL/**/)itr_ptr->destruct();}
			template<class target_iterator_>
			const my_type& operator=(target_iterator_ titr_){
				if(itr_ptr!=/*C++0x_nullptr/*/NULL/**/){
					itr_ptr->destruct();
					itr_ptr=/*C++0x_nullptr/*/NULL/**/;
				}
				itr_ptr=create(titr_);
				return *this;
			}
			const my_type& operator=(const my_type& my_){
				if(itr_ptr!=/*C++0x_nullptr/*/NULL/**/){
					itr_ptr->destruct();
					itr_ptr=/*C++0x_nullptr/*/NULL/**/;
				}
				if(my_.itr_ptr!=/*C++0x_nullptr/*/NULL/**/)itr_ptr=my_.itr_ptr->clone();
				return *this;
			}
		};
		template<typename value_type_,typename difference_type_>
		template<typename target_iterator_>
		chain<typename random_access_const_iterator_ghost<value_type_,difference_type_>::template target_adapter<target_iterator_>::iterator> random_access_const_iterator_ghost<value_type_,difference_type_>::target_adapter<target_iterator_>::MyChain;
	}
}
#
#endif
