#include <ui/app.hpp>
#include <ui/app/components/CommandLineToken.hpp>

void ui::App::tab(CLTokenVector& tokens) {
    if (tokens.size() < 2) {
        qDebug() << "Usage: tab <index>";
        return;
    }

    bool ok = false;
    int index = tokens.at(1).token.toInt(&ok)-1; /* so first tab will be 1 not 0 */

    if (!ok || index < 0 || index >= this->work_view_tabs_->count()) {
        qDebug() << "Invalid tab index:" << tokens.at(1).token;
        return;
    }

    this->work_view_tabs_->setCurrentIndex(index);
}
