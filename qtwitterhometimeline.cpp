/* Copyright (c) 2010, Antonie Jovanoski
 *
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

#include <QNetworkRequest>
#include <QNetworkReply>
#include "qtwitterhometimeline.h"

QtwitterHomeTimeline::QtwitterHomeTimeline(QObject *parent) :
    QtwitterNetBase(parent)
{
}

QtwitterHomeTimeline::QtwitterHomeTimeline(OAuthTwitter *oauthTwitter, QObject *parent) :
        QtwitterNetBase(oauthTwitter, parent)
{
}

void QtwitterHomeTimeline::fetch(ResponseType respType,
                                 qint64 sinceid,
                                 qint64 maxid,
                                 int count,
                                 int page,
                                 bool skipuser,
                                 bool includeEntities)
{
    Q_ASSERT(oauthTwitter() != 0);

    QUrl url;

    if (respType == QtwitterNetBase::JSON)
        url.setUrl("http://api.twitter.com/1/statuses/home_timeline.json");
    else
        url.setUrl("http://api.twitter.com/1/statuses/home_timeline.xml");

    if (sinceid != 0)
        url.addQueryItem("since_id", QString::number(sinceid));

    if (maxid != 0)
        url.addQueryItem("max_id", QString::number(maxid));

    if (count != 0)
        url.addQueryItem("count", QString::number(count));

    if (page != 0)
        url.addQueryItem("page", QString::number(page));

    if (skipuser)
        url.addQueryItem("skip_user", "true");

    if (includeEntities)
        url.addQueryItem("include_entities", "true");

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
}

void QtwitterHomeTimeline::reply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    m_response = reply->readAll();

    if (reply) {
        emit finished(m_response);

        reply->deleteLater();
    }
}

QByteArray QtwitterHomeTimeline::response() const
{
    return m_response;
}

void QtwitterHomeTimeline::error()
{
    // ### TODO: Better error detection
    qCritical("Home Timeline error");
}