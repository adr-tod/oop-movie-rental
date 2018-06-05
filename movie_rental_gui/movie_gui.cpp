#include "movie_gui.h"

#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QVBoxLayout>
#include <qlist.h>
#include <qgroupbox.h>
#include <qpainter.h>
#include <QPaintEvent>

#include <algorithm>
#include <string>

void MainWindow::init_gui()
{
	// window size
	setFixedWidth(1340);
	setMinimumHeight(625);

	// main horizontal layout => LHS and RHS
	QHBoxLayout* main_layout = new QHBoxLayout;
	setLayout(main_layout);

	// LHS 
	QWidget* lhs = new QWidget;
	QVBoxLayout* lhs_layout = new QVBoxLayout;
	lhs->setLayout(lhs_layout);

	main_layout->addWidget(lhs);

	// movies table
	table_movies = new QTableWidget;
	table_movies->setColumnCount(5);
	table_movies->setColumnWidth(0, 30);
	table_movies->setColumnWidth(1, 175);
	table_movies->setColumnWidth(2, 125);
	table_movies->setColumnWidth(4, 100);
	table_movies->setHorizontalHeaderItem(0, new QTableWidgetItem("ID"));
	table_movies->setHorizontalHeaderItem(1, new QTableWidgetItem("Title"));
	table_movies->setHorizontalHeaderItem(2, new QTableWidgetItem("Genre"));
	table_movies->setHorizontalHeaderItem(3, new QTableWidgetItem("Actor"));
	table_movies->setHorizontalHeaderItem(4, new QTableWidgetItem("Year"));

	lhs_layout->addWidget(table_movies);

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

	// shopping cart buttons
	QGroupBox* shopping_cart_buttons = new QGroupBox("SHOPPING CART");
	shopping_cart_buttons->setAlignment(Qt::AlignCenter);
	QVBoxLayout* shopping_cart_buttons_layout = new QVBoxLayout;
	shopping_cart_buttons->setLayout(shopping_cart_buttons_layout);
	QHBoxLayout* shopping_cart_buttons_layout_first_row = new QHBoxLayout;
	QHBoxLayout* shopping_cart_buttons_layout_second_row = new QHBoxLayout;
	shopping_cart_buttons_layout->addLayout(shopping_cart_buttons_layout_first_row);
	shopping_cart_buttons_layout->addLayout(shopping_cart_buttons_layout_second_row);
	shopping_cart_buttons_layout_first_row->addWidget(button_shopping_cart_add = new QPushButton("Add"));
	shopping_cart_buttons_layout_first_row->addWidget(button_shopping_cart_delete = new QPushButton("Delete"));
	shopping_cart_buttons_layout_first_row->addWidget(button_shopping_cart_empty = new QPushButton("Empty"));
	shopping_cart_buttons_layout_second_row->addWidget(button_shopping_cart_generate_random = new QPushButton("Generate random"));
	shopping_cart_buttons_layout_second_row->addWidget(button_shopping_cart_show = new QPushButton("Show"));
	shopping_cart_buttons_layout_second_row->addWidget(button_shopping_cart_show_rdonly = new QPushButton("Show RDONLY"));

	rhs_layout->addWidget(shopping_cart_buttons);
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

	// Shopping cart buttons
	QObject::connect(button_shopping_cart_add, &QPushButton::clicked, this, &MainWindow::movie_shopping_cart_add);
	QObject::connect(button_shopping_cart_delete, &QPushButton::clicked, this, &MainWindow::movie_shopping_cart_delete);
	QObject::connect(button_shopping_cart_empty, &QPushButton::clicked, this, &MainWindow::movie_shopping_cart_empty);
	QObject::connect(button_shopping_cart_generate_random, &QPushButton::clicked, this, &MainWindow::movie_shopping_cart_generate_random);
	QObject::connect(button_shopping_cart_show, &QPushButton::clicked, this, &MainWindow::movie_shopping_cart_show);
	QObject::connect(button_shopping_cart_show_rdonly, &QPushButton::clicked, this, &MainWindow::movie_shopping_cart_show_rdonly);

}

void MainWindow::movie_add()
{
	try {
		service.movie_add(text_id->text().toUInt(), text_title->text().toStdString(), text_genre->text().toStdString(), text_actor->text().toStdString(), text_release_year->text().toUInt());
		reload_table(service.movie_get_all());
		QMessageBox::information(this, "Succes", "Added");
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
		QMessageBox::information(this, "Succes", "Found");
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
		QMessageBox::information(this, "Succes", "Updated");;
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
		QMessageBox::information(this, "Succes", "Deleted");
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
		QMessageBox::information(this, "Succes", "Undoed");
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

void MainWindow::movie_shopping_cart_add()
{
	service.movie_add_to_cart(text_id->text().toUInt());
	QMessageBox::information(this, "Succes", "Added to shopping cart");
}

void MainWindow::movie_shopping_cart_delete()
{
	service.movie_remove_from_cart(text_id->text().toUInt());
	QMessageBox::information(this, "Succes", "Deleted from shopping cart");
}

void MainWindow::movie_shopping_cart_empty()
{
	service.movie_empty_cart();
	QMessageBox::information(this, "Succes", "Cleared the shopping cart");
}

void MainWindow::movie_shopping_cart_generate_random()
{
	service.movie_generate_random_cart();
	QMessageBox::information(this, "Succes", "Generated random shopping cart");
}

void MainWindow::movie_shopping_cart_show()
{
	shopping_cart_window->show();
}

void MainWindow::movie_shopping_cart_show_rdonly()
{
	shopping_cart_window_rdonly->show();
}

MainWindow::MainWindow(MovieService& service, ShoppingCartWindow* shopping_cart_window, ShoppingCartWindowRDONLY* shopping_cart_window_rdonly) : service{ service }, shopping_cart_window{ shopping_cart_window }, shopping_cart_window_rdonly{ shopping_cart_window_rdonly }
{
	init_gui();
	connect_signals_slots();
	reload_table(service.movie_get_all());
	service.add_observer(this);
}

void MainWindow::update()
{
	reload_table(service.movie_get_all());
}

void MainWindow::reload_table(const std::vector<Movie>& movies)
{
	table_movies->clearContents();
	table_movies->setRowCount((int)movies.size());
	int current_row = 0;
	for (const auto& m : movies) {
		QTableWidgetItem* id = new QTableWidgetItem(QString::number(m.get_id()));
		QTableWidgetItem* title = new QTableWidgetItem(QString::fromStdString(m.get_title()));
		QTableWidgetItem* genre = new QTableWidgetItem(QString::fromStdString(m.get_genre()));
		QTableWidgetItem* actor = new QTableWidgetItem(QString::fromStdString(m.get_actor()));
		QTableWidgetItem* release_year = new QTableWidgetItem(QString::number(m.get_release_year()));
		id->setTextAlignment(Qt::AlignCenter);
		id->setTextColor("red");
		title->setTextAlignment(Qt::AlignCenter);
		title->setTextColor("indigo");
		genre->setTextAlignment(Qt::AlignCenter);
		genre->setTextColor("green");
		actor->setTextAlignment(Qt::AlignCenter);
		actor->setTextColor("blue");
		release_year->setTextAlignment(Qt::AlignCenter);
		release_year->setTextColor("orange");
		table_movies->setItem(current_row, 0, id);
		table_movies->setItem(current_row, 1, title);
		table_movies->setItem(current_row, 2, genre);
		table_movies->setItem(current_row, 3, actor);
		table_movies->setItem(current_row, 4, release_year);
		current_row++;
	}
}

void ShoppingCartWindow::init()
{
	// Main layout => LHS & RHS
	QHBoxLayout* main_layout = new QHBoxLayout;
	setLayout(main_layout);

	// LHS
	QWidget* lhs = new QWidget;
	QVBoxLayout* lhs_layout = new QVBoxLayout;
	lhs->setLayout(lhs_layout);

	main_layout->addWidget(lhs);

	// RHS
	QWidget* rhs = new QWidget;
	QVBoxLayout* rhs_layout = new QVBoxLayout;
	rhs->setLayout(rhs_layout);

	main_layout->addWidget(rhs);

	// List
	shopping_cart_list = new QListWidget;

	lhs_layout->addWidget(shopping_cart_list);

	// Buttons
	button_generate_random = new QPushButton("Generate random");
	button_empty = new QPushButton("Empty");

	rhs_layout->addWidget(button_generate_random);
	rhs_layout->addWidget(button_empty);
}

void ShoppingCartWindow::connect_signals_slots()
{
	QObject::connect(button_generate_random, &QPushButton::clicked, this, &ShoppingCartWindow::shopping_cart_generate_random);
	QObject::connect(button_empty, &QPushButton::clicked, this, &ShoppingCartWindow::shopping_cart_empty);
}

void ShoppingCartWindow::reload_list(std::vector<Movie> movies)
{
	shopping_cart_list->clear();
	for (const auto& m : movies) {
		shopping_cart_list->addItem(new QListWidgetItem(QString::fromStdString(m.get_title())));
	}
}

void ShoppingCartWindow::shopping_cart_generate_random()
{
	service.movie_generate_random_cart();
	reload_list(service.movie_get_all_cart());
	QMessageBox::information(this, "Succes", "Generated a random shopping cart");
}

void ShoppingCartWindow::shopping_cart_empty()
{
	service.movie_empty_cart();
	reload_list(service.movie_get_all_cart());
	QMessageBox::information(this, "Succes", "Cleared the shopping cart");
}

ShoppingCartWindow::ShoppingCartWindow(MovieService & service) :service{ service }
{
	init();
	connect_signals_slots();
	service.add_observer(this);
}

void ShoppingCartWindow::update()
{
	reload_list(service.movie_get_all_cart());
}

void ShoppingCartWindow::show()
{
	ShoppingCartWindow* window = new ShoppingCartWindow{ service };
	window->QWidget::show();
}

void ShoppingCartWindowRDONLY::update()
{
	QWidget::update();
}

void ShoppingCartWindowRDONLY::show()
{
	ShoppingCartWindowRDONLY* window = new ShoppingCartWindowRDONLY{ service };
	window->QWidget::show();
}

void ShoppingCartWindowRDONLY::paintEvent(QPaintEvent* e)
{
	e->rect(); // useless (had warning that e is not used)
	QPainter p{ this };
	for (auto i = 0; i < service.movie_size_cart(); i++) {
		int x = rand() % (width() - 128) + 64; // such that image does not
		int y = rand() % (height() - 128) + 64; // cross the window
		p.drawImage(QPoint(x, y), QImage("smiley_face.jpg"));
	}
}

ShoppingCartWindowRDONLY::ShoppingCartWindowRDONLY(MovieService & service) : service{ service }
{
	service.add_observer(this);
}
