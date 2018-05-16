#pragma once

#include "movie_repository.h"
#include "movie_validator.h"

#include <memory>

class UndoAction;

class MovieService
{
private:
	Repository<Movie>* repository;
	MovieValidator& validator;
	std::vector<std::unique_ptr<UndoAction>> undo_actions;
public:
	MovieService(Repository<Movie>* repository, MovieValidator& validator) noexcept : repository{ repository }, validator{ validator } {};
	void movie_add(const unsigned int id, const std::string& title, const std::string& genre, const std::string& actor, const unsigned int release_year);
	Movie& movie_find(const unsigned int id);
	void movie_update(const unsigned int id, const std::string& new_title, const std::string& new_genre, const std::string& new_actor, const unsigned int new_release_year);
	void movie_delete(const unsigned int id);
	void movie_undo();
	std::vector<Movie>& movie_get_all() noexcept;
	std::vector<Movie> movie_get_all_filtered_by_title(const std::string filter);
	std::vector<Movie> movie_get_all_filtered_by_release_year(const unsigned int filter);
	std::vector<Movie> movie_get_all_sorted_by_title(const bool desc);
	std::vector<Movie> movie_get_all_sorted_by_genre(const bool desc);
	std::vector<Movie> movie_get_all_sorted_by_actor(const bool desc);
	std::vector<Movie> movie_get_all_sorted_by_release_year(const bool desc);
	void movie_add_to_cart(const unsigned int id);
	void movie_remove_from_cart(const unsigned int id);
	size_t movie_size_cart();
	void movie_empty_cart();
	std::vector<Movie> movie_get_all_cart();
	void movie_generate_random_cart();
};

class UndoAction
{
public:
	virtual ~UndoAction() = default;
	virtual void do_undo() = 0;
};

class UndoAdd : public UndoAction
{
private:
	Movie added;
	Repository<Movie>* repository;
public:
	UndoAdd(const Movie& added, Repository<Movie>* repository);
	void do_undo() override;
};

class UndoUpdate : public UndoAction
{
private:
	Movie updated;
	Repository<Movie>* repository;
public:
	UndoUpdate(const Movie& updated, Repository<Movie>* repository);
	void do_undo() override;
};

class UndoDelete : public UndoAction
{
private:
	Movie deleted;
	Repository<Movie>* repository;
public:
	UndoDelete(const Movie& deleted, Repository<Movie>* repository);
	void do_undo() override;
};
