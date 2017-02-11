//SDev_SErr0r.cpp
//File to define the standard dev and error function


#ifndef SDev_SError_CPP
#define SDev_SError_CPP

#include <algorithm> // for max()
#include <cmath>
#include <iostream>
#include <vector>
#include <numeric>
#include "UtilitiesDJD/VectorsAndMatrices/ArrayMechanisms.cpp"
#include "SDev_SError.hpp"
#include <boost/tuple/tuple.hpp>	// Tuple class
#include <boost/tuple/tuple_io.hpp> // I/O for tuples
using boost::tuple;

template <typename P>
boost::tuple<double,double> StDevStErr(const std::vector<P>& v, double r, double T)
{
	double m = double(v.size());	//m=NSIM
	double k=0;						//Initialize variables
	double sum=0.0;
	double sqsum=0.0;

	std::vector<P>:: const_iterator i;
	for (i = v.begin(); i != v.end(); ++i)
	{
		sum += *i;
		sqsum = (*i) * (*i) + sqsum;	
		k++;
	}
	double tmp = sqsum - ((sum * sum)/m);		//implement formula
	double sd = sqrt(((tmp) * exp(-2*r*T))/(m-1));
	double se = sd/sqrt(double (m));
	boost::tuple<double,double> sdse = boost::make_tuple(sd,se);

	cout<<"size = "<<k<<endl;//To check if all elements have been taken into the sum
	cout<<"size ="<<m<<endl;
	return sdse;
}






#endif
