#include <ui/app.hpp>
#include <ui/app/components/CommandLineToken.hpp>

#include <QDebug>

#include <vector>

void ui::App::DispatchCommandEvent(CLTokenVector& tokens) {
    if (tokens.empty())
        return;

    QString tkn = tokens.at(0).token;
    if (tkn == "help") {
        this->help();
    } else if (tkn == "tab") {
        this->tab(tokens);
    } else if (tkn == "find") {
        this->find(tokens);
    } else if (tkn == "openbin") {
        this->openbin();
    } else if (tkn == "exit") {
        this->exit();
    }
}
