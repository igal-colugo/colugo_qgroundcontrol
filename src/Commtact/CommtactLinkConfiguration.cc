/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "CommtactLinkConfiguration.h"
#include "CommtactTCPLink.h"
#include "CommtactUDPLink.h"

#define LINK_SETTING_ROOT "CommtactLinkConfigurations"

CommtactLinkConfiguration::CommtactLinkConfiguration(const QString &name) : _name(name)
{
}

CommtactLinkConfiguration::CommtactLinkConfiguration(CommtactLinkConfiguration *copy)
{
    _link = copy->_link;
    _name = copy->name();
    Q_ASSERT(!_name.isEmpty());
}

void CommtactLinkConfiguration::copyFrom(CommtactLinkConfiguration *source)
{
    Q_ASSERT(source != nullptr);
    _link = source->_link;
    _name = source->name();
}

/*!
  Where the settings are saved
  @return The root path of the setting.
*/
const QString CommtactLinkConfiguration::settingsRoot()
{
    return QString(LINK_SETTING_ROOT);
}

/*!
  Configuration Factory
  @return A new instance of the given type
*/
CommtactLinkConfiguration *CommtactLinkConfiguration::createSettings(int type, const QString &name)
{
    CommtactLinkConfiguration *config = nullptr;
    switch (type)
    {
    case CommtactLinkConfiguration::TypeUdp:
        config = new CommtactUDPConfiguration(name);
        break;
    case CommtactLinkConfiguration::TypeTcp:
        config = new CommtactTCPConfiguration(name);
        break;
    }
    return config;
}

/*!
  Duplicate link settings
  @return A new copy of the given settings instance
*/
CommtactLinkConfiguration *CommtactLinkConfiguration::duplicateSettings(CommtactLinkConfiguration *source)
{
    CommtactLinkConfiguration *dupe = nullptr;
    switch (source->type())
    {
    case TypeUdp:
        dupe = new CommtactUDPConfiguration(qobject_cast<CommtactUDPConfiguration *>(source));
        break;
    case TypeTcp:
        dupe = new CommtactTCPConfiguration(qobject_cast<CommtactTCPConfiguration *>(source));
        break;
    case TypeLast:
        break;
    }
    return dupe;
}

void CommtactLinkConfiguration::setName(const QString name)
{
    _name = name;
    emit nameChanged(name);
}

void CommtactLinkConfiguration::setLink(SharedCommtactLinkInterfacePtr link)
{
    _link = link;
    emit linkChanged();
}
