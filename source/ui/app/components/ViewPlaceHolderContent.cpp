#include <ui/app/components/ViewPlaceHolderContent.hpp>
#include <QLabel>
#include <QVBoxLayout>

namespace ui::views {
    ViewPlaceHolderContent::ViewPlaceHolderContent(QWidget* parent) : QWidget(parent) {
        auto* label = new QLabel(
            "No binary is loaded!<br>Press <b>Ctrl+O</b> to open a binary",
            this
        );
        label->setAlignment(Qt::AlignCenter);
        label->setTextFormat(Qt::RichText);
        label->setWordWrap(false);
        label->setStyleSheet("color: gray;");

        { /* font setup */
            QString font_path = QDir(QCoreApplication::applicationDirPath()).filePath("assets/Satoshi-MediumItalic.otf");
            int font_id = QFontDatabase::addApplicationFont(font_path);
            if (font_id != -1) {
                const QString family = QFontDatabase::applicationFontFamilies(font_id).at(0);

                QFont font(family, 16);
                font.setStyleName("Medium Italic");
                label->setFont(font);
            } else {
                qWarning() << "Failed to load font from" << font_path;
                label->setFont(QFont(label->font().family(), 16));
            }
        }

        auto* layout = new QVBoxLayout(this);
        layout->addWidget(label, 0, Qt::AlignCenter);
        setLayout(layout);
    }
}
