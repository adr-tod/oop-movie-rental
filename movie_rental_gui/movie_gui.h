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
	QTableWidget* table;
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
public:
	MainWindow(MovieService& service);
};