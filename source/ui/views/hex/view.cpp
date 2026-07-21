#include <ui/views/hex/view.hpp>

#include <QVBoxLayout>
#include <QFontDatabase>
#include <QFile>

namespace ui::views {
    HexView::HexView(QWidget* parent) : QWidget(parent) {
        this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        this->stack_ = new QStackedWidget(this);
        this->stack_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        this->placeholder_ = new ViewPlaceHolderContent(this->stack_);

        this->hex_view_ = new HexViewport(this->stack_);
        this->hex_view_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        this->stack_->addWidget(this->placeholder_); /* index 0 */
        this->stack_->addWidget(this->hex_view_);     /* index 1 */
        this->stack_->setCurrentIndex(0);

        auto* layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(this->stack_);

        this->SetupStyle();
    }

    void HexView::SetupStyle() {
        QFont font("JetBrains Mono");
        if (!QFontDatabase::hasFamily(font.family()))
            font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
        font.setPointSize(10);
        this->hex_view_->setFont(font);
        this->hex_view_->SetReadOnly(true);

        this->hex_view_->setStyleSheet(
            "QScrollBar:vertical { width: 14px; }"
            "QScrollBar:horizontal { height: 14px; }"
        );
    }

    bool HexView::LoadFile(const QString& path) {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly))
            return false;

        QByteArray bytes = file.readAll();
        file.close();

        this->LoadBytes(bytes);
        return true;
    }

    void HexView::LoadBytes(const QByteArray& data) {
        this->hex_view_->SetData(data);
    }

    void HexView::Clear() {
        this->hex_view_->Clear();
    }

    void HexView::OnBinaryOpen() {
        this->stack_->setCurrentIndex(1);
    }

    void HexView::OnBinaryClosed() {
        this->Clear();
        this->stack_->setCurrentIndex(0);
    }
}
