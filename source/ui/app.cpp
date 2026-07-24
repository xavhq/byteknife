#include <ui/app.hpp>

#include <QApplication>
#include <QLabel>

namespace ui {
    App::App(int argc, char** argv, QWidget* parent) : QMainWindow(parent) {
        /* TODO: byteknife binary (opening binaries directly) */
        (void)argc;
        (void)argv;

        ui::OnOpenBinaryRequested = [this]() {
            this->OpenBinaryEvent();
        };

        this->WindowInit();
        this->WorkViewsInit();
        this->CommandBarInit();
        this->OutputInit();
        this->MenuInit();


        if (g_output)
            g_output->Info("byteknife fully loaded.");
    }
}
