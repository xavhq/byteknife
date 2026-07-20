#include <ui/views/binary/view.hpp>
#include <QVBoxLayout>

namespace ui::views {
    BinaryView::BinaryView(QWidget* parent) : QWidget(parent) {
        this->main_splitter_ = new QSplitter(Qt::Horizontal, this);

        this->functions_ = new QWidget(this);
        this->functions_->setMinimumWidth(250);
        this->functions_->setMaximumWidth(400);

        this->disassembly_ = new QWidget(this);
        this->disassembly_->setMinimumWidth(400);

        this->xrefs_ = new QWidget(this);
        this->xrefs_->setMinimumWidth(200);
        this->xrefs_->setMaximumWidth(350);

        this->main_splitter_->addWidget(this->functions_);
        this->main_splitter_->addWidget(this->disassembly_);
        this->main_splitter_->addWidget(this->xrefs_);
        this->main_splitter_->setStretchFactor(0, 1);
        this->main_splitter_->setStretchFactor(1, 4);
        this->main_splitter_->setStretchFactor(2, 1);
        this->main_splitter_->setChildrenCollapsible(false);

        auto* layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(this->main_splitter_);
    }
}
