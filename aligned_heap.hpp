#ifndef HMLIB_ALIGNEDHEAP_INC
#define HMLIB_ALIGNEDHEAP_INC 100
#include<memory>
namespace hmLib{
	template<typename T>
	struct aligned_heap{
		using pointer = T*;
		using const_pointer = const T*;
	private:
		unsigned char* Ptr;
		T* Beg;
		T* End;
	public:
		aligned_heap():Ptr(nullptr),Beg(nullptr),End(nullptr){}
		aligned_heap(aligned_heap<T>&& my){

		}
		aligned_heap<T>& operator=(const aligned_heap<T>&)=delete;
		aligned_heap(const aligned_heap<T>&)=delete;
		aligned_heap<T>& operator=(const aligned_heap<T>&)=delete;

	public:
		void allocate(unsigned int n){
			std::size_t bufsize = sizeof(T)*n + alignof(T);
			Ptr = new unsigned char[bufsize];

			Beg = static_cast<T*>(std::align(a, sizeof(T), Ptr, Ptr + bufsize));
			if(Beg)End = Beg + n;
		}
		void deallocate(){
			if(Ptr)delete[] Ptr;
		}
		T* begin(){return Beg;}
		T* end(){return End;}
	};
#
#endif
