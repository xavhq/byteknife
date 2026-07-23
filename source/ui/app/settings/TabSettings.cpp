#include <ui/app.hpp>

#include <QSettings>

namespace ui {
    void App::SaveTabOrder() {
        QSettings settings;
        QStringList order;

        for (int i = 0; i < this->work_view_tabs_->count(); i++)
            order << this->work_view_tabs_->tabText(i);

        settings.setValue("work_view_tabs/order", order);
    }

    void App::RestoreTabOrder() {
        QSettings settings;
        QStringList order = settings.value("work_view_tabs/order").toStringList();
        if (order.isEmpty())
            return;

        for (int target_index = 0; target_index < order.size(); target_index++) {
            /* find which current index holds this saved tab title */
            for (int i = 0; i < this->work_view_tabs_->count(); i++) {
                if (this->work_view_tabs_->tabText(i) == order.at(target_index)) {
                    this->work_view_tabs_->tabBar()->moveTab(i, target_index);
                    break;
                }
            }
        }
    }
}
