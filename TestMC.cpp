// HardCoded.cpp
//
// C++ code to price an option, essential algorithms.
//
//Required to add the folder "Murugaraj Odiathevar Level 9 HW submission" under additional
//include libraries under project properties, VC++Directories

#include "OptionData.hpp" 
#include "UtilitiesDJD/RNG/NormalGenerator.hpp"
#include "UtilitiesDJD/Geometry/Range.cpp"
#include "UtilitiesDJD/VectorsAndMatrices/ArrayMechanisms.cpp"
#include <boost/tuple/tuple.hpp>	// Tuple class
#include <boost/tuple/tuple_io.hpp> // I/O for tuples
#include "SDev_SError.hpp"
using boost::tuple;
#include <cmath>
#include <vector>
#include <iostream>
#include <algorithm> // for max()
#include <numeric>



template <class T> void print(const std::vector<T>& myList)
{  // A generic print function for vectors
	
	std::cout << std::endl << "Size of vector is " << l.size() << "\n[";

	// We must use a const iterator here, otherwise we get a compiler error.We do not want to change the values that they point to.
	std::vector<T>::const_iterator i; //Because We are printing a const vector
	for (i = myList.begin(); i != myList.end(); ++i)
	{
			std::cout << *i << ",";

	}

	std::cout << "]\n";
}

namespace SDEDefinition
{ // Defines drift + diffusion + data

	OptionData* data;				// The data for the option MC. A pointer to the data

	double drift(double t, double X)
	{ // Drift term
	
		return (data->r)*X; // r - D
	}

	
	double diffusion(double t, double X)
	{ // Diffusion term
	
		double betaCEV = 1.0;
		return data->sig * pow(X, betaCEV);
		
	}

	double diffusionDerivative(double t, double X)
	{ // Diffusion term, needed for the Milstein method
	
		double betaCEV = 1.0;
		return 0.5 * (data->sig) * (betaCEV) * pow(X, 2.0 * betaCEV - 1.0);
	}
} // End of namespace


int main()
{

	std::cout <<  "1 factor MC with explicit Euler\n";
	OptionData myOption;	//Entering option data
	myOption.K = 120.0;//65.0;		//Strike
	myOption.T = 1.5;//0.25;		//Expiry
	myOption.r = 0.04;//0.08;		//Interest rate
	myOption.sig = 0.4;//0.3;		//volatility
	myOption.type = -1;	// Put -1, Call +1
	double S_0 = 100;//60;		//Stock Price at t=0
	
	long N = 100;		
	std::cout << "Number of subintervals in time: ";
	std::cin >> N;			//N subintervals. 

	// Create the basic SDE (Context class)
	Range<double> range (0.0, myOption.T); //A range from 0 to T, expiry
	double VOld = S_0;		//Old value for stock price
	double VNew;

	Vector<double, long> x = range.mesh(N);	//Create a discrete mesh of N steps of range
	

	// V2 mediator stuff
	long NSim = 50000;				
	std::cout << "Number of simulations: ";
	std::cin >> NSim;			//Nbr of simulations for paths of underlying stock along the mesh points

	double k = myOption.T / double (N);	//Change in t(time interval)
	double sqrk = sqrt(k);				//sqrt of change in t

	// Normal random number
	double dW;							
	double price = 0.0;	// Option price

	// NormalGenerator is a base class
	NormalGenerator* myNormal = new BoostNormal();

	using namespace SDEDefinition;
	data = &myOption;			//pointer to myoption

	Vector<double> res;
	vector<double> v;
	int coun = 0; // Number of times S hits origin. When stock price becomes 0
	//Our SDE becomes invalid??

	// A.
	for (long i = 1; i <= NSim; ++i)
	{ // Calculate a path at each iteration
			
		if ((i/10000) * 10000 == i)			//To show that the computer is at each ten thousand iteration
		{// Give status after each 1000th iteration

				std::cout << i << std::endl;
		}

		VOld = S_0;
		for (long index = x.MinIndex()+1; index <= x.MaxIndex(); ++index)	//x is the discrete mesh of time
		{//Min index is the start, max index is start+size-1

			// Create a random number
			dW = myNormal->getNormal();
				
			// The FDM (in this case explicit Euler)
			VNew = VOld  + (k * drift(x[index-1], VOld))		//Equation 10 from HW 
						+ (sqrk * diffusion(x[index-1], VOld) * dW);

			VOld = VNew;

			// Spurious values
			if (VNew <= 0.0) coun++;
		}	//Run the simulations and output a VNew for each simulation at t=T
			
		double tmp = myOption.myPayOffFunction(VNew);//Call price for each VNew
		v.push_back(tmp);							// add call price as elements of vector v
		price += (tmp)/double(NSim);			//Average
	}
	
	// D. Finally, discounting the average price
	price *= exp(-myOption.r * myOption.T);

	// Cleanup; V2 use scoped pointer
	delete myNormal;

	std::cout << "Price, after discounting: " << price << ", " << std::endl;
	std::cout << "Number of times origin is hit: " << coun << endl;


	boost::tuple<double,double> Results;
	Results = StDevStErr(v, myOption.r, myOption.T);
	std::cout << "The standard deviation is " << Results.get<0>() << endl;	//Print Standard Dev
	std::cout << "The standard error is " << Results.get<1>() << endl;		//Print Standard Error

	return 0;
}