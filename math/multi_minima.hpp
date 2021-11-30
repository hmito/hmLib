#include<vector>
namespace hmLib{
	template<typename value_type,typename ans_type>
	struct nelder_mead_minima_stepper{
		using vertex_type = std::pair<ans_type, value_type>;
	public:
		using state_type = std::vector<vertex_type>;
		struct vertex_compare{
			bool operator()(const state_type& v1,const state_type& v2){return v1.first < v2.first;}
		};
	public:
		nelder_mead_minima_stepper(value_type step_, int bits_):step(step_), bits(bits_) {}
		template<typename fn, typename value_type, typename error_type>
		state_type make_state(fn Fn, const value_type& start,error_type abs_value=0.0001, error_type rel_value=0.05) {
			std::size_t size = std::distance(start.begin(),start.end());
			//initialize vertex
			std::vector<vertex_t> V(size+1, std::make_pair(Fn(start),start));
			for(std::size_t i=1; i < V.size(); ++i){
				auto itr = std::next(V[i].second.begin(),i-1);
				(*itr) += abs_value + rel_value * (*itr);
				V[i].first = Fn(V[i].second);
			}
			return V;
		}
		value_type state_to_value(const state_type& State){
			return std::max_element(State.begin(),State.end(),vertex_compare())->second;
		}
		template<typename fn, typename ans_type>
		void operator()(fn Fn, state_type& State) {
			std::size_t n = State.size()-1;

			std::sort(State.begin(),State.end(),vertex_compare());
			vertex_type Center = State.front();
			for(std::size_t i=0; i < n; ++i){
				for(std::size_t j = 1; j < n+1; ++j){
					Center.second[i] += State[j].second[i];
				}
				Center.second[i] /= n;
			}
			Center.first = Fn(Center.second);

			auto Refrect = Center;
			for(std::size_t i=0; i < n; ++i){
				Refrect.second[i] += alpha*(Refrect.second[i] - State[n].second[i]);
			}
			Refrect.first = Fn(Refrect.second);
			
			if(State[0].first <= Center.first && Center.first < State[n-1].first){
				//refrection
				State[n] = Center;
			}else if(Center.first < State[0].first){
				//expansion
				auto Expand = Center;
				for(std::size_t i=0; i < n; ++i){
					Expand.second[i] += gamma*(Refrect.second[i] - Center.second[i]);
				}
				Expand.first = Fn(Expand.second);

				if(Expand.first < Refrect.first){
					State[n] = Center;
				}else{
					State[n] = Refrect;
				}
			}else{
				//shrink
				Shrink = Center;
				for(std::size_t i=0; i < n; ++i){
					Shrink.second[i] += rho*(State[n].second[i] - Center.second[i]);
				}				
				Shrink.first = Fn(Shrink.second);
				if(Shrink.first < State[n].first){
					State[n] = Shrink;
				}else{
					for(std::size_t j = 1; j < n+1; ++j){
						for(std::size_t i=0; i < n; ++i){
							State[j].second[i] = State[0].second[i] + sigma*(State[j].second[i] - State[0].second[i]);
						}
						State[j].first = Fn(State[j].second);
					}
				}
			}
		}
	private:
		double alpha;
		double gamma;
		double rho;
		double sigma;
	};
}
template<typename fn, typename point>
void nelder_mead_find_minima (fn Fn, point ini, int n, double xmin[], double *ynewlo, double reqmin, double step[], int konvge, int kcount){
	using eval_t = decltype(Fn(ini));
	using vertex_t = std::pair<eval_t, point>;

	std::size_t size = std::distance(ini.begin(),ini.end());

	//initialize vertex
	std::vector<vertex_t> V(size+1, std::make_pair(ini,0));
	for(std::size_t i=1; i < V.size(); ++i){
		auto itr = std::next(V[i].second.begin(),i-1);
		if(*itr==0){
			*itr += 0.00025;
		}else{
			*itr *= 1.05;
		}
	}
	for(std::size_t i=0; i < V.size(); ++i){
		V[i].first = Fn(V[i].second);
	}
	std::sort(V.begin(), V.end(), [](const vertex_t& v1,const vertex_t& v2){return v1.first < v2.first;});

	while(terminator()){
		point C = V[0];
		for(std::size_t i = 1; i < V.size(); ++i){
			C += V[i];

		}
	}

	for(unsigned int i = 0; )

	double alpha = 1;
	double gamma = 2;
	double rho = 0.5;
	double sigma = 0.5;
	
	std::vector<point> points;
 	double ccoeff = 0.5;
	double del;
	double dn;
	double dnn;
	double ecoeff = 2.0;
	double eps = 0.001;
	int i;
	int ihi;
	int ilo;
	int j;
	int jcount;
	int l;
	int nn;
	double *p;
	double *p2star;
	double *pbar;
	double *pstar;
	double rcoeff = 1.0;
	double rq;
	double x;
	double *y;
	double y2star;
	double ylo;
	double ystar;
	double z;
	if ( reqmin <= 0.0 ){
		*ifault = 1;
		return;
	}

	if ( n < 1 ){
		*ifault = 1;
		return;
	}｀

	if ( konvge < 1 )
	{
	*ifault = 1;
	return;
	}

  p = new double[n*(n+1)];
  pstar = new double[n];
  p2star = new double[n];
  pbar = new double[n];
  y = new double[n+1];

  *icount = 0;
  *numres = 0;

  jcount = konvge; 
  dn = ( double ) ( n );
  nn = n + 1;
  dnn = ( double ) ( nn );
  del = 1.0;
  rq = reqmin * dn;
//
//  Initial or restarted loop.
//
  for ( ; ; ){
	for ( i = 0; i < n; i++ ){ 
	  p[i+n*n] = start[i];
	}
	y[n] = fn ( start );
	*icount = *icount + 1;

	for ( j = 0; j < n; j++ )
	{
	  x = start[j];
	  start[j] = start[j] + step[j] * del;
	  for ( i = 0; i < n; i++ )
	  {
		p[i+j*n] = start[i];
	  }
	  y[j] = fn ( start );
	  *icount = *icount + 1;
	  start[j] = x;
	}
//                    
//  The simplex construction is complete.
//                    
//  Find highest and lowest Y values.  YNEWLO = Y(IHI) indicates
//  the vertex of the simplex to be replaced.
//                
	ylo = y[0];
	ilo = 0;

	for ( i = 1; i < nn; i++ )
	{
	  if ( y[i] < ylo )
	  {
		ylo = y[i];
		ilo = i;
	  }
	}
//
//  Inner loop.
//
	for ( ; ; ){
	  if ( kcount <= *icount )
	  {
		break;
	  }
	  *ynewlo = y[0];
	  ihi = 0;

	  for ( i = 1; i < nn; i++ )
	  {
		if ( *ynewlo < y[i] )
		{
		  *ynewlo = y[i];
		  ihi = i;
		}
	  }
//
//  Calculate PBAR, the centroid of the simplex vertices
//  excepting the vertex with Y value YNEWLO.
//
	  for ( i = 0; i < n; i++ )
	  {
		z = 0.0;
		for ( j = 0; j < nn; j++ )
		{ 
		  z = z + p[i+j*n];
		}
		z = z - p[i+ihi*n];  
		pbar[i] = z / dn;
	  }
//
//  Reflection through the centroid.
//
	  for ( i = 0; i < n; i++ )
	  {
		pstar[i] = pbar[i] + rcoeff * ( pbar[i] - p[i+ihi*n] );
	  }
	  ystar = fn ( pstar );
	  *icount = *icount + 1;
//
//  Successful reflection, so extension.
//
	  if ( ystar < ylo )
	  {
		for ( i = 0; i < n; i++ )
		{
		  p2star[i] = pbar[i] + ecoeff * ( pstar[i] - pbar[i] );
		}
		y2star = fn ( p2star );
		*icount = *icount + 1;
//
//  Check extension.
//
		if ( ystar < y2star )
		{
		  for ( i = 0; i < n; i++ )
		  {
			p[i+ihi*n] = pstar[i];
		  }
		  y[ihi] = ystar;
		}
//
//  Retain extension or contraction.
//
		else
		{
		  for ( i = 0; i < n; i++ )
		  {
			p[i+ihi*n] = p2star[i];
		  }
		  y[ihi] = y2star;
		}
	  }
//
//  No extension.
//
	  else
	  {
		l = 0;
		for ( i = 0; i < nn; i++ )
		{
		  if ( ystar < y[i] )
		  {
			l = l + 1;
		  }
		}

		if ( 1 < l )
		{
		  for ( i = 0; i < n; i++ )
		  {
			p[i+ihi*n] = pstar[i];
		  }
		  y[ihi] = ystar;
		}
//
//  Contraction on the Y(IHI) side of the centroid.
//
		else if ( l == 0 )
		{
		  for ( i = 0; i < n; i++ )
		  {
			p2star[i] = pbar[i] + ccoeff * ( p[i+ihi*n] - pbar[i] );
		  }
		  y2star = fn ( p2star );
		  *icount = *icount + 1;
//
//  Contract the whole simplex.
//
		  if ( y[ihi] < y2star )
		  {
			for ( j = 0; j < nn; j++ )
			{
			  for ( i = 0; i < n; i++ )
			  {
				p[i+j*n] = ( p[i+j*n] + p[i+ilo*n] ) * 0.5;
				xmin[i] = p[i+j*n];
			  }
			  y[j] = fn ( xmin );
			  *icount = *icount + 1;
			}
			ylo = y[0];
			ilo = 0;

			for ( i = 1; i < nn; i++ )
			{
			  if ( y[i] < ylo )
			  {
				ylo = y[i];
				ilo = i;
			  }
			}
			continue;
		  }
//
//  Retain contraction.
//
		  else
		  {
			for ( i = 0; i < n; i++ )
			{
			  p[i+ihi*n] = p2star[i];
			}
			y[ihi] = y2star;
		  }
		}
//
//  Contraction on the reflection side of the centroid.
//
		else if ( l == 1 )
		{
		  for ( i = 0; i < n; i++ )
		  {
			p2star[i] = pbar[i] + ccoeff * ( pstar[i] - pbar[i] );
		  }
		  y2star = fn ( p2star );
		  *icount = *icount + 1;
//
//  Retain reflection?
//
		  if ( y2star <= ystar )
		  {
			for ( i = 0; i < n; i++ )
			{
			  p[i+ihi*n] = p2star[i];
			}
			y[ihi] = y2star;
		  }
		  else
		  {
			for ( i = 0; i < n; i++ )
			{
			  p[i+ihi*n] = pstar[i];
			}
			y[ihi] = ystar;
		  }
		}
	  }
//
//  Check if YLO improved.
//
	  if ( y[ihi] < ylo )
	  {
		ylo = y[ihi];
		ilo = ihi;
	  }
	  jcount = jcount - 1;

	  if ( 0 < jcount )
	  {
		continue;
	  }
//
//  Check to see if minimum reached.
//
	  if ( *icount <= kcount )
	  {
		jcount = konvge;

		z = 0.0;
		for ( i = 0; i < nn; i++ )
		{
		  z = z + y[i];
		}
		x = z / dnn;

		z = 0.0;
		for ( i = 0; i < nn; i++ )
		{
		  z = z + pow ( y[i] - x, 2 );
		}

		if ( z <= rq )
		{
		  break;
		}
	  }
	}
//
//  Factorial tests to check that YNEWLO is a local minimum.
//
	for ( i = 0; i < n; i++ )
	{
	  xmin[i] = p[i+ilo*n];
	}
	*ynewlo = y[ilo];

	if ( kcount < *icount )
	{
	  *ifault = 2;
	  break;
	}

	*ifault = 0;

	for ( i = 0; i < n; i++ )
	{
	  del = step[i] * eps;
	  xmin[i] = xmin[i] + del;
	  z = fn ( xmin );
	  *icount = *icount + 1;
	  if ( z < *ynewlo )
	  {
		*ifault = 2;
		break;
	  }
	  xmin[i] = xmin[i] - del - del;
	  z = fn ( xmin );
	  *icount = *icount + 1;
	  if ( z < *ynewlo )
	  {
		*ifault = 2;
		break;
	  }
	  xmin[i] = xmin[i] + del;
	}

	if ( *ifault == 0 )
	{
	  break;
	}
//
//  Restart the procedure.
//
	for ( i = 0; i < n; i++ )
	{
	  start[i] = xmin[i];
	}
	del = eps;
	*numres = *numres + 1;
  }
  delete [] p;
  delete [] pstar;
  delete [] p2star;
  delete [] pbar;
  delete [] y;

  return;
}
