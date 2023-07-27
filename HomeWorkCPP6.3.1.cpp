#include <iostream>
#include <iterator>
#include <random>
#include <future>
#include <list>
#include <Windows.h>

void search_minimum(std::list<int>& list_number, std::promise <std::list<int>::iterator> iter_min, std::list<int>::iterator iter) {

	int number = *iter;
	std::list<int>::iterator minimum = iter;

	for (; iter != list_number.end(); iter++) {

		if (number > *iter) {

			number = *iter;
			minimum = iter;
		}
	}

	iter_min.set_value(minimum);
}

void sorting_choice(std::list<int>& list_number) {

	std::list<int>::iterator iter = list_number.begin();
	std::list<int>::iterator iter_min;

	for (; iter != list_number.end(); iter++) {

		std::promise<std::list<int>::iterator> prom;
		std::future<std::list<int>::iterator> minimum = prom.get_future();
		auto list_sort = std::async(search_minimum, std::ref(list_number), std::move(prom), iter);

		list_sort.wait();

		iter_min = minimum.get();

		if (iter != iter_min) {

			std::swap(*iter, *iter_min);
		}
	}
}

void list_random(std::list<int>& list_number, int size_list) {

	std::random_device rv;
	std::mt19937 gen(rv());
	std::uniform_int_distribution<> dist(0, 100);

	for (int i = 0; i < size_list; i++) {

		list_number.push_back(dist(gen));
	}
}

void list_print(std::list<int>& list_number) {

	std::cout << "Список: ";

	for (const auto num : list_number) {

		std::cout << num << " ";
	}

	std::cout << std::endl;
}

int main()
{
	setlocale(LC_ALL, "Russian");

	int size_list = 20;
	std::list<int> list_number;

	list_random(list_number, size_list);

	list_print(list_number);

	sorting_choice(list_number);

	list_print(list_number);
}