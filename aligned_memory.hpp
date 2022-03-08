#ifndef HMLIB_ALIGNEDMEMORY_INC
#define HMLIB_ALIGNEDMEMORY_INC 100
#include<memory>
namespace hmLib{
	template<typename T, std::size_t StackSize>
	struct aligned_stack final{
		using this_type = aligned_stack<T,StackSize>;
		using pointer = T*;
		using const_pointer = const T*;
	public:
		aligned_stack() = default;
		aligned_stack(this_type&&) = delete;
		this_type& operator=(this_type&&) = delete;
		aligned_stack(const this_type&) = delete;
		this_type& operator=(const this_type&) = delete;
		~aligned_stack() = default;
	public: //common functions for aligned memory classes
		operator bool()const{return true;}
		pointer begin(){return static_cast<pointer>(static_cast<void*>(Buf));}
		pointer end(){return static_cast<pointer>(static_cast<void*>(Buf))+StackSize;}
		const_pointer begin()const{return cbegin();}
		const_pointer end()const{return cend();}
		const_pointer cbegin()const{return static_cast<pointer>(static_cast<void*>(Buf));}
		const_pointer cend()const{return static_cast<pointer>(static_cast<void*>(Buf))+StackSize;}
		std::size_t size()const{return static_size();}
	public:
		static std::size_t static_size(){return StackSize;}
	private:
		alignas(alignof(T)) unsigned char Buf[sizeof(T)*StackSize];
	};
	template<typename T>
	struct aligned_heap final{
		using this_type = aligned_heap<T>;
		using pointer = T*;
		using const_pointer = const T*;
	public:
		aligned_heap()noexcept:Ptr(nullptr),Beg(nullptr),End(nullptr){}
		explicit aligned_heap(std::size_t n):aligned_heap(){
			allocate(n);
		}
		aligned_heap(this_type&& other):aligned_heap(){
			swap(other);
		}
		this_type& operator=(this_type&& other){
			if(this != &other){
				swap(other);
			}			
		}
		aligned_heap(const this_type&)=delete;
		this_type& operator=(const this_type&)=delete;
		~aligned_heap()noexcept{deallocate();}
	public: //common functions for aligned memory classes
		operator bool()const{return Beg!=nullptr;}
		pointer begin(){return Beg;}
		pointer end(){return End;}
		const_pointer begin()const{return cbegin();}
		const_pointer end()const{return cend();}
		const_pointer cbegin()const{return Beg;}
		const_pointer cend()const{return End;}
		std::size_t size()const{return End-Beg;}
	public:
		void allocate(unsigned int n){
			deallocate();

			std::size_t bufsize = sizeof(T)*n + alignof(T);
			Ptr = new unsigned char[bufsize];
            
            void* vPtr = Ptr;
            std::align(alignof(T), sizeof(T), vPtr,bufsize);
			Beg = static_cast<pointer>(vPtr);
			if(Beg)End = Beg + n;
		}
		void deallocate()noexcept{
			if(Ptr){
				delete[] Ptr;
				Ptr = nullptr;
				Beg = nullptr;
				End = nullptr;
			}
		}
		void swap(this_type& other){
			std::swap(Ptr, other.Ptr);
			std::swap(Beg, other.Beg);
			std::swap(End, other.End);
		}
	private:
		unsigned char* Ptr;
		pointer Beg;
		pointer End;
	};
	template<typename T, unsigned int SOOSize>
	struct aligned_soo final{
		using this_type = aligned_soo<T,SOOSize>;
		using pointer = T*;
		using const_pointer = const T*;
	public:
		aligned_soo()noexcept:Ptr(nullptr),Beg(nullptr),End(nullptr){}
		explicit aligned_soo(std::size_t n):soo_aligned_memory(){
			allocate(n);
		}
		aligned_soo(this_type&&) = delete;
		this_type& operator=(this_type&&) = delete;
		soo_aligned_memory(const this_type&) = delete;
		this_type& operator=(const this_type&) = delete;
		~soo_aligned_memory(){deallocate();}
	public: //common functions for aligned memory classes
		operator bool()const{return Beg!=nullptr;}
		pointer begin(){return Beg;}
		pointer end(){return End;}
		const_pointer begin()const{return cbegin();}
		const_pointer end()const{return cend();}
		const_pointer cbegin()const{return Beg;}
		const_pointer cend()const{return End;}
		std::size_t size()const{return End-Beg;}
	public:
		void allocate(unsigned int n){
			if(on_heap()){
				delete[] Ptr;
				Ptr = nullptr;
			}

			if(n>soo_size()){
				std::size_t bufsize = sizeof(T)*n + alignof(T);
				Ptr = new unsigned char[bufsize];
				
				void* vPtr = Ptr;
				std::align(alignof(T), sizeof(T), vPtr,bufsize);
				Beg = static_cast<pointer>(vPtr);
				if(Beg)End = Beg + n;
			}else{
				Beg = static_cast<pointer>(static_cast<void*>(Ptr));
				End = Beg + n;
			}
		}
		void deallocate()noexcept{
			if(on_heap()){
				delete[] Ptr;
				Ptr = nullptr;
			}
			Beg = nullptr;
			End = nullptr;
		}
		bool try_move(this_type&& other)noexcept{
			if(!other.on_heap()) return true;
	
			if(on_heap())delete[] Ptr;
			Ptr = other.Ptr;
			Beg = other.Beg;
			End = other.End;
			other.Ptr = nullptr;
			other.Beg = nullptr;
			other.End = nullptr;

			return false;
		}
		bool try_swap(this_type& other)noexcept{
			if(!on_heap() || !other.on_heap()) return true;

			std::swap(Ptr, other.Ptr);
			std::swap(Beg, other.Beg);
			std::swap(End, other.End);

			return false;
		}
		bool on_heap()noexcept const{return Ptr!=nullptr;}
		static std::size_t soo_size(){return SOOSize;}
	private:
		alignas(alignof(T)) unsigned char Buf[sizeof(T)*SOOSize];
		unsigned char* Ptr;
		pointer Beg;
		pointer End;
	};
}
#
#endif
