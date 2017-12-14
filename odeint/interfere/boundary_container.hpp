#ifndef HMLIB_ODEINT_INTERFERE_BOUNDARYCONTAINER_INC
#define HMLIB_ODEINT_INTERFERE_BOUNDARYCONTAINER_INC 100
#
#include<vector>
namespace hmLib {
	namespace odeint {
		template<typename boundary_>
		struct for_each_boundary{
			using boundary_type = boundary_;
			using state_type = typename boundary_type::state_type;
			using container = std::vector<boundary_type>;
			using iterator = typename container::iterator;
			using const_iterator = typename container::const_iterator;
		private:
			boundary_type Original;
			container Container;
		public:
			for_each_boundary(boundary_type Original_):Original(std::move(Original_)){}
		public://for interfere system
			template<typename state_type>
			void ready(const state_type& x) { 
				if(Container.size() != x.size()) {
					Container.assign(x.size(), Original);
				}

				auto itr = x.begin();
				
				for(auto bitr = begin(); bitr != end(); ++bitr, ++itr) {
					bitr->ready(*itr);
				}
			}
			template<typename state_type>
			bool valid_step(const state_type& x)const {
				auto itr = x.begin();

				for(auto bitr = begin(); bitr != end(); ++bitr, ++itr) {
					if(!bitr->valid_step(*itr))return false;
				}
				return true;
			}
			template<typename state_type>
			bool validate(const state_type& x, state_type& vx) const {
				bool IsValidated = false;
				auto itr = x.begin();
				auto vitr = vx.begin();

				for(auto bitr = begin(); bitr != end(); ++bitr, ++itr, ++vitr) {
					IsValidated |= bitr->validate(*itr,*vitr);
				}
				return IsValidated;
			}
		public:
			iterator begin() { return Container.begin(); }
			iterator end() { return Container.end(); }
			const_iterator begin()const { return Container.begin(); }
			const_iterator end()const { return Container.end(); }
			const_iterator cbegin()const { return Container.begin(); }
			const_iterator cend()const { return Container.end(); }
			std::size_t size()const { return Container.size(); }
			bool empty()const { return Container.empty(); }
			boundary_type& at(std::size_t n) { return Container.at(n); }
			const boundary_type& at(std::size_t n)const{ return Container.at(n); }
		};
		template<typename boundary>
		auto make_for_each_boundary(boundary b) {
			return boundary_container<typename std::decay<state_type>::type, typename std::decay<boundary>::type>(std::move(b));
		}
	}
}
#
#endif
