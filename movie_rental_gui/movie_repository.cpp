#include "movie_repository.h"

#include <fstream>
#include <algorithm>

bool MovieRepository::exist(const unsigned int id) noexcept
{
	auto it = std::find_if(movies.begin(), movies.end(), [=](const Movie& movie) noexcept {return movie.get_id() == id; });
	return it < movies.end();
}

void MovieRepository::store(const Movie& movie)
{
	if (exist(movie.get_id()))
		throw RepositoryException("This ID already exists!");
	movies.push_back(movie);
}

Movie& MovieRepository::find(const unsigned int id)
{
	auto it = std::find_if(movies.begin(), movies.end(), [=](const Movie& movie) noexcept {return movie.get_id() == id; });
	if (it == movies.end())
		throw RepositoryException("No movie with given ID!");
	return *it;
}

Movie MovieRepository::update(const unsigned int id, const Movie& new_movie)
{
	if (!exist(id))
		throw RepositoryException("No movie with given ID!");
	Movie original = find(id);
	std::replace_if(movies.begin(), movies.end(), [=](const Movie& movie) noexcept {return movie.get_id() == id; }, new_movie);
	return original;
}

Movie MovieRepository::remove(const unsigned int id)
{
	Movie deleted = find(id); // raises ex if not found
	movies.erase(std::remove_if(movies.begin(), movies.end(), [=](const Movie& movie) noexcept {return movie.get_id() == id; }), movies.end());
	return deleted;
}

size_t MovieRepository::size() noexcept
{
	return movies.size();
}

std::vector<Movie>& MovieRepository::get_all() noexcept
{
	return movies;
}

void MovieRepository::clear()
{
	movies.clear();
}

std::string RepositoryException::get_err_msg() const
{
	return err_msg;
}

void MovieRepositoryFile::load_from_file()
{
	std::ifstream fin;
	fin.open(file_name, std::ios::in);
	MovieRepository::clear();
	std::string id, title, genre, actor, year, in; // in = is_in_shopping_cart
	while (!fin.eof())
	{
		std::getline(fin, id, ',');
		if (id.size() == 0) break;
		std::getline(fin, title, ',');
		std::getline(fin, genre, ',');
		std::getline(fin, actor, ',');
		std::getline(fin, year, ',');
		std::getline(fin, in);
		Movie m{ (unsigned int)std::stoi(id), title, genre, actor, (unsigned int)std::stoi(year), (unsigned int)std::stoi(in) };
		MovieRepository::store(m);
	}
	fin.close();
}

void MovieRepositoryFile::save_to_file()
{
	std::ofstream fout;
	fout.open(file_name, std::ios::out);
	auto& all_movies = MovieRepository::get_all();
	for (const auto& m : all_movies)
	{
		fout << m.get_id() << ',';
		fout << m.get_title() << ',';
		fout << m.get_genre() << ',';
		fout << m.get_actor() << ',';
		fout << m.get_release_year() << ',';
		fout << (unsigned int)m.get_is_in_shopping_cart() << std::endl;
	}
	fout.close();
}

MovieRepositoryFile::MovieRepositoryFile(const std::string file_name) : MovieRepository(), file_name{ file_name }
{
}

void MovieRepositoryFile::store(const Movie & m)
{
	load_from_file();
	MovieRepository::store(m);
	save_to_file();
}

Movie& MovieRepositoryFile::find(const unsigned int id)
{
	load_from_file();
	return MovieRepository::find(id);
}

Movie MovieRepositoryFile::update(const unsigned int id, const Movie & new_movie)
{
	load_from_file();
	Movie original = MovieRepository::update(id, new_movie);
	save_to_file();
	return original;
}

Movie MovieRepositoryFile::remove(const unsigned int id)
{
	//load_from_file();
	Movie deleted = MovieRepository::remove(id);
	save_to_file();
	return deleted;
}

size_t MovieRepositoryFile::size() noexcept
{
	load_from_file();
	return MovieRepository::size();
}

std::vector<Movie>& MovieRepositoryFile::get_all() noexcept
{
	load_from_file();
	return MovieRepository::get_all();
}

#include <random>
#include <iostream>

DictionaryRepository::DictionaryRepository(const double probability) : probability{ probability }
{
	//std::cout << "Probability: " << probability << std::endl;
}

void DictionaryRepository::store(const Movie & movie)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0.0, 1.0);
	double random = dis(gen);
	//std::cout << "Store: " << random << std::endl;
	if (random < probability)
		throw RepositoryException("Random exception!");
	auto it = movies.insert(std::pair<unsigned int, Movie>(movie.get_id(), movie));
	if (it.second == false)
		throw RepositoryException("This ID already exists!");
}

Movie & DictionaryRepository::find(const unsigned int id)
{
	auto it = movies.find(id);
	if (it == movies.end())
		throw RepositoryException("No movie with given ID!");
	return it->second;
}

Movie DictionaryRepository::update(const unsigned int id, const Movie & new_movie)
{
	auto it = movies.find(id);
	if (it == movies.end())
		throw RepositoryException("No movie with given ID!");
	Movie original = it->second;
	it->second = new_movie;
	return original;
}

Movie DictionaryRepository::remove(const unsigned int id)
{
	auto it = movies.find(id);
	if (it == movies.end())
		throw RepositoryException("No movie with given ID!");
	Movie deleted = it->second;
	movies.erase(it);
	return deleted;
}

size_t DictionaryRepository::size() noexcept
{
	return movies.size();
}

std::vector<Movie>& DictionaryRepository::get_all() noexcept
{
	std::vector<Movie>* all_movies = new std::vector<Movie>();
	for (const auto& m : movies)
		all_movies->push_back(m.second);
	return *all_movies;
}

void DictionaryRepository::clear()
{
	movies.clear();
}
