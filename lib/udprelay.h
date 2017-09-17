/*
 * udprelay.h - the header file of UdpRelay class
 *
 * Copyright (C) 2014-2016 Symeon Huang <hzwhuang@gmail.com>
 *
 * This file is part of the libQtShadowsocks.
 *
 * libQtShadowsocks is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * libQtShadowsocks is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libQtShadowsocks; see the file LICENSE. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef UDPRELAY_H
#define UDPRELAY_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <map>
#include "address.h"
#include "encryptor.h"

namespace QSS {

class QSS_EXPORT UdpRelay : public QObject
{
    Q_OBJECT
public:
    UdpRelay(const std::string& method,
             const std::string& password,
             bool is_local,
             bool auto_ban,
             const Address &serverAddress,
             QObject *parent = 0);

    void setup(const QHostAddress &localAddr,
               const uint16_t &localPort);

    bool isListening() const;

public slots:
    bool listen(const QHostAddress& addr, uint16_t port);
    void close();

signals:
    /*
     * The same situation here.
     * We only count "listen" socket's read and written bytes
     */
    void bytesRead(quint64);
    void bytesSend(quint64);

private:
    //64KB, same as shadowsocks-python (udprelay)
    static const int64_t RemoteRecvSize = 65536;

    const Address serverAddress;
    const bool isLocal;
    const bool autoBan;
    QUdpSocket listenSocket;
    std::unique_ptr<Encryptor> encryptor;

    std::map<Address, QUdpSocket*> m_cache;

private slots:
    void onSocketError();
    void onListenStateChanged(QAbstractSocket::SocketState);
    void onServerUdpSocketReadyRead();
    void onClientUdpSocketReadyRead();
    void onClientDisconnected();
};

}

#endif // UDPRELAY_H
