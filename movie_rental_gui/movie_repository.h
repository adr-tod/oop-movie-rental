#pragma once

#include "movie.h"

#include <vector>
#include <map>

template <class T>
class Repository
{
private:
	std::vector<T> elements;
public:
	virtual ~Repository() = default;
	virtual void store(const T& element) = 0;
	virtual T& find(const unsigned int id) = 0;
	virtual T update(const unsigned int id, const T& new_element) = 0;
	virtual T remove(const unsigned int id) = 0;
	virtual size_t size() = 0;
	virtual std::vector<T>& get_all() = 0;
	virtual void clear() = 0;
};

class DictionaryRepository : public Repository<Movie>
{
private:
	std::map<unsigned int, Movie> movies;
	double probability;
public:
	DictionaryRepository(const double probability);
	void store(const Movie& movie);
	Movie& find(const unsigned int id);
	Movie update(const unsigned int id, const Movie& new_movie);
	Movie remove(const unsigned int id);
	size_t size() noexcept;
	std::vector<Movie>& get_all() noexcept;
	void clear();
};

class MovieRepository: public Repository<Movie>
{
private:
	std::vector<Movie> movies;
	bool exist(const unsigned int id) noexcept;
public:
	MovieRepository() = default;
	// MovieRepository(const Movie& other) = delete;
	void store(const Movie& movie);
	Movie& find(const unsigned int id);
	Movie update(const unsigned int id, const Movie& new_movie);
	Movie remove(const unsigned int id);
	size_t size() noexcept;
	std::vector<Movie>& get_all() noexcept;
	void clear();
};

class MovieRepositoryFile : public MovieRepository
{
private:
	std::string file_name;
	void load_from_file();
	void save_to_file();
public:
	MovieRepositoryFile(const std::string file_name);
	void store(const Movie& m);
	Movie& find(const unsigned int id);
	Movie update(const unsigned int id, const Movie& new_movie);
	Movie remove(const unsigned int id);
	size_t size() noexcept;
	std::vector<Movie>& get_all() noexcept;
};


class RepositoryException
{
private:
	std::string err_msg;
public:
	RepositoryException(std::string msg) :err_msg{ msg } {};
	std::string get_err_msg() const;
};

