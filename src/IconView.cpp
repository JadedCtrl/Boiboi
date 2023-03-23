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

    QRectF multiSelectRect(multiSelectionOrigin, multiSelectionEnd);
    if (isMultiSelecting)
    	painter.fillRect(multiSelectRect, palette().color(QPalette::Highlight));

    for (int i = 0; i < itemIcons.count(); i++)
	if (event->rect().contains(itemPositions[i].toPoint()))
	    drawItem(&painter, itemLabels[i], itemPositions[i], itemRects[i],
		     itemsSelected[i], &pixmaps[itemIcons[i]]);

    if (isDragging == DRAGGING)
	drawItemSilhouette(&painter, draggedItemIndex);

    if (isMultiSelecting) {
	QPen oldPen(painter.pen());
	painter.setPen(palette().color(QPalette::HighlightedText));
	painter.drawRect(multiSelectRect);
	painter.setPen(oldPen);
    }
}


// Select/deselect items, as appropriate.
void
IconView::mousePressEvent(QMouseEvent* event)
{
    // First, deselect everything else
    for (int i = 0; i < itemsSelected.count(); i++)
	itemsSelected[i] = false;

    // Select the current item, and kick off a potential dragging.
    int item = itemAtPoint(event->position());
    if (item >= 0) {
	itemsSelected[item] = true;
	isDragging = MAYBE_DRAGGING;
	draggedItemIndex = item;
	draggedItemOrigin = event->position();
    }
    else {
	isMultiSelecting = true;
	multiSelectionOrigin = event->position();
	multiSelectionEnd = event->position();
    }
    update();
}


// Stops drag operation, if currently being done
void
IconView::mouseReleaseEvent(QMouseEvent* event)
{
    if (isDragging == DRAGGING) {
	isDragging = NOT_DRAGGING;

	if (draggedPosition.x() >= 0
	    && draggedPosition.y() >= 0)
	{
	    itemPositions[draggedItemIndex] = draggedPosition;
	    itemRects[draggedItemIndex] = iconBoundingBox(draggedPosition);
	}
    }

    if (isMultiSelecting)
	isMultiSelecting = false;

    update();
}


// Handle updating of drag operation, if being undertaken
void
IconView::mouseMoveEvent(QMouseEvent* event)
{
    QPointF pos = event->position();

    // Don't actually start dragging unless the item's moved a good bit.
    if (isDragging == MAYBE_DRAGGING
	&& (abs(pos.x() - draggedItemOrigin.x()) >= 15
	    && abs(pos.y() - draggedItemOrigin.x()) >= 15))
    {
	isDragging = DRAGGING;
    } else if (isDragging == DRAGGING)
	draggedPosition = pos;

    if (isMultiSelecting) {
	multiSelectionEnd = pos;
	QRectF selectRect = QRectF(multiSelectionOrigin, multiSelectionEnd);
	for (int i = 0; i < itemsSelected.count(); i++)
	    itemsSelected[i] = (selectRect.intersects(itemRects[i]));
    }

    update();
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


// Draw an individual item in the view, of the given index.
void
IconView::drawItem(QPainter* painter, int item_index)
{
    drawItem(painter, itemLabels[item_index], itemPositions[item_index],
	     itemRects[item_index], itemsSelected[item_index],
	     &pixmaps[itemIcons[item_index]]);
}


// Draw an item's “silhouette”, ex. to draw it when being dragged, from data.
void
IconView::drawItemSilhouette(QPainter* painter, QString name, QPointF point, QPixmap* icon)
{
    drawItem(painter, name, point, iconBoundingBox(point), false, icon);
}



// Draw an item's “silhouette”, ex. to draw it when being dragged, from index.
void
IconView::drawItemSilhouette(QPainter* painter, int item_index)
{
    drawItemSilhouette(painter, itemLabels[item_index], draggedPosition,
		       &pixmaps[itemIcons[item_index]]);
}


// Returns the item at the given point, by index. -1 is returned if there is no item.
int
IconView::itemAtPoint(QPointF point)
{
    for (int i = 0; i < itemRects.count(); i++)
	if (itemRects[i].contains(point))
	    return i;
    return -1;
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
