/*
 * BitQuick - a QtQuick utility library
 *
 * Copyright (C) 2017, bitWelder Ltd
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see
 * <http://www.gnu.org/licenses/>
 *
 * Author: Zsombor Egri <zsombor.egri@bitwelder.fi>
 */

#ifndef LAYOUTCONTAINER_P_H
#define LAYOUTCONTAINER_P_H

#include <QtCore/QObject>
#include <QtQml/QQmlParserStatus>
#include <BitQuick/BitQuickGlobals>

namespace BitQuick {

class QQmlComponent;
class LayoutContainerPrivate;
class BITQUICK_EXPORT LayoutContainer : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_DISABLE_COPY(LayoutContainer)
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(bool isDefault READ isDefault WRITE setDefault)
    Q_PROPERTY(bool when READ when WRITE setWhen NOTIFY whenChanged)
    Q_PROPERTY(QQmlComponent* layout READ layout WRITE setLayout NOTIFY layoutChanged)
    Q_CLASSINFO("DefaultProperty", "layout")

public:
    LayoutContainer(QObject *parent = nullptr);

    bool isDefault() const;
    void setDefault(bool value);
    bool when() const;
    void setWhen(bool when);
    QQmlComponent* layout() const;
    void setLayout(QQmlComponent* layout);

Q_SIGNALS:
    void whenChanged();
    void layoutChanged();

protected:
    void classBegin() override;
    void componentComplete() override;

private:
    Q_DECLARE_PRIVATE(LayoutContainer)
};

} // namespace BitQuick

#endif // LAYOUTCONTAINER_P_H
