#include <ui/views/disassembly/view.hpp>

#include <QVBoxLayout>
#include <QResizeEvent>

namespace ui::views {
    DisassemblyView::DisassemblyView(QWidget* parent) : QWidget(parent) {
        this->main_splitter_ = new QSplitter(Qt::Horizontal, this);

        {   /* disassembly */
            this->code_ = new CodeWidget(this);
            this->code_->setMinimumWidth(400);
            this->code_->setVisible(false);
        }

        {   /* symbols */
            this->symbols_ = new QWidget(this);
            this->symbols_->setMinimumWidth(250);
            this->symbols_->setMaximumWidth(400);
            this->symbols_->setVisible(false);
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

            this->code_ = new CodeWidget(this->disassembly_stack_);
            this->placeholder_ = new ViewPlaceHolderContent(this->disassembly_stack_);

            this->disassembly_stack_->addWidget(this->placeholder_);
            this->disassembly_stack_->addWidget(this->code_);
            this->disassembly_stack_->setCurrentIndex(0);
        }

        {   /* splitter */
            this->main_splitter_->addWidget(this->symbols_);
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

    void DisassemblyView::AddInstructionRow(const QString& address, const QString& bytes, const QString& text) {
        this->code_->AddInstructionRow(address, bytes, text);
    }

    void DisassemblyView::ClearDisassembly() {
        this->code_->ClearDisassembly();
    }

    void DisassemblyView::OnBinaryOpen() {
        this->disassembly_stack_->setCurrentIndex(1); /* show code */
    }

    void DisassemblyView::OnBinaryClosed() {
        this->symbols_->setVisible(false);
        this->xrefs_->setVisible(false);
        this->ClearDisassembly();
        this->disassembly_stack_->setCurrentIndex(0); /* show placeholder */
    }
}
