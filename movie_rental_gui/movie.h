#pragma once

#include <string>

class Movie
{
private:
	unsigned int id;
	std::string title;
	std::string genre;
	std::string actor;
	unsigned int release_year;
	bool is_in_shopping_cart;
public:
	Movie(const unsigned int id, const std::string& title, const std::string& genre, const std::string& actor, const unsigned int release_year) noexcept;
	unsigned int get_id() const noexcept;
	unsigned int set_id(const unsigned int new_id) noexcept;
	std::string get_title() const noexcept;
	std::string set_title(const std::string& new_title) noexcept;
	std::string get_genre() const noexcept;
	std::string set_genre(const std::string& new_genre) noexcept;
	std::string get_actor() const noexcept;
	std::string set_actor(const std::string& new_actor) noexcept;
	unsigned int get_release_year() const noexcept;
	unsigned int set_release_year(const unsigned int new_year) noexcept;
	bool get_is_in_shopping_cart() const noexcept;
	bool set_is_in_shopping_cart(const bool new_status) noexcept;
	bool operator==(const Movie& other) const noexcept;
	friend std::ostream& operator<<(std::ostream& os, const Movie& movie);
};