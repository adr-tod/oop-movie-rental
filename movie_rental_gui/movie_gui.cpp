#include "movie_gui.h"

#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QVBoxLayout>
#include <qlist.h>
#include <qgroupbox.h>

#include <algorithm>
#include <string>

void MainWindow::init_gui()
{
	// window size
	setFixedWidth(1340);
	setMinimumHeight(615);

	// main horizontal layout => LHS and RHS
	QHBoxLayout* main_layout = new QHBoxLayout;
	setLayout(main_layout);

	// LHS 
	QWidget* lhs = new QWidget;
	QVBoxLayout* lhs_layout = new QVBoxLayout;
	lhs->setLayout(lhs_layout);

	main_layout->addWidget(lhs);

	// movies table
	table = new QTableWidget;
	table->setColumnCount(5);
	table->setColumnWidth(0, 30);
	table->setColumnWidth(1, 175);
	table->setColumnWidth(2, 125);
	table->setColumnWidth(4, 100);
	table->setHorizontalHeaderItem(0, new QTableWidgetItem("ID"));
	table->setHorizontalHeaderItem(1, new QTableWidgetItem("Title"));
	table->setHorizontalHeaderItem(2, new QTableWidgetItem("Genre"));
	table->setHorizontalHeaderItem(3, new QTableWidgetItem("Actor"));
	table->setHorizontalHeaderItem(4, new QTableWidgetItem("Year"));

	lhs_layout->addWidget(table);

	// RHS
	QWidget* rhs = new QWidget;
	QVBoxLayout* rhs_layout = new QVBoxLayout;
	rhs->setLayout(rhs_layout);

	main_layout->addWidget(rhs);

	// input data form for CRUD
	QWidget* input_data = new QWidget;
	QFormLayout* input_data_form = new QFormLayout;
	input_data->setLayout(input_data_form);
	input_data_form->addRow(new QLabel("ID:"), text_id = new QLineEdit);
	input_data_form->addRow(new QLabel("Title:"), text_title = new QLineEdit);
	input_data_form->addRow(new QLabel("Genre:"), text_genre = new QLineEdit);
	input_data_form->addRow(new QLabel("Actor:"), text_actor = new QLineEdit);
	input_data_form->addRow(new QLabel("Year:"), text_release_year = new QLineEdit);

	rhs_layout->addWidget(input_data);

	// CRUD buttons
	QGroupBox* crud_buttons = new QGroupBox("CRUD");
	crud_buttons->setAlignment(Qt::AlignCenter);
	QVBoxLayout* crud_buttons_layout = new QVBoxLayout;
	crud_buttons->setLayout(crud_buttons_layout);
	QHBoxLayout* crud_buttons_layout_first_row = new QHBoxLayout;
	QHBoxLayout* crud_buttons_layout_second_row = new QHBoxLayout;
	QHBoxLayout* crud_buttons_layout_third_row = new QHBoxLayout;
	crud_buttons_layout->addLayout(crud_buttons_layout_first_row);
	crud_buttons_layout->addLayout(crud_buttons_layout_second_row);
	crud_buttons_layout->addLayout(crud_buttons_layout_third_row);

	crud_buttons_layout_first_row->addWidget(button_add = new QPushButton("Add"));
	crud_buttons_layout_first_row->addWidget(button_update = new QPushButton("Update"));
	crud_buttons_layout_second_row->addWidget(button_find = new QPushButton("Find"));
	crud_buttons_layout_second_row->addWidget(button_delete = new QPushButton("Delete"));
	crud_buttons_layout_third_row->addWidget(button_undo = new QPushButton("Undo"));

	rhs_layout->addWidget(crud_buttons);

	// filter buttons
	QGroupBox* filter_buttons = new QGroupBox("FILTER");
	filter_buttons->setAlignment(Qt::AlignCenter);
	QHBoxLayout* filter_buttons_layout = new QHBoxLayout;
	filter_buttons->setLayout(filter_buttons_layout);
	filter_buttons_layout->addWidget(button_filter_title = new QPushButton("By title"));
	filter_buttons_layout->addWidget(button_filter_year = new QPushButton("By year"));

	rhs_layout->addWidget(filter_buttons);

	// sort buttons
	QGroupBox* sort_buttons = new QGroupBox("SORT");
	sort_buttons->setAlignment(Qt::AlignCenter);
	QHBoxLayout* sort_buttons_layout = new QHBoxLayout;
	sort_buttons->setLayout(sort_buttons_layout);
	sort_buttons_layout->addWidget(button_sort_title = new QPushButton("By title"));
	sort_buttons_layout->addWidget(button_sort_genre = new QPushButton("By genre"));
	sort_buttons_layout->addWidget(button_sort_year = new QPushButton("By year"));

	rhs_layout->addWidget(sort_buttons);
}

bool desc_title = false;
bool desc_genre = false;
bool desc_release_year = false;

void MainWindow::connect_signals_slots()
{
	// CRUD buttons
	QObject::connect(button_add, &QPushButton::clicked, this, &MainWindow::movie_add);
	QObject::connect(button_find, &QPushButton::clicked, this, &MainWindow::movie_find);
	QObject::connect(button_update, &QPushButton::clicked, this, &MainWindow::movie_update);
	QObject::connect(button_delete, &QPushButton::clicked, this, &MainWindow::movie_delete);
	QObject::connect(button_undo, &QPushButton::clicked, this, &MainWindow::movie_undo);

	// Sort buttons
	// (sort and reload list when button is pressed)
	QObject::connect(button_sort_title, &QPushButton::clicked, [&]() {
		reload_table(service.movie_get_all_sorted_by_title(desc_title));
		if (desc_title) desc_title = 0;
		else desc_title = 1;
	});
	QObject::connect(button_sort_genre, &QPushButton::clicked, [&]() {
		reload_table(service.movie_get_all_sorted_by_genre(desc_genre));
		if (desc_genre) desc_genre = 0;
		else desc_genre = 1;
	});
	QObject::connect(button_sort_year, &QPushButton::clicked, [&]() {
		reload_table(service.movie_get_all_sorted_by_release_year(desc_release_year));
		if (desc_release_year) desc_release_year = 0;
		else desc_release_year = 1;
	});

	// Filter buttons
	QObject::connect(button_filter_title, &QPushButton::clicked, this, &MainWindow::movie_filter_title);
	QObject::connect(button_filter_year, &QPushButton::clicked, this, &MainWindow::movie_filter_year);
}

void MainWindow::movie_add()
{
	try {
		service.movie_add(text_id->text().toUInt(), text_title->text().toStdString(), text_genre->text().toStdString(), text_actor->text().toStdString(), text_release_year->text().toUInt());
		reload_table(service.movie_get_all());
		QMessageBox output;
		output.setText("Succes!");
		output.exec();
	}
	catch (const RepositoryException& ex) {
		QMessageBox::warning(this, "Warning", QString::fromStdString(ex.get_err_msg()));
	}
	catch (const ValidatorException& ex) {
		std::string err_msg;
		for (const auto& err : ex.get_err_msg())
			err_msg += err + '\n';
		QMessageBox::warning(this, "Warning", QString::fromStdString(err_msg));
	}
}

void MainWindow::movie_find()
{
	try {
		Movie m = service.movie_find(text_id->text().toUInt());
		QMessageBox output;
		output.setText(QString::fromStdString(m.get_title()));
		output.exec();
	}
	catch (const RepositoryException& ex) {
		QMessageBox::warning(this, "Warning", QString::fromStdString(ex.get_err_msg()));
	}
}

void MainWindow::movie_update()
{
	try {
		service.movie_update(text_id->text().toUInt(), text_title->text().toStdString(), text_genre->text().toStdString(), text_actor->text().toStdString(), text_release_year->text().toUInt());
		reload_table(service.movie_get_all());
		QMessageBox output;
		output.setText("Succes!");
		output.exec();
	}
	catch (const RepositoryException& ex) {
		QMessageBox::warning(this, "Warning", QString::fromStdString(ex.get_err_msg()));
	}
	catch (const ValidatorException& ex) {
		std::string err_msg;
		for (const auto& err : ex.get_err_msg())
			err_msg += err + '\n';
		QMessageBox::warning(this, "Warning", QString::fromStdString(err_msg));
	}
}

void MainWindow::movie_delete()
{
	try {
		service.movie_delete(text_id->text().toUInt());
		reload_table(service.movie_get_all());
		QMessageBox output;
		output.setText("Succes!");
		output.exec();
	}
	catch (const RepositoryException& ex) {
		QMessageBox::warning(this, "Warning", QString::fromStdString(ex.get_err_msg()));
	}
}

void MainWindow::movie_undo()
{
	try {
		service.movie_undo();
		reload_table(service.movie_get_all());
		QMessageBox output;
		output.setText("Succes!");
		output.exec();
	}
	catch (const RepositoryException& ex) {
		QMessageBox::warning(this, "Warning", QString::fromStdString(ex.get_err_msg()));
	}
}

void MainWindow::movie_filter_title()
{
	auto movies = service.movie_get_all_filtered_by_title(text_title->text().toStdString());
	if (movies.size() == 0) {
		QMessageBox::warning(this, "Warning", "No movie matches the filter!");
	}
	else {
		QTableWidget* filtered_movies = new QTableWidget;
		filtered_movies->setMinimumWidth(625);
		filtered_movies->setMinimumHeight(300);
		filtered_movies->setColumnCount(5);
		filtered_movies->setRowCount((int)movies.size());
		filtered_movies->setColumnWidth(0, 30);
		filtered_movies->setColumnWidth(1, 175);
		filtered_movies->setColumnWidth(2, 125);
		filtered_movies->setColumnWidth(4, 100);
		filtered_movies->setHorizontalHeaderItem(0, new QTableWidgetItem("ID"));
		filtered_movies->setHorizontalHeaderItem(1, new QTableWidgetItem("Title"));
		filtered_movies->setHorizontalHeaderItem(2, new QTableWidgetItem("Genre"));
		filtered_movies->setHorizontalHeaderItem(3, new QTableWidgetItem("Actor"));
		filtered_movies->setHorizontalHeaderItem(4, new QTableWidgetItem("Year"));
		int current_row = 0;
		for (const auto& m : movies) {
			QTableWidgetItem* id = new QTableWidgetItem(QString::number(m.get_id()));
			QTableWidgetItem* title = new QTableWidgetItem(QString::fromStdString(m.get_title()));
			QTableWidgetItem* genre = new QTableWidgetItem(QString::fromStdString(m.get_genre()));
			QTableWidgetItem* actor = new QTableWidgetItem(QString::fromStdString(m.get_actor()));
			QTableWidgetItem* release_year = new QTableWidgetItem(QString::number(m.get_release_year()));
			id->setTextAlignment(Qt::AlignCenter);
			title->setTextAlignment(Qt::AlignCenter);
			genre->setTextAlignment(Qt::AlignCenter);
			actor->setTextAlignment(Qt::AlignCenter);
			release_year->setTextAlignment(Qt::AlignCenter);
			filtered_movies->setItem(current_row, 0, id);
			filtered_movies->setItem(current_row, 1, title);
			filtered_movies->setItem(current_row, 2, genre);
			filtered_movies->setItem(current_row, 3, actor);
			filtered_movies->setItem(current_row, 4, release_year);
			current_row++;
		}
		filtered_movies->show();
	}
}

void MainWindow::movie_filter_year()
{
	auto movies = service.movie_get_all_filtered_by_release_year(text_release_year->text().toUInt());
	if (movies.size() == 0) {
		QMessageBox::warning(this, "Warning", "No movie matches the filter!");
	}
	else {
		QTableWidget* filtered_movies = new QTableWidget;
		filtered_movies->setMinimumWidth(625);
		filtered_movies->setMinimumHeight(300);
		filtered_movies->setColumnCount(5);
		filtered_movies->setRowCount((int)movies.size());
		filtered_movies->setColumnWidth(0, 30);
		filtered_movies->setColumnWidth(1, 175);
		filtered_movies->setColumnWidth(2, 125);
		filtered_movies->setColumnWidth(4, 100);
		filtered_movies->setHorizontalHeaderItem(0, new QTableWidgetItem("ID"));
		filtered_movies->setHorizontalHeaderItem(1, new QTableWidgetItem("Title"));
		filtered_movies->setHorizontalHeaderItem(2, new QTableWidgetItem("Genre"));
		filtered_movies->setHorizontalHeaderItem(3, new QTableWidgetItem("Actor"));
		filtered_movies->setHorizontalHeaderItem(4, new QTableWidgetItem("Year"));
		int current_row = 0;
		for (const auto& m : movies) {
			QTableWidgetItem* id = new QTableWidgetItem(QString::number(m.get_id()));
			QTableWidgetItem* title = new QTableWidgetItem(QString::fromStdString(m.get_title()));
			QTableWidgetItem* genre = new QTableWidgetItem(QString::fromStdString(m.get_genre()));
			QTableWidgetItem* actor = new QTableWidgetItem(QString::fromStdString(m.get_actor()));
			QTableWidgetItem* release_year = new QTableWidgetItem(QString::number(m.get_release_year()));
			id->setTextAlignment(Qt::AlignCenter);
			title->setTextAlignment(Qt::AlignCenter);
			genre->setTextAlignment(Qt::AlignCenter);
			actor->setTextAlignment(Qt::AlignCenter);
			release_year->setTextAlignment(Qt::AlignCenter);
			filtered_movies->setItem(current_row, 0, id);
			filtered_movies->setItem(current_row, 1, title);
			filtered_movies->setItem(current_row, 2, genre);
			filtered_movies->setItem(current_row, 3, actor);
			filtered_movies->setItem(current_row, 4, release_year);
			current_row++;
		}
		filtered_movies->show();
	}
}

MainWindow::MainWindow(MovieService& service)
	: service{ service }
{
	init_gui();
	connect_signals_slots();
	reload_table(service.movie_get_all());
}

void MainWindow::reload_table(const std::vector<Movie>& movies)
{
	table->clearContents();
	table->setRowCount((int)movies.size());
	int current_row = 0;
	for (const auto& m : movies) {
		QTableWidgetItem* id = new QTableWidgetItem(QString::number(m.get_id()));
		QTableWidgetItem* title = new QTableWidgetItem(QString::fromStdString(m.get_title()));
		QTableWidgetItem* genre = new QTableWidgetItem(QString::fromStdString(m.get_genre()));
		QTableWidgetItem* actor = new QTableWidgetItem(QString::fromStdString(m.get_actor()));
		QTableWidgetItem* release_year = new QTableWidgetItem(QString::number(m.get_release_year()));
		id->setTextAlignment(Qt::AlignCenter);
		title->setTextAlignment(Qt::AlignCenter);
		genre->setTextAlignment(Qt::AlignCenter);
		actor->setTextAlignment(Qt::AlignCenter);
		release_year->setTextAlignment(Qt::AlignCenter);
		table->setItem(current_row, 0, id);
		table->setItem(current_row, 1, title);
		table->setItem(current_row, 2, genre);
		table->setItem(current_row, 3, actor);
		table->setItem(current_row, 4, release_year);
		current_row++;
	}
}