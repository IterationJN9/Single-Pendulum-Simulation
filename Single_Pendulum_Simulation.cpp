/*This C++ program is to simulate the movement of a single pendulum using Euler's method and
Runge-Kutta 4th Order.  It prints out the time, angle in radians, omega, and potential energy
at each time step and writes the results to .dat files.*/

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

//Declare the variables that will remain constant.
//Acceleration due to gravity.
const double g = 9.81;

//Length of pendulum rod in meters.
const double len = 1.0;

//Mass of pendulum bob in grams.
const double mass = 1.0;

//Create data structure Point to represent location of the pendulum.
struct Point
{
    //Time.
    double t;
    
    //Theta is the angle
    double theta;
    
    //Omega is
    double omega;
    
    //Potential energy of the pendulum
    double pe;
};

void EulerStep(Point& p, double h)
{
    double t = p.t;
    double theta = p.theta;
    double omega = p.omega;
    double pe = p.pe;
    
    //Placeholder for previous omega.
    double tempOmega = omega;
	
	//Potential energy = mass * gravity * height
    //height = 1 - cos(theta)
    pe = mass * g * (1 - cos(theta));
    
    //Use Euler's method.  Note that h is the time step.
	//y = y + h * f(x,y)
	
    omega = omega - h * (g / len) * sin(theta);
	
	//Calculating the next theta uses the 'current' omega, not the new one.
    theta = theta + h * tempOmega;
	
    t += h;
    
    p.t = t;
    p.theta = theta;
    p.omega = omega;
    p.pe = pe;
}

void RungeKutta(Point& p, double h)
{
	double t = p.t;
	double theta = p.theta;
	double omega = p.omega;
	double pe = p.pe;
	
	//Declare the ks.
	double k1a, k2a, k3a, k4a;
	double k1b, k2b, k3b, k4b;
	
	//Placeholder for previous values.
	double tempOmega = omega;
	
	//Potential energy = mass * gravity * height
    //height = 1 - cos(theta)
    pe = mass * g * (1 - cos(theta));
	
	//Use Runge-Kutta 4th Order method for theta.
	k1a = h * omega;
	k2a = h * (omega + k1a) / 2;
	k3a = h * (omega + k2a) / 2;
	k4a = h * (omega + k3a);
	
	theta = theta + (k1a + 2 * k2a + 2 * k3a + k4a) / 6;
	
	//Use Runge-Kutta 4th Order method for omega.
	k1b = h * (-g/len)*sin(theta);
	k2b = h * (-g/len)*sin(theta + (k1b)/2);
	k3b = h * (-g/len)*sin(theta + (k2b)/2);
	k4b = h * (-g/len)*sin(theta + k3b);
	
	omega = omega + (k1b + 2 * k2b + 2 * k3b + k4b) / 6;
	
	t += h;
	
	p.t = t;
	p.theta = theta;
	p.omega = omega;
	p.pe = pe;
	
}

//Simulate runs the selected method given the time step and maximum time.
void simulate(void method(Point&, double), double tMax, double h, string simData)
{
    //Initialize.
    Point p;
    p.t = 0;
    p.theta = 0;
    p.omega = 1;
    p.pe = 0;
    ofstream file(simData.c_str());
    
	//Print the column titles
	printf("Time\t\t\tTheta\t\t\tOmega\t\t\tPotential Energy\n");
	
    //Algorithm loop.
    while(p.t <= tMax)
    {
        method(p, h);
        file << p.t << '\t' << p.theta << '\t' << p.omega << '\t' << p.pe << '\n';
		printf("%.2lf\t\t\t%.4lf\t\t\t%.4lf\t\t\t%.4lf\n", p.t, p.theta, p.omega, p.pe);
    }
    
    //Finalize and close the file.
    cout << "Trajectory data in file " << simData << endl;
    file.close();
    
}

int main()
{
    cout << "Simulation of Simple Pendulum using Euler and Runge-Kutta 4th Order\n"
         << "-----------------------------------------\n";
         
    double tMax;
    double h;
    
	//Ask the user for input for the time step and maximum time.
    cout << "Enter time step h (s): ";
    cin >> h;
    cout << "Enter maximum time tMax ";
    cin >> tMax;
    
    cout << "Simulation with Euler method" << endl;
    simulate(EulerStep, tMax, h, "eulerDAT.dat\n");
	
	cout << "Simulation with Runge-Kutta 4th Order" << endl;
	simulate(RungeKutta, tMax, h, "RK4DAT.dat");
}