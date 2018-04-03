#include "write_vector.h"
#include "Option.h"
#include "time_series.h"


void writeVector(const std::string& pi_name, const std::vector< double >& pi_values, std::ofstream& pio_stream)
{
	pio_stream << pi_name.c_str() << ";";
	for (auto strikeIterator = pi_values.cbegin(); strikeIterator != pi_values.cend(); ++strikeIterator)
	{
		pio_stream << (*strikeIterator) << ";";
	}
	pio_stream << std::endl;
}

void writeResults(const std::vector< double >& pi_strikes, const std::vector< double >& pi_strikes_in_percentages, const std::vector< double >& pi_vol_from_calls,
	const std::vector< double >& pi_vol_from_puts, const std::vector< double >& pi_vol_robust_from_calls,
	const std::vector< double >& pi_vol_robust_from_puts, const double pi_rate, const std::tm pi_writing_date, const std::tm pi_exercise_date,
	const double pi_vol_min, const double pi_vol_max, const double pi_tol, std::ofstream& pio_stream)
{
	
	pio_stream.precision(8);

	writeVector("Strikes", pi_strikes, pio_stream);
	writeVector("Strikes in % of underlying", pi_strikes_in_percentages, pio_stream);
	pio_stream << std::endl;

	writeVector("Volatility from calls", pi_vol_from_calls, pio_stream);
	pio_stream << std::endl;
	writeVector("Volatility from puts", pi_vol_from_puts, pio_stream);
	pio_stream << std::endl;
	pio_stream << std::endl;
	writeVector("Volatility robust from calls", pi_vol_robust_from_calls, pio_stream);
	pio_stream << std::endl;
	writeVector("Volatility robust from puts", pi_vol_robust_from_puts, pio_stream);
	pio_stream << std::endl;
	pio_stream << std::endl;

	pio_stream << "Parameters" << std::endl;
	pio_stream << std::endl;

	pio_stream << "Rate; " << pi_rate << std::endl;
	pio_stream << "Writing Date; " << date_to_string(pi_writing_date) << std::endl;
	pio_stream << "Exercise Date; " << date_to_string(pi_exercise_date) << std::endl;


	pio_stream << std::endl;
	pio_stream << std::endl;

	pio_stream << "Vol min for the brent algo; " << pi_vol_min << std::endl;
	pio_stream << "Vol max for the brent algo; " << pi_vol_max << std::endl;
	pio_stream << "Tolerance for the brent algo; " << pi_tol << std::endl;
	
}