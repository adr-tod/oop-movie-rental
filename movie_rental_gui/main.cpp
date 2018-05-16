#include "movie_ui.h"
#include "movie_gui.h"

// FOR TESTS
#define CATCH_CONFIG_RUNNER
#include "catch.hpp" // include after defining CATCH_CONFIG_RUNNER

#include <QtWidgets>

int main(int argc, char *argv[])
{
	// RUN TESTS
	Catch::Session().run(argc, argv);

	// RUN APP
	QApplication a(argc, argv);
	//a.setStyle("fusion");

	Repository<Movie>* repository = new MovieRepositoryFile{ "movies.txt" };
	MovieValidator validator;
	MovieService service{ repository, validator };
	//MovieUI ui{ service };
	//ui.start();
	MainWindow gui{ service };
	//gui.showMaximized();
	gui.show();

	return a.exec();
}