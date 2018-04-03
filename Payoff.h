#ifndef __PAY_OFF_HPP
#define __PAY_OFF_HPP

#include <algorithm> //for std::max 


class PayOff {
public:
	PayOff(); // Default (no parameter) constructor
	virtual ~PayOff() {}; // Virtual destructor

						  // Overloaded () operator, turns the PayOff into an abstract function object
	virtual double operator() (const double& S) const = 0;
};

class PayOffCall : public PayOff {
private:
	double m_K; // Strike price

public:
	PayOffCall(const double& pi_K);
	virtual ~PayOffCall() {};


	virtual double operator() (const double& S) const;			// Virtual function is over-ridden
};

class PayOffPut : public PayOff {
private:
	double m_K; // Strike

public:
	PayOffPut(const double& pi_K);
	virtual ~PayOffPut() {};
	virtual double operator() (const double& S) const;			// Virtual function is over-ridden
};

#endif