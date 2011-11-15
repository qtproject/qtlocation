/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
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
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QDECLARATIVEPLACE_P_H
#define QDECLARATIVEPLACE_P_H

#include <QObject>
#include <QDeclarativeListProperty>
#include <qplace.h>
#include "qdeclarativegeolocation_p.h"
#include "qdeclarativecategory_p.h"
#include "qdeclarativecontactdetail_p.h"
#include "qdeclarativesupplier_p.h"
#include "qdeclarativeratings_p.h"
#include "qdeclarativereviewmodel_p.h"
#include "qdeclarativeplaceimagemodel_p.h"
#include "qdeclarativeplaceeditorialmodel.h"
#include <QDeclarativePropertyMap>

QT_BEGIN_NAMESPACE

class QPlaceReply;

class QPlaceManager;
class QDeclarativePlaceIcon;

class QDeclarativePlace : public QObject, public QDeclarativeParserStatus
{
    Q_OBJECT

    Q_ENUMS(Status Visibility)

    Q_PROPERTY(QPlace place READ place WRITE setPlace)
    Q_PROPERTY(QDeclarativeGeoServiceProvider *plugin READ plugin WRITE setPlugin NOTIFY pluginChanged)
    Q_PROPERTY(QDeclarativeListProperty<QDeclarativeCategory> categories READ categories NOTIFY categoriesChanged)
    Q_PROPERTY(QDeclarativeGeoLocation* location READ location WRITE setLocation NOTIFY locationChanged);
    Q_PROPERTY(QDeclarativeRatings* ratings READ ratings WRITE setRatings NOTIFY ratingsChanged);
    Q_PROPERTY(QDeclarativeSupplier *supplier READ supplier WRITE setSupplier NOTIFY supplierChanged)
    Q_PROPERTY(QDeclarativePlaceIcon* icon READ icon WRITE setIcon NOTIFY iconChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged);
    Q_PROPERTY(QString placeId READ placeId WRITE setPlaceId NOTIFY placeIdChanged);
    Q_PROPERTY(QString attribution READ attribution WRITE setAttribution NOTIFY attributionChanged)

    Q_PROPERTY(QDeclarativeReviewModel *reviewModel READ reviewModel NOTIFY reviewModelChanged)
    Q_PROPERTY(QDeclarativePlaceImageModel *imageModel READ imageModel NOTIFY imageModelChanged)
    Q_PROPERTY(QDeclarativePlaceEditorialModel *editorialModel READ editorialModel NOTIFY editorialModelChanged)

    Q_PROPERTY(QDeclarativePropertyMap *extendedAttributes READ extendedAttributes WRITE setExtendedAttributes NOTIFY extendedAttributesChanged);
    Q_PROPERTY(QDeclarativePropertyMap *contactDetails READ contactDetails WRITE setContactDetails NOTIFY contactDetailsChanged);
    Q_PROPERTY(bool detailsFetched READ detailsFetched WRITE setDetailsFetched NOTIFY detailsFetchedChanged);
    Q_PROPERTY(Status status READ status NOTIFY statusChanged);

    Q_PROPERTY(QString primaryPhone READ primaryPhone NOTIFY primaryPhoneChanged);
    Q_PROPERTY(QString primaryFax READ primaryFax NOTIFY primaryFaxChanged);
    Q_PROPERTY(QString primaryEmail READ primaryEmail NOTIFY primaryEmailChanged);
    Q_PROPERTY(QUrl primaryWebsite READ primaryWebsite NOTIFY primaryWebsiteChanged);

    Q_PROPERTY(Visibility visibility READ visibility WRITE setVisibility NOTIFY visibilityChanged)

    Q_INTERFACES(QDeclarativeParserStatus)

public:
    explicit QDeclarativePlace(QObject* parent = 0);
    QDeclarativePlace(const QPlace &src, QDeclarativeGeoServiceProvider *plugin, QObject* parent = 0);
    ~QDeclarativePlace();

    enum Status {Ready, Saving, Fetching, Removing, Error};
    enum Visibility {
        UnspecifiedVisibility = QtLocation::UnspecifiedVisibility,
        DeviceVisibility = QtLocation::DeviceVisibility,
        PrivateVisibility = QtLocation::PrivateVisibility,
        PublicVisibility = QtLocation::PublicVisibility
    };

    //From QDeclarativeParserStatus
    virtual void classBegin() {}
    virtual void componentComplete();

    void setPlugin(QDeclarativeGeoServiceProvider *plugin);
    QDeclarativeGeoServiceProvider* plugin() const;

    QDeclarativeReviewModel *reviewModel();
    QDeclarativePlaceImageModel *imageModel();
    QDeclarativePlaceEditorialModel *editorialModel();

    QPlace place();
    void setPlace(const QPlace &src);

    QDeclarativeListProperty<QDeclarativeCategory> categories();
    static void category_append(QDeclarativeListProperty<QDeclarativeCategory> *prop,
                                  QDeclarativeCategory* value);
    static int category_count(QDeclarativeListProperty<QDeclarativeCategory> *prop);
    static QDeclarativeCategory* category_at(QDeclarativeListProperty<QDeclarativeCategory> *prop, int index);
    static void category_clear(QDeclarativeListProperty<QDeclarativeCategory> *prop);

    QDeclarativeGeoLocation *location();
    void setLocation(QDeclarativeGeoLocation *location);
    QDeclarativeRatings *ratings();
    void setRatings(QDeclarativeRatings *ratings);
    QDeclarativeSupplier *supplier() const;
    void setSupplier(QDeclarativeSupplier *supplier);
    QDeclarativePlaceIcon *icon() const;
    void setIcon(QDeclarativePlaceIcon *icon);
    QString name() const;
    void setName(const QString &name);
    QString placeId() const;
    void setPlaceId(const QString &placeId);
    QString attribution() const;
    void setAttribution(const QString &attribution);
    bool detailsFetched() const;
    void setDetailsFetched(bool fetched);

    Status status() const;
    void setStatus(Status status);

    Q_INVOKABLE void getDetails();
    Q_INVOKABLE void save();
    Q_INVOKABLE void remove();
    Q_INVOKABLE QString errorString() const;

    QString primaryPhone() const;
    QString primaryFax() const;
    QString primaryEmail() const;
    QUrl primaryWebsite() const;

    QDeclarativePropertyMap *extendedAttributes() const;
    void setExtendedAttributes(QDeclarativePropertyMap *attrib);

    QDeclarativePropertyMap *contactDetails() const;
    void setContactDetails(QDeclarativePropertyMap *contactDetails);

    Visibility visibility() const;
    void setVisibility(Visibility visibility);

signals:
    void pluginChanged();
    void categoriesChanged();
    void locationChanged();
    void ratingsChanged();
    void supplierChanged();
    void iconChanged();
    void nameChanged();
    void placeIdChanged();
    void attributionChanged();
    void detailsFetchedChanged();
    void reviewModelChanged();
    void imageModelChanged();
    void editorialModelChanged();

    void primaryPhoneChanged();
    void primaryFaxChanged();
    void primaryEmailChanged();
    void primaryWebsiteChanged();

    void extendedAttributesChanged();
    void contactDetailsChanged();
    void statusChanged();
    void visibilityChanged();

private slots:
    void finished();
    void contactsModified(const QString &, const QVariant &);

private:
    void synchronizeCategories();
    void synchronizeExtendedAttributes();
    void synchronizeContacts();
    void primarySignalsEmission(const QString &type = QString());

private:
    QPlaceManager *manager();

    QList<QDeclarativeCategory*> m_categories;
    QDeclarativeGeoLocation *m_location;
    QDeclarativeRatings *m_ratings;
    QDeclarativeSupplier *m_supplier;
    QDeclarativePlaceIcon *m_icon;
    QDeclarativeReviewModel *m_reviewModel;
    QDeclarativePlaceImageModel *m_imageModel;
    QDeclarativePlaceEditorialModel *m_editorialModel;
    QDeclarativePropertyMap *m_extendedAttributes;
    QDeclarativePropertyMap *m_contactDetails;

    QPlace m_src;

    QPlaceReply *m_reply;

    QDeclarativeGeoServiceProvider *m_plugin;
    bool m_complete;

    QString m_prevPrimaryPhone;
    QString m_prevPrimaryEmail;
    QString m_prevPrimaryFax;
    QUrl m_prevPrimaryWebsite;

    Status m_status;
    QString m_errorString;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QT_PREPEND_NAMESPACE(QDeclarativePlace));

#endif // QDECLARATIVEPLACE_P_H
