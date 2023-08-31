/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "NextVisionLinkConfiguration.h"
#include "NextVisionTCPLink.h"
#include "NextVisionUDPLink.h"

#define LINK_SETTING_ROOT "NextVisionLinkConfigurations"

NextVisionLinkConfiguration::NextVisionLinkConfiguration(const QString &name) : _name(name)
{
}

NextVisionLinkConfiguration::NextVisionLinkConfiguration(NextVisionLinkConfiguration *copy)
{
    _link = copy->_link;
    _name = copy->name();
    Q_ASSERT(!_name.isEmpty());
}

void NextVisionLinkConfiguration::copyFrom(NextVisionLinkConfiguration *source)
{
    Q_ASSERT(source != nullptr);
    _link = source->_link;
    _name = source->name();
}

/*!
  Where the settings are saved
  @return The root path of the setting.
*/
const QString NextVisionLinkConfiguration::settingsRoot()
{
    return QString(LINK_SETTING_ROOT);
}

/*!
  Configuration Factory
  @return A new instance of the given type
*/
NextVisionLinkConfiguration *NextVisionLinkConfiguration::createSettings(int type, const QString &name)
{
    NextVisionLinkConfiguration *config = nullptr;
    switch (type)
    {
    case NextVisionLinkConfiguration::TypeUdp:
        config = new NextVisionUDPConfiguration(name);
        break;
    case NextVisionLinkConfiguration::TypeTcp:
        config = new NextVisionTCPConfiguration(name);
        break;
    }
    return config;
}

/*!
  Duplicate link settings
  @return A new copy of the given settings instance
*/
NextVisionLinkConfiguration *NextVisionLinkConfiguration::duplicateSettings(NextVisionLinkConfiguration *source)
{
    NextVisionLinkConfiguration *dupe = nullptr;
    switch (source->type())
    {
    case TypeUdp:
        dupe = new NextVisionUDPConfiguration(qobject_cast<NextVisionUDPConfiguration *>(source));
        break;
    case TypeTcp:
        dupe = new NextVisionTCPConfiguration(qobject_cast<NextVisionTCPConfiguration *>(source));
        break;
    case TypeLast:
        break;
    }
    return dupe;
}

void NextVisionLinkConfiguration::setName(const QString name)
{
    _name = name;
    emit nameChanged(name);
}

void NextVisionLinkConfiguration::setLink(SharedNextVisionLinkInterfacePtr link)
{
    _link = link;
    emit linkChanged();
}
