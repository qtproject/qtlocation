/****************************************************************************
**
** Copyright (C) 2013 Aaron McCarthy <mccarthy.aaron@gmail.com>
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtLocation module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qgeotiledmappingmanagerengineosm.h"
#include "qgeotilefetcherosm.h"
#include "qgeotiledmapdataosm.h"

#include <QtLocation/private/qgeocameracapabilities_p.h>
#include <QtLocation/private/qgeomaptype_p.h>
#include <QtLocation/private/qgeotiledmapdata_p.h>

QT_BEGIN_NAMESPACE

QGeoTiledMappingManagerEngineOsm::QGeoTiledMappingManagerEngineOsm(const QVariantMap &parameters, QGeoServiceProvider::Error *error, QString *errorString)
:   QGeoTiledMappingManagerEngine()
{
    QGeoCameraCapabilities cameraCaps;
    cameraCaps.setMinimumZoomLevel(0.0);
    cameraCaps.setMaximumZoomLevel(18.0);
    setCameraCapabilities(cameraCaps);

    setTileSize(QSize(256, 256));

    QList<QGeoMapType> mapTypes;
    // See map type implementations in QGeoTiledMapDataOsm and QGeoTileFetcherOsm.
    mapTypes << QGeoMapType(QGeoMapType::StreetMap, tr("Street Map"), tr("Street map view in daylight mode"), false, false, 1);
    mapTypes << QGeoMapType(QGeoMapType::SatelliteMapDay, tr("Satellite Map"), tr("Satellite map view in daylight mode"), false, false, 2);
    mapTypes << QGeoMapType(QGeoMapType::CycleMap, tr("Cycle Map"), tr("Cycle map view in daylight mode"), false, false, 3);
    mapTypes << QGeoMapType(QGeoMapType::TransitMap, tr("Transit Map"), tr("Public transit map view in daylight mode"), false, false, 4);
    mapTypes << QGeoMapType(QGeoMapType::TransitMap, tr("Night Transit Map"), tr("Public transit map view in night mode"), false, true, 5);
    mapTypes << QGeoMapType(QGeoMapType::TerrainMap, tr("Terrain Map"), tr("Terrain map view"), false, false, 6);
    mapTypes << QGeoMapType(QGeoMapType::PedestrianMap, tr("Hiking Map"), tr("Hiking map view"), false, false, 7);
    setSupportedMapTypes(mapTypes);

    QGeoTileFetcherOsm *tileFetcher = new QGeoTileFetcherOsm(this);
    if (parameters.contains(QStringLiteral("useragent"))) {
        const QByteArray ua = parameters.value(QStringLiteral("useragent")).toString().toLatin1();
        tileFetcher->setUserAgent(ua);
    }

    setTileFetcher(tileFetcher);

    *error = QGeoServiceProvider::NoError;
    errorString->clear();
}

QGeoTiledMappingManagerEngineOsm::~QGeoTiledMappingManagerEngineOsm()
{
}

QGeoMap *QGeoTiledMappingManagerEngineOsm::createMapData()
{
    return new QGeoTiledMapDataOsm(this);
}

QT_END_NAMESPACE
