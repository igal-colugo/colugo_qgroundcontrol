/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "EpsilonLinkConfiguration.h"
#include "EpsilonTCPLink.h"
#include "EpsilonUDPLink.h"

#define LINK_SETTING_ROOT "EpsilonLinkConfigurations"

EpsilonLinkConfiguration::EpsilonLinkConfiguration(const QString &name) : _name(name)
{
}

EpsilonLinkConfiguration::EpsilonLinkConfiguration(EpsilonLinkConfiguration *copy)
{
    _link = copy->_link;
    _name = copy->name();
    Q_ASSERT(!_name.isEmpty());
}

void EpsilonLinkConfiguration::copyFrom(EpsilonLinkConfiguration *source)
{
    Q_ASSERT(source != nullptr);
    _link = source->_link;
    _name = source->name();
}

/*!
  Where the settings are saved
  @return The root path of the setting.
*/
const QString EpsilonLinkConfiguration::settingsRoot()
{
    return QString(LINK_SETTING_ROOT);
}

/*!
  Configuration Factory
  @return A new instance of the given type
*/
EpsilonLinkConfiguration *EpsilonLinkConfiguration::createSettings(int type, const QString &name)
{
    EpsilonLinkConfiguration *config = nullptr;
    switch (type)
    {
    case EpsilonLinkConfiguration::TypeUdp:
        config = new EpsilonUDPConfiguration(name);
        break;
    case EpsilonLinkConfiguration::TypeTcp:
        config = new EpsilonTCPConfiguration(name);
        break;
    }
    return config;
}

/*!
  Duplicate link settings
  @return A new copy of the given settings instance
*/
EpsilonLinkConfiguration *EpsilonLinkConfiguration::duplicateSettings(EpsilonLinkConfiguration *source)
{
    EpsilonLinkConfiguration *dupe = nullptr;
    switch (source->type())
    {
    case TypeUdp:
        dupe = new EpsilonUDPConfiguration(qobject_cast<EpsilonUDPConfiguration *>(source));
        break;
    case TypeTcp:
        dupe = new EpsilonTCPConfiguration(qobject_cast<EpsilonTCPConfiguration *>(source));
        break;
    case TypeLast:
        break;
    }
    return dupe;
}

void EpsilonLinkConfiguration::setName(const QString name)
{
    _name = name;
    emit nameChanged(name);
}

void EpsilonLinkConfiguration::setLink(SharedEpsilonLinkInterfacePtr link)
{
    _link = link;
    emit linkChanged();
}
