#pragma once


#include <string>
#include <iostream>
#include <vector>
#include <fstream>

void writeVector(const std::string & pi_name, std::vector<double>& pi_values, std::ofstream & pio_stream);

void writeResults(const std::vector< double >& pi_strikes, const std::vector< double >& pi_strikes_in_percentages, const std::vector< double >& pi_vol_from_calls,
	const std::vector< double >& pi_vol_from_puts, const std::vector< double >& pi_vol_robust_from_calls,
	const std::vector< double >& pi_vol_robust_from_puts, const double pi_rate, const std::tm pi_writing_date, const std::tm pi_exercise_date,
	const double pi_vol_min, const double pi_vol_max, const double pi_tol, std::ofstream& pio_stream);
