		template<typename T>
		struct nelder_mead_minima_stepper{
		private:
			using point = std::vector<T>;
			struct vertex {
			private:
				point val;
				T fval;
			public:
				vertex() = default;
				vertex(point val_, T fval_) :val(std::move(val_)), fval(fval_) {}
				point& value(){ return val; }
				T& fvalue(){ return fval; }
				const point& value()const{ return val; }
				const T& fvalue()const { return fval; }
				friend bool operator==(const vertex& v1, const vertex& v2) { return v1.fvalue() == v2.fvalue(); }
				friend bool operator!=(const vertex& v1, const vertex& v2) { return v1.fvalue() != v2.fvalue(); }
				friend bool operator<(const vertex& v1, const vertex& v2) { return v1.fvalue() < v2.fvalue(); }
				friend bool operator<=(const vertex& v1, const vertex& v2) { return v1.fvalue() <= v2.fvalue(); }
				friend bool operator>(const vertex& v1, const vertex& v2) { return v1.fvalue() > v2.fvalue(); }
				friend bool operator>=(const vertex& v1, const vertex& v2) { return v1.fvalue() >= v2.fvalue(); }
			};
		public:
			struct state{
				friend struct nelder_mead_minima_stepper<T>;
			private:
				using container = std::vector<vertex>;
			public:
				using iterator = decltype(std::declval<container>().begin());
				using const_iterator = decltype(std::declval<container>().cbegin());
				using reference = vertex&;
				using const_reference = const vertex&;
			private:
				std::vector<vertex> Amoeba;
			public:
				state() = default;
				template<typename fn, typename error_type>
				state(fn Fn, const point& ini, error_type rel_value, error_type abs_value) {
					std::size_t size = std::distance(ini.begin(), ini.end());
					//initialize vertex
					Amoeba.assign(size + 1, vertex(ini, Fn(ini)));
					for (std::size_t i = 1; i < Amoeba.size(); ++i) {
						auto itr = std::next(Amoeba[i].value().begin(), i - 1);
						(*itr) += abs_value + rel_value * (*itr);
						Amoeba[i].fvalue() = Fn(Amoeba[i].value());
					}
				}
				point& value() {
					return std::min_element(Amoeba.begin(), Amoeba.end())->value();
				}
				T& fvalue() {
					return std::min_element(Amoeba.begin(), Amoeba.end())->fvalue();
				}
				const point& value()const{
					return std::min_element(Amoeba.begin(),Amoeba.end())->value();
				}
				const T& fvalue()const {
					return std::min_element(Amoeba.begin(),Amoeba.end())->fvalue();
				}
				iterator begin() { return Amoeba.begin(); }
				iterator end() { return Amoeba.end(); }
				const_iterator begin()const { return Amoeba.begin(); }
				const_iterator end()const { return Amoeba.end(); }
				const_iterator cbegin()const { return Amoeba.cbegin(); }
				const_iterator cend()const { return Amoeba.cend(); }
				reference at(unsigned int n) { return Amoeba.at(n); }
				const_reference at(unsigned int n)const { return Amoeba.at(n); }
				reference operator[](unsigned int n) { return Amoeba[n]; }
				const_reference operator[](unsigned int n)const { return Amoeba[n]; }
			};