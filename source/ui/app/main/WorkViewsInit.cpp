#include <ui/app.hpp>
#include <ui/views/disassembly/view.hpp>
#include <ui/views/hex/view.hpp>

#include <QApplication>
#include <QMenu>
#include <QAction>

void ui::App::WorkViewsInit() {
    this->work_view_tabs_ = new QTabWidget(this);
    this->work_view_tabs_->setMovable(true);

    this->disassembly_view_ = new ui::views::DisassemblyView(this);
    this->work_view_tabs_->addTab(this->disassembly_view_, "Disassembly View");

    //this->decompiler_view_ = new QWidget(this);
    // this->work_view_tabs_->addTab(this->decompiler_view_, "Decompiler View");

    this->hex_view_ = new ui::views::HexView(this);
    this->work_view_tabs_->addTab(this->hex_view_, "Hex View");

    this->setCentralWidget(this->work_view_tabs_);

    this->RestoreTabOrder(); /* apply saved order */

    connect(this->work_view_tabs_->tabBar(), &QTabBar::tabMoved, this, [this](int, int) {
        this->SaveTabOrder(); /* whenever rearranged, save the order */
    });

    /* tab keybinds */
    for (int i = 0; i < this->work_view_tabs_->count() && i < 9; i++) {
        /* this does some-what render the tab command useless, but its there if you need it */
        QShortcut* shortcut = new QShortcut(QKeySequence(QString("Ctrl+%1").arg(i + 1)), this);
        connect(shortcut, &QShortcut::activated, this, [this, i]() {
            this->work_view_tabs_->setCurrentIndex(i);
        });
    }
}
