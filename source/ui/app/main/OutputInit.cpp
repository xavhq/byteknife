#include <ui/app.hpp>

#include <QDockWidget>
#include <QShortcut>
#include <QSettings>

/* TODO: add output to view menu */
void ui::App::OutputInit() {
    this->output_ = new ui::views::Output(this);

    this->output_dock_ = new QDockWidget("Output", this);
    this->output_dock_->setObjectName("OutputDock");
    this->output_dock_->setWidget(this->output_);

    this->output_dock_->setFeatures(
        QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable
    );

    this->output_dock_->setStyleSheet(
        "QDockWidget::title {"
        "  padding: 6px 8px;"
        "  background-color: palette(window);"
        "}"
        "QDockWidget::close-button {"
        "  border: none;"
        "  background: transparent;"
        "  padding: 4px;"
        "  icon-size: 16px;"
        "}"
        "QDockWidget::close-button:hover {"
        "  background-color: palette(highlight);"
        "  border-radius: 3px;"
        "}"
    );

    this->addDockWidget(Qt::BottomDockWidgetArea, this->output_dock_);

    QShortcut* toggle_shortcut = new QShortcut(QKeySequence("Ctrl+J"), this);
    connect(toggle_shortcut, &QShortcut::activated, this, [this]() {
        this->output_dock_->setVisible(!this->output_dock_->isVisible());
    });
}
