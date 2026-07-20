#include <ui/views/binary/view.hpp>
#include <QVBoxLayout>

namespace ui::views {
    BinaryView::BinaryView(QWidget* parent) : QWidget(parent) {
        // Create a container widget for everything
        QWidget* container = new QWidget(this);
        QVBoxLayout* container_layout = new QVBoxLayout(container);
        container_layout->setContentsMargins(0, 0, 0, 0);
        container_layout->setSpacing(0);

        // Create the message label
        QLabel* message_label = new QLabel(this);
        message_label->setText("No binary is loaded yet; Press <b>Ctrl+O</b> to open a binary");
        message_label->setAlignment(Qt::AlignCenter);
        message_label->setStyleSheet("QLabel {"
                                     "    font-size: 14px;"
                                     "    color: #666;"
                                     "    padding: 20px;"
                                     "    background-color: #f5f5f5;"
                                     "    border-bottom: 1px solid #ddd;"
                                     "}");
        message_label->setVisible(false); // Initially hidden, show when no binary is loaded

        // Create the splitter
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

        // Add everything to the container layout
        container_layout->addWidget(message_label);
        container_layout->addWidget(this->main_splitter_);

        // Set the container as the main layout
        auto* layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(container);
    }
}
