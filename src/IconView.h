/*
 * Copyright 2023, Jaidyn Ann <jadedctrl@posteo.at>
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
#ifndef _ICON_VIEW_H
#define _ICON_VIEW_H

#include <QList>
#include <QPixmap>
#include <QRectF>
#include <QStaticText>
#include <QStringList>
#include <QWidget>


class IconView : public QWidget {
public:
    IconView(QWidget* parent = nullptr);

    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);

    void addItem(QString label, QPointF position);
    void addItem(QString label);

    void clear();

private:
    void drawItem(QPainter* painter, QString name, QPointF point, QRectF box,
		  bool selected, QPixmap* icon = nullptr);

    int itemAtPoint(QPointF point);

    QRectF iconBoundingBox(QPointF top_left_corner);
    QRectF iconLabelBox(QRect bounding_box, QString label);

    QList<int> itemIcons; // Indices of each file's icon in `iconPixmaps`
    QStringList itemLabels; // Each file's name
    QList<QPointF> itemPositions; // Top-left corner positions of each file
    QList<QRectF> itemRects; // Bounding boxes for each file
    QList<bool> itemsSelected; // Whether or not each item is selected

    QList<QPixmap> pixmaps;
};

#endif // _ICON_VIEW_H
