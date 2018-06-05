#pragma once

#include "movie.h"
#include "movie_service.h"
#include "custom_list_model.h"
#include "custom_table_model.h"

#include <qwidget.h>
#include <qlistwidget.h>
#include <qtablewidget.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qboxlayout.h>
#include <vector>
#include <qlabel.h>

class ShoppingCartWindow;
class ShoppingCartWindowRDONLY;

class MainWindow : public QWidget, public Observer {
	// refs to linked windows
	ShoppingCartWindow* shopping_cart_window;
	ShoppingCartWindowRDONLY* shopping_cart_window_rdonly;

	MovieService& service;
	QTableView* table_movies;
	MyTableModel* table_movies_model;
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
	QPushButton* button_shopping_cart_show_rdonly;
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
	void movie_shopping_cart_show_rdonly();
public:
	MainWindow(MovieService& service, ShoppingCartWindow* shopping_cart_window, ShoppingCartWindowRDONLY* shopping_cart_window_rdonly);
	void update() override;
};


class ShoppingCartWindow : public QWidget, public Observer {
	MovieService& service;
	QListView* shopping_cart_list;
	MyListModel* shopping_cart_list_model;
	QPushButton* button_generate_random;
	QPushButton* button_empty;
	void init();
	void connect_signals_slots();
	void reload_list(std::vector<Movie> movies);
	void shopping_cart_generate_random();
	void shopping_cart_empty();
public:
	ShoppingCartWindow(MovieService& service);
	void update() override;
	void show();
};


class ShoppingCartWindowRDONLY :public QWidget, public Observer {
	MovieService& service;
	void paintEvent(QPaintEvent* e) override;
public:
	ShoppingCartWindowRDONLY(MovieService& service);
	void update() override;
	void show();
};