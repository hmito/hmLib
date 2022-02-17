#ifndef HMLIB_ALIGNEDHEAP_INC
#define HMLIB_ALIGNEDHEAP_INC 100
#include<memory>
namespace hmLib{
	template<typename T>
	struct aligned_heap final{
		using pointer = T*;
		using const_pointer = const T*;
	private:
		unsigned char* Ptr;
		T* Beg;
		T* End;
	public:
		aligned_heap():Ptr(nullptr),Beg(nullptr),End(nullptr){}
		explicit aligned_heap(std::size_t n):aligned_heap(){
			allocate(n);
		}
		aligned_heap(aligned_heap<T>&& other):aligned_heap(){
			swap(other);
		}
		aligned_heap<T>& operator=(aligned_heap<T>&& other){
			if(this != &other){
				swap(other);
			}			
		}
		aligned_heap(const aligned_heap<T>&)=delete;
		aligned_heap<T>& operator=(const aligned_heap<T>&)=delete;
		~aligned_heap(){deallocate();}
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
		void swap(aligned_heap<T>& other){
			std::swap(Ptr, other.Ptr);
			std::swap(Beg, other.Beg);
			std::swap(End, other.End);
		}
		operator bool()const{return Beg!=nullptr;}
		T* begin(){return Beg;}
		T* end(){return End;}
		std::size_t size()const{return End-Beg;}
	};
}
#
#endif
