#include <ui/views/hex/view.hpp>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFontDatabase>
#include <QFile>

namespace ui::views {
    HexView::HexView(QWidget* parent) : QWidget(parent) {
        this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        {   /* stack */
            this->stack_ = new QStackedWidget(this);
            this->stack_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            this->stack_->setContentsMargins(0, 0, 0, 0);
        }

        {   /* placeholder */
            this->placeholder_ = new ViewPlaceHolderContent(this->stack_);
            this->placeholder_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            this->stack_->addWidget(this->placeholder_);
        }

        {   /* hex view */
            this->hex_view_ = new HexViewport(this->stack_);
            this->hex_view_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            this->stack_->addWidget(this->hex_view_);
        }

        {   /* stack index */
            this->stack_->setCurrentIndex(0);
        }

        {   /* toolbar */
            this->SetupToolbar();
            this->toolbar_->setVisible(false);
        }

        {   /* layout */
            auto* layout = new QVBoxLayout(this);
            layout->setContentsMargins(8, 8, 8, 8);
            layout->setSpacing(8);
            layout->addWidget(this->toolbar_);
            layout->addWidget(this->stack_);
            layout->setStretchFactor(this->toolbar_, 0);
            layout->setStretchFactor(this->stack_, 1);
        }

        this->SetupStyle();
    }

    void HexView::SetupToolbar() {
        this->toolbar_ = new QWidget(this);
        this->toolbar_->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

        this->scan_button_ = new QPushButton("Find Pattern", this->toolbar_);
        this->find_next_button_ = new QPushButton("Find Next", this->toolbar_);
        this->scan_button_->hide();
        this->find_next_button_->hide();

        auto* toolbar_layout = new QHBoxLayout(this->toolbar_);
        toolbar_layout->setContentsMargins(0, 0, 0, 0);
        toolbar_layout->addWidget(this->scan_button_);
        toolbar_layout->addWidget(this->find_next_button_);
        toolbar_layout->addStretch(); /* push buttons to the left */

        connect(this->scan_button_, &QPushButton::clicked, this->hex_view_, &HexViewport::FindPatternFromUser);
        connect(this->find_next_button_, &QPushButton::clicked, this->hex_view_, &HexViewport::FindNext);
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
        this->OnBinaryOpen();
    }

    void HexView::Clear() {
        this->hex_view_->Clear();
    }

    void HexView::OnBinaryOpen() {
        this->stack_->setCurrentIndex(1);
        this->toolbar_->setVisible(true);
        this->scan_button_->show();
        this->find_next_button_->show();
    }

    void HexView::OnBinaryClosed() {
        this->Clear();
        this->stack_->setCurrentIndex(0);
        this->toolbar_->setVisible(false);
    }
}
