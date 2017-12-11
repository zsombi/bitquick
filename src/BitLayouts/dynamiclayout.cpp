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

#include "dynamiclayout_p_p.h"
#include "layoutcontainer_p_p.h"

namespace BitQuick {

DynamicLayoutPrivate::DynamicLayoutPrivate()
    : QQuickItemPrivate()
{
}

/*!
 * \qmltype DynamicLayout
 * \ingroup bitquick_layouts
 * \inqmlmodule BitQuick.Layouts 1.0
 * \brief Dynamically transfers elements from one layout to the other.
 *
 * DynamicLayouts provides dynamic rearrangement of your application views' content
 * without the need to reload content and loose components' status information.
 *
 * Each layout is defined by a layout container. The containers specify what to
 * lay out and how. These containers can be defined in separate documents or within
 * the DynamicLayout element.
 *
 * Each container has a condition property which tells when to activate the layout
 * defined by it. When a condition is evaluated to true, its content will be loaded,
 * and the marked element swill be transferred from the previous layout to the new
 * one.
 *
 * There must be a default container specified. This can be specified by setting
 * the container's default proeprty to true. The DynamicLayout however can have only
 * one default container set. If there are multiple containers set as default,
 * DynamicLayout will choose the last one set as default, and will reset the others'
 * default status.
 * In case none of the containers is set to default, the last container with its
 * condition evaluating to true will be set as default. A DymanicLayout without
 * a default container is not functional.
 *
 * Example:
 * \qml
 * import QtQuick 2.9
 * import BitQuick.Layouts 1.0
 *
 * DynamicLayout {
 *     id: layout
 *     LayoutContainer {
 *         default: true
 *         Flow {
 *             anchors.fill: parent
 *             Repeater {
 *                 model: 10
 *                 Rectangle {
 *                     width: 50
 *                     height: 50
 *                     color: "blue"
 *                     LayoutElement.identifier: "element" + index
 *                     Text {
 *                         text: "Text#" + index
 *                         anchors.centerIn: parent
 *                     }
 *                 }
 *             }
 *         }
 *     }
 *     LayoutContainer {
 *         when: layout.width > 450
 *         Column {
 *             anchors.fill: parent
 *             LayoutElement {
 *                 identifier: "element2"
 *                 width: parent.width
 *                 height: 45
 *                 color: "red"
 *             }
 *             LayoutElement {
 *                 identifier: "element5"
 *                 width: parent.width
 *                 height: 100
 *                 color: "lightblue"
 *             }
 *             LayoutElement {
 *                 identifier: "element8"
 *                 width: parent.width
 *                 height: 25
 *                 color: "green"
 *             }
 *         }
 *     }
 * }
 * \endqml
 */
DynamicLayout::DynamicLayout(QQuickItem *parent)
    : QQuickItem(*(new DynamicLayoutPrivate), parent)
{
}

void DynamicLayout::componentComplete()
{
    Q_D(DynamicLayout);
    // Make sure we have the default set before we seal the component. Pick the
    // last one marked as default to make sure there are not more than one container
    // is marked as default.
    if (d->layouts.size()) {
        if (d->defaultContainer < 0) {
            for (LayoutContainer* container : d->layouts) {
                if (container->isDefault()) {
                    d->setDefault(container);
                }
            }
        }
        // If we still don't have any, we pich the one which has the condition set.
        if (d->defaultContainer < 0) {
            // If we have a current one set, we make that as default
            if (d->currentContainer >= 0) {
                d->defaultContainer = d->currentContainer;
            } else {
                // We must check whether the containers have their condition set.
                // This can happen when the containers were created in a different
                // element than the current DynamicLayout, and were re-parented.
                // We pick the first container which has its condition set to true.
                for (LayoutContainer* container : d->layouts) {
                    if (container->when()) {
                        d->setDefault(container);
                        break;
                    }
                }
            }
        }
    }

    // Seal the layout.
    QQuickItem::componentComplete();

    // Activate the default container first.
    if (d->defaultContainer >= 0) {
        d->activateContainer(d->layouts[d->defaultContainer]);
    } else {
        qmlWarning(this) << QStringLiteral("No default container set. DynamicLayout is not functional.");
    }
}

/*!
 * \qmlproperty list<LayoutContainer> DynamicLayout::layouts
 * \default
 * The list of containers holding the layouts activated on a well defined condition.
 */
void DynamicLayoutPrivate::layoutAppend(QQmlListProperty<LayoutContainer> *list, LayoutContainer* container)
{
    DynamicLayout *layout = static_cast<DynamicLayout*>(list->object);
    DynamicLayoutPrivate* dLayout = DynamicLayoutPrivate::get(layout);
    LayoutContainerPrivate* dContainer = LayoutContainerPrivate::get(container);

    dContainer->layout = layout;
    dContainer->containerIndex = dLayout->layouts.size();
    dLayout->layouts.append(container);
}
QQmlListProperty<LayoutContainer> DynamicLayout::layouts()
{
    return QQmlListProperty<LayoutContainer>(this, nullptr,
                                             &DynamicLayoutPrivate::layoutAppend,
                                             nullptr, nullptr, nullptr);
}

/*!
 * \qmlproperty LayoutContainer DynamicLayout::currentLayout
 * \readonly
 * The property contains the current layout.
 */
LayoutContainer* DynamicLayout::currentLayout() const
{
    Q_D(const DynamicLayout);
    return d->currentContainer >= 0 ? d->layouts[d->currentContainer] : nullptr;
}
const QList<LayoutContainer*> DynamicLayout::layouts() const
{
    return d_func()->layouts;
}

void DynamicLayoutPrivate::activateContainer(LayoutContainer *container)
{
    // Do not activate any container before we are sealed!
    if (!componentComplete) {
        return;
    }
    Q_Q(DynamicLayout);
    // Deactivate the current layout first, then activate the new one.
    if (currentContainer >= 0) {
        q->layoutChange(DynamicLayout::LayoutDeactivated, layouts[currentContainer]);
    }
    currentContainer = LayoutContainerPrivate::get(container)->containerIndex;
    q->layoutChange(DynamicLayout::LayoutActivated, container);
    Q_EMIT q->currentLayoutChanged();
}
void DynamicLayout::layoutChange(LayoutChange change, LayoutContainer *container)
{
    Q_UNUSED(container);
    switch (change) {
    case LayoutActivated:
        // TODO: move content from default to the current container
        break;
    case LayoutDeactivated:
        // TODO: roll back the layout to the default one
        break;
    }
}

void DynamicLayoutPrivate::setDefault(LayoutContainer *container)
{
    if (componentComplete) {
        qmlWarning(container) << QStringLiteral("Cannot change the default layout after component completion.");
        return;
    }
    LayoutContainer *prevContainer = (defaultContainer >= 0) ? layouts[defaultContainer] : nullptr;
    if (prevContainer) {
        LayoutContainerPrivate::get(prevContainer)->isDefault = false;
    }
    defaultContainer = LayoutContainerPrivate::get(container)->containerIndex;
}

} // namespace BitQuick
