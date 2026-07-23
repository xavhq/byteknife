#include <ui/app.hpp>
#include <ui/app/components/CommandLineToken.hpp>

#include <vector>

void ui::App::CommandSendEvent() {
    QString input = this->command_input_->text();
    this->command_input_->clear();
    QStringList tokens_sl = input.split(' ', Qt::SkipEmptyParts);

    CLTokenVector tokens;

    for (const QString& token : tokens_sl) {
        CommandLineToken tkn;
        tkn.token = token;

        for (QChar ch : token)
            tkn.chars.push_back(ch);

        tokens.push_back(tkn);
    }

    this->DispatchCommandEvent(tokens);
}
