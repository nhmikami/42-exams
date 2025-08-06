#include "bigint.hpp"

// constructors and destructor
bigint::bigint(void) : _digits("0") {
}

bigint::bigint(unsigned int value) {
	_digits = std::to_string(value);
}

bigint::bigint(const std::string& value) : _digits(value) {
}

bigint::bigint(const bigint& other) : _digits(other._digits) {
}

bigint::~bigint(void) {
}

// assignment operator
bigint& bigint::operator=(const bigint& other) {
	if (this != &other)
		_digits = other._digits;
	return *this;
}

// arithmetic operators
bigint bigint::operator+(const bigint& other) const {
	std::string result;
	int i = _digits.size() - 1;
	int j = other._digits.size() - 1;
	int carry = 0;

	while (i >= 0 || j >= 0 || carry > 0) {
		int digit1 = (i >= 0) ? _digits[i--] - '0' : 0;
		int digit2 = (j >= 0) ? other._digits[j--] - '0' : 0;
		int sum = digit1 + digit2 + carry;
		carry = sum / 10;
		result.push_back((sum % 10) + '0');
	}
	std::reverse(result.begin(), result.end());
	return bigint(result);
}

bigint& bigint::operator+=(const bigint& other) {
	*this = *this + other;
	return *this;
}

// increment operators
bigint& bigint::operator++(void) {
	*this = *this + bigint(1);
	return *this;
}

bigint bigint::operator++(int) {
	bigint temp(*this);
	*this = *this + bigint(1);
	return temp;
}

// comparison operators
bool bigint::operator>(const bigint& other) const {
	if (_digits.size() != other._digits.size())
		return _digits.size() > other._digits.size();

	return _digits > other._digits;
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
	result <<= i;
	return result;
}

bigint bigint::operator>>(int i) const {
	bigint result(*this);
	result >>= i;
	return result;
}

bigint& bigint::operator<<=(int i) {
	if (i < 0)
		return *this >>= -i;
	_digits += std::string(i, '0');
	return *this;
}

bigint& bigint::operator>>=(int i) {
	if (i < 0)
		return *this <<= -i;
	if ((size_t)i >= _digits.size())
		_digits = "0";
	else
		_digits.erase(_digits.size() - i);
	return *this;
}

bigint& bigint::operator<<=(const bigint& other) {
	int shift = std::stoi(other._digits);
	return *this <<= shift;
}

bigint& bigint::operator>>=(const bigint& other) {
	int shift = std::stoi(other._digits);
	return *this >>= shift;
}

// getter
const std::string& bigint::getDigits(void) const {
	return _digits;
}

// output operator
std::ostream& operator<<(std::ostream& out, const bigint& n) {
	out << n.getDigits();
	return out;
}

// c++ -Wall -Werror -Wextra -std=c++98 main.cpp bigint.cpp 