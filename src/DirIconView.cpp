/*
 * Copyright 2022, Jaidyn Ann <jadedctrl@posteo.at>
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "DirIconView.h"

#include <QDir>


DirIconView::DirIconView(QWidget* parent)
    : IconView(parent)
{
}


// Sets the displayed directory: Populates the view with items pertaining to all the directory's files.
void
DirIconView::setDirectory(QString dir_path)
{
    clear();

    QPointF point(10.0, 10.0);
    QStringList files = QDir(dir_path).entryList(QDir::NoDotAndDotDot | QDir::AllEntries | QDir::Hidden);
    for (int i = 0; i < files.count(); i++) {
	addItem(files[i], point);

	if (point.x() < 300)
	    point += QPointF(60.0, 0);
	else
	    point += QPointF(-300, 60.0);
    }
}
