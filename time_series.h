#ifndef _DATE_AND_PRICE_H
#define _DATE_AND_PRICE_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iterator>
#include <ctime>
#include <iomanip>
#include <tuple>


class  time_series
{
public:
	

	time_series(std::ifstream& pi_csv_file);
	~time_series();

	const std::size_t getSize() const;
	const std::tm getDate(size_t pi_iterator) const;
	const double getPrice(size_t pi_iterator) const;
	const std::vector<std::tm> getDates() const;
	const std::vector<double> getPrices() const;
	const size_t getIterator(const std::tm pi_day) const;

	void setDate(const size_t iterator, const std::tm pi_date);
	void setPrice(const size_t iterator, const  double pi_price);
	void setDates(const std::vector<std::tm> pi_dates);
	void setPrices(const std::vector<double> pi_prices);


private:
	std::vector<std::tm> m_dates;
	std::vector<double> m_prices;


};

const std::tm string_to_date(const  std::string pi_stringdate);
const std::string date_to_string(const std::tm pi_date);

const double delta_date(const  std::tm pi_date_end, const std::tm pi_date_beginning);

bool operator==(const std::tm date_1, const  std::tm pi_date_2);
bool operator<( std::tm pi_date_1,  std::tm pi_date_2);
bool operator>( std::tm pi_date_1,  std::tm pi_date_2);
bool operator<=( std::tm pi_date_1,  std::tm pi_date_2);
bool operator>=( std::tm pi_date_1,  std::tm pi_date_2);

#endif