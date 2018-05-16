#include "catch.hpp"

#include "movie_validator.h"

TEST_CASE("test movie validator", "[validator]")
{
	// setup
	MovieValidator validator;

	SECTION("test validate")
	{
		// test validate a valid movie
		Movie movie1{ 1, "Interstellar", "Sci-Fi", "McConaughey", 2014 };
		REQUIRE_NOTHROW(validator.validate(movie1));
		// test validate an invalid movie
		Movie movie2{ 0, "", "", "", 0 };
		REQUIRE_THROWS_AS(validator.validate(movie2), ValidatorException);
	}

	SECTION("test get_err_msg")
	{
		std::vector<std::string> err_msg = { "err1", "err2" };
		try {
			throw ValidatorException(err_msg);
		}
		catch (ValidatorException& ex) {
			REQUIRE(ex.get_err_msg().size() == 2);
			REQUIRE(ex.get_err_msg()[0] == "err1");
			REQUIRE(ex.get_err_msg()[1] == "err2");
		}
	}
}