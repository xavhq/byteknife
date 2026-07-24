#include <ui/app.hpp>
#include <ui/app/components/CommandLineToken.hpp>

#include <QDesktopServices>
#include <QUrl>

#include <ui/views/output/view.hpp>

void ui::App::help() {
    QDesktopServices::openUrl(QUrl("https://github.com/xavhq/byteknife/blob/master/docs/byteknife.md"));

    if (g_output)
        g_output->Info("Get help at: https://github.com/xavhq/byteknife/blob/master/docs/byteknife.md");
}
