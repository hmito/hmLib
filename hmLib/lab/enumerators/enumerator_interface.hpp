#ifndef HMLIB_ENUMERATORS_ENUMERATORINTERFACE_INC
#define HMLIB_ENUMERATORS_ENUMERATORINTERFACE_INC 100
#
namespace hmLib {
	template<typename T>
	class enumerator_interaface {
	public:
		//return true if the object can be accessed to.
		virtual operator bool()const=0;
		//access to object.
		virtual T& operator*()=0;
		//move to next object.
		virtual void operator++()=0;
		//restart from the initial object.
		virtual void reset()=0;
	};
}
#
#endif
