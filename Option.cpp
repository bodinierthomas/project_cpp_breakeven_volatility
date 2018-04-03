#include "Option.h"


Option::Option(const std::tm pi_writing_date, const std::tm pi_exercise_date, const time_series pi_time_series, 
			   const double pi_rate, const double pi_strike, const eOptionType pi_optiontype, const PayOff* pi_payoff)   
	
	:m_writing_date(pi_writing_date),
	m_exercise_date(pi_exercise_date),
	m_time_series(pi_time_series),
	m_rate(pi_rate),
	m_strike(pi_strike),
	m_optiontype(pi_optiontype),
	m_payoff(pi_payoff)
{
	//constructor
}

Option::~Option()
{
	//destructor
}


const std::tm Option::getWriting_date()  const
{
	return m_writing_date;
}

const std::tm Option::getExercise_date() const
{
	return m_exercise_date;
}

const time_series Option::getTime_series() const
{
	return m_time_series;
}


const double Option::getMaturity() const  //in years
{
	double K=delta_date(m_exercise_date, m_writing_date);
	return K;
}

const double Option::getRate() const
{
	return m_rate;
}



const double Option::getStrike() const
{
	return m_strike;
}



const Option::eOptionType  Option::getOptionType() const
{
	return m_optiontype;
}

const double  Option::getPayoff(double pi_underlying_at_maturity) const
{
	if (m_optiontype == eOptionType::Call)
	{
		PayOffCall payoff = PayOffCall(m_strike);
		return payoff(pi_underlying_at_maturity);
	}
	else
	{
		PayOffPut payoff = PayOffPut(m_strike);
		return payoff(pi_underlying_at_maturity);		
	}

	
}


void Option::setRate(const double pi_rate)
{
	m_rate = pi_rate;
}

void Option::setStrike(const double pi_strike)
{
	m_strike = pi_strike;

}

double Option::getUnderlying_Price() 
{
	return m_time_series.getPrice(m_time_series.getIterator(m_writing_date));
}

double Option::getOption_price(const double pi_vol ) 
{
	if (m_optiontype == eOptionType::Call) 
	{
		return BSCallPrice(pi_vol, m_rate, m_strike, m_time_series.getPrice(m_time_series.getIterator(m_writing_date)), Option::getMaturity());
	}
	else if (m_optiontype == eOptionType::Put)
	{
		return BSPutPrice(pi_vol, m_rate, m_strike, m_time_series.getPrice(m_time_series.getIterator(m_writing_date)), Option::getMaturity());
	}
	else
	{
		std::cout << "error, wrong type of option";
		return 0.0;
	}
}

double Option::getDelta(const double pi_vol) 
{
	const double d1 = (log(m_time_series.getPrice(m_time_series.getIterator(m_writing_date)) / m_strike) + (m_rate +pi_vol*pi_vol / 2)*Option::getMaturity()) / (pi_vol*sqrt(Option::getMaturity()));
	if (m_optiontype == eOptionType::Call)
	{
		return cumulative_normal_distribution_function(d1);
	}
	else if (m_optiontype == eOptionType::Put)
	{
		return cumulative_normal_distribution_function(d1)- 1.0 ;
	}
	else
	{
		std::cout << "error, wrong type of option";
		return 0.0;
	}
}

double Option::getGamma(const double pi_vol)
{
	const double d1 = (log(m_time_series.getPrice(m_time_series.getIterator(m_writing_date)) / m_strike) + (m_rate + pi_vol*pi_vol / 2)*Option::getMaturity()) / (pi_vol*sqrt(Option::getMaturity()));

	return probability_normal_density_function(d1) / (m_time_series.getPrice(m_time_series.getIterator(m_writing_date))*pi_vol*sqrt(Option::getMaturity()));
}

double Option::getVega(const double pi_vol) 
{
	const double d1 = (log(m_time_series.getPrice(m_time_series.getIterator(m_writing_date)) / m_strike) + (m_rate + pi_vol*pi_vol / 2)*Option::getMaturity()) / (pi_vol*sqrt(Option::getMaturity()));

	return m_time_series.getPrice(m_time_series.getIterator(m_writing_date))*sqrt(Option::getMaturity())*probability_normal_density_function(d1)/100;
}

double Option::getPnL(const double pi_vol)
{
	double pnl = Option::getOption_price(pi_vol);
	double delta = Option::getDelta(pi_vol);
	double risk_free = pnl - delta*Option::getUnderlying_Price();

	size_t iterator_writing_date = m_time_series.getIterator(m_writing_date);
	size_t iterator_exercise_date = m_time_series.getIterator(m_exercise_date);

	for (size_t ite = iterator_writing_date + 1; ite <= iterator_exercise_date ; ite++)
	{
		pnl += delta*(m_time_series.getPrice(ite) - m_time_series.getPrice(ite - 1)) + risk_free*(exp(m_rate*delta_date(m_time_series.getDate(ite),m_time_series.getDate(ite - 1) ))-1);

		Option option(m_time_series.getDate(ite), m_exercise_date, m_time_series,  m_rate, m_strike, m_optiontype, m_payoff);
		
		if (ite <iterator_exercise_date)
		{
			delta = option.getDelta(pi_vol);
		}
		risk_free = pnl - delta*option.getUnderlying_Price();
	}


	double c= Option::getPayoff(m_time_series.getPrice(m_time_series.getIterator(m_exercise_date)));
	return pnl - c;
	
}

 double Option::getRobustPnL(const double pi_vol)
{
	 size_t iterator_writing_date = m_time_series.getIterator(m_writing_date);
	 size_t iterator_exercise_date = m_time_series.getIterator(m_exercise_date);

	 double pnl = 0.0;

	 double gamma = Option::getGamma(pi_vol);
	 double underlying = Option::getUnderlying_Price();

	 for (size_t ite = iterator_writing_date + 1; ite <= iterator_exercise_date; ite++)
	 {
		 Option option(m_time_series.getDate(ite), m_exercise_date, m_time_series, m_rate, m_strike, m_optiontype, m_payoff);

		 double underlying_2 = option.getUnderlying_Price();

		 double ds = (underlying_2 - underlying) / underlying;

		 pnl += gamma*underlying*underlying*(ds*ds - pi_vol*pi_vol*delta_date(m_time_series.getDate(ite), m_time_series.getDate(ite - 1)));

		 if (ite < iterator_exercise_date)
		 {
			 gamma = option.getGamma(pi_vol);
		 }
		 underlying = underlying_2;
	 }
	 return pnl;
}




double BSCallPrice(const double pi_vol, const  double pi_rate, const double pi_strike, const  double  pi_price, const double pi_T)

{
	double prob1;
	double prob2;
	double d1, d2;
	double callprice;


	double a = log(pi_price / pi_strike);
	double b= (pi_rate + pi_vol*pi_vol / 2)*pi_T ;
	d1 = (a + b) / (pi_vol*sqrt(pi_T));
	d2 = d1 - pi_vol*sqrt(pi_T);
	prob1 = cumulative_normal_distribution_function(d1);
	prob2 = cumulative_normal_distribution_function(d2);

	callprice = pi_price*prob1 - pi_strike*exp(-pi_rate*pi_T)*prob2;

	return callprice;

}

double BSPutPrice(const double pi_vol, const  double pi_rate, const double pi_strike, const  double  pi_price, const double pi_T)

{
	double prob1;
	double prob2;
	double d1, d2;
	double putprice;

	d1 = (log(pi_price / pi_strike) + (pi_rate + pi_vol*pi_vol / 2)*pi_T) / (pi_vol*sqrt(pi_T));
	d2 = d1 - pi_vol*sqrt(pi_T);
	prob1 = cumulative_normal_distribution_function(-d1);
	prob2 = cumulative_normal_distribution_function(-d2);

	putprice = pi_strike*exp(-pi_rate*pi_T)*prob2 - pi_price*prob1;

	return putprice;

}


class PnL_Functor : public Functor
{
public:
	PnL_Functor(Option pi_option)

		:m_option(pi_option)
	{

	}



	double operator() (double pi_vol)
	{

		
	
		double functorValue =  m_option.getPnL(pi_vol);

		return functorValue;
	}

private:

	Option m_option;

};

class Robust_PnL_Functor : public Functor
{
public:
	Robust_PnL_Functor(Option pi_option)

		:m_option(pi_option)
	{

	}



	double operator() (double pi_vol)
	{



		double functorValue = m_option.getRobustPnL(pi_vol);

		return functorValue;
	}

private:

	Option m_option;

};



 std::vector <double> Zero_PnL(const double pi_vol_min, const double pi_vol_max, const  double pi_tol, const  Option pi_option)
{

	PnL_Functor myFunctor(pi_option);

	//template <class Functor>
	double x_root = 0.0;
	double f_at_x_root = 0.0;
	double x_root_2 = 0.0;
	double f_at_x_root_2 = 0.0;
	long iterationNumber = 0;
	bool isOK = zbrent(myFunctor, pi_vol_min, pi_vol_max, pi_tol,
		x_root, f_at_x_root,
		x_root_2, f_at_x_root_2, iterationNumber);



	return{ x_root,f_at_x_root,x_root_2,f_at_x_root_2,double(iterationNumber) };

}

 std::vector <double> Zero_Robust_PnL(const double pi_vol_min, const double pi_vol_max, const  double pi_tol, const  Option pi_option)
 {

	 Robust_PnL_Functor myFunctor(pi_option);

	 //template <class Functor>
	 double x_root = 0.0;
	 double f_at_x_root = 0.0;
	 double x_root_2 = 0.0;
	 double f_at_x_root_2 = 0.0;
	 long iterationNumber = 0;
	 bool isOK = zbrent(myFunctor, pi_vol_min, pi_vol_max, pi_tol,
		 x_root, f_at_x_root,
		 x_root_2, f_at_x_root_2, iterationNumber);



	 return{ x_root,f_at_x_root,x_root_2,f_at_x_root_2,double(iterationNumber) };

 }
