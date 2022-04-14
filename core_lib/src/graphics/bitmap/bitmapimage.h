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
#ifndef BITMAP_IMAGE_H
#define BITMAP_IMAGE_H

#include <memory>
#include <QPainter>
#include "keyframe.h"
#include "layer.h"
#include "layerbitmap.h"

class Editor;

class BitmapImage : public KeyFrame
{
public:
    const QRgb transp = qRgba(0, 0, 0, 0);
    const QRgb rosa = qRgba(255,230,230,255);
    const QRgb blackline = qRgba(1, 1, 1, 255);
    const QRgb redline = qRgba(254,0,0,255);
    const QRgb greenline = qRgba(0,254,0,255);
    const QRgb blueline = qRgba(0,0,254,255);

    BitmapImage();
    BitmapImage(const BitmapImage&);
    BitmapImage(const QRect &rectangle, const QColor& color);
    BitmapImage(const QPoint& topLeft, const QImage& image);
    BitmapImage(const QPoint& topLeft, const QString& path);

    ~BitmapImage() override;
    BitmapImage& operator=(const BitmapImage& a);

    BitmapImage* clone() const override;
    void loadFile() override;
    void unloadFile() override;
    bool isLoaded() const override;
    quint64 memoryUsage() override;

    void paintImage(QPainter& painter);
    void paintImage(QPainter &painter, QImage &image, QRect sourceRect, QRect destRect);

    QImage* image();
    void    setImage(QImage* pImg);

    BitmapImage copy();
    BitmapImage copy(QRect rectangle);
    void paste(BitmapImage*, QPainter::CompositionMode cm = QPainter::CompositionMode_SourceOver);

    void moveTopLeft(QPoint point);
    void moveTopLeft(QPointF point) { moveTopLeft(point.toPoint()); }
    void transform(QRect rectangle, bool smoothTransform);
    void transform(QRectF rectangle, bool smoothTransform) { transform(rectangle.toRect(), smoothTransform); }
    BitmapImage transformed(QRect selection, QTransform transform, bool smoothTransform);
    BitmapImage transformed(QRect rectangle, bool smoothTransform);
    BitmapImage transformed(QRectF rectangle, bool smoothTransform) { return transformed(rectangle.toRect(), smoothTransform); }

    bool contains(QPoint P) { return mBounds.contains(P); }
    bool contains(QPointF P) { return contains(P.toPoint()); }
    void autoCrop();

    QRgb pixel(int x, int y);
    QRgb pixel(QPoint p);
    void setPixel(int x, int y, QRgb color);
    void setPixel(QPoint p, QRgb color);
    void fillNonAlphaPixels(const QRgb color);

    QRgb constScanLine(int x, int y) const;
    void scanLine(int x, int y, QRgb color);
    void clear();
    void clear(QRect rectangle);
    void clear(QRectF rectangle) { clear(rectangle.toRect()); }

    static inline bool compareColor(QRgb newColor, QRgb oldColor, int tolerance, QHash<QRgb, bool> *cache);
    static bool floodFill(BitmapImage* replaceImage, BitmapImage* targetImage, QRect cameraRect, QPoint point, QRgb fillColor, int tolerance);
    static void expandFill(BitmapImage* targetImage, QRgb fillColor, int expand);
    static QVector<QVector<int> > manhattanDistance(BitmapImage* image, QRgb& searchColor);

    void drawLine(QPointF P1, QPointF P2, QPen pen, QPainter::CompositionMode cm, bool antialiasing);
    void drawRect(QRectF rectangle, QPen pen, QBrush brush, QPainter::CompositionMode cm, bool antialiasing);
    void drawEllipse(QRectF rectangle, QPen pen, QBrush brush, QPainter::CompositionMode cm, bool antialiasing);
    void drawPath(QPainterPath path, QPen pen, QBrush brush, QPainter::CompositionMode cm, bool antialiasing);

    QPoint topLeft() { autoCrop(); return mBounds.topLeft(); }
    QPoint topRight() { autoCrop(); return mBounds.topRight(); }
    QPoint bottomLeft() { autoCrop(); return mBounds.bottomLeft(); }
    QPoint bottomRight() { autoCrop(); return mBounds.bottomRight(); }
    int left() { autoCrop(); return mBounds.left(); }
    int right() { autoCrop(); return mBounds.right(); }
    int top() { autoCrop(); return mBounds.top(); }
    int bottom() { autoCrop(); return mBounds.bottom(); }
    int width() { autoCrop(); return mBounds.width(); }
    int height() { autoCrop(); return mBounds.height(); }
    QSize size() { autoCrop(); return mBounds.size(); }


    QRect& bounds() { autoCrop(); return mBounds; }
    void setBounds(QRect rect);

    // coloring methods
    int getThreshold() { return mThreshold; }
    int getSpotArea() { return mSpotArea; }
    BitmapImage* scanToTransparent(BitmapImage* bitmapimage, bool redEnabled, bool greenEnabled, bool blueEnabled);
    BitmapImage* prepDrawing(BitmapImage* img, bool redEnabled, bool greenEnabled, bool blueEnabled);
    void traceLine(BitmapImage* bitmapimage, bool blackEnabled, bool redEnabled, bool greenEnabled, bool blueEnabled);
    void eraseRedGreenBlueLines(BitmapImage* img);
    void fillSpotAreas(BitmapImage* img);
    void toThinLine(BitmapImage* colorImage, bool black, bool red, bool green, bool blue);
    void blendLines(BitmapImage* bitmapimage, bool black, bool red, bool green, bool blue);
    int fillWithColor(QPoint point, QRgb orgColor, QRgb newColor, BitmapImage* img);

    /** Determines if the BitmapImage is minimally bounded.
     *
     *  A BitmapImage is minimally bounded if all edges contain
     *  at least 1 non-transparent pixel (alpha > 0). In other words,
     *  the size of the image cannot be decreased without
     *  cropping visible data.
     *
     *  @return True only if bounds() is the minimal bounding box
     *          for the contained image.
     */
    bool isMinimallyBounded() const { return mMinBound; }
    void enableAutoCrop(bool b) { mEnableAutoCrop = b; }
    void setOpacity(qreal opacity) { mOpacity = opacity; }
    qreal getOpacity() const { return mOpacity; }

    Status writeFile(const QString& filename);

public slots:
    void setThreshold(int threshold) { mThreshold = threshold; }
    void setSpotArea(int spotArea) { mSpotArea = spotArea; }

protected:
    void updateBounds(QRect rectangle);
    void extend(const QPoint& p);
    void extend(QRect rectangle);

    void setCompositionModeBounds(BitmapImage *source, QPainter::CompositionMode cm);
    void setCompositionModeBounds(QRect sourceBounds, bool isSourceMinBounds, QPainter::CompositionMode cm);

private:
    std::unique_ptr<QImage> mImage;
    QRect mBounds;

    /** @see isMinimallyBounded() */
    bool mMinBound = true;
    bool mEnableAutoCrop = false;

    int mSpotArea = 6;
    int mThreshold = 200;
    const int mLowThreshold = 30;   // threshold for images to be given transparency
    const int COLORDIFF = 5; // difference in color values to decide color
    const int GRAYSCALEDIFF = 15; // difference in grasycale values to decide color
    const int TRANSP_THRESHOLD = 60;// threshold when tracing black for two layer coloring

    const int RED_FACTOR = 20;
    qreal mOpacity = 1.0;
};

#endif
