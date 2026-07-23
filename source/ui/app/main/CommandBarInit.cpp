#include <ui/app.hpp>

#include <QApplication>
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QWidget>
#include <QToolBar>
#include <QCompleter>
#include <QShortcut>

void ui::App::CommandBarInit() {
    QWidget* bar = new QWidget(this);
    bar->setFixedHeight(26);

    QHBoxLayout* layout = new QHBoxLayout(bar);
    layout->setContentsMargins(8, 0, 6, 0);
    layout->setSpacing(6);

    this->command_input_ = new QLineEdit(bar);
    this->command_input_->setPlaceholderText("Enter command...");
    this->command_input_->setFrame(false);
    this->command_input_->setFixedHeight(22);
    this->command_input_->setStyleSheet(
        "QLineEdit {"
        "  font-size: 13px;"
        "  color: palette(text);"
        "  background-color: palette(window);"
        "  border: none;"
        "}"
    );

    QPalette faded_palette = this->command_input_->palette();
    QColor muted = faded_palette.color(QPalette::Text);
    muted.setAlpha(60);

    faded_palette.setColor(QPalette::Highlight, muted);
    faded_palette.setColor(QPalette::HighlightedText, faded_palette.color(QPalette::Text));
    this->command_input_->setPalette(faded_palette);

    layout->addWidget(this->command_input_);

    QPushButton* send_button = new QPushButton("Send", bar);
    send_button->setFixedHeight(22);
    send_button->setFixedWidth(60);
    send_button->setCursor(Qt::PointingHandCursor);
    send_button->setStyleSheet(
        "QPushButton {"
        "  background-color: palette(button);"
        "  color: palette(button-text);"
        "  border: none;"
        "  border-radius: 4px;"
        "  font-size: 12px;"
        "  font-weight: 500;"
        "}"
        "QPushButton:hover {"
        "  background-color: palette(highlight);"
        "  color: palette(highlighted-text);"
        "}"
        "QPushButton:pressed {"
        "  background-color: palette(dark);"
        "}"
    );
    layout->addWidget(send_button);

    QToolBar* command_toolbar = new QToolBar("Command Bar", this);
    command_toolbar->setMovable(false);
    command_toolbar->setFloatable(false);
    command_toolbar->setContentsMargins(0, 0, 0, 0);
    command_toolbar->addWidget(bar);
    this->addToolBar(Qt::BottomToolBarArea, command_toolbar);

    QStringList known_commands = {
        "help",
        "find",
        "goto",
        "tab"
    };

    /* flat-list command completion */
    QCompleter* completer = new QCompleter(known_commands, this);
    completer->setCompletionMode(QCompleter::InlineCompletion);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    this->command_input_->setCompleter(completer);

    connect(send_button, &QPushButton::clicked, this, &ui::App::CommandSendEvent);
    connect(this->command_input_, &QLineEdit::returnPressed, this, &ui::App::CommandSendEvent);

    /* focus keybind */
    QShortcut* focus_shortcut = new QShortcut(QKeySequence("Ctrl+`"), this);
    connect(focus_shortcut, &QShortcut::activated, this, [this]() {
        this->command_input_->setFocus();
        this->command_input_->selectAll();
    });
}
