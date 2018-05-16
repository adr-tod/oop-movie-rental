//#include "movie_ui.h"
//
//void add_some_movies(MovieService& service)
//{
//	service.movie_add(1, "Hacksaw Ridge", "War", "Andrew Garfield", 2016);
//	service.movie_add(2, "Interstellar", "Sci-Fi", "Matthew McConaughey", 2014);
//	service.movie_add(3, "V for Vendetta", "Action", "Hugo Weaving", 2005);
//	service.movie_add(4, "The Godfather", "Mafia", "Al Pacino", 1972);
//	service.movie_add(5, "Grown Ups", "Comedy", "Adam Sandler", 2010);
//	service.movie_add(6, "I Origins", "Drama", "Michael Pitt", 2014);
//	service.movie_add(7, "Pirates of the Caribbean: The Curse of the Black Pearl", "Sci-Fi", "Johnny Depp", 2003);
//	service.movie_add(8, "Life Is Beautiful", "Drama", "Roberto Benigni", 1997);
//	service.movie_add(9, "The Shawshank Redemption", "Drama", "Tim Robbins", 1994);
//	service.movie_add(10, "Yes Man", "Comedy", "Jim Carrey", 2008);
//}
//
//#include <iostream>
//
//int main()
//{
//
//	Repository<Movie>* repository = new DictionaryRepository{ 0.0 };
//	//Repository<Movie>* repository = new MovieRepositoryFile{ "movies.txt" };
//
//	MovieValidator validator;
//	MovieService service{ repository, validator };
//	MovieUI ui{ service };
//	try
//	{
//		add_some_movies(service);
//	}
//	catch (const RepositoryException& ex)
//	{
//		std::cout << ex.get_err_msg() << std::endl;
//	}
//	ui.start();
//}

//#define _CRTDBG_MAP_ALLOC  
//#include <stdlib.h>  
//#include <crtdbg.h>
//
//int main()
//{
//	_CrtDumpMemoryLeaks();
//	return 0;
//}