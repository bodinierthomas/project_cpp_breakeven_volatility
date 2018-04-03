#include "time_series.h"


time_series::time_series(std::ifstream& pi_csv_file)
{
	std::vector<std::tm> dates;
	std::vector<double> prices;

	if (!pi_csv_file.is_open())
	{
		std::cout << "Could not open file.\n";
	}


	std::string line;
	while (std::getline(pi_csv_file, line)) // read the current line
	{
		std::istringstream iss{ line }; // construct a string stream from line

										// read the tokens from current line separated by comma
		std::vector<std::string> tokens; // here we store the tokens
		std::string token; // current token

		while (std::getline(iss, token, ';'))
		{
			tokens.push_back(token); // add the token to the vector
		}


		// map the tokens into our variables
		const std::tm  date = string_to_date(tokens[0]); // first is a date, convert it with the function string_to_date 
		double price = std::stod(tokens[1]); // second is a double, convert it with stod

		
		dates.push_back(date);
		prices.push_back(price);
	}
	
	time_series::setDates(dates);
	time_series::setPrices(prices);
	
}



time_series::~time_series()
{
}

const std::size_t  time_series::getSize() const
{
	return m_prices.size();
}

const std::tm  time_series::getDate(size_t pi_iterator) const
{
	return m_dates[pi_iterator];
}

const double  time_series::getPrice(size_t pi_iterator) const
{
	return m_prices[pi_iterator];
}

const std::vector<std::tm>  time_series::getDates() const
{
	return m_dates;
}

const std::vector<double>  time_series::getPrices() const
{
	return m_prices;
}

const size_t time_series::getIterator(const std::tm pi_day) const
{
	if (m_dates[0] == pi_day)
	{
		return size_t(0);
	}
	else 
	{
		size_t ite = 0;
		while (ite < time_series::getSize())
		{
			if ((m_dates[ite] == pi_day))
			{
				return ite;
			}

			else
			{
				ite++;
			}
		}


		std::cout << "Error: date not found" << std::endl;
		return 0;
	}
}

void  time_series::setDate(const size_t pi_iterator, const std::tm pi_date)
{
	m_dates[pi_iterator] = pi_date;
}

void  time_series::setPrice(const size_t pi_iterator, const  double pi_price)
{
	m_prices[pi_iterator] = pi_price;
}

void  time_series::setDates(const std::vector<std::tm> pi_dates)
{
	m_dates = pi_dates;
}

void  time_series::setPrices(const std::vector<double> pi_prices)
{
	m_prices = pi_prices;
}




const std::tm string_to_date(const std::string pi_stringdate)
{
	std::tm date;

	std::istringstream datestream(pi_stringdate);
	datestream >> std::get_time(&date, "%d/%m/%Y");

	date.tm_hour = 0;

	date.tm_min = 0;

	date.tm_sec = 0;

	return date;
}

const std::string date_to_string(const std::tm pi_date)
{
	std::string date = std::to_string(pi_date.tm_mday) + "/" + std::to_string(pi_date.tm_mon +1) + "/" + std::to_string(1900 + pi_date.tm_year);
	return date;
}

const double delta_date( std::tm  pi_date_end, std::tm  pi_date_beginning) // in seconds
{
	return difftime(std::mktime(&pi_date_end), std::mktime(&pi_date_beginning))/3600.0/24.0/365.0;
}

bool operator==(const std::tm pi_date_1, const std::tm  pi_date_2)
{
	// We cannot use delta_date because an equality on doubles is not possible, we will always have an epsilon error (equality on ints is ok though)

	return std::tie(pi_date_1.tm_year, pi_date_1.tm_mon, pi_date_1.tm_mday) == std::tie(pi_date_2.tm_year, pi_date_2.tm_mon, pi_date_2.tm_mday);
}

bool operator<(std::tm  pi_date_1, std::tm pi_date_2)
{
	return (delta_date(pi_date_1, pi_date_2) < 0);
}

bool operator>(std::tm pi_date_1, std::tm  pi_date_2)
{
	return (delta_date(pi_date_1, pi_date_2) > 0);
}

bool operator<=(std::tm pi_date_1, std::tm  pi_date_2)
{
	return (delta_date(pi_date_1, pi_date_2) <= 0);
}

bool operator>=(std::tm  pi_date_1, std::tm pi_date_2)
{
	return (delta_date(pi_date_1, pi_date_2) >= 0);
}