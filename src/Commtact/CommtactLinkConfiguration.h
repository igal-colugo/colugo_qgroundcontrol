/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include <QSettings>
#include <memory>

class CommtactLinkInterface;
/// Interface holding link specific settings.
class CommtactLinkConfiguration : public QObject
{
    Q_OBJECT

  public:
    CommtactLinkConfiguration(const QString &name);
    CommtactLinkConfiguration(CommtactLinkConfiguration *copy);
    virtual ~CommtactLinkConfiguration()
    {
    }

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(CommtactLinkInterface *link READ link NOTIFY linkChanged)
    Q_PROPERTY(LinkType linkType READ type CONSTANT)
    Q_PROPERTY(QString settingsURL READ settingsURL CONSTANT)
    Q_PROPERTY(QString settingsTitle READ settingsTitle CONSTANT)

    // Property accessors
    QString name(void) const
    {
        return _name;
    }
    CommtactLinkInterface *link(void)
    {
        return _link.lock().get();
    }

    void setName(const QString name);
    void setLink(std::shared_ptr<CommtactLinkInterface> link);

    ///  The link types supported by QGC
    ///  Any changes here MUST be reflected in LinkManager::linkTypeStrings()
    enum LinkType
    {
        TypeUdp, ///< UDP Link
        TypeTcp, ///< TCP Link
        TypeLast // Last type value (type >= TypeLast == invalid)
    };
    Q_ENUM(LinkType)

    /// Virtual Methods
    /*!
     * @brief Connection type
     *
     * Pure virtual method returning one of the -TypeXxx types above.
     * @return The type of links these settings belong to.
     */
    virtual LinkType type() = 0;

    /*!
     * @brief Load settings
     *
     * Pure virtual method telling the instance to load its configuration.
     * @param[in] settings The QSettings instance to use
     * @param[in] root The root path of the setting.
     */
    virtual void loadSettings(QSettings &settings, const QString &root) = 0;

    /*!
     * @brief Save settings
     *
     * Pure virtual method telling the instance to save its configuration.
     * @param[in] settings The QSettings instance to use
     * @param[in] root The root path of the setting.
     */
    virtual void saveSettings(QSettings &settings, const QString &root) = 0;
    /*!
     * @brief Settings URL
     *
     * Pure virtual method providing the URL for the (QML) settings dialog
     */
    virtual QString settingsURL() = 0;

    /*!
     * @brief Settings Title
     *
     * Pure virtual method providing the Title for the (QML) settings dialog
     */
    virtual QString settingsTitle() = 0;

    /*!
     * @brief Copy instance data
     *
     * When manipulating data, you create a copy of the configuration using the
     * copy constructor, edit it and then transfer its content to the original
     * using this method.
     * @param[in] source The source instance (the edited copy)
     */
    virtual void copyFrom(CommtactLinkConfiguration *source);

    /// Helper static methods

    /*!
     * @brief Root path for QSettings
     *
     * @return The root path of the settings.
     */
    static const QString settingsRoot();

    /*!
     * @brief Create new link configuration instance
     *
     * Configuration Factory. Creates an appropriate configuration instance based
     * on the given type.
     * @return A new instance of the given type
     */
    static CommtactLinkConfiguration *createSettings(int type, const QString &name);

    /*!
     * @brief Duplicate configuration instance
     *
     * Helper method to create a new instance copy for editing.
     * @return A new copy of the given settings instance
     */
    static CommtactLinkConfiguration *duplicateSettings(CommtactLinkConfiguration *source);

  signals:
    void nameChanged(const QString &name);
    void linkChanged();

  protected:
    std::weak_ptr<CommtactLinkInterface> _link; ///< Link currently using this configuration (if any)

  private:
    QString _name;
};

typedef std::shared_ptr<CommtactLinkConfiguration> SharedCommtactLinkConfigurationPtr;
typedef std::weak_ptr<CommtactLinkConfiguration> WeakCommtactLinkConfigurationPtr;
