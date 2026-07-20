#include <ui/app.hpp>
#include <ui/views/binary/view.hpp>
#include <ui/views/hex/view.hpp>

#include <QApplication>
#include <QMenu>
#include <QAction>

void ui::App::WorkViewsInit() {
    QTabWidget* tabs = new QTabWidget(this);

    ui::views::BinaryView* binary_view = new ui::views::BinaryView(this);
    tabs->addTab(binary_view, "Binary View");

    QWidget* hexview = new QWidget(this);
    tabs->addTab(hexview, "Hex View");

    this->setCentralWidget(tabs);
}
