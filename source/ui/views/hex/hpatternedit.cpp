#include <ui/views/hex/hpatternedit.hpp>
#include <QKeyEvent>
#include <QClipboard>
#include <QApplication>

namespace ui::views {
    HexPatternEdit::HexPatternEdit(QWidget* parent) : QLineEdit(parent) { }

    bool HexPatternEdit::IsHexChar(QChar ch) const {
        return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');
    }

    void HexPatternEdit::Render() {
        QStringList parts;
        for (const QString& token : this->committed_)
            parts << token;
        if (!this->pending_.isEmpty())
            parts << this->pending_;
        QString text = parts.join(' ');
        QLineEdit::setText(text);
        this->setCursorPosition(text.size());
    }

    void HexPatternEdit::Clear() {
        this->committed_.clear();
        this->pending_.clear();
        this->Render();
    }

    void HexPatternEdit::SetPattern(const QString& pattern) {
        this->committed_.clear();
        this->pending_.clear();
        for (const QString& part : pattern.split(' ', Qt::SkipEmptyParts)) {
            if (part == "??" || (part.size() == 2 && this->IsHexChar(part.at(0)) && this->IsHexChar(part.at(1))))
                this->committed_.push_back(part.toUpper());
        }
        this->Render();
    }

    void HexPatternEdit::AppendChar(QChar ch) {
        if (ch == '?') {
            if (this->pending_.isEmpty())
                this->committed_.push_back("??");

            return;
        }
        if (this->IsHexChar(ch)) {
            ch = ch.toUpper();
            if (this->pending_.isEmpty()) {
                this->pending_ = ch;
            } else {
                this->committed_.push_back(this->pending_ + ch);
                this->pending_.clear();
            }
        }
    }

    void HexPatternEdit::PasteFromClipboard() {
        /* TODO: fix bug with pasting removing everything */
        QString clip = QApplication::clipboard()->text();

        if (clip.isEmpty()) {
            return;
        }

        this->Clear();
        for (const QChar& ch : clip) {
            this->AppendChar(ch);
        }

        this->Render();
    }

    void HexPatternEdit::CopyToClipboard() {
        QApplication::clipboard()->setText(this->text());
    }

    void HexPatternEdit::CutToClipboard() {
        this->CopyToClipboard();
        this->Clear();
    }

    void HexPatternEdit::keyPressEvent(QKeyEvent* event) {
        if (event->matches(QKeySequence::Copy)) {
            this->CopyToClipboard();
            event->accept();
            return;
        }
        if (event->matches(QKeySequence::Cut)) {
            this->CutToClipboard();
            event->accept();
            return;
        }
        if (event->matches(QKeySequence::Paste)) {
            this->PasteFromClipboard();
            event->accept();
            return;
        }

        /* select-all then backspace/delete/type clears everything */
        if (this->selectedText() == this->text() && !this->text().isEmpty() &&
            (event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Delete)) {
            this->Clear();
            event->accept();
            return;
        }
        if (event->key() == Qt::Key_Backspace) {
            if (!this->pending_.isEmpty()) {
                this->pending_.clear();
            } else if (!this->committed_.isEmpty()) {
                QString last = this->committed_.takeLast();
                if (last != "??")
                    this->pending_ = last.left(1); /* "AD" -> pending "A" */
            }
            this->Render();
            event->accept();
            return;
        }
        QString input = event->text();
        if (input.isEmpty()) {
            QLineEdit::keyPressEvent(event);
            return;
        }
        QChar ch = input.at(0);
        if (ch == '?') {
            if (this->pending_.isEmpty()) {
                this->committed_.push_back("??");
                this->Render();
            }
            event->accept();
            return;
        }
        if (this->IsHexChar(ch)) {
            ch = ch.toUpper();
            if (this->pending_.isEmpty()) {
                this->pending_ = ch;
            } else {
                this->committed_.push_back(this->pending_ + ch);
                this->pending_.clear();
            }
            this->Render();
            event->accept();
            return;
        }
        event->accept(); /* swallow anything else */
    }
}
