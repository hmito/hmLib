namespace hmLib{
	namespace lattices{
		template<unsigned int dim_>
		using point = std::array<int, dim_>;
		template<typename ...others>
		auto make_point(others... Others)->point<sizeof...(others)>{
			return point<sizeof...(others)>{Others...};
		}
	}
}
