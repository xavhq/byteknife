#include <ui/views/binary/view.hpp>

#include <QVBoxLayout>
#include <QResizeEvent>

namespace ui::views {
    BinaryView::BinaryView(QWidget* parent) : QWidget(parent) {
        this->main_splitter_ = new QSplitter(Qt::Horizontal, this);

        {   /* disassembly */
            this->disassembly_ = new DisassemblyWidget(this);
            this->disassembly_->setMinimumWidth(400);
            this->disassembly_->setVisible(false);
        }

        {   /* functions */
            this->functions_ = new QWidget(this);
            this->functions_->setMinimumWidth(250);
            this->functions_->setMaximumWidth(400);
            this->functions_->setVisible(false);
        }

        {   /* xrefs */
            this->xrefs_ = new QWidget(this);
            this->xrefs_->setMinimumWidth(250);
            this->xrefs_->setMaximumWidth(400);
            this->xrefs_->setVisible(false);
        }

        {   /* palceholder */
            this->disassembly_stack_ = new QStackedWidget(this);
            this->disassembly_stack_->setMinimumWidth(400);

            this->disassembly_ = new DisassemblyWidget(this->disassembly_stack_);
            this->placeholder_ = new ViewPlaceHolderContent(this->disassembly_stack_);

            this->disassembly_stack_->addWidget(this->placeholder_);
            this->disassembly_stack_->addWidget(this->disassembly_);
            this->disassembly_stack_->setCurrentIndex(0);
        }

        {   /* splitter */
            this->main_splitter_->addWidget(this->functions_);
            this->main_splitter_->addWidget(this->disassembly_stack_);
            this->main_splitter_->addWidget(this->xrefs_);
            this->main_splitter_->setStretchFactor(0, 1);
            this->main_splitter_->setStretchFactor(1, 4);
            this->main_splitter_->setStretchFactor(2, 1);
            this->main_splitter_->setChildrenCollapsible(false);
        }

        {   /* layout */
            auto* layout = new QVBoxLayout(this);
            layout->setContentsMargins(0, 0, 0, 0);
            layout->addWidget(this->main_splitter_);
        }
    }

    void BinaryView::AddInstructionRow(const QString& address, const QString& bytes, const QString& text) {
        this->disassembly_->AddInstructionRow(address, bytes, text);
    }

    void BinaryView::ClearDisassembly() {
        this->disassembly_->ClearDisassembly();
    }

    void BinaryView::OnBinaryOpen() {
        this->functions_->setVisible(true);
        this->xrefs_->setVisible(true);
        this->disassembly_stack_->setCurrentIndex(1); /* show disassembly */
    }

    void BinaryView::OnBinaryClosed() {
        this->functions_->setVisible(false);
        this->xrefs_->setVisible(false);
        this->ClearDisassembly();
        this->disassembly_stack_->setCurrentIndex(0); /* show placeholder */
    }
}
