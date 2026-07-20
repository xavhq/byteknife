#include <ui/app.hpp>
#include <ui/app/styles/Theme.hpp>
#include <ui/app/styles/MnemonicStyle.hpp>

#include <QCoreApplication>
#include <QFontDatabase>
#include <QDir>

void ui::App::WindowInit() {
    this->setWindowTitle("byteknife");
    this->resize(1200, 800);

    {   /* font setup */
        QString font_path = QDir(QCoreApplication::applicationDirPath()).filePath("assets/Satoshi-Medium.otf");
        int font_id = QFontDatabase::addApplicationFont(font_path);
        if (font_id != -1) {
            const QString family = QFontDatabase::applicationFontFamilies(font_id).at(0);
            // qDebug() << "Loaded font family:" << family;
            qApp->setFont(QFont(family, 10));
        } else {
            qWarning() << "Failed to load font from" << font_path;
        }
    }

    // ui::Theme::ApplyDarkTheme();

    qApp->setStyle(new ui::Theme::MnemonicStyle(QStyleFactory::create("Fusion")));
    qApp->setStyleSheet("QToolButton::menu-indicator { image: none; width: 0px; }");
    this->setContextMenuPolicy(Qt::NoContextMenu);
}
