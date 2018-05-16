#pragma once

#include "movie.h"
#include "movie_service.h"

#include <qwidget.h>
#include <qlistwidget.h>
#include <qtablewidget.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qboxlayout.h>
#include <vector>
#include <qlabel.h>


class MainWindow : public QWidget {
private:
	MovieService & service;
	QTableWidget* table_movies;
	QTableWidget* table_shopping_cart;
	QLineEdit* text_id;
	QLineEdit* text_title;
	QLineEdit* text_genre;
	QLineEdit* text_actor;
	QLineEdit* text_release_year;
	QPushButton* button_add;
	QPushButton* button_find;
	QPushButton* button_update;
	QPushButton* button_delete;
	QPushButton* button_undo;
	QPushButton* button_filter_title;
	QPushButton* button_filter_year;
	QPushButton* button_sort_title;
	QPushButton* button_sort_genre;
	QPushButton* button_sort_year;
	QPushButton* button_shopping_cart_add;
	QPushButton* button_shopping_cart_delete;
	QPushButton* button_shopping_cart_empty;
	QPushButton* button_shopping_cart_generate_random;
	QPushButton* button_shopping_cart_show;
	void init_gui();
	void connect_signals_slots();
	void reload_table(const std::vector<Movie>& movies);
	void movie_add();
	void movie_find();
	void movie_update();
	void movie_delete();
	void movie_undo();
	void movie_filter_title();
	void movie_filter_year();
	void movie_shopping_cart_add();
	void movie_shopping_cart_delete();
	void movie_shopping_cart_empty();
	void movie_shopping_cart_generate_random();
	void movie_shopping_cart_show();
public:
	MainWindow(MovieService& service);
};