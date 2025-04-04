/*

Pencil2D - Traditional Animation Software
Copyright (C) 2005-2007 Patrick Corrieri & Pascal Naidon
Copyright (C) 2012-2020 Matthew Chiawen Chang

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

*/

#ifndef HANDTOOL_H
#define HANDTOOL_H

#include "basetool.h"
#include "preferencemanager.h"


class HandTool : public BaseTool
{
    Q_OBJECT
public:
    explicit HandTool( QObject* parent = 0 );
    ToolType type() override { return HAND; }
    void loadSettings() override;
    void saveSettings() override;
    QCursor cursor() override;

    void pointerPressEvent(PointerEvent *) override;
    void pointerReleaseEvent(PointerEvent *) override;
    void pointerMoveEvent(PointerEvent *) override;
    void pointerDoubleClickEvent(PointerEvent *) override;

private:
    void updateSettings(const SETTING setting);
    void transformView(Qt::KeyboardModifiers keyMod, const QPointF& pos, Qt::MouseButtons buttons);

    QPointF mLastPixel;
    QPointF mStartPoint;
    bool mIsHeld = false;
    int mDeltaFactor = 1;
};

#endif
