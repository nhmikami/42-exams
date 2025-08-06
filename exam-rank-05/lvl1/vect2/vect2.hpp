#ifndef VECT2_HPP
#define VECT2_HPP

#include <iostream>

class vect2 {
	private:
		int _x;
		int _y;

	public:
		vect2(void);
		vect2(int x, int y);
		vect2(const vect2& other);
		~vect2(void);

		vect2&	operator=(const vect2& other);

		// arithmetic operators
		vect2	operator+(const vect2& other) const;
		vect2	operator-(const vect2& other) const;
		vect2	operator*(const int scalar) const;
		vect2&	operator+=(const vect2& other);
		vect2&	operator-=(const vect2& other);
		vect2&	operator*=(const int scalar);

		// increment and decrement operators
		vect2&	operator++(void);
		vect2	operator++(int);
		vect2&	operator--(void);
		vect2	operator--(int);

		// unary minus operator
		vect2	operator-(void) const;

		// comparison operators
		bool	operator==(const vect2& other) const;
		bool	operator!=(const vect2& other) const;

		// access operators
		int&	operator[](size_t index);
		const int&	operator[](size_t index) const;

		// getter
		int	getX() const;
		int	getY() const;
};

std::ostream&	operator<<(std::ostream& out, const vect2& v);
vect2	operator*(const int scalar, const vect2& v);

#endif
