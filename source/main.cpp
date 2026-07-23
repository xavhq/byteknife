#include <QApplication>
#include <ui/app.hpp>

int main(int argc, char** argv) {
    QApplication qapp(argc, argv);
    qapp.setOrganizationName("byteknife");
    qapp.setApplicationName("byteknife");

    ui::App app(argc, argv);
    app.show();

    return qapp.exec();
}
