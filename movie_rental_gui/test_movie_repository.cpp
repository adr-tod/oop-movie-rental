#include "catch.hpp"

#include "movie_repository.h"

TEST_CASE("test dictionary repository", "[repository]")
{
	// setup
	DictionaryRepository repository{ 0.0 };
	repository.store(Movie{ 1, "Interstellar", "Sci-Fi", "McConaughey", 2014 });
	repository.store(Movie{ 2, "V for Vendetta", "Action", "Hugo Weaving", 2005 });
	repository.store(Movie{ 3, "Hacksaw Ridge", "War", "Andrew Garfield", 2016 });

	SECTION("test store")
	{
		// store already existent movie
		REQUIRE_THROWS(repository.store(Movie{ 1, "qwe", "qwe", "qwe", 2000 }));
		// store inexistent movie
		REQUIRE_NOTHROW(repository.store(Movie{ 4, "qwe", "qwe", "qwe", 2000 }));
		REQUIRE_NOTHROW(repository.find(4));
		// random exception
		DictionaryRepository test{ 1.1 };
		REQUIRE_THROWS(test.store(Movie{ 1, "qwe", "qwe", "qwe", 2000 }));
	}

	SECTION("test find")
	{
		// find inexistent movie
		REQUIRE_THROWS(repository.find(5));
		// find existent movie
		REQUIRE_NOTHROW(repository.find(3));
	}

	SECTION("test update")
	{
		// update inexistent movie
		REQUIRE_THROWS(repository.update(5, Movie{ 5, "qwe", "qwe", "qwe", 2000 }));
		// update existent movie
		REQUIRE_NOTHROW(repository.update(3, Movie{ 3, "qwe", "qwe", "qwe", 2000 }));
		REQUIRE(repository.find(3).get_title() == "qwe");
	}

	SECTION("test delete")
	{
		// delete inexistent movie
		REQUIRE_THROWS(repository.remove(5));
		// delete existent movie
		REQUIRE_NOTHROW(repository.remove(1));
		REQUIRE_THROWS(repository.find(1));
	}

	SECTION("test size")
	{
		REQUIRE(repository.size() == 3);
	}

	SECTION("test get_all")
	{
		auto all_movies = repository.get_all();
		REQUIRE(all_movies.size() == 3);
		REQUIRE(all_movies[0].get_title() == "Interstellar");
	}

	SECTION("test clear")
	{
		REQUIRE(repository.size() == 3);
		repository.clear();
		REQUIRE(repository.size() == 0);
	}
}

TEST_CASE("test movie repository", "[repository]")
{
	// setup
	MovieRepository repository;
	repository.store(Movie{ 1, "Interstellar", "Sci-Fi", "McConaughey", 2014 });
	repository.store(Movie{ 2, "V for Vendetta", "Action", "Hugo Weaving", 2005 });
	repository.store(Movie{ 3, "Hacksaw Ridge", "War", "Andrew Garfield", 2016 });

	SECTION("test store")
	{
		// store movie with valid id (inexistent)
		REQUIRE(repository.size() == 3);
		repository.store(Movie{ 5, "Grown Ups", "Comedy", "Adam Sandler", 2010 });
		REQUIRE(repository.size() == 4);
		// store movie with invalid id (existent)
		REQUIRE_THROWS_AS(repository.store(Movie{ 1, "Grown Ups", "Comedy", "Adam Sandler", 2010 }), RepositoryException);
	}

	SECTION("test find")
	{
		// find existent movie
		REQUIRE_NOTHROW(repository.find(3));
		Movie found = repository.find(3);
		REQUIRE(found.get_id() == 3);
		REQUIRE(found.get_title() == "Hacksaw Ridge");
		REQUIRE(found.get_genre() == "War");
		REQUIRE(found.get_actor() == "Andrew Garfield");
		REQUIRE(found.get_release_year() == 2016);
		// find inexistent movie
		REQUIRE_THROWS_AS(repository.find(5), RepositoryException); // inexistent id
	}

	SECTION("test update")
	{
		// update existent movie
		// before: Movie{ 2, "V for Vendetta", "Action", "Hugo Weaving", 2005 }
		REQUIRE(repository.find(2).get_title() == "V for Vendetta");
		REQUIRE(repository.find(2).get_release_year() == 2005);
		repository.update(2, Movie{ 2, "Matrix", "Action", "Hugo Weaving", 1999 });
		REQUIRE(repository.find(2).get_title() == "Matrix");
		REQUIRE(repository.find(2).get_release_year() == 1999);
		// update inexistent movie
		REQUIRE_THROWS_AS(repository.update(5, Movie{ 5, "Matrix", "Action", "Hugo Weaving", 1999 }), RepositoryException);
	}

	SECTION("test remove")
	{
		// delete existent movie
		REQUIRE_NOTHROW(repository.find(2));
		REQUIRE_NOTHROW(repository.remove(2));
		REQUIRE_THROWS(repository.find(2));
		// delete inexistent movie
		REQUIRE_THROWS(repository.remove(5));
	}

	SECTION("test get_all")
	{
		std::vector<Movie> all_movies;
		REQUIRE(all_movies.size() == 0);
		all_movies = repository.get_all();
		REQUIRE(all_movies.size() == 3);
	}

	SECTION("test get_err_msg")
	{
		try {
			throw RepositoryException("just to test it");
		}
		catch (RepositoryException& ex) {
			REQUIRE(ex.get_err_msg() == "just to test it");
		}

	}
}

TEST_CASE("test movie repository file", "[repository]")
{
	// setup
	MovieRepositoryFile repository{ "test_movies.txt" };

	SECTION("test store")
	{
		// store invalid movie (existent id)
		REQUIRE_THROWS(repository.store(Movie{ 5, "Grown Ups", "Comedy", "Adam Sandler", 2010 }));
		// store valid movie (inexistent id)
		REQUIRE_NOTHROW(repository.store(Movie{ 11, "Grown Ups", "Comedy", "Adam Sandler", 2010 }));
		REQUIRE_NOTHROW(repository.find(11));
		// cleanup
		repository.remove(11);
	}

	SECTION("test find")
	{
		// find inexistent movie
		REQUIRE_THROWS_AS(repository.find(11), RepositoryException);
		// find existent movie
		REQUIRE_NOTHROW(repository.find(10));
	}

	SECTION("test update")
	{
		// update inexistent movie
		REQUIRE_THROWS(repository.update(11, Movie{ 11, "Grown Ups", "Comedy", "Adam Sandler", 2010 }));
		// update existent movie
		Movie before = repository.find(10);
		REQUIRE(before.get_title() == "Yes Man");
		REQUIRE_NOTHROW(repository.update(10, Movie{ 10, "Grown Ups", "Comedy", "Adam Sandler", 2010 }));
		REQUIRE(repository.find(10).get_title() == "Grown Ups");
		// cleanup
		repository.update(10, before);
	}

	SECTION("test remove")
	{
		// remove inexistent movie
		REQUIRE_THROWS(repository.remove(11));
		// remove existent movie
		Movie backup = repository.find(10);
		REQUIRE_NOTHROW(repository.remove(10));
		REQUIRE_THROWS(repository.find(10));
		// cleanup
		repository.store(backup);
	}

	SECTION("test size")
	{
		REQUIRE(repository.size() == 10);
	}

	SECTION("test get_all")
	{
		std::vector<Movie> all_movies;
		// empty before
		REQUIRE(all_movies.size() == 0);
		all_movies = repository.get_all();
		// not empty anymore
		REQUIRE(all_movies.size() == 10);
	}
}