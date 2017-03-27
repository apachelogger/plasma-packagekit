/*
    Copyright © 2017 Harald Sitter <sitter@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Transaction.h"

#include <QDebug>
#include <QMetaEnum>

Transaction::Transaction(const QDBusObjectPath &path)
    : PackageKit::Transaction(path)
{
    connect(this, &PackageKit::Transaction::lastPackageChanged,
            this, &Transaction::lastPackageChanged);
    connect(this, &PackageKit::Transaction::package,
            [&](PackageKit::Transaction::Info, const QString &packageID, const QString &){
        m_lastPackage = packageName(packageID);
        emit lastPackageChanged();
    });
}

QString Transaction::tidString() const
{
    return tid().path();
}

QString Transaction::lastPackage() const
{
    return m_lastPackage;
}

QString Transaction::enumString(Transaction::Role role)
{
    static auto index = staticMetaObject.indexOfEnumerator("Role");
    return staticMetaObject.enumerator(index).valueToKey(role);
}

void Transaction::cancel()
{
    qDebug() << "CANCEL";
    QDBusPendingReply<> ret =PackageKit::Transaction::cancel();
    ret.waitForFinished();
    qDebug() << "CANCEL DONE";
}

#include "Transaction.moc"