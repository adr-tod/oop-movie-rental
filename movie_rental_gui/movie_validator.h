#pragma once

#include <string>
#include <vector>
#include "movie.h"

class MovieValidator
{
public:
	void validate(const Movie& movie);
};

class ValidatorException
{
private:
	std::vector<std::string> err_msg;
public:
	ValidatorException(const std::vector<std::string>& msg) :err_msg{ msg } {};
	std::vector<std::string> get_err_msg() const noexcept;
};
