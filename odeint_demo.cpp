#define _CRT_SECURE_NO_WARNINGS
/*
 Copyright 2010-2012 Karsten Ahnert
 Copyright 2011-2013 Mario Mulansky
 Copyright 2013 Pascal Germroth
 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <iostream>
#include <vector>

#include <boost/numeric/odeint.hpp>


 //[ rhs_function
// The type of container used to hold the state vector */
typedef std::vector<double> state_type;

const double gam = 0.15;

double gamma_x(double var) {
    return gam * var;
}

// The rhs of x' = f(x) 
void harmonic_oscillator(const state_type& x, state_type& dxdt, const double t )
{
    dxdt[0] = x[1];
    //dxdt[1] = -x[0] - gam*x[1];
    dxdt[1] = -x[0] - gamma_x(x[1]);
}
//]

// The rhs of x' = f(x) 
void minus_harmonic_oscillator(const state_type& x, state_type& dxdt, const double t)
{
    dxdt[0] = -x[1];
    dxdt[1] = -x[0] - gam * x[1];
}
//]

class harm_osc {

    double m_gam;

public:
    harm_osc(double gam) : m_gam(gam) { }

    double gamma_x(double var) {
        if (var > 0) {
            return m_gam * var;
        }
        else {
            return -m_gam * var;
        }
    }

    void operator() (const state_type& x, state_type& dxdt, const double /* t */)
    {
        dxdt[0] = x[1];
        dxdt[1] = -x[0] - gamma_x(x[1]);
    }
};

int main2()
{
    using namespace std;
    using namespace boost::numeric::odeint;

    //[ state_initialization
    state_type x(2);
    x[0] = 1.0; // start at x=1.0, p=0.0
    x[1] = 0.0;
    //]

    harm_osc ho(0.15);

    //[ define_const_stepper
    runge_kutta4<state_type> stepper;

    const double dt = 0.01;
    for (double t = 0.0; t < 10.0; t += dt) {
        if (x[0] < 0) {
            stepper.do_step(ho, x, t, dt);
        }
        else {
            stepper.do_step(ho, x, t, dt);
        }
    }
    //]

    cout << x[0] << "\t" << x[1];
    return 0;
}