#include <ui/views/hex/hpatternedit.hpp>
#include <ui/views/hex/hviewport.hpp>

#include <QStringList>
#include <QInputDialog>
#include <QMessageBox>
#include <algorithm>

namespace {
    QString TextToHexPattern(const QString& text) {
        QByteArray bytes = text.toUtf8();
        QStringList parts;

        for (int i = 0; i < bytes.size(); i++)
            parts << QString("%1").arg(static_cast<quint8>(bytes.at(i)), 2, 16, QChar('0')).toUpper();

        return parts.join(' ');
    }
}

namespace ui::views {
    qint64 HexViewport::FindPattern(const QString& pattern, qint64 start_offset, qint64& out_length) {
        struct Token { quint8 byte; bool wildcard; };
        QVector<Token> tokens;

        QStringList parts = pattern.split(' ', Qt::SkipEmptyParts);
        for (const QString& part : parts) {
            if (part == "??" || part == "?") {
                tokens.push_back({0, true});
                continue;
            }

            if (part.size() < 1 || part.size() > 2)
                return -1; /* reject malformed tokens instead of silently truncating */

            bool ok = false;
            quint8 b = static_cast<quint8>(part.toUInt(&ok, 16));
            if (!ok)
                return -1;
            tokens.push_back({b, false});
        }

        if (tokens.isEmpty())
            return -1;

        out_length = tokens.size();

        qint64 data_size = this->data_.size();
        qint64 pattern_size = tokens.size();

        for (qint64 offset = std::max<qint64>(0, start_offset); offset <= data_size - pattern_size; offset++) {
            bool match = true;

            for (qint64 i = 0; i < pattern_size; i++) {
                if (tokens[i].wildcard)
                    continue;

                if (static_cast<quint8>(this->data_.at(offset + i)) != tokens[i].byte) {
                    match = false;
                    break;
                }
            }

            if (match)
                return offset;
        }

        return -1;
    }

    void HexViewport::RunSearch(const QString& pattern, qint64 start_offset, bool wrap_message_if_missing) {
        qint64 length = 0;
        qint64 found = this->FindPattern(pattern, start_offset, length);

        if (found == -1 && start_offset > 0) {
            /* wrap around from the start */
            found = this->FindPattern(pattern, 0, length);
        }

        if (found == -1) {
            if (wrap_message_if_missing)
                QMessageBox::information(this, "Find Pattern",  "Pattern '" + pattern + "' not found.");
            return;
        }

        this->last_pattern_ = pattern;
        this->last_match_offset_ = found;
        this->last_match_length_ = length;

        this->selection_start_ = found;
        this->selection_end_ = found + length - 1; /* select the whole match, not just the first byte */

        qint64 line = found / this->bytes_per_line_;
        this->verticalScrollBar()->setValue(static_cast<int>(std::max<qint64>(0, line - this->VisibleLines() / 2)));

        this->viewport()->update();
    }

    void HexViewport::FindPatternFromUser() {
        QDialog dialog(this);
        dialog.setWindowTitle("Find Pattern");
        dialog.setWindowModality(Qt::WindowModal);
        dialog.setFixedSize(450, 160);

        auto* layout = new QVBoxLayout(&dialog);
        layout->addWidget(new QLabel(
            "• Scan Hex patterns in the static binary\n"
            "• Use ? (?\?) for wild card regions\n"
            "• Use find next to filter all available sections that match your pattern\n"
        ));

        auto* hex_edit = new HexPatternEdit(&dialog);
        hex_edit->setText(this->last_pattern_);
        hex_edit->setPlaceholderText("?? 66 70 72 69 6E 74 66 ??");
        hex_edit->setFont(QFont("Arial", 10));

        auto* text_edit = new QLineEdit(&dialog);
        text_edit->setPlaceholderText("fprintf");
        text_edit->setVisible(false);
        text_edit->setFont(QFont("Arial", 10));

        layout->addWidget(hex_edit);
        layout->addWidget(text_edit);

        auto* bottom_row = new QHBoxLayout();

        auto* hex_radio = new QRadioButton("Match Pattern", &dialog);
        auto* text_radio = new QRadioButton("Match Text", &dialog);
        hex_radio->setChecked(!this->last_search_mode_was_text_);
        text_radio->setChecked(this->last_search_mode_was_text_);

        bottom_row->addWidget(hex_radio);
        bottom_row->addWidget(text_radio);
        bottom_row->addStretch();

        auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
        bottom_row->addWidget(buttons);

        layout->addLayout(bottom_row);

        connect(hex_radio, &QRadioButton::toggled, this, [hex_edit, text_edit, layout](bool checked) {
            layout->setEnabled(false);
            hex_edit->setVisible(checked);
            text_edit->setVisible(!checked);
            layout->setEnabled(true);
        });

        connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

        hex_edit->setFocus();

        if (dialog.exec() != QDialog::Accepted)
            return;

        QString pattern;
        if (text_radio->isChecked()) {
            QString input = text_edit->text();
            if (input.isEmpty())
                return;
            pattern = TextToHexPattern(input);
        } else {
            pattern = hex_edit->text().trimmed();
            if (pattern.isEmpty())
                return;
        }

        this->last_search_mode_was_text_ = text_radio->isChecked();
        this->RunSearch(pattern, 0, true);
    }

    void HexViewport::FindNext() {
        if (this->last_pattern_.isEmpty()) {
            this->FindPatternFromUser();
            return;
        }

        qint64 start = this->last_match_offset_ >= 0 ? this->last_match_offset_ + 1 : 0;
        this->RunSearch(this->last_pattern_, start, true);
    }
}
