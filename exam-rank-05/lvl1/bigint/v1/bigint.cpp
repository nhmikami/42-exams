#include "bigint.hpp"

// constructors and destructor
bigint::bigint(void) {
	_digits.push_back(0);
}

bigint::bigint(unsigned int value) {
	if (value == 0)
		_digits.push_back(0);
	else {
		while (value > 0) {
			_digits.push_back(value % 10);
			value /= 10;
		}
	}
}

bigint::bigint(const bigint& other) : _digits(other._digits) {
}

bigint::~bigint(void) {
}

// assignment operator
bigint& bigint::operator=(const bigint& other) {
	if (this != &other) {
		_digits = other._digits;
	}
	return *this;
}

// arithmetic operators
bigint bigint::operator+(const bigint& other) const {
	bigint			result;
	size_t			maxSize = std::max(_digits.size(), other._digits.size());
	unsigned int	carry = 0;

	result._digits.clear();
	for (size_t i = 0; i < maxSize || carry; ++i) {
		unsigned int digit1 = (i < _digits.size()) ? _digits[i] : 0;
		unsigned int digit2 = (i < other._digits.size()) ? other._digits[i] : 0;
		unsigned int sum = digit1 + digit2 + carry;
		carry = sum / 10;
		result._digits.push_back(sum % 10);
	}
	return result;
}

bigint& bigint::operator+=(const bigint& other) {
	*this = *this + other;
	return *this;
}

// increment operators
bigint& bigint::operator++(void) {
	*this = *this + 1;
	return *this;
}

bigint bigint::operator++(int) {
	bigint temp(*this);
	*this = *this + 1;
	return temp;
}

// comparison operators
bool bigint::operator>(const bigint& other) const {
	if (_digits.size() != other._digits.size())
		return _digits.size() > other._digits.size();

	for (size_t i = _digits.size(); i-- > 0;) {
		if (_digits[i] != other._digits[i])
			return _digits[i] > other._digits[i];
	}
	return false;
}

bool bigint::operator<(const bigint& other) const {
	return other > *this;
}

bool bigint::operator>=(const bigint& other) const {
	return !(*this < other);
}

bool bigint::operator<=(const bigint& other) const {
	return !(*this > other);
}

bool bigint::operator==(const bigint& other) const {
	return _digits == other._digits;
}

bool bigint::operator!=(const bigint& other) const {
	return !(*this == other);
}

// shift operators
bigint bigint::operator<<(int i) const {
	bigint result(*this);
	while (i-- > 0) {
		result._digits.insert(result._digits.begin(), 0);
	}
	return result;
}

bigint bigint::operator>>(int i) const {
	bigint result(*this);
	while (i-- > 0 && !result._digits.empty()) {
		result._digits.erase(result._digits.begin());
	}
	return result;
}

bigint& bigint::operator<<=(int i) {
	while (i-- > 0) {
		_digits.insert(_digits.begin(), 0);
	}
	return *this;
}

bigint& bigint::operator>>=(int i) {
	while (i-- > 0 && !_digits.empty()) {
		_digits.erase(_digits.begin());
	}
	return *this;
}

bigint& bigint::operator<<=(const bigint& other) {
	std::vector<unsigned int>::const_iterator start = other._digits.begin();
	std::vector<unsigned int>::const_iterator end = other._digits.end();
	int shift = 0;
	int power = 1;

	while (start != end) {
		shift += (*start) * power;
		power *= 10;
		++start;
	}

	if (shift > 0)
		_digits.insert(_digits.begin(), shift, 0);

	return *this;
}

bigint& bigint::operator>>=(const bigint& other) {
	std::vector<unsigned int>::const_iterator start = other._digits.begin();
	std::vector<unsigned int>::const_iterator end = other._digits.end();
	int shift = 0;
	int power = 1;

	while (start != end) {
		shift += (*start) * power;
		power *= 10;
		++start;
	}

	if (shift >= (int)_digits.size()) {
		_digits.clear();
		_digits.push_back(0);  // resultado é 0
	} else {
		_digits.erase(_digits.begin(), _digits.begin() + shift);
	}

	return *this;
}

// getter
const std::vector<unsigned int>& bigint::getDigits() const {
	return _digits;
}

// output operator
std::ostream& operator<<(std::ostream& out, const bigint& n) {
	std::vector<unsigned int>::const_reverse_iterator start = n.getDigits().rbegin();
	std::vector<unsigned int>::const_reverse_iterator end = n.getDigits().rend();

	while (start != end) {
		out << *start;
		++start;
	}
	return out;
}

// c++ -Wall -Werror -Wextra -std=c++98 main.cpp bigint.cpp 