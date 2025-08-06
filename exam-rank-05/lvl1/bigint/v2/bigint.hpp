#ifndef BIGINT_HPP
#define BIGINT_HPP

#include <iostream>
#include <algorithm>
#include <string>

class bigint {
	private:
		std::string _digits;

	public:
		bigint(void);
		bigint(unsigned int value);
		bigint(const std::string& value);
		bigint(const bigint& other);
		~bigint(void);

		bigint&	operator=(const bigint& other);

		// arithmetic operators
		bigint	operator+(const bigint& other) const;
		bigint&	operator+=(const bigint& other);

		// increment operators
		bigint&	operator++(void);
		bigint	operator++(int);

		// comparison operators
		bool	operator>(const bigint& other) const;
		bool	operator<(const bigint& other) const;
		bool	operator>=(const bigint& other) const;
		bool	operator<=(const bigint& other) const;
		bool	operator==(const bigint& other) const;
		bool	operator!=(const bigint& other) const;

		// shift operators
		bigint	operator<<(int i) const;
		bigint	operator>>(int i) const;
		bigint&	operator<<=(int i);
		bigint&	operator>>=(int i);
		bigint&	operator<<=(const bigint& other);
		bigint&	operator>>=(const bigint& other);

		// getter
		const std::string&	getDigits(void) const;
};

std::ostream&	operator<<(std::ostream& out, const bigint& num);

#endif
