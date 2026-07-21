#pragma once
#include <ui/app/components/ViewPlaceHolderContent.hpp>

#include <ui/views/hex/hviewport.hpp>
#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>

namespace ui::views {
    class HexView : public QWidget {
        Q_OBJECT
    public:
        explicit HexView(QWidget* parent = nullptr);

        bool LoadFile(const QString& path);
        void LoadBytes(const QByteArray& data);
        void Clear();

        void OnBinaryOpen();
        void OnBinaryClosed();

    private:
        QStackedWidget* stack_;
        HexViewport* hex_view_;
        ui::views::ViewPlaceHolderContent* placeholder_;

        QWidget* toolbar_;
        QPushButton* scan_button_;
        QPushButton* find_next_button_;

        void SetupStyle();
        void SetupToolbar();
    };
}
