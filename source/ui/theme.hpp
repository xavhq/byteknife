#pragma once

#include <QApplication>
#include <QStyleFactory>
#include <QPalette>
#include <QColor>

namespace ui {
    inline static void ApplyDarkTheme() {
        qApp->setStyle(QStyleFactory::create("Fusion"));

        QPalette palette;

        QColor bg(30, 31, 34);
        QColor bg_alt(37, 38, 42);
        QColor bg_elevated(45, 46, 51);
        QColor text(220, 220, 224);
        QColor text_disabled(120, 120, 126);
        QColor accent(64, 128, 224);

        palette.setColor(QPalette::Window, bg);
        palette.setColor(QPalette::WindowText, text);
        palette.setColor(QPalette::Base, bg_alt);
        palette.setColor(QPalette::AlternateBase, bg_elevated);
        palette.setColor(QPalette::ToolTipBase, bg_elevated);
        palette.setColor(QPalette::ToolTipText, text);
        palette.setColor(QPalette::Text, text);
        palette.setColor(QPalette::Disabled, QPalette::Text, text_disabled);
        palette.setColor(QPalette::Button, bg_elevated);
        palette.setColor(QPalette::ButtonText, text);
        palette.setColor(QPalette::Disabled, QPalette::ButtonText, text_disabled);
        palette.setColor(QPalette::BrightText, Qt::red);
        palette.setColor(QPalette::Link, accent);

        palette.setColor(QPalette::Highlight, accent);
        palette.setColor(QPalette::HighlightedText, Qt::black);
        palette.setColor(QPalette::Disabled, QPalette::Highlight, bg_elevated);
        palette.setColor(QPalette::Disabled, QPalette::HighlightedText, text_disabled);

        palette.setColor(QPalette::PlaceholderText, text_disabled);

        qApp->setPalette(palette);
    }

    inline static void ApplyLightTheme() {
        qApp->setStyle(QStyleFactory::create("Fusion"));

        QPalette palette;

        palette.setColor(QPalette::Window, QColor(240, 240, 240));
        palette.setColor(QPalette::WindowText, Qt::black);
        palette.setColor(QPalette::Base, Qt::white);
        palette.setColor(QPalette::AlternateBase, QColor(233, 233, 238));
        palette.setColor(QPalette::ToolTipBase, QColor(255, 255, 220));
        palette.setColor(QPalette::ToolTipText, Qt::black);
        palette.setColor(QPalette::Text, Qt::black);
        palette.setColor(QPalette::Disabled, QPalette::Text, QColor(120, 120, 120));
        palette.setColor(QPalette::Button, QColor(240, 240, 240));
        palette.setColor(QPalette::ButtonText, Qt::black);
        palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(120, 120, 120));
        palette.setColor(QPalette::BrightText, Qt::red);
        palette.setColor(QPalette::Link, QColor(0, 85, 255));
        palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        palette.setColor(QPalette::HighlightedText, Qt::white);
        palette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(145, 145, 145));
        palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(190, 190, 190));
        palette.setColor(QPalette::PlaceholderText, QColor(160, 160, 160));

        qApp->setPalette(palette);
    }
}
