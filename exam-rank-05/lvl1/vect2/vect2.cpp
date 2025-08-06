#include "vect2.hpp"

// constructors and destructor
vect2::vect2(void) : _x(0), _y(0) {}

vect2::vect2(int x, int y) : _x(x), _y(y) {}

vect2::vect2(const vect2& other) : _x(other._x), _y(other._y) {}

vect2::~vect2(void) {}

// assignment operator
vect2& vect2::operator=(const vect2& other) {
	if (this != &other) {
		_x = other._x;
		_y = other._y;
	}
	return *this;
}

// arithmetic operators
vect2 vect2::operator+(const vect2& other) const {
	return vect2(_x + other._x, _y + other._y);
}

vect2 vect2::operator-(const vect2& other) const {
	return vect2(_x - other._x, _y - other._y);
}

vect2 vect2::operator*(const int scalar) const {
	return vect2(_x * scalar, _y * scalar);
}

vect2& vect2::operator+=(const vect2& other) {
	*this = *this + other;
	return *this;
}

vect2& vect2::operator-=(const vect2& other) {
	*this = *this - other;
	return *this;
}

vect2& vect2::operator*=(const int scalar) {
	*this = *this * scalar;
	return *this;
}

vect2 operator*(const int scalar, const vect2& v) {
	return v * scalar;
}

// increment operators
vect2& vect2::operator++(void) {
	_x += 1;
	_y += 1;
	return *this;
}

vect2 vect2::operator++(int) {
	vect2 temp(*this);
	_x += 1;
	_y += 1;
	return temp;
}

vect2& vect2::operator--(void) {
	_x -= 1;
	_y -= 1;
	return *this;
}

vect2 vect2::operator--(int) {
	vect2 temp(*this);
	_x -= 1;
	_y -= 1;
	return temp;
}

// unary minus operator
vect2 vect2::operator-(void) const {
	return vect2(-_x, -_y);
}

// comparison operators
bool vect2::operator==(const vect2& other) const {
	return (_x == other._x && _y == other._y);
}

bool vect2::operator!=(const vect2& other) const {
	return (_x != other._x || _y != other._y);
}

// access operators
int& vect2::operator[](size_t index) {
	if (index == 0)
		return _x;
	else if (index == 1)
		return _y;
	else
		throw std::out_of_range("Index out of range");
}

const int& vect2::operator[](size_t index) const {
	if (index == 0)
		return _x;
	else if (index == 1)
		return _y;
	else
		throw std::out_of_range("Index out of range");
}

// getter
int vect2::getX() const {
	return _x;
}

int vect2::getY() const {
	return _y;
}

// output operator
std::ostream& operator<<(std::ostream& out, const vect2& v) {
	out << "{" << v.getX() << ", " << v.getY() << "}";
	return out;
}

// c++ -Wall -Werror -Wextra -std=c++98 main.cpp vect2.cpp 