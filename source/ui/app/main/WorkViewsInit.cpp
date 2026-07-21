#include <ui/app.hpp>
#include <ui/views/binary/view.hpp>
#include <ui/views/hex/view.hpp>
#include <QApplication>
#include <QMenu>
#include <QAction>

void ui::App::WorkViewsInit() {
    QTabWidget* tabs = new QTabWidget(this);

    this->binary_view_ = new ui::views::BinaryView(this);
    tabs->addTab(this->binary_view_, "Binary View");

    this->hex_view_ = new ui::views::HexView(this);
    tabs->addTab(this->hex_view_, "Hex View");

    this->setCentralWidget(tabs);
}
