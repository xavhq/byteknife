#include <ui/app.hpp>
#include <ui/views/binary/view.hpp>
#include <ui/views/hex/view.hpp>
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

    QProgressDialog progress("Loading binary\n'" + file + "'", QString(), 0, 0, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setCancelButton(nullptr);
    progress.setMinimumDuration(0);
    progress.show();
    QApplication::processEvents(); /* force the dialog to actually paint before blocking work starts */

    if (!this->hex_view_->LoadFile(file)) {
        progress.close();
        QMessageBox::warning(this, "Error", "Failed to load hex view");
        return;
    }

    this->binary_view_->OnBinaryOpen();
    this->hex_view_->OnBinaryOpen();

    /* TODO: replace with real disassembly output. */
    this->binary_view_->AddInstructionRow("0x00401000", "55 8B EC", "push ebp");
    this->binary_view_->AddInstructionRow("0x00401001", "8B 45 08", "mov eax, [ebp+8]");

    progress.close();
}
