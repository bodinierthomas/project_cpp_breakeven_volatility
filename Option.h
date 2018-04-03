#ifndef _OPTION_H
#define _OPTION_H

#include "Payoff.h"
#include "cumulative_distribution_function.h"
#include "time_series.h"
#include "brent.h"


class Option							   // generic plain-vanilla option 
{
public:

	enum class eOptionType					// 2 types possible : European Call or European Put
	{
		Call,
		Put,
	};
	

	Option(const std::tm pi_writing_date, const std::tm pi_exercise_date, const time_series pi_time_series, const double pi_rate,
		   const double pi_strike, const eOptionType pi_optiontype, const PayOff* pi_payoff);			//Plain-Vanilla Option constructor
	~Option();

	const  std::tm getWriting_date() const;
	const  std::tm getExercise_date() const;
	const  time_series getTime_series() const;
	const  double getMaturity() const;
	const  double getStrike() const;
	const  double getRate() const;
	const  eOptionType getOptionType() const;
	const  double getPayoff(const double pi_underlying_at_maturity) const;



	void setRate(const double pi_rate);
	void setStrike(const double pi_strike);

	double getUnderlying_Price();
	double getOption_price(const double pi_vol) ;
	double getDelta(const double pi_vol) ;
	double getGamma(const double pi_vol) ;
	double getVega(const double pi_vol) ;

	double getPnL(const double vol);
    double getRobustPnL(const double pi_vol);

	
	




private:

	const std::tm m_writing_date;
	const std::tm m_exercise_date;
	double m_strike;
	time_series m_time_series;
	double m_rate;
	const eOptionType m_optiontype;
	const PayOff* m_payoff;

};


double BSCallPrice(const double pi_vol, const double pi_rate, const  double pi_strike, const double  pi_price, const double pi_T);
double BSPutPrice(const double pi_vol, const  double pi_rate, const  double pi_strike, const double  pi_price, const double pi_T);

std::vector<double> Zero_PnL(const double pi_vol_min, const double pi_vol_max,const  double pi_tol, const  Option pi_option);
std::vector <double> Zero_Robust_PnL(const double pi_vol_min, const double pi_vol_max, const  double pi_tol, const  Option pi_option);

#endif