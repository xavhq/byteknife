#include <QApplication>
#include <ui/app.hpp>

int main(int argc, char** argv) {
    QApplication qapp(argc, argv);

    ui::App app(argc, argv);
    app.show();

    return qapp.exec();
}
