#ifndef HMLIB_STATICBUFFER_INC
#define HMLIB_STATICBUFFER_INC 100
#
namespace hmLib{
	template<typename type, unsigned int N, typename identifier>
	struct static_buffer{
	private:
		static type Buffer[N];
	public:
		type* begin()noexcept{ return Buffer; }
		type* end()noexcept{ return Buffer + N; }
		const type* cbegin()const noexcept{ return Buffer; }
		const type* cend()const noexcept{ return Buffer + N; }
		const type* cbegin()const noexcept{ return Buffer; }
		const type* cend()const noexcept{ return Buffer + N; }
	};
	template<typename type, unsigned int N, typename identifier>
	type static_buffer<type, N, identifier>::Buffer[N];
}
#
#endif
