#include "catch.hpp"

#include "movie_service.h"

TEST_CASE("test movie service", "[service]")
{
	// setup
	Repository<Movie>* repository = new MovieRepository;
	MovieValidator validator;
	MovieService service{ repository, validator };
	service.movie_add(1, "Hacksaw Ridge", "War", "Andrew Garfield", 2016);
	service.movie_add(3, "V for Vendetta", "Action", "Hugo Weaving", 2005);
	service.movie_add(6, "The Godfather", "Mafia", "Al Pacino", 1972);
	service.movie_add(7, "Grown Ups", "Comedy", "Adam Sandler", 2010);
	service.movie_find(1).set_is_in_shopping_cart(true);


	SECTION("test movie_add")
	{
		// test add invalid movie (existent id)
		REQUIRE_THROWS_AS(service.movie_add(1, "Grown Ups", "Comedy", "Adam Sandler", 2010), RepositoryException);
		// test add invalid movie (invalid movie data)
		REQUIRE_THROWS_AS(service.movie_add(2, "", "", "", 0), ValidatorException);
		// test add valid movie (inexistent id)
		REQUIRE_THROWS_AS(repository->find(2), RepositoryException);
		service.movie_add(2, "Interstellar", "Sci-Fi", "McConaughey", 2014);
		REQUIRE_NOTHROW(repository->find(2));
		Movie added = repository->find(2);
		REQUIRE(added.get_id() == 2);
		REQUIRE(added.get_title() == "Interstellar");
		REQUIRE(added.get_release_year() == 2014);
	}

	SECTION("test movie_find")
	{
		// test find inexistent movie
		REQUIRE_THROWS_AS(service.movie_find(2), RepositoryException);
		// test find existent movie
		service.movie_add(2, "Interstellar", "Sci-Fi", "McConaughey", 2014);
		REQUIRE_NOTHROW(service.movie_find(2));
	}

	SECTION("test movie_update")
	{
		// test update inexistent movie
		REQUIRE_THROWS_AS(service.movie_update(2, "The Godfather", "Mafia", "Al Pacino", 1972), RepositoryException);
		// test update existent movie with invalid data
		REQUIRE_THROWS_AS(service.movie_update(1, "", "", "", 0), ValidatorException);
		// test update existent movie with valid data
		REQUIRE_NOTHROW(service.movie_update(1, "The Godfather", "Mafia", "Al Pacino", 1972));
		REQUIRE(service.movie_find(1).get_id() == 1);
		REQUIRE(service.movie_find(1).get_title() == "The Godfather");
		REQUIRE(service.movie_find(1).get_release_year() == 1972);
	}

	SECTION("test movie_delete")
	{
		// test delete inexistent movie
		REQUIRE_THROWS_AS(service.movie_delete(2), RepositoryException);
		// test delete existent movie
		REQUIRE_NOTHROW(service.movie_find(1));
		REQUIRE_NOTHROW(service.movie_delete(1));
		REQUIRE_THROWS_AS(service.movie_find(1), RepositoryException);
	}

	SECTION("test movie_undo: store")
	{
		REQUIRE_THROWS(service.movie_find(10));
		service.movie_add(10, "The Green Mile", "Drama", "Tom Hanks", 1999);
		REQUIRE_NOTHROW(service.movie_find(10));
		service.movie_undo();
		REQUIRE_THROWS(service.movie_find(10));
	}

	SECTION("test movie_undo: update")
	{
		REQUIRE(service.movie_find(1).get_title() == "Hacksaw Ridge");
		service.movie_update(1, "Hacksaw Ridgeyyy", "War", "Andrew Garfield", 2016);
		REQUIRE(service.movie_find(1).get_title() == "Hacksaw Ridgeyyy");
		service.movie_undo();
		REQUIRE(service.movie_find(1).get_title() == "Hacksaw Ridge");
	}

	SECTION("test movie_undo: delete")
	{
		REQUIRE_NOTHROW(service.movie_find(1));
		service.movie_delete(1);
		REQUIRE_THROWS(service.movie_find(1));
		service.movie_undo();
		REQUIRE_NOTHROW(service.movie_find(1));
	}

	SECTION("test movie_undo: exception")
	{
		REQUIRE_NOTHROW(service.movie_undo());
		REQUIRE_NOTHROW(service.movie_undo());
		REQUIRE_NOTHROW(service.movie_undo());
		REQUIRE_NOTHROW(service.movie_undo());
		REQUIRE_THROWS(service.movie_undo());
	}

	SECTION("test movie_get_all")
	{
		std::vector<Movie> all_movies;
		REQUIRE(all_movies.size() == 0);
		all_movies = service.movie_get_all();
		REQUIRE(all_movies.size() > 0);
	}

	SECTION("test movie_get_all_filtered_by_title")
	{
		std::vector<Movie> movies_filtered = service.movie_get_all_filtered_by_title("saw");
		REQUIRE(movies_filtered.size() == 1);
		REQUIRE(movies_filtered[0].get_title() == "Hacksaw Ridge");
	}

	SECTION("test movie_get_all_filtered_by_release_year")
	{
		std::vector<Movie> movies_filtered = service.movie_get_all_filtered_by_release_year(2005);
		REQUIRE(movies_filtered.size() == 1);
		REQUIRE(movies_filtered[0].get_title() == "V for Vendetta");
	}

	SECTION("test movie_get_all_sorted_by_title")
	{
		// test sort ascending
		bool desc = false;
		std::vector<Movie> movies_sorted = service.movie_get_all_sorted_by_title(desc);
		REQUIRE(movies_sorted[0].get_title() < movies_sorted[1].get_title());
		// test sort descending
		desc = true;
		movies_sorted = service.movie_get_all_sorted_by_title(desc);
		REQUIRE(movies_sorted[0].get_title() > movies_sorted[1].get_title());
	}

	SECTION("test movie_get_all_sorted_by_genre")
	{
		// test sort ascending
		bool desc = false;
		std::vector<Movie> movies_sorted = service.movie_get_all_sorted_by_genre(desc);
		REQUIRE(movies_sorted[0].get_genre() < movies_sorted[1].get_genre());
		// test sort descending
		desc = true;
		movies_sorted = service.movie_get_all_sorted_by_genre(desc);
		REQUIRE(movies_sorted[0].get_genre() > movies_sorted[1].get_genre());
	}

	SECTION("test movie_get_all_sorted_by_actor")
	{
		// test sort ascending
		bool desc = false;
		std::vector<Movie> movies_sorted = service.movie_get_all_sorted_by_actor(desc);
		REQUIRE(movies_sorted[0].get_actor() < movies_sorted[1].get_actor());
		// test sort descending
		desc = true;
		movies_sorted = service.movie_get_all_sorted_by_actor(desc);
		REQUIRE(movies_sorted[0].get_actor() > movies_sorted[1].get_actor());
	}

	SECTION("test movie_get_all_sorted_by_release_year")
	{
		// test sort ascending
		bool desc = false;
		std::vector<Movie> movies_sorted = service.movie_get_all_sorted_by_release_year(desc);
		REQUIRE(movies_sorted[0].get_release_year() < movies_sorted[1].get_release_year());
		// test sort descending
		desc = true;
		movies_sorted = service.movie_get_all_sorted_by_release_year(desc);
		REQUIRE(movies_sorted[0].get_release_year() > movies_sorted[1].get_release_year());
	}

	SECTION("test movie_add_to_cart")
	{
		// test add to cart a movie that doesn't exist
		REQUIRE_THROWS_AS(service.movie_add_to_cart(5), RepositoryException);
		// test add to cart a movie that exists
		REQUIRE_NOTHROW(service.movie_add_to_cart(1));
		REQUIRE(service.movie_find(1).get_is_in_shopping_cart() == true);
	}

	SECTION("test movie_remove_from_cart")
	{
		// test remove from cart a movie that doesn't exist
		REQUIRE_THROWS_AS(service.movie_remove_from_cart(5), RepositoryException);
		// test remove from cart a movie that exists
		REQUIRE_NOTHROW(service.movie_remove_from_cart(1));
		REQUIRE(service.movie_find(1).get_is_in_shopping_cart() == false);
	}

	SECTION("test movie_size_cart")
	{
		REQUIRE(service.movie_size_cart() == 1);
		service.movie_add_to_cart(6);
		REQUIRE(service.movie_size_cart() == 2);
		service.movie_empty_cart();
		REQUIRE(service.movie_size_cart() == 0);
	}

	SECTION("test movie_empty_cart")
	{
		std::vector<Movie>& all_movies = repository->get_all();
		// test shopping cart isn't empty
		auto it = std::find_if(all_movies.begin(), all_movies.end(), [](const Movie& movie) noexcept {return movie.get_is_in_shopping_cart() == true; });
		REQUIRE(it != all_movies.end());
		// empty shopping cart
		service.movie_empty_cart();
		// test shopping cart is empty
		it = std::find_if(all_movies.begin(), all_movies.end(), [](const Movie& movie) noexcept {return movie.get_is_in_shopping_cart() == true; });
		REQUIRE(it == all_movies.end());
	}

	SECTION("test movie_get_all_cart")
	{
		auto cart_movies = service.movie_get_all_cart();
		REQUIRE(cart_movies.size() == 1);
		service.movie_empty_cart();
		cart_movies = service.movie_get_all_cart();
		REQUIRE(cart_movies.size() == 0);
	}

	SECTION("test movie_generate_random_cart")
	{
		service.movie_empty_cart();
		REQUIRE(service.movie_size_cart() == 0);
		service.movie_generate_random_cart();
		REQUIRE(service.movie_size_cart() != 0);
	}
}