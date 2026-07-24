#include <ui/app.hpp>

#include <QSettings>

void ui::App::BindCheckableSetting(QAction* action, const QString& key, bool default_value, std::function<void(bool)> apply) {
    QSettings settings;
    bool saved = settings.value(key, default_value).toBool();

    action->setChecked(saved);
    apply(saved);

    connect(action, &QAction::toggled, this, [key, apply](bool checked) {
        QSettings settings;
        settings.setValue(key, checked);
        apply(checked);
    });
}
