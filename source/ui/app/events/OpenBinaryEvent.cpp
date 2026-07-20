#include <ui/app.hpp>

#include <QProgressDialog>
#include <QApplication>
#include <QFileDialog>
#include <QStatusBar>
#include <QString>

void ui::App::OpenBinaryEvent() {
    QString file = QFileDialog::getOpenFileName(
        this, "Open Binary", QDir::homePath(),
        "Executable Files (*.elf *.exe *.bin);;All Files (*)"
    );

    if (file.isEmpty())
        return;

    auto* progress = new QProgressDialog("Loading binary...\n'" + file +"'", QString(), 0, 0, this);
    progress->setWindowModality(Qt::WindowModal);
    progress->setCancelButton(nullptr);
    progress->setMinimumDuration(0);
    progress->show();
}
