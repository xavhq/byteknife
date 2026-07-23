#pragma once

#include <QString>
#include <QChar>

struct CommandLineToken {
    QString token;
    std::vector<QChar> chars;
};

using CLTokenVector = std::vector<CommandLineToken>;
