#ifndef HMLIB_LATTICES_LATTICE_INC
#define HMLIB_LATTICES_LATTICE_INC 100
#
#include<vector>
#include "lattice_view.hpp"

namespace hmLib{
    template<typename T, unsigned int dim_>
    struct lattice{
    private:
        using this_type = lattice<dim_>;
        using container = std::vector<T>;
        using iterator_base = typename container::iterator;
        using const_iterator_base = typename container::const_iterator;
    public:
        using view = lattice_view<iterator_base, dim_>;
        using const_view = lattice_view<const_iterator_base, dim_>;
    public:
        static constexpr unsigned int dim(){return dim_;}
    private:
        container Data;
        view View;
    private:
        void reset_view(const size_type& Size){
            View = view(Data.begin(),Data.end(),Size);
        }
    public:
        lattice() = default;
        lattice(const this_type& other){
            Data = other.Data;
            reset_view(other.size());
        }
    };
}
#
#endif
