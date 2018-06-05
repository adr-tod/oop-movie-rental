#pragma once

#include "movie.h"

#include "qabstractitemmodel.h"

class MyTableModel : public QAbstractTableModel {
	std::vector<Movie> movies;
public:
	MyTableModel() {

	}
	MyTableModel(const std::vector<Movie>& movies) :movies{ movies } {
	}

	int rowCount(const QModelIndex & parent = QModelIndex()) const override {
		parent.isValid(); // useless, had warning that parent is not used
		return (int)movies.size();
	}
	int columnCount(const QModelIndex & parent = QModelIndex()) const override {
		parent.isValid();
		return 5;
	}
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override {
		if (role == Qt::DisplayRole) {
			Movie m = movies[index.row()];
			if (index.column() == 0) {
				return QString::number(m.get_id());
			}
			else if (index.column() == 1) {
				return QString::fromStdString(m.get_title());
			}
			else if (index.column() == 2) {
				return QString::fromStdString(m.get_genre());
			}
			else if (index.column() == 3) {
				return QString::fromStdString(m.get_actor());
			}
			else if (index.column() == 4) {
				return QString::number(m.get_release_year());
			}
		}
		return QVariant{};
	}
	Movie get_movie(const QModelIndex & index) {
		return movies[index.row()];
	}

	void set_movies(const std::vector<Movie>& new_movies) {
		this->movies = new_movies;
		QModelIndex topLeft = createIndex(0, 0);
		QModelIndex bottomRight = createIndex(rowCount(), columnCount());
		emit dataChanged(topLeft, bottomRight);
	}
};
