//SDev_SError.hpp
//To declare standard deviation and standard error functions

#ifndef SDev_SError_HPP
#define SDev_SError_HPP

#include <algorithm> // for max()
#include <cmath>
#include <iostream>
#include <vector>
#include "UtilitiesDJD/VectorsAndMatrices/ArrayMechanisms.cpp"
#include <boost/tuple/tuple.hpp>	// Tuple class
#include <boost/tuple/tuple_io.hpp> // I/O for tuples

 
template <typename P>			//Use P to avoid clash with Expiry time T in the argument
boost::tuple<double,double> StDevStErr(const std::vector<P>& v, double r, double T);

#ifndef SDev_SError_CPP
#include "SDev_SError.cpp"
#endif

#endif

