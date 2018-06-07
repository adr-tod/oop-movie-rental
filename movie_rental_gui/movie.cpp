#include "movie.h"

Movie::Movie(const unsigned int id, const std::string& title, const std::string& genre, const std::string& actor, const unsigned int release_year) noexcept : id{ id }, title{ title }, genre{ genre }, actor{ actor }, release_year{ release_year }, is_in_shopping_cart{ false } 
{
	add_to_history("Initial: " + std::to_string(id) + " / " + title + " / " + genre + " / " + actor + " / " + std::to_string(release_year));
}

Movie::Movie(const unsigned int id, const std::string & title, const std::string & genre, const std::string & actor, const unsigned int release_year, const unsigned int is_in_shopping_cart) noexcept : id{ id }, title{ title }, genre{ genre }, actor{ actor }, release_year{ release_year }, is_in_shopping_cart{ (bool)is_in_shopping_cart }
{
}

unsigned int Movie::get_id() const noexcept
{
	return id;
}

unsigned int Movie::set_id(const unsigned int new_id) noexcept
{
	id = new_id;
	return id;
}

std::string Movie::get_title() const noexcept
{
	return title;
}

std::string Movie::set_title(const std::string& new_title) noexcept
{
	title = new_title;
	return title;
}

std::string Movie::get_genre() const noexcept
{
	return genre;
}

std::string Movie::set_genre(const std::string& new_genre) noexcept
{
	genre = new_genre;
	return genre;
}

std::string Movie::get_actor() const noexcept
{
	return actor;
}

std::string Movie::set_actor(const std::string& new_actor) noexcept
{
	actor = new_actor;
	return actor;
}

unsigned int Movie::get_release_year() const noexcept
{
	return release_year;
}

unsigned int Movie::set_release_year(const unsigned int new_year) noexcept
{
	release_year = new_year;
	return release_year;
}

std::string Movie::get_history() const noexcept
{
	return history;
}

std::string Movie::set_history(const std::string & new_history) noexcept
{
	history = new_history;
	return history;
}

void Movie::add_to_history(const std::string & event) noexcept
{
	history += event + "\n";
}

bool Movie::get_is_in_shopping_cart() const noexcept
{
	return is_in_shopping_cart;
}

bool Movie::set_is_in_shopping_cart(const bool new_status) noexcept
{
	is_in_shopping_cart = new_status;
	return is_in_shopping_cart;
}

bool Movie::operator==(const Movie& other) const noexcept
{
	if (this->title != other.title)
		return false;
	if (this->genre != other.genre)
		return false;
	if (this->actor != other.actor)
		return false;
	if (this->release_year != other.release_year)
		return false;
	return true;
}

std::ostream& operator<<(std::ostream& os, const Movie& movie)
{
	os << "[MOVIE ID " << movie.id << "]\n";
	os << "Title: " << movie.title << '\n';
	os << "Genre: " << movie.genre << '\n';
	os << "Actor: " << movie.actor << '\n';
	os << "Release year: " << movie.release_year;
	return os;
}
