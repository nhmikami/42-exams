#ifndef SET_HPP
#define SET_HPP

#include "searchable_bag.hpp"

class set {
	private:
		searchable_bag* bag;

	public:
		set();
		set(searchable_bag& b);
		set(const set& other);
		~set();

		set&	operator=(const set& other);

		void	insert(int item);
		void	insert(int* items, int size);
		void	print() const;
		void	clear();
		bool	has(int item) const;

		searchable_bag&	get_bag() const;
};

#endif