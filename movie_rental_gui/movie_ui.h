#pragma once

#include "movie_service.h"

class MovieUI
{
private:
	MovieService& service;
	void help();
	unsigned int input_id();
	std::string input_title();
	std::string input_genre();
	std::string input_actor();
	unsigned int input_release_year();
	std::string input_filter();
	bool input_desc();
public:
	MovieUI(MovieService& service) noexcept :service{ service } {};
	void movie_add();
	void movie_find();
	void movie_update();
	void movie_delete();
	void movie_undo();
	void movie_print_all();
	void movie_print_all_filtered_by_title();
	void movie_print_all_filtered_by_release_year();
	void movie_print_all_sorted_by_title();
	void movie_print_all_sorted_by_genre();
	void movie_print_all_sorted_by_actor();
	void movie_print_all_sorted_by_release_year();
	void movie_add_to_cart();
	void movie_remove_from_cart();
	void movie_empty_cart();
	void movie_generate_random_cart();
	void movie_print_cart();
	void start();
};

