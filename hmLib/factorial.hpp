#ifndef HMLIB_FACTORIAL_INC
#define HMLIB_FACTORIAL_INC 100
#
#include<utility>
#include<vector>
#include<algorithm>
namespace hmLib{
	namespace hmLib{
		struct factorial_complex;
		struct factorial{
		private:
			unsigned int val;
		public:
			factorial(unsigned int val_) :val(val_){}
			unsigned int evaluate()const{
				unsigned int ans = 1;
				for(unsigned int i = 2; i <= val; ++i){
					ans *= i;
				}
				return ans;
			}
			unsigned int value()const{ return val; }
		public:
			operator unsigned int(){ return evaluate(); }
			friend factorial_complex operator*(factorial v1, factorial v2);
			friend factorial_complex operator/(factorial v1, factorial v2);
		};
		struct factorial_complex{
		private:
			std::vector<unsigned int> Up;
			std::vector<unsigned int> Down;
		public:
			double evaluate(){
				std::sort(Up.begin(), Up.end(), [](unsigned int v1, unsigned int v2){return v1 > v2; });
				std::sort(Down.begin(), Down.end(), [](unsigned int v1, unsigned int v2){return v1 > v2; });

				double Ans = 1.0;
				auto uItr = Up.begin();
				auto dItr = Down.begin();
				for(; uItr != Up.end() && dItr != Down.end(); ++uItr, ++dItr){
					if(*uItr > *dItr){
						unsigned int val = 1;
						for(unsigned int i = *dItr + 1; i <= *uItr; ++i){
							val *= i;
						}
						Ans *= val;
					} else{
						unsigned int val = 1;
						for(unsigned int i = *uItr + 1; i <= *dItr; ++i){
							val *= i;
						}
						Ans /= val;
					}
				}
				{
					unsigned int val = 1;
					for(; uItr != Up.end(); ++uItr){
						for(unsigned int i = 2; i <= *uItr; ++i){
							val *= i;
						}
					}
					Ans *= val;
				}
				{
					unsigned int val = 1;
					for(; dItr != Down.end(); ++dItr){
						for(unsigned int i = 2; i <= *dItr; ++i){
							val *= i;
						}
					}
					Ans /= val;
				}
				return Ans;
			}
		public:
			factorial_complex() = default;
			factorial_complex(unsigned int val){ Up.push_back(val); }
			operator double(){ return evaluate(); }
			friend factorial_complex&& operator*(factorial_complex&& v1, factorial v2){
				v1.Up.push_back(v2.value());
				return std::move(v1);
			}
			friend factorial_complex&& operator/(factorial_complex&& v1, factorial v2){
				v1.Down.push_back(v2.value());
				return std::move(v1);
			}
			friend factorial_complex&& operator*(factorial v1, factorial_complex&& v2){
				v2.Up.push_back(v1.value());
				return std::move(v2);
			}
			friend factorial_complex&& operator/(factorial v1, factorial_complex&& v2){
				std::swap(v2.Up, v2.Down);
				v2.Up.push_back(v1.value());
				return std::move(v2);
			}
			friend factorial_complex&& operator*(factorial_complex&& v1, factorial_complex&& v2){
				v1.Up.insert(v1.Up.end(), v2.Up.begin(), v2.Up.end());
				v1.Down.insert(v1.Down.end(), v2.Down.begin(), v2.Down.end());
				return std::move(v1);
			}
			friend factorial_complex&& operator/(factorial_complex&& v1, factorial_complex&& v2){
				v1.Up.insert(v1.Up.end(), v2.Down.begin(), v2.Down.end());
				v1.Down.insert(v1.Down.end(), v2.Up.begin(), v2.Up.end());
				return std::move(v1);
			}
		};
		inline factorial_complex operator*(factorial v1, factorial v2){
			return factorial_complex(v1.val)*v2;
		}
		inline factorial_complex operator/(factorial v1, factorial v2){
			return factorial_complex(v1.val) / v2;
		}
	}
}
#
#endif
