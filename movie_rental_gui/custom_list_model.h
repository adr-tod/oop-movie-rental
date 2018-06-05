#pragma once

#include "movie.h"

#include <qabstractitemmodel.h>

class MyListModel : public QAbstractListModel {
	std::vector<Movie> movies;
public:
	MyListModel() = default;
	MyListModel(std::vector<Movie> movies) :movies{ movies } {}

	int rowCount(const QModelIndex &parent = QModelIndex()) const override {
		parent.isValid();
		return (int)movies.size();
	}

	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override {
		if (role == Qt::DisplayRole) {
			auto sp = movies[index.row()].get_title();
			return QString::fromStdString(sp);
		}
		if (role == Qt::UserRole) {
			auto ty = movies[index.row()].get_id();
			return QString::number(ty);
		}
		return QVariant{};
	}

	void set_movies(const std::vector<Movie>& new_movies) {
		this->movies = new_movies;
		QModelIndex first = createIndex(0, 0);
		QModelIndex last = createIndex(rowCount(), 0);
		emit dataChanged(first, last);
	}
};
