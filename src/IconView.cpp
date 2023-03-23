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

#include "IconView.h"

#include <QPainter>
#include <QPaintEvent>
#include <QStyle>


const int ICON_HEIGHT = 32;
const int FONT_PT = 11;


IconView::IconView(QWidget* parent)
    : QWidget(parent)
{
    pixmaps << style()->standardIcon(QStyle::SP_FileIcon).pixmap(ICON_HEIGHT, ICON_HEIGHT);
}


// Draw the view, overriding QWidget::paintEvent.
void
IconView::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    for (int i = 0; i < itemIcons.count(); i++)
	if (event->rect().contains(itemPositions[i].toPoint()))
	    drawItem(&painter, itemLabels[i], itemPositions[i], itemRects[i],
		     itemsSelected[i], &pixmaps[itemIcons[i]]);
}


// Add a new item to the IconView for display.
void
IconView::addItem(QString label, QPointF position)
{
    itemLabels << label;
    itemPositions << position;
    itemRects << iconBoundingBox(position);
    itemsSelected << false;
    itemIcons << 0;
}


// Add a new item to the IconView for display, letting IconView algorithmically choose
// the “ideal” position (read: literally no algorithim yet c:).
void
IconView::addItem(QString label)
{
    addItem(label, QPointF(20.0, 20.0));
}


// Remove all current items, and refresh the display. Squeaky-clean!
void
IconView::clear()
{
    itemLabels.clear();
    itemPositions.clear();
    itemRects.clear();
    itemsSelected.clear();
    itemIcons.clear();

    update();
}


// Draw an individual item in the view, with the given information.
void
IconView::drawItem(QPainter* painter, QString name, QPointF point, QRectF box, bool selected,
		   QPixmap* icon)
{
    const QRect icon_rect = QRect(point.x(), point.y(),
				  ICON_HEIGHT,
				  ICON_HEIGHT);

    const QRect label_rect =
	iconLabelBox(QRect(point.x() - ICON_HEIGHT, point.y() + ICON_HEIGHT,
			   ICON_HEIGHT * 3, FONT_PT * 3),
		     name).toRect();

    if (selected)
	painter->fillRect(label_rect, palette().color(QPalette::Highlight));
    style()->drawItemText(painter, label_rect, Qt::AlignHCenter, palette(), true, name);
    style()->drawItemPixmap(painter, icon_rect, Qt::AlignHCenter, *icon);
}


// Find the maximum bounding box of an item, including label and icon, given the
// point for the top-left corner.
QRectF
IconView::iconBoundingBox(QPointF top_left_corner)
{
    return QRectF(top_left_corner.x() - ICON_HEIGHT, top_left_corner.y(),
		  ICON_HEIGHT * 3,
		  ICON_HEIGHT + FONT_PT * 3);
}


// Find the optimum bounding box for the label to be drawn in.
QRectF
IconView::iconLabelBox(QRect bounding_box, QString label)
{
    return style()->itemTextRect(QFontMetrics(font()), bounding_box,
				 Qt::AlignHCenter, true, label);
}
