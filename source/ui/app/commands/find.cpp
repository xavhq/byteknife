#include <ui/app.hpp>
#include <ui/app/components/CommandLineToken.hpp>

void ui::App::find(CLTokenVector& tokens) {
    if (tokens.size() < 2) {
        qDebug() << "Usage: find <hex pattern | text | address>";
        return;
    }

    QStringList remainder;
    for (int i = 1; i < (int)tokens.size(); i++)
        remainder << tokens.at(i).token;

    /* detect: single token that is EXPLICITLY an address
       either "0x" prefixed, or longer than 2 chars
       (too long to be a single hex byte pattern like "AD") */
    if (remainder.size() == 1) {
        QString token = remainder.first();
        bool explicit_address = token.startsWith("0x", Qt::CaseInsensitive) || token.size() > 2;

        if (explicit_address) {
            QString hex_part = token.startsWith("0x", Qt::CaseInsensitive) ? token.mid(2) : token;
            bool ok = false;
            quint64 address = hex_part.toULongLong(&ok, 16); /* TODO: clamp to loaded binary's bit width */

            if (ok) {
                /* TODO: address search stub */
                qDebug() << "find: address mode not implemented yet, got:" << token;
                return;
            }
        }
    }

    /* detect: hex pattern (every token is "??" or exactly 2 hex chars) */
    bool looks_like_hex = true;
    for (const QString& token : remainder) {
        QString t = token.toUpper();
        bool valid = t == "??" || (t.size() == 2 &&
            ((t.at(0) >= '0' && t.at(0) <= '9') || (t.at(0) >= 'A' && t.at(0) <= 'F')) &&
            ((t.at(1) >= '0' && t.at(1) <= '9') || (t.at(1) >= 'A' && t.at(1) <= 'F')));
        if (!valid) {
            looks_like_hex = false;
            break;
        }
    }

    this->work_view_tabs_->setCurrentWidget(this->hex_view_);

    if (looks_like_hex) {
        QStringList upper;
        for (const QString& token : remainder)
            upper << token.toUpper();
        this->hex_view_->RunSearch(upper.join(' '));
        return;
    }

    /* fallback: treat as plain text, convert to a hex pattern */
    QByteArray bytes = remainder.join(' ').toUtf8();
    QStringList parts;
    for (int i = 0; i < bytes.size(); i++)
        parts << QString("%1").arg(static_cast<quint8>(bytes.at(i)), 2, 16, QChar('0')).toUpper();

    this->hex_view_->RunSearch(parts.join(' '));
}
