#include <ui/app.hpp>
#include <ui/app/components/CommandLineToken.hpp>

#include <QDesktopServices>
#include <QUrl>

void ui::App::help() {
    QDesktopServices::openUrl(QUrl("https://github.com/xavhq/byteknife/blob/master/docs/byteknife.md"));
}
