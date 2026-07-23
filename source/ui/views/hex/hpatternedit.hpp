#pragma once

#include <QLineEdit>
#include <QVector>
#include <QLineEdit>
#include <QStringList>

namespace ui::views {
    class HexPatternEdit : public QLineEdit {
        Q_OBJECT

    public:
        explicit HexPatternEdit(QWidget* parent = nullptr);

        void Clear();
        void SetPattern(const QString& pattern);

    protected:
        void keyPressEvent(QKeyEvent* event) override;

    private:
        bool IsHexChar(QChar ch) const;
        void Render();
        void AppendChar(QChar ch);

        void PasteFromClipboard();
        void CopyToClipboard();
        void CutToClipboard();

        int GapCharOffset(int gap_index) const;
        int GapIndexNear(int char_pos) const;
        int GapIndexAtCursor() const;
        void RemoveSelectedTokens();

        QStringList committed_;
        QString pending_;
    };
}
