#pragma once
#include <QWidget>

namespace ui::views {
    class ViewPlaceHolderContent : public QWidget {
        Q_OBJECT
    public:
        explicit ViewPlaceHolderContent(QWidget* parent = nullptr);
    signals:
        void OpenBinaryRequested();
    };
}
