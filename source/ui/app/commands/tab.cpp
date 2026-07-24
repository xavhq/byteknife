#include <ui/app.hpp>
#include <ui/app/components/CommandLineToken.hpp>
#include <ui/views/output/view.hpp>

void ui::App::tab(CLTokenVector& tokens) {
    if (tokens.size() < 2) {
        if (g_output)
            g_output->Warn("Usage: tab <index>");

        return;
    }

    bool ok = false;
    int index = tokens.at(1).token.toInt(&ok)-1; /* so first tab will be 1 not 0 */

    if (!ok || index < 0 || index >= this->work_view_tabs_->count()) {
        return;
    }

    this->work_view_tabs_->setCurrentIndex(index);
}
