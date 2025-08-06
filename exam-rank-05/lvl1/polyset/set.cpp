#include "set.hpp"

set::set() : bag(0) {}

set::set(searchable_bag& b) : bag(&b) {}

set::set(const set& other) {
	if (other.bag)
		this->bag = other.bag;
	else
		this->bag = 0;
}

set::~set() {}

set& set::operator=(const set& other) {
	if (this != &other)
		this->bag = other.bag;
	return *this;
}

void set::insert(int item) {
	if (bag && !(*bag).has(item))
		(*bag).insert(item);
}

void set::insert(int* items, int size) {
	for (int i = 0; i < size; i++)
		if (!has(items[i]))
			(*bag).insert(items[i]);
}

void set::print() const {
	(*bag).print();
}

void set::clear() {
	(*bag).clear();
}

bool set::has(int item) const {
	return (*bag).has(item);
}

searchable_bag& set::get_bag() const {
	return *bag;
}
