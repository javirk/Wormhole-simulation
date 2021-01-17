#ifndef EVOLUTION_H
#define EVOLUTION_H

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include "variables.h"
#include "utils.h"

#include <boost/numeric/odeint.hpp>
typedef std::vector<double> state_type;

class evolution {

	double b, B2, b2;

	public:
		evolution(double initial_b, double initial_B2) : b(initial_b), B2(initial_B2) {
			b2 = b * b;
		}

		static double r(double l) {
			double absl = abs(l);
			if (absl > par_a) {
				auto tmp = 2 * (absl - par_a) / (pi * par_M);
				return par_rho + par_M * (tmp * atan(tmp) - 0.5 * log(1 + tmp * tmp));
			}
			else {
				return par_rho;
			}
		}

		double drdl(double l) {
			double absl = abs(l);
			if (absl > par_a) {
				return (2 * l / (pi * absl)) * atan(2 * (absl - par_a) / (pi * par_M));
			}
			else {
				return 0.;
			}
		}

		void operator() (const state_type& x, state_type& dxdt, const double /* t */)
		{
			dxdt[0] = x[3];
			dxdt[1] = x[4] / pow(r(x[0]), 2);
			dxdt[2] = b / (pow(r(x[0]), 2) * sin(x[1]) * sin(x[1]));
			dxdt[3] = B2 * drdl(x[0]) / pow(r(x[0]), 3);
			dxdt[4] = b2 / pow(r(x[0]), 2) * (cos(x[1]) / pow(sin(x[1]), 3));
		}

};


#endif