#include <iostream>
#include "movie_ui.h"


void MovieUI::help()
{
	std::cout << "\n[AVAILABLE COMMANDS]\n";
	std::cout << "0.  Exit\n";
	std::cout << "1.  Add a movie\n";
	std::cout << "2.  Find a movie by ID\n";
	std::cout << "3.  Update a movie\n";
	std::cout << "4.  Delete a movie\n";
	std::cout << "5.  Print all movies\n";
	std::cout << "6.  Print all movies filtered by title\n";
	std::cout << "7.  Print all movies filtered by release year\n";
	std::cout << "8.  Print all movies sorted by title\n";
	std::cout << "9.  Print all movies sorted by genre\n";
	std::cout << "10. Print all movies sorted by actor\n";
	std::cout << "11. Print all movies sorted by release year\n";
	std::cout << "12. Add a movie to shopping cart\n";
	std::cout << "13. Remove a movie from shopping cart\n";
	std::cout << "14. Empty the shopping cart\n";
	std::cout << "15. Generate a new random shopping cart\n";
	std::cout << "16. Print the shopping cart\n";
	std::cout << "17. Undo last add/update/delete\n";
	std::cout << "19. Print this help\n\n";
}

unsigned int MovieUI::input_id()
{
	unsigned int id;
	std::cout << "ID: ";
	std::cin >> id;
	return id;
}

std::string MovieUI::input_title()
{
	std::string title;
	std::cout << "Title: ";
	std::cin >> title;
	return title;
}

std::string MovieUI::input_genre()
{
	std::string genre;
	std::cout << "Genre: ";
	std::cin >> genre;
	return genre;
}

std::string MovieUI::input_actor()
{
	std::string actor;
	std::cout << "Actor: ";
	std::cin >> actor;
	return actor;
}

unsigned int MovieUI::input_release_year()
{
	unsigned int release_year;
	std::cout << "Release year: ";
	std::cin >> release_year;
	return release_year;
}

std::string MovieUI::input_filter()
{
	std::string filter;
	std::cout << "Filter: ";
	std::cin >> filter;
	return filter;
}

bool MovieUI::input_desc()
{
	bool desc;
	std::cout << "Descending: ";
	std::cin >> desc;
	return desc;
}

void MovieUI::movie_add()
{
	const unsigned int id = input_id();
	std::string title = input_title();
	std::string genre = input_genre();
	std::string actor = input_actor();
	const unsigned int release_year = input_release_year();
	service.movie_add(id, title, genre, actor, release_year);
	std::cout << "Succesfully added!\n";
}

void MovieUI::movie_find()
{
	const unsigned int id = input_id();
	Movie found = service.movie_find(id);
	std::cout << found << '\n';
}

void MovieUI::movie_update()
{
	const unsigned int id = input_id();
	std::string new_title = input_title();
	std::string new_genre = input_genre();
	std::string new_actor = input_actor();
	const unsigned int new_release_year = input_release_year();
	service.movie_update(id, new_title, new_genre, new_actor, new_release_year);
	std::cout << "Succesfully updated!\n";
}

void MovieUI::movie_delete()
{
	const unsigned int id = input_id();
	service.movie_delete(id);
	std::cout << "Succesfully deleted!\n";
}

void MovieUI::movie_undo()
{
		service.movie_undo();
}

void MovieUI::movie_print_all()
{
	std::vector<Movie>& all_movies = service.movie_get_all();
	if (all_movies.size() == 0)
		std::cout << "No movies available!\n";
	else
	{
		std::cout << '\n';
		for (const auto& movie : all_movies)
			std::cout << movie << "\n\n";
	}

}

void MovieUI::movie_print_all_filtered_by_title()
{
	std::string filter = input_filter();
	std::vector<Movie> movies_filtered = service.movie_get_all_filtered_by_title(filter);
	if (movies_filtered.size() == 0)
		std::cout << "No movie matches the filter!\n";
	else
	{
		std::cout << '\n';
		for (const auto& movie : movies_filtered)
			std::cout << movie << "\n\n";
	}
}

void MovieUI::movie_print_all_filtered_by_release_year()
{
	std::string filter = input_filter();
	std::vector<Movie> movies_filtered = service.movie_get_all_filtered_by_release_year(stoul(filter, nullptr, 10));
	if (movies_filtered.size() == 0)
		std::cout << "No movie matches the filter!\n";
	else
	{
		std::cout << '\n';
		for (const auto& movie : movies_filtered)
			std::cout << movie << "\n\n";
	}
}

void MovieUI::movie_print_all_sorted_by_title()
{
	const bool desc = input_desc();
	std::vector<Movie> movies_sorted = service.movie_get_all_sorted_by_title(desc);
	if (movies_sorted.size() == 0)
		std::cout << "No movies available!\n";
	else
	{
		std::cout << '\n';
		for (const auto& movie : movies_sorted)
			std::cout << movie << "\n\n";
	}
}

void MovieUI::movie_print_all_sorted_by_genre()
{
	const bool desc = input_desc();
	std::vector<Movie> movies_sorted = service.movie_get_all_sorted_by_genre(desc);
	if (movies_sorted.size() == 0)
		std::cout << "No movies available!\n";
	else
	{
		std::cout << '\n';
		for (const auto& movie : movies_sorted)
			std::cout << movie << "\n\n";
	}
}

void MovieUI::movie_print_all_sorted_by_actor()
{
	const bool desc = input_desc();
	std::vector<Movie> movies_sorted = service.movie_get_all_sorted_by_actor(desc);
	if (movies_sorted.size() == 0)
		std::cout << "No movies available!\n";
	else
	{
		std::cout << '\n';
		for (const auto& movie : movies_sorted)
			std::cout << movie << "\n\n";
	}
}

void MovieUI::movie_print_all_sorted_by_release_year()
{
	const bool desc = input_desc();
	std::vector<Movie> movies_sorted = service.movie_get_all_sorted_by_release_year(desc);
	if (movies_sorted.size() == 0)
		std::cout << "No movies available!\n";
	else
	{
		std::cout << '\n';
		for (const auto& movie : movies_sorted)
			std::cout << movie << "\n\n";
	}
}

void MovieUI::movie_add_to_cart()
{
	const unsigned int id = input_id();
	service.movie_add_to_cart(id);
	std::cout << "Succesfully added to shopping cart!\n";
}

void MovieUI::movie_remove_from_cart()
{
	const unsigned int id = input_id();
	service.movie_remove_from_cart(id);
	std::cout << "Succesfully removed from shopping cart!\n";
}

void MovieUI::movie_empty_cart()
{
	service.movie_empty_cart();
	std::cout << "Succesfully cleared the shopping cart!\n";
}

void MovieUI::movie_generate_random_cart()
{
	service.movie_generate_random_cart();
	std::cout << "Succesfully generated a new random shopping cart!\n";
}

void MovieUI::movie_print_cart()
{
	auto cart_movies = service.movie_get_all_cart();
	if (cart_movies.size() == 0)
		std::cout << "No movies available!\n";
	else
	{
		std::cout << '\n';
		for (const auto& movie : cart_movies)
			std::cout << movie << "\n\n";
	}
}


void MovieUI::start()
{
	bool over = false;
	unsigned int command = 0;
	std::cout << "Welcome!\n";
	std::cout << "Press 19 for help menu.\n\n";
	while (!over)
	{
		std::cout << "Your command: ";
		std::cin >> command;
		try
		{
			switch (command)
			{
			case 0:
				over = true;
				std::cout << "Have a nice day!\n";
				break;
			case 1:
				movie_add();
				break;
			case 2:
				movie_find();
				break;
			case 3:
				movie_update();
				break;
			case 4:
				movie_delete();
				break;
			case 5:
				movie_print_all();
				break;
			case 6:
				movie_print_all_filtered_by_title();
				break;
			case 7:
				movie_print_all_filtered_by_release_year();
				break;
			case 8:
				movie_print_all_sorted_by_title();
				break;
			case 9:
				movie_print_all_sorted_by_genre();
				break;
			case 10:
				movie_print_all_sorted_by_actor();
				break;
			case 11:
				movie_print_all_sorted_by_release_year();
				break;
			case 12:
				movie_add_to_cart();
				break;
			case 13:
				movie_remove_from_cart();
				break;
			case 14:
				movie_empty_cart();
				break;
			case 15:
				movie_generate_random_cart();
				break;
			case 16:
				movie_print_cart();
				break;
			case 17:
				movie_undo();
				break;
			case 19:
				help();
				break;
			default:
				std::cout << "Invalid command!\n";
				break;
			}
		}
		catch (const RepositoryException& ex)
		{
			std::cout << ex.get_err_msg() << '\n';
		}
		catch (const ValidatorException& ex)
		{
			for (const auto& err : ex.get_err_msg())
				std::cout << err << '\n';
		}
	}
}