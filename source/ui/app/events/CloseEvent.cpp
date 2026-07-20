#include <ui/app.hpp>

#include <QProgressDialog>
#include <QApplication>
#include <QFileDialog>
#include <QStatusBar>
#include <QString>

void ui::App::CloseEvent(QCloseEvent* event) {
    QDialog dialog(this);
    dialog.setWindowTitle("Exit byteknife");
    dialog.setFixedWidth(240);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(16, 10, 16, 10);
    layout->setSpacing(14);

    QLabel* label = new QLabel("Are you sure you want to exit?", &dialog);
    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true);
    layout->addWidget(label);

    QHBoxLayout* button_layout = new QHBoxLayout();
    button_layout->addStretch();

    QPushButton* yes_button = new QPushButton("Yes", &dialog);
    QPushButton* no_button = new QPushButton("No", &dialog);
    yes_button->setDefault(true);
    yes_button->setFixedWidth(80);
    no_button->setFixedWidth(80);

    button_layout->addWidget(yes_button);
    button_layout->addWidget(no_button);
    button_layout->addStretch();

    layout->addLayout(button_layout);

    connect(yes_button, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(no_button, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted)
        event->accept();
    else
        event->ignore();
}
