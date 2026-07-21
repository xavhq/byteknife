#pragma once
#include <ui/app/components/ViewPlaceHolderContent.hpp>

#include <ui/views/hex/hviewport.hpp>
#include <QWidget>
#include <QStackedWidget>

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

        void SetupStyle();
    };
}
