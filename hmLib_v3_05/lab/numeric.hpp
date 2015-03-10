#ifndef HMLIB_NUMERIC_INC
#define HMLIB_NUMERIC_INC 1
#
/*===hmLib::numeric===
”’l‰‰Z‚ÉŠÖ‚·‚éA•W€ƒ‰ƒCƒuƒ‰ƒŠ‚Ì•â•ŠÖ”ŒQ

v1_00:130805 hmIto
	•½‹ÏA•ªUA‹¤•ªU‚ğ’Ç‰Á
*/
#include<iterator>
#include<numeric>

namespace hmLib{
	namespace numeric{
		template<typename input_iterator,typename T>
		T average(input_iterator Begin,input_iterator End,T Init){
			return std::accumulate(Begin,End,Init)/std::distance(Begin,End);
		}
		template<typename input_iterator,typename T,typename accumulate_functor>
		T average(input_iterator Begin,input_iterator End,T Init,accumulate_functor Func){
			return std::accumulate(Begin,End,Init,Func)/std::distance(Begin,End);
		}
		template<typename input_iterator,typename T>
		T variance(input_iterator Begin,input_iterator End,T Init){
			auto Ave=average(Begin,End,Init);
			return average(Begin,End,Init,[](const T& Val)->T{return Val*Val;})-Ave*Ave;
		}
		template<typename input_iterator,typename T,typename accumulate_functor,typename square_accumulate_functor>
		T variance(input_iterator Begin,input_iterator End,T Init,accumulate_functor Func1,square_accumulate_functor Func2){
			auto Ave=average(Begin,End,Init,Func1);
			return average(Begin,End,Init,Func2)-Ave*Ave;
		}
		template<typename input_iterator1,typename input_iterator2,typename T>
		T covariance(input_iterator1 Begin1,input_iterator1 End1,input_iterator2 Begin2,input_iterator2 End2,T Init){
			auto Dis1=std::distance(Begin1,End1);
			auto Dis2=std::distance(Begin2,End2);

			auto Ave1=std::accumulate(Begin1,End1,Init)/Dis1;
			auto Ave2=std::accumulate(Begin2,End2,Init)/Dis2;

			while(Begin1!=End1 && Begub2!=End2){
				Init+=(*Begin1-Ave1)*(*Begin2-Ave2);
				++Begin1;
				++Begin2;
			}
			return Init/Dis1;
		}
	}
}
#
#endif
