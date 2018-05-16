#include "catch.hpp"

#include <sstream>
#include "movie.h"

TEST_CASE("test movie", "[movie]")
{
	// setup
	Movie temp{ 1, "Interstellar", "Sci-Fi", "McConaughey", 2014 };

	SECTION("test getters")
	{
		REQUIRE(temp.get_id() == 1);
		REQUIRE(temp.get_title() == "Interstellar");
		REQUIRE(temp.get_genre() == "Sci-Fi");
		REQUIRE(temp.get_actor() == "McConaughey");
		REQUIRE(temp.get_release_year() == 2014);
	}

	SECTION("test setters")
	{
		// set_id
		REQUIRE(temp.set_id(2) == 2);
		REQUIRE(temp.get_id() == 2);
		// set_title
		REQUIRE(temp.set_title("V for Vendetta") == "V for Vendetta");
		REQUIRE(temp.get_title() == "V for Vendetta");
		// set_genre
		REQUIRE(temp.set_genre("Action") == "Action");
		REQUIRE(temp.get_genre() == "Action");
		// set_actor
		REQUIRE(temp.set_actor("Hugo Weaving") == "Hugo Weaving");
		REQUIRE(temp.get_actor() == "Hugo Weaving");
		// set_release_year
		REQUIRE(temp.set_release_year(2005) == 2005);
		REQUIRE(temp.get_release_year() == 2005);
	}

	SECTION("test overloaded operator==")
	{
		Movie temp1{ 1, "Intersteliea", "Sci-Fi", "McConaughey", 2014 }; // different title
		Movie temp2{ 1, "Interstellar", "Action", "McConaughey", 2014 }; // different genre
		Movie temp3{ 1, "Interstellar", "Sci-Fi", "McDonaldddd", 2014 }; // different actor
		Movie temp4{ 1, "Interstellar", "Sci-Fi", "McConaughey", 2015 }; // different release_year
		Movie temp5{ 1, "Interstellar", "Sci-Fi", "McConaughey", 2014 }; // equal
		REQUIRE_FALSE(temp == temp1);
		REQUIRE_FALSE(temp == temp2);
		REQUIRE_FALSE(temp == temp3);
		REQUIRE_FALSE(temp == temp4);
		REQUIRE(temp == temp5);
	}

	SECTION("test overloaded operator<<")
	{
		std::ostringstream out;
		out << temp;
		REQUIRE(out.str() == "[MOVIE ID 1]\nTitle: Interstellar\nGenre: Sci-Fi\nActor: McConaughey\nRelease year: 2014");
	}
}