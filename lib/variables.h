#ifndef VARIABLES_H
#define VARIABLES_H

double par_rho = 1;
double par_W = 0.05 * par_rho;
double par_M = par_W / 1.42953; // TODO: Change to the theoretical number
double par_a = 0.01 / 2 * par_rho;
double par_lc = 6.26 * par_rho + par_a;

int n_samples = 1;

unsigned char* saturnImageData, * gargantuaImageData;
int width_saturn, height_saturn, components;
int width_gargantua, height_gargantua;

#endif // !VARIABLES_H


