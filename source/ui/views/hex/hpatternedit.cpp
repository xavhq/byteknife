#include <ui/views/hex/hpatternedit.hpp>

#include <QKeyEvent>
#include <QClipboard>
#include <QApplication>
#include <cstdlib>

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

        QLineEdit::setText(parts.join(' '));
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
        this->setCursorPosition(this->text().size());
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

    int HexPatternEdit::GapCharOffset(int gap_index) const {
        return this->committed_.mid(0, gap_index).join(' ').size();
    }

    int HexPatternEdit::GapIndexNear(int char_pos) const {
        int n = this->committed_.size();
        int best_gap = 0;
        int best_distance = std::abs(char_pos - this->GapCharOffset(0));

        for (int i = 1; i <= n; i++) {
            int distance = std::abs(char_pos - this->GapCharOffset(i));
            if (distance < best_distance) {
                best_distance = distance;
                best_gap = i;
            }
        }
        return best_gap;
    }

    int HexPatternEdit::GapIndexAtCursor() const {
        return this->GapIndexNear(this->cursorPosition());
    }

    void HexPatternEdit::RemoveSelectedTokens() {
        if (!this->hasSelectedText())
            return;

        int sel_start = this->selectionStart();
        int sel_end = sel_start + this->selectedText().size();

        int start_gap = this->GapIndexNear(sel_start);
        int end_gap = this->GapIndexNear(sel_end);

        if (end_gap > start_gap)
            this->committed_.erase(this->committed_.begin() + start_gap, this->committed_.begin() + end_gap);

        this->pending_.clear();
        this->Render();
        this->setCursorPosition(this->GapCharOffset(start_gap));
    }

    void HexPatternEdit::PasteFromClipboard() {
        QString clip = QApplication::clipboard()->text();
        if (clip.isEmpty())
            return;

        QStringList tokens;
        for (const QString& part : clip.split(' ', Qt::SkipEmptyParts)) {
            if (part == "??" || (part.size() == 2 && this->IsHexChar(part.at(0)) && this->IsHexChar(part.at(1))))
                tokens.push_back(part.toUpper());
        }
        if (tokens.isEmpty())
            return;

        if (this->hasSelectedText())
            this->RemoveSelectedTokens();

        this->pending_.clear();

        int insert_at = this->GapIndexAtCursor();
        for (int i = 0; i < tokens.size(); i++)
            this->committed_.insert(insert_at + i, tokens.at(i));

        this->Render();
        this->setCursorPosition(this->GapCharOffset(insert_at + tokens.size()));
    }

    void HexPatternEdit::CopyToClipboard() {
        QString text_to_copy = this->hasSelectedText() ? this->selectedText() : this->text();
        QApplication::clipboard()->setText(text_to_copy);
    }

    void HexPatternEdit::CutToClipboard() {
        this->CopyToClipboard();
        if (this->hasSelectedText())
            this->RemoveSelectedTokens();
        else
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

        if (this->hasSelectedText() &&
            (event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Delete)) {
            this->RemoveSelectedTokens();
            event->accept();
            return;
        }

        if (event->key() == Qt::Key_Backspace) {
            if (!this->pending_.isEmpty()) {
                this->pending_.clear();
            } else if (!this->committed_.isEmpty()) {
                QString last = this->committed_.takeLast();
                if (last != "??")
                    this->pending_ = last.left(1);
            }
            this->Render();
            this->setCursorPosition(this->text().size());
            event->accept();
            return;
        }

        QString input = event->text();
        if (input.isEmpty()) {
            QLineEdit::keyPressEvent(event);
            return;
        }

        if (this->hasSelectedText())
            this->RemoveSelectedTokens();

        QChar ch = input.at(0);
        if (ch == '?' || this->IsHexChar(ch)) {
            this->AppendChar(ch);
            this->Render();
            this->setCursorPosition(this->text().size());
            event->accept();
            return;
        }

        event->accept(); /* swallow anything else */
    }
}
