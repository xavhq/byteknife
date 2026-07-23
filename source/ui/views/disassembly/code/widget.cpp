#include <ui/views/disassembly/code/widget.hpp>
#include <ui/views/disassembly/code/delegate.hpp>

#include <QVBoxLayout>
#include <QHeaderView>
#include <QScrollBar>
#include <QFontDatabase>

namespace ui::views {
    CodeWidget::CodeWidget(QWidget* parent) : QWidget(parent) {
        this->table_ = new CodeTable(this);
        this->model_ = new CodeModel(this);

        this->table_->setModel(this->model_);
        this->table_->setItemDelegate(new CodeDelegate(this->table_));

        this->table_->setSelectionBehavior(QAbstractItemView::SelectRows);
        this->table_->setSelectionMode(QAbstractItemView::ExtendedSelection);
        this->table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
        this->table_->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        this->table_->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        this->table_->verticalScrollBar()->setSingleStep(5);
        this->table_->verticalHeader()->hide();
        this->table_->verticalHeader()->setDefaultSectionSize(28);
        this->table_->setFocusPolicy(Qt::StrongFocus);

        auto* header = this->table_->horizontalHeader();
        header->setSectionResizeMode(QHeaderView::Interactive);
        header->setFixedHeight(0);
        header->setSectionsClickable(false);
        header->setHighlightSections(false);

        this->table_->setColumnWidth(0, 300);
        this->table_->setColumnWidth(1, 180);
        this->table_->setColumnWidth(2, 500);

        this->table_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        this->table_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        this->table_->setShowGrid(false);

        QFont font = this->table_->font();
        font.setFamily(QFontDatabase::systemFont(QFontDatabase::FixedFont).family());
        font.setPointSize(14);
        this->table_->setFont(font);

        this->table_->setStyleSheet(
            "QScrollBar:vertical { width: 20px; }"
            "QScrollBar:horizontal { height: 20px; }"
            "QTableView::item:selected { background-color: rgba(255,255,255,45); color: #EAEAEA; }"
        );

        auto* layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(this->table_);
    }

    void CodeWidget::AddInstructionRow(const QString& address, const QString& bytes, const QString& text) {
        this->model_->AddRow(address, bytes, text);
    }

    void CodeWidget::ClearDisassembly() {
        this->model_->Clear();
    }
}
