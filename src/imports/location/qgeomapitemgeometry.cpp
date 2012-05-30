/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtLocation module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qgeomapitemgeometry_p.h"
#include "qdeclarativegeomap_p.h"
#include "qlocationutils_p.h"
#include <QtQuick/QSGGeometry>

QT_BEGIN_NAMESPACE

QGeoMapItemGeometry::QGeoMapItemGeometry(QObject *parent) :
    QObject(parent),
    sourceDirty_(true),
    screenDirty_(true),
    clipToViewport_(true),
    preserveGeometry_(false)
{
}

/*!
    \internal
*/
void QGeoMapItemGeometry::translate(const QPointF &offset)
{
    for (int i = 0; i < screenVertices_.size(); ++i) {
        screenVertices_[i].x += offset.x();
        screenVertices_[i].y += offset.y();
    }
    firstPointOffset_ += offset;
    screenOutline_.translate(offset);
    screenBounds_.translate(offset);
}

/*!
    \internal
*/
void QGeoMapItemGeometry::allocateAndFill(QSGGeometry *geom) const
{
    const QVector<QGeoMapItemGeometry::Point> &vx = screenVertices_;
    const QVector<quint32> &ix = screenIndices_;

    if (isIndexed()) {
        geom->allocate(vx.size(), ix.size());
        if (geom->indexType() == GL_UNSIGNED_SHORT) {
            quint16 *its = geom->indexDataAsUShort();
            for (int i = 0; i < ix.size(); ++i)
                its[i] = ix[i];
        } else if (geom->indexType() == GL_UNSIGNED_INT) {
            quint32 *its = geom->indexDataAsUInt();
            for (int i = 0; i < ix.size(); ++i)
                its[i] = ix[i];
        }
    } else {
        geom->allocate(vx.size());
    }

    QSGGeometry::Point2D *pts = geom->vertexDataAsPoint2D();
    for (int i = 0; i < vx.size(); ++i)
        pts[i].set(vx[i].x, vx[i].y);
}

/*!
    \internal
*/
QRectF QGeoMapItemGeometry::translateToCommonOrigin(const QList<QGeoMapItemGeometry *> &geoms)
{
    QGeoCoordinate origin = geoms.at(0)->origin();

    QPainterPath brects;

    // first get max offset
    QPointF maxOffset = geoms.at(0)->firstPointOffset();
    foreach (QGeoMapItemGeometry *g, geoms) {
        Q_ASSERT(g->origin() == origin);
        QPointF o = g->firstPointOffset();
        maxOffset.setX(qMax(o.x(), maxOffset.x()));
        maxOffset.setY(qMax(o.y(), maxOffset.y()));
    }

    // then translate everything
    foreach (QGeoMapItemGeometry *g, geoms) {
        g->translate(maxOffset - g->firstPointOffset());
        brects.addRect(g->sourceBoundingBox());
    }

    return brects.boundingRect();
}

/*!
    \internal
*/
double QGeoMapItemGeometry::geoDistanceToScreenWidth(const QGeoMap &map,
                                                     const QGeoCoordinate &fromCoord,
                                                     const QGeoCoordinate &toCoord)
{
    QGeoCoordinate mapMid = map.screenPositionToCoordinate(QPointF(map.width()/2.0, 0));
    double halfGeoDist = toCoord.longitude() - fromCoord.longitude();
    if (toCoord.longitude() < fromCoord.longitude())
        halfGeoDist += 360;
    halfGeoDist /= 2.0;
    QGeoCoordinate geoDelta =  QGeoCoordinate(0,
                    QLocationUtils::wrapLong(mapMid.longitude() + halfGeoDist));
    QPointF halfScreenDist = map.coordinateToScreenPosition(geoDelta, false)
                                - QPointF(map.width()/2.0, 0);
    return halfScreenDist.x() * 2.0;
}

QT_END_NAMESPACE
