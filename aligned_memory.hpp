#ifndef HMLIB_ALIGNEDMEMORY_INC
#define HMLIB_ALIGNEDMEMORY_INC 100
#include<memory>
namespace hmLib{
	template<typename T>
	struct aligned_memory final{
		using pointer = T*;
		using const_pointer = const T*;
	private:
		unsigned char* Ptr;
		T* Beg;
		T* End;
	public:
		aligned_memory():Ptr(nullptr),Beg(nullptr),End(nullptr){}
		explicit aligned_memory(std::size_t n):aligned_memory(){
			allocate(n);
		}
		aligned_memory(aligned_memory<T>&& other):aligned_memory(){
			swap(other);
		}
		aligned_memory<T>& operator=(aligned_memory<T>&& other){
			if(this != &other){
				swap(other);
			}			
		}
		aligned_memory(const aligned_memory<T>&)=delete;
		aligned_memory<T>& operator=(const aligned_memory<T>&)=delete;
		~aligned_memory(){deallocate();}
	public:
		void allocate(unsigned int n){
			deallocate();

			std::size_t bufsize = sizeof(T)*n + alignof(T);
			Ptr = new unsigned char[bufsize];
            
            void* vPtr = Ptr;
            std::align(alignof(T), sizeof(T), vPtr,bufsize);
			Beg = static_cast<T*>(vPtr);
			if(Beg)End = Beg + n;
		}
		void deallocate(){
			if(Ptr){
				delete[] Ptr;
				Ptr = nullptr;
				Beg = nullptr;
				End = nullptr;
			}
		}
		void swap(aligned_memory<T>& other){
			std::swap(Ptr, other.Ptr);
			std::swap(Beg, other.Beg);
			std::swap(End, other.End);
		}
		operator bool()const{return Beg!=nullptr;}
		T* begin(){return Beg;}
		T* end(){return End;}
		std::size_t size()const{return End-Beg;}
	};
	template<typename T, unsigned int Size>
	struct soo_aligned_memory final{
		static_assert(std::is_trivial_copyable<T>::value, "sooaligned_memory can use only for a trivial-copyable class.")
		using pointer = T*;
		using const_pointer = const T*;
	private:
		unsigned char* Ptr;
		alignof(T) unsigned char Buf[sizeof(T)*Size];
		T* Beg;
		T* End;
	public:
		soo_aligned_memory():Ptr(nullptr),Beg(nullptr),End(nullptr){}
		explicit soo_aligned_memory(std::size_t n):soo_aligned_memory(){
			allocate(n);
		}
		soo_aligned_memory(soo_aligned_memory<T>&& other):soo_aligned_memory(){
			if(other.ptr){
				if(other.Ptr!=static_cast<unsigned char*>(other.Buf)){
					Ptr = other.Ptr;
					Beg = other.Beg;
					End = other.End;
				}else{
					Ptr = Buf;
					std::memcpy(Buf, other.Buf, sizeof(T)*other.size());
					Beg = static_cast<T*>(static_cast<void*>(Ptr));
					End = Beg + n;
				}
				other.Ptr = nullptr;
				other.Beg = nullptr;
				other.End = nullptr;
			}
		}
		soo_aligned_memory<T>& operator=(soo_aligned_memory<T>&& other){
			if(this != &other){
				if(Ptr){
					if(Ptr==Buf){

					}
				}
			}			
		}
		soo_aligned_memory(const soo_aligned_memory<T>&)=delete;
		soo_aligned_memory<T>& operator=(const soo_aligned_memory<T>&)=delete;
		~soo_aligned_memory(){deallocate();}
	public:
		void allocate(unsigned int n){
			deallocate();

			if(n>Size){
				std::size_t bufsize = sizeof(T)*n + alignof(T);
				Ptr = new unsigned char[bufsize];
				
				void* vPtr = Ptr;
				std::align(alignof(T), sizeof(T), vPtr,bufsize);
				Beg = static_cast<T*>(vPtr);
				if(Beg)End = Beg + n;
			}else{
				Ptr = Buf;
				Beg = static_cast<T*>(static_cast<void*>(Ptr));
				End = Beg + n;
			}
		}
		void deallocate(){
			if(Ptr){
				if(Ptr!=Buf) delete[] Ptr;
				Ptr = nullptr;
				Beg = nullptr;
				End = nullptr;
			}
		}
		void swap(soo_aligned_memory<T>& other){
			if(Ptr!=static_cast<unsigned char*>(Buf)){
				std::swap(Ptr, other.Ptr);
				std::swap(Beg, other.Beg);
				std::swap(End, other.End);
			}else{
				Ptr = static_cast<unsigned char*>(Buf);
				std::memcpy(Buf, other.Buf, Size);
				Beg = static_cast<T*>(static_cast<void*>(Ptr));
				End = Beg + n;
			}
		}
		operator bool()const{return Beg!=nullptr;}
		T* begin(){return Beg;}
		T* end(){return End;}
		std::size_t size()const{return End-Beg;}
	};
}
#
#endif
