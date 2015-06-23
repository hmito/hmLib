#ifndef HMLIB_ITERATORPROXY_INC
#define HMLIB_ITERATORPROXY_INC 100
#
#include<memory>
#include<boost/iterator/iterator_facade.hpp>
#include<boost/pool/object_pool.hpp>
#ifndef HMLIB_ITERATORINTERFACE_INC
#	include<hmLib/iterator_interface.hpp>
#endif
#ifndef HMLIB_OBJECTINTERFACE_INC
#	include<hmLib/object_interface.hpp>
#endif
namespace hmLib{
	namespace proxy{
		template<typename value_type_,typename difference_type_=int>
		struct random_access_iterator_proxy:public boost::iterator_facade<random_access_iterator_proxy<value_type_,difference_type_>,value_type_,boost::random_access_traversal_tag,value_type_&,difference_type_>{
		private:
			typedef random_access_iterator_proxy<value_type_,difference_type_> my_type;
		private:
			struct basic_iterator
				:public interfaces::random_access_iterator_interface<basic_iterator,value_type_,difference_type_>
				,public interfaces::clone_interface<basic_iterator>
				,public interfaces::destroy_interface<basic_iterator>{
			};
			template<typename iterator_>
			struct iterator_adaptor:public basic_iterator{
			private:
				typedef iterator_ iterator;
				typedef iterator_adaptor<iterator_> adaptor;
			private:
				iterator titr;
			protected:
				void itf_operator_increment()override{++titr;}
				void itf_operator_decrement()override{--titr;}
				my_type itf_operator_forward_add(const difference_type_& s_)const override{return adaptor(titr+s_);}
				difference_type_ itf_operator_sub(const my_type& s_)const override{
					adaptor* adp=dynamic_cast<adaptor*>(&val_);
					if(adp==nullptr)return false;
					return titr - adp->titr;
				}
				const value_type_& itf_operator_reference()const override{return *titr;}
				bool itf_operator_equal(const basic_iterator& itr_)const override{
					adaptor* adp=dynamic_cast<adaptor*>(&val_);
					if(adp==nullptr)return false;
					return adp->titr==titr;
				}
				basic_iterator* itf_clone()const override{return adaptor::construct(titr);}
				void itf_destroy(){adaptor::destroy(this);}
			public:
				iterator_adaptor(iterator titr_):titr(titr_){}
			private:
				static boost::object_pool<adaptor> Pool;
				static basic_iterator* construct(){return Pool.construct();}
				static basic_iterator* construct(iterator titr_){return Pool.construct(titr_);}
				static void destroy(basic_iterator* ptr){Pool.destroy(ptr);}
			};
		private:
			std::unique_ptr<basic_iterator> itr_ptr;
		public:
			random_access_iterator_proxy():itr_ptr(nullptr){}
			template<typename iterator_>
			random_access_iterator_proxy(iterator_ itr):itr_ptr(iterator_adaptor<iterator_>::construct(itr)){}
			random_access_iterator_proxy(const my_type& My_):itr_ptr(My_.itr_ptr->clone()){}
			random_access_iterator_proxy(my_type&& My_):itr_ptr(std::move(My_.itr_ptr)){}
			virtual ~random_access_iterator_proxy(){if(itr_ptr)itr_ptr->destroy();}
		public:
			my_type& operator++(){
				++(*itr_ptr);
				return *this;
			}
			my_type& operator--(){
				--(*itr_ptr);
				return *this;
			}
			value_type_ operator*(){return *(*itr_ptr);}
			bool operator==(const my_type& My_)const{return (*itr_ptr)==(*(My_->itr_ptr));}
			my_type& operator+=(difference_type_ d){
				(*itr_ptr)+=d;
				return *this;
			}
			difference_type_ operator-(const my_type& My_)const{return (*itr_ptr)-(*(My_->itr_ptr));}
		};
	}
}
#
#endif
