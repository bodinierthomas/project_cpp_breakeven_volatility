#pragma once
#include <math.h>
#ifdef SIGN
#undef SIGN
#endif

class Functor {
public:
	virtual double operator() (double) = 0;
};

inline double SIGN(const double &a, const double &b)
{
	return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);
}



template <class Functor>
bool zbrent(Functor& pi_functor_f, const double pi_x1, const double pi_x2, const double pi_f_tolerance,
	double& po_x_root, double& po_f_at_x_root,
	double& po_x_root2, double& po_f_at_x_root2, long& po_iterationsNumber)
	//Using Brent's method, find the root of a function func known to lie between x1 and x2. The
	//root, returned as zbrent, will be refined until its accuracy is pi_f_tolerance.
{
	static const unsigned long BRENT_ITMAX = 100;
	static const double BRENT_EPS = 1.0e-15; // Machine floating-point precision.

	po_iterationsNumber = 0;
	int iter;
	double a = pi_x1, b = pi_x2, c = pi_x2, d = 0., e = 0., min1, min2;
	double fa = pi_functor_f(a), fb = pi_functor_f(b), fc, p, q, r, s, tol1, xm;
	if ((fa > 0.0 && fb > 0.0) || (fa < 0.0 && fb < 0.0))
	{
		return false; //Root must be bracketed in zbrent
	}
	fc = fb;
	for (iter = 1; iter <= BRENT_ITMAX; iter++)
	{
		if ((fb > 0.0 && fc > 0.0) || (fb < 0.0 && fc < 0.0))
		{
			c = a; //Rename a, b, c and adjust bounding interval d.
			fc = fa;
			e = d = b - a;
		}
		if (fabs(fc) < fabs(fb))
		{
			a = b;
			b = c;
			c = a;
			fa = fb;
			fb = fc;
			fc = fa;
		}
		tol1 = 2.0*BRENT_EPS*fabs(b) + 0.5*pi_f_tolerance; //Convergence check.
		xm = 0.5*(c - b);
		if (fabs(xm) <= tol1 || fb == 0.0)
		{
			po_x_root = b;
			po_f_at_x_root = fb;
			po_x_root2 = c;
			po_f_at_x_root2 = fc;
			po_iterationsNumber = iter;
			return true;
		}
		if (fabs(e) >= tol1 && fabs(fa) > fabs(fb))
		{
			s = fb / fa; //Attempt inverse quadratic interpolation.
			if (a == c)
			{
				p = 2.0*xm*s;
				q = 1.0 - s;
			}
			else
			{
				q = fa / fc;
				r = fb / fc;
				p = s*(2.0*xm*q*(q - r) - (b - a)*(r - 1.0));
				q = (q - 1.0)*(r - 1.0)*(s - 1.0);
			}
			if (p > 0.0) q = -q; //Check whether in bounds.
			p = fabs(p);
			min1 = 3.0*xm*q - fabs(tol1*q);
			min2 = fabs(e*q);
			if (2.0*p < (min1 < min2 ? min1 : min2)) {
				e = d; //Accept interpolation.
				d = p / q;
			}
			else
			{
				d = xm; //Interpolation failed, use bisection.
				e = d;
			}
		}
		else
		{ //Bounds decreasing too slowly, use bisection.
			d = xm;
			e = d;
		}
		a = b; //Move last best guess to a.
		fa = fb;
		if (fabs(d) > tol1) //Evaluate new trial root.
			b += d;
		else
			b += SIGN(tol1, xm);
		fb = pi_functor_f(b);
	}
	po_iterationsNumber = iter;
	return false; //Maximum number of iterations exceeded in zbrent
}