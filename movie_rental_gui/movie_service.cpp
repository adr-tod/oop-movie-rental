#include <algorithm>
#include <random>

#include "movie_service.h"

void MovieService::movie_add(const unsigned int id, const std::string & title, const std::string & genre, const std::string & actor, const unsigned int release_year)
{
	Movie to_add{ id, title, genre, actor, release_year };
	validator.validate(to_add);
	repository->store(to_add);
	undo_actions.push_back(std::make_unique<UndoAdd>(to_add, repository));
	notify_observers();
}

Movie& MovieService::movie_find(const unsigned int id)
{
	return repository->find(id);
}

void MovieService::movie_update(const unsigned int id, const std::string & new_title, const std::string & new_genre, const std::string & new_actor, const unsigned int new_release_year)
{
	Movie new_movie{ id, new_title, new_genre, new_actor, new_release_year };
	validator.validate(new_movie);
	new_movie.set_history(repository->find(id).get_history());
	new_movie.add_to_history("Updated to " + new_title + " / " + new_genre + " / " + new_actor + " / " + std::to_string(new_release_year));
	Movie original = repository->update(id, new_movie);
	undo_actions.push_back(std::make_unique<UndoUpdate>(original, repository));
	notify_observers();
}

void MovieService::movie_delete(const unsigned int id)
{
	Movie deleted = repository->remove(id);
	deleted.add_to_history("REMOVED");
	undo_actions.push_back(std::make_unique<UndoDelete>(deleted, repository));
	notify_observers();
}

void MovieService::movie_undo()
{
	if (undo_actions.size() == 0)
		throw RepositoryException{ "There's nothing to undo!" };
	undo_actions.back()->do_undo();
	undo_actions.pop_back();
	notify_observers();
}

std::string MovieService::movie_get_history(const unsigned int id)
{
	return repository->find(id).get_history();
}

bool compare_by_id_asc(const Movie& movie1, const Movie& movie2) noexcept
{
	return movie1.get_id() < movie2.get_id();
}

std::vector<Movie>& MovieService::movie_get_all() noexcept
{
	std::vector<Movie>& all_movies = repository->get_all();
	std::sort(all_movies.begin(), all_movies.end(), compare_by_id_asc);
	return repository->get_all();
}

std::vector<Movie> MovieService::movie_get_all_filtered_by_title(const std::string filter)
{
	std::vector<Movie> all_movies = repository->get_all();
	std::vector<Movie> movies_filtered;
	std::copy_if(all_movies.begin(), all_movies.end(), std::back_inserter(movies_filtered), [=](const Movie& movie) noexcept {return (movie.get_title().find(filter) != std::string::npos); });
	return movies_filtered;
}

std::vector<Movie> MovieService::movie_get_all_filtered_by_release_year(const unsigned int filter)
{
	std::vector<Movie> all_movies = repository->get_all();
	std::vector<Movie> movies_filtered;
	std::copy_if(all_movies.begin(), all_movies.end(), std::back_inserter(movies_filtered), [=](const Movie& movie) noexcept {return (movie.get_release_year() == filter); });
	return movies_filtered;
}

bool compare_by_title_asc(const Movie& movie1, const Movie& movie2) noexcept
{
	return (movie1.get_title() < movie2.get_title());
}

bool compare_by_title_desc(const Movie& movie1, const Movie& movie2) noexcept
{
	return (movie1.get_title() > movie2.get_title());
}

bool compare_by_genre_asc(const Movie& movie1, const Movie& movie2) noexcept
{
	return (movie1.get_genre() < movie2.get_genre());
}

bool compare_by_genre_desc(const Movie& movie1, const Movie& movie2) noexcept
{
	return (movie1.get_genre() > movie2.get_genre());
}

bool compare_by_actor_asc(const Movie& movie1, const Movie& movie2) noexcept
{
	return (movie1.get_actor() < movie2.get_actor());
}

bool compare_by_actor_desc(const Movie& movie1, const Movie& movie2) noexcept
{
	return (movie1.get_actor() > movie2.get_actor());
}

bool compare_by_release_year_asc(const Movie& movie1, const Movie& movie2) noexcept
{
	return (movie1.get_release_year() < movie2.get_release_year());
}

bool compare_by_release_year_desc(const Movie& movie1, const Movie& movie2) noexcept
{
	return (movie1.get_release_year() > movie2.get_release_year());
}

std::vector<Movie> MovieService::movie_get_all_sorted_by_title(const bool desc)
{
	std::vector<Movie> all_movies = repository->get_all();
	std::sort(all_movies.begin(), all_movies.end(), desc ? compare_by_title_desc : compare_by_title_asc);
	return all_movies;
}

std::vector<Movie> MovieService::movie_get_all_sorted_by_genre(const bool desc)
{
	std::vector<Movie> all_movies = repository->get_all();
	std::sort(all_movies.begin(), all_movies.end(), desc ? compare_by_genre_desc : compare_by_genre_asc);
	return all_movies;
}

std::vector<Movie> MovieService::movie_get_all_sorted_by_actor(const bool desc)
{
	std::vector<Movie> all_movies = repository->get_all();
	std::sort(all_movies.begin(), all_movies.end(), desc ? compare_by_actor_desc : compare_by_actor_asc);
	return all_movies;
}

std::vector<Movie> MovieService::movie_get_all_sorted_by_release_year(const bool desc)
{
	std::vector<Movie> all_movies = repository->get_all();
	std::sort(all_movies.begin(), all_movies.end(), desc ? compare_by_release_year_desc : compare_by_release_year_asc);
	return all_movies;
}

void MovieService::movie_add_to_cart(const unsigned int id)
{
	// raise RepositoryException if there's no movie with given id
	Movie& to_add = repository->find(id);
	// mark it's in
	to_add.set_is_in_shopping_cart(true);
	notify_observers();
}

void MovieService::movie_remove_from_cart(const unsigned int id)
{
	// raise RepositoryException if there's no movie with given id
	Movie& to_remove = repository->find(id);
	// mark it's out
	to_remove.set_is_in_shopping_cart(false);
	notify_observers();
}

size_t MovieService::movie_size_cart()
{
	auto& all_movies = repository->get_all();
	return std::count_if(all_movies.begin(), all_movies.end(), [](const Movie& movie) noexcept {return movie.get_is_in_shopping_cart() == true; });
}

void MovieService::movie_empty_cart()
{
	auto& all_movies = repository->get_all();
	// change is_in_shopping_cart to false for each movie
	std::for_each(all_movies.begin(), all_movies.end(), [](Movie& movie) noexcept {movie.set_is_in_shopping_cart(false); });
	notify_observers();
}

std::vector<Movie> MovieService::movie_get_all_cart()
{
	auto& all_movies = repository->get_all();
	std::vector<Movie> cart_movies;
	std::copy_if(all_movies.begin(), all_movies.end(), std::back_inserter(cart_movies), [](const Movie& movie) noexcept {return movie.get_is_in_shopping_cart() == true; });
	return cart_movies;
}

void MovieService::movie_generate_random_cart()
{
	// clear first
	movie_empty_cart();

	auto& all_movies = repository->get_all();
	// total number of movies
	const size_t repository_size = repository->size();

	// how many random movies will be added to shopping cart
	std::mt19937 mt{ std::random_device{}() };
	const std::uniform_int_distribution<> dist(1, (int)repository_size);
	unsigned int how_many = dist(mt);

	// add how_many random movies to cart
	while (how_many)
	{
		const int random_index = dist(mt);
		if (all_movies[random_index - 1].get_is_in_shopping_cart() == false)
		{
			all_movies[random_index - 1].set_is_in_shopping_cart(true);
			how_many--;
		}
	}

	notify_observers();
}

UndoAdd::UndoAdd(const Movie& added, Repository<Movie>* repository) : added{ added }, repository{ repository }
{
}

void UndoAdd::do_undo()
{
	repository->remove(added.get_id());
}

UndoUpdate::UndoUpdate(const Movie& updated, Repository<Movie>* repository) : updated{ updated }, repository{ repository }
{
}

void UndoUpdate::do_undo()
{
	repository->update(updated.get_id(), updated);
}

UndoDelete::UndoDelete(const Movie& deleted, Repository<Movie>* repository) : deleted{ deleted }, repository{ repository }
{
}

void UndoDelete::do_undo()
{
	repository->store(deleted);
}

void Observable::notify_observers()
{
	std::for_each(observers.begin(), observers.end(), [](Observer* o) {o->update(); });
}

void Observable::add_observer(Observer * o)
{
	observers.push_back(o);
}
