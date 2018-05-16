#include "movie_validator.h"

void MovieValidator::validate(const Movie& movie)
{
	std::vector<std::string> err_msg;
	if (movie.get_id() <= 0)
		err_msg.push_back("ID should be a positive integer!");
	if (movie.get_title().length() == 0)
		err_msg.push_back("Title can not be empty!");
	if (movie.get_genre().length() == 0)
		err_msg.push_back("Genre can not be empty!");
	if (movie.get_actor().length() == 0)
		err_msg.push_back("Actor can not be empty!");
	if (movie.get_release_year() < 1900 || movie.get_release_year() > 2050)
		err_msg.push_back("Year should be in range 1900-2050!");
	if (err_msg.size() > 0)
		throw ValidatorException(err_msg);
}

std::vector<std::string> ValidatorException::get_err_msg() const noexcept
{
	return err_msg;
}
