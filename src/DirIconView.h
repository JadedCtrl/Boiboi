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
#ifndef _DIR_ICON_VIEW_H
#define _DIR_ICON_VIEW_H

#include "IconView.h"


class DirIconView : public IconView {
public:
    DirIconView(QWidget* parent = nullptr);

    void setDirectory(QString dir_path);

private:

};

#endif // _DIR_ICON_VIEW_H
