#include "time_series.h"
#include "brent.h"
#include "Option.h"
#include "write_vector.h"


int main(int argc, char* argv[])
{
	//opening csv + loading the data into a time serie

	std::cout << "Opening file SP500.csv\n";
	std::ifstream file("SP500.csv");
	file.ignore(256, '\n');  // ignore first line of csv.file which contains heads of the columns

	time_series ts= time_series(file);
	file.close();

	//show the data on the console

	std::cout << "We have the following csv file";
	for (size_t i = 0; i < ts.getSize(); i++)
	{	
		std::cout << "the price was of " << ts.getPrice(i) << "\n";
		std::cout << "on the " << date_to_string(ts.getDate(i)) << "\n";
	}


	std::tm date_beginning = ts.getDate(0);
	std::tm date_end = ts.getDate(ts.getSize()-1);

	std::cout << "the date of beginning of the file is " << date_to_string(date_beginning) << "\n";
	std::cout <<"the end date of the file is " << date_to_string(date_end) << "\n";


	// computation of the smile

	const double price_at_day_0 = ts.getPrice(0);  //price of S&P500 on January 3rd 2017

	const double rate =  0.01;  // on the 3rd Januar 2017


	const double lowerStrike = 0.5;			 // % of the price at the time of the writing of the option								
	const double upperStrike = 1.5;		     // % of the price at the time of the writing of the option	
	const unsigned long strikesNumber = 101;	     //number of strikes

	const double lower_bound_vol = 0.01;			//parameters for the brent algorithm
	const double upper_bound_vol = 1.5;
	const double tol = 0.00000001;

	std::vector< double > strikes(strikesNumber);
	std::vector< double > strikes_in_percentages(strikesNumber);
	std::vector< double > vol_from_calls(strikesNumber);
	std::vector< double > vol_from_puts(strikesNumber);
	std::vector< double > vol_robust_from_calls(strikesNumber);
	std::vector< double > vol_robust_from_puts(strikesNumber);

	for (unsigned long strikeIndex = 0u; strikeIndex < strikesNumber; ++strikeIndex)
	{
		const double strike_in_percentage = 100.0*(lowerStrike + (upperStrike - lowerStrike) * strikeIndex / (strikesNumber-1));
		const double strike = strike_in_percentage*price_at_day_0/100.0;

		strikes_in_percentages.at(strikeIndex) = strike_in_percentage;
		strikes.at(strikeIndex) = strike;

		PayOffCall* payoff_call = new PayOffCall(strike);
		PayOffPut* payoff_put = new PayOffPut(strike);

		Option call = Option(date_beginning, date_end, ts, rate, strike, Option::eOptionType::Call, payoff_call);
		Option put = Option(date_beginning, date_end, ts, rate, strike, Option::eOptionType::Put, payoff_put);

		vol_from_calls.at(strikeIndex) = Zero_PnL(lower_bound_vol, upper_bound_vol, tol, call)[0];
		vol_from_puts.at(strikeIndex) = Zero_PnL(lower_bound_vol, upper_bound_vol, tol, put)[0];

		vol_robust_from_calls.at(strikeIndex) = Zero_Robust_PnL(lower_bound_vol, upper_bound_vol, tol, call)[0];
		vol_robust_from_puts.at(strikeIndex) = Zero_Robust_PnL(lower_bound_vol, upper_bound_vol, tol, put)[0];


		delete payoff_call;
		delete payoff_put;
	}

	std::ofstream myFileStream("C:\\temp\\breakeven_volatility.csv");				//you need to have a file named temp on the C disk


	writeResults(strikes, strikes_in_percentages, vol_from_calls,
				 vol_from_puts, vol_robust_from_calls, vol_robust_from_puts, rate, date_beginning, date_end,
				 lower_bound_vol, upper_bound_vol, tol, myFileStream);
	

	
	std::cout << "The End,  that's all folks!" << std::endl;
	return 0;
}

