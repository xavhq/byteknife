#include <ui/app.hpp>
#include <ui/app/components/ViewPlaceHolderContent.hpp>

#include <QLabel>
#include <QVBoxLayout>
#include <QDir>
#include <QCoreApplication>
#include <QFontDatabase>
#include <QDebug>

namespace ui::views {
    ViewPlaceHolderContent::ViewPlaceHolderContent(QWidget* parent) : QWidget(parent) {
        this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        auto* label = new QLabel(
            "No binary is loaded!<br>Press <a href=\"open\" style=\"color: gray;\">Ctrl+O</a> to open a binary",
            this
        );

        label->setAlignment(Qt::AlignCenter);
        label->setTextFormat(Qt::RichText);
        label->setWordWrap(false);
        label->setStyleSheet("color: gray;");
        label->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
        label->setOpenExternalLinks(false);
        label->setCursor(Qt::PointingHandCursor);
        label->setFocusPolicy(Qt::NoFocus);
        label->setContextMenuPolicy(Qt::NoContextMenu);

        connect(label, &QLabel::linkActivated, this, [](const QString&) {
            if (ui::OnOpenBinaryRequested)
                ui::OnOpenBinaryRequested();
        });

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
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        layout->addStretch(1);
        layout->addWidget(label, 0, Qt::AlignCenter);
        layout->addStretch(1);
        setLayout(layout);
    }
}
