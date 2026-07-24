#include <ui/app.hpp>

#include <QPlainTextEdit>
#include <QApplication>
#include <QDockWidget>
#include <QTreeWidget>
#include <QToolButton>
#include <QTabWidget>
#include <QStatusBar>
#include <QSplitter>
#include <QToolBar>
#include <QString>
#include <QLabel>
#include <QMenu>
#include <QDesktopServices>
#include <QUrl>
#include <QProxyStyle>

void ui::App::MenuInit() {
    /* main toolbar */
    QToolBar* toolbar = new QToolBar("Main Toolbar", this);
    toolbar->setMovable(false);
    toolbar->setFloatable(false);
    toolbar->setVisible(true);
    this->addToolBar(Qt::TopToolBarArea, toolbar);

    /* file menu */
    {
        QMenu* file_menu = new QMenu(this);
        QToolButton* file_button = new QToolButton(this);
        file_button->setText("&File");
        file_button->setMenu(file_menu);
        file_button->setPopupMode(QToolButton::InstantPopup);
        toolbar->addWidget(file_button);

        QAction* open_binary_action = file_menu->addAction("Open &Binary...");
        open_binary_action->setShortcut(QKeySequence::Open);

        QAction* open_project_action = file_menu->addAction("&Open Project...");
        open_project_action->setShortcut(QKeySequence("Ctrl+Shift+O"));

        QAction* save_project_action = file_menu->addAction("&Save Project...");
        save_project_action->setShortcut(QKeySequence::Save);

        QAction* save_project_as_action = file_menu->addAction("Sa&ve Project as...");
        save_project_as_action->setShortcut(QKeySequence::SaveAs);

        QAction* export_patched_binary_action = file_menu->addAction("&Export Patched binary...");
        (void)export_patched_binary_action;

        file_menu->addSeparator();

        QAction* recent_files_action = file_menu->addAction("Recent Files");
        recent_files_action->setEnabled(false);
        (void)recent_files_action;

        file_menu->addSeparator();

        QAction* exit_action = file_menu->addAction("E&xit");
        exit_action->setShortcut(QKeySequence::Quit);

        connect(exit_action, &QAction::triggered, this, &QWidget::close);
        connect(open_binary_action, &QAction::triggered, this, &ui::App::OpenBinaryEvent);
    }

    /* edit menu */
    {
        QMenu* edit_menu = new QMenu(this);
        QToolButton* edit_button = new QToolButton(this);
        edit_button->setText("&Edit");
        edit_button->setMenu(edit_menu);
        edit_button->setPopupMode(QToolButton::InstantPopup);
        toolbar->addWidget(edit_button);

        QAction* undo_action = edit_menu->addAction("&Undo");
        undo_action->setShortcut(QKeySequence::Undo);
        (void)undo_action;

        QAction* redo_action = edit_menu->addAction("&Redo");
        redo_action->setShortcut(QKeySequence::Redo);
        (void)redo_action;

        edit_menu->addSeparator();

        QAction* cut_action = edit_menu->addAction("Cu&t");
        cut_action->setShortcut(QKeySequence::Cut);
        (void)cut_action;

        QAction* copy_action = edit_menu->addAction("&Copy");
        copy_action->setShortcut(QKeySequence::Copy);
        (void)copy_action;

        QAction* paste_action = edit_menu->addAction("&Paste");
        paste_action->setShortcut(QKeySequence::Paste);
        (void)paste_action;

        edit_menu->addSeparator();

        QAction* select_all_action = edit_menu->addAction("Select A&ll");
        select_all_action->setShortcut(QKeySequence::SelectAll);
        (void)select_all_action;

        QAction* find_action = edit_menu->addAction("&Find...");
        find_action->setShortcut(QKeySequence::Find);
        (void)find_action;

        QAction* find_next_action = edit_menu->addAction("Find &Next");
        find_next_action->setShortcut(QKeySequence::FindNext);
        (void)find_next_action;

        QAction* find_previous_action = edit_menu->addAction("Find &Previous");
        find_previous_action->setShortcut(QKeySequence::FindPrevious);
        (void)find_previous_action;

        edit_menu->addSeparator();

        QAction* patch_action = edit_menu->addAction("&Patch...");
        patch_action->setShortcut(QKeySequence("Ctrl+P"));
        (void)patch_action;
    }

    /* view menu */
    {
        QMenu* view_menu = new QMenu(this);
        QToolButton* view_button = new QToolButton(this);
        view_button->setText("&View");
        view_button->setMenu(view_menu);
        view_button->setPopupMode(QToolButton::InstantPopup);
        toolbar->addWidget(view_button);

        QAction* disassembly_action = view_menu->addAction("&Disassembly");
        disassembly_action->setCheckable(true);
        disassembly_action->setChecked(true);
        this->BindCheckableSetting(disassembly_action, "view/disassemblytab", true, [this](bool checked) {
            this->work_view_tabs_->setTabVisible(this->work_view_tabs_->indexOf(this->disassembly_view_), checked);
        });

        QAction* decompiler_action = view_menu->addAction("&Decompiler");
        decompiler_action->setCheckable(true);
        decompiler_action->setChecked(true);
        (void)decompiler_action;

        QAction* hex_dump_action = view_menu->addAction("&Hex Dump");
        hex_dump_action->setCheckable(true);
        hex_dump_action->setChecked(true);
        this->BindCheckableSetting(hex_dump_action, "view/hexdumptab", true, [this](bool checked) {
            this->work_view_tabs_->setTabVisible(this->work_view_tabs_->indexOf(this->hex_view_), checked);
        });

        view_menu->addSeparator();

        QAction* symbols_tab_action = view_menu->addAction("&Symbols");
        symbols_tab_action->setCheckable(true);
        symbols_tab_action->setChecked(true);
        this->BindCheckableSetting(symbols_tab_action, "view/symbolspanel", true, [this](bool checked) {
            this->disassembly_view_->GetSymbolsPanel()->setVisible(checked);
        });

        QAction* functions_tab_action = view_menu->addAction("&Functions");
        functions_tab_action->setCheckable(true);
        functions_tab_action->setChecked(true);
        (void)functions_tab_action;

        QAction* imports_tab_action = view_menu->addAction("&Imports");
        imports_tab_action->setCheckable(true);
        imports_tab_action->setChecked(true);
        (void)imports_tab_action;

        QAction* xref_tab_action = view_menu->addAction("&XRefs");
        xref_tab_action->setCheckable(true);
        xref_tab_action->setChecked(true);
        this->BindCheckableSetting(xref_tab_action, "view/xrefpanel", true, [this](bool checked) {
            this->disassembly_view_->GetXRefsPanel()->setVisible(checked);
        });

        view_menu->addSeparator();

        QAction* show_toolbar_action = view_menu->addAction("&Toolbar");
        show_toolbar_action->setCheckable(true);
        show_toolbar_action->setChecked(true);
        (void)show_toolbar_action;

        QAction* show_statusbar_action = view_menu->addAction("&Status Bar");
        show_statusbar_action->setCheckable(true);
        show_statusbar_action->setChecked(true);
        this->BindCheckableSetting(show_statusbar_action, "view/statusbar", true, [this](bool checked) {
            this->statusBar()->setVisible(checked);
        });
    }

    /* debug menu */
    {
        QMenu* debug_menu = new QMenu(this);
        QToolButton* debug_button = new QToolButton(this);
        debug_button->setText("&Debug");
        debug_button->setMenu(debug_menu);
        debug_button->setPopupMode(QToolButton::InstantPopup);
        toolbar->addWidget(debug_button);

        QAction* run_action = debug_menu->addAction("&Run");
        run_action->setShortcut(QKeySequence("F9"));
        (void)run_action;

        QAction* step_into_action = debug_menu->addAction("Step &Into");
        step_into_action->setShortcut(QKeySequence("F7"));
        (void)step_into_action;

        QAction* step_over_action = debug_menu->addAction("Step &Over");
        step_over_action->setShortcut(QKeySequence("F8"));
        (void)step_over_action;

        QAction* step_out_action = debug_menu->addAction("Step O&ut");
        step_out_action->setShortcut(QKeySequence("Ctrl+F9"));
        (void)step_out_action;

        debug_menu->addSeparator();

        QAction* toggle_breakpoint_action = debug_menu->addAction("Toggle &Breakpoint");
        toggle_breakpoint_action->setShortcut(QKeySequence("F2"));
        (void)toggle_breakpoint_action;

        QAction* clear_breakpoints_action = debug_menu->addAction("Clear &Breakpoints");
        clear_breakpoints_action->setShortcut(QKeySequence("Ctrl+Shift+F2"));
        (void)clear_breakpoints_action;

        debug_menu->addSeparator();

        QAction* attach_to_process_action = debug_menu->addAction("&Attach to Process...");
        attach_to_process_action->setShortcut(QKeySequence("Ctrl+A"));
        (void)attach_to_process_action;

        QAction* detach_from_process_action = debug_menu->addAction("&Detach from Process");
        detach_from_process_action->setEnabled(false);
        (void)detach_from_process_action;
    }

    /* plugins menu */
    {
        QMenu* plugins_menu = new QMenu(this);
        QToolButton* plugins_button = new QToolButton(this);
        plugins_button->setText("&Plugins");
        plugins_button->setMenu(plugins_menu);
        plugins_button->setPopupMode(QToolButton::InstantPopup);
        toolbar->addWidget(plugins_button);

        QAction* manage_plugins_action = plugins_menu->addAction("&Manage Plugins...");
        (void)manage_plugins_action;

        plugins_menu->addSeparator();

        QAction* no_plugins_action = plugins_menu->addAction("(no plugins loaded)");
        no_plugins_action->setEnabled(false);
        (void)no_plugins_action;
    }

    /* tools menu */
    {
        QMenu* tools_menu = new QMenu(this);
        QToolButton* tools_button = new QToolButton(this);
        tools_button->setText("&Tools");
        tools_button->setMenu(tools_menu);
        tools_button->setPopupMode(QToolButton::InstantPopup);
        toolbar->addWidget(tools_button);

        QAction* analyze_action = tools_menu->addAction("&Analyze...");
        analyze_action->setShortcut(QKeySequence("Ctrl+A"));
        (void)analyze_action;

        QAction* compare_action = tools_menu->addAction("&Compare...");
        (void)compare_action;

        QAction* entropy_action = tools_menu->addAction("&Entropy Calculator...");
        (void)entropy_action;

        QAction* signature_scan_action = tools_menu->addAction("&Signature Scan...");
        (void)signature_scan_action;

        QAction* find_crypto_action = tools_menu->addAction("&Find Crypto...");
        (void)find_crypto_action;

        tools_menu->addSeparator();

        QAction* options_action = tools_menu->addAction("&Options...");
        options_action->setShortcut(QKeySequence::Preferences);
        (void)options_action;
    }

    /* window menu */
    {
        QMenu* window_menu = new QMenu(this);
        QToolButton* window_button = new QToolButton(this);
        window_button->setText("&Window");
        window_button->setMenu(window_menu);
        window_button->setPopupMode(QToolButton::InstantPopup);
        toolbar->addWidget(window_button);

        QAction* close_all_tabs_action = window_menu->addAction("&Close All Tabs");
        (void)close_all_tabs_action;

        window_menu->addSeparator();

        QAction* next_tab_action = window_menu->addAction("&Next Tab");
        next_tab_action->setShortcut(QKeySequence::NextChild);
        (void)next_tab_action;

        QAction* previous_tab_action = window_menu->addAction("&Previous Tab");
        previous_tab_action->setShortcut(QKeySequence::PreviousChild);
        (void)previous_tab_action;

        window_menu->addSeparator();

        QAction* reset_layout_action = window_menu->addAction("&Reset Layout");
        (void)reset_layout_action;
    }

    /* help menu */
    {
        QMenu* help_menu = new QMenu(this);
        QToolButton* help_button = new QToolButton(this);
        help_button->setText("&Help");
        help_button->setMenu(help_menu);
        help_button->setPopupMode(QToolButton::InstantPopup);
        toolbar->addWidget(help_button);

        QAction* documentation_action = help_menu->addAction("&Documentation");
        documentation_action->setShortcut(QKeySequence::HelpContents);
        connect(documentation_action, &QAction::triggered, []() {
            QDesktopServices::openUrl(QUrl("https://github.com/xavhq/byteknife/blob/master/docs/byteknife.md"));
        });

        QAction* about_action = help_menu->addAction("&About");
        about_action->setShortcut(QKeySequence("Ctrl+Shift+A"));
        (void)about_action;

        QAction* about_qt_action = help_menu->addAction("About &Qt");
        connect(about_qt_action, &QAction::triggered, []() {
            QDesktopServices::openUrl(QUrl("https://www.qt.io/group"));
        });
    }

    /* status bar */
    QStatusBar* status_bar = this->statusBar();
    status_bar->showMessage("ready");
}
