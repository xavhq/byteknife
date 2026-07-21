#pragma once
#include <QLineEdit>
#include <QVector>

namespace ui::views {
    class HexPatternEdit : public QLineEdit {
        Q_OBJECT
    public:
        explicit HexPatternEdit(QWidget* parent = nullptr);

        void SetPattern(const QString& pattern);

    protected:
        void keyPressEvent(QKeyEvent* event) override;

    private:
        QVector<QString> committed_; /* each entry is "XX" or "??" */
        QString pending_;            /* "" or a single hex digit awaiting its second half */

        bool IsHexChar(QChar ch) const;
        void Render();
        void AppendChar(QChar ch);
        void PasteFromClipboard();
        void CopyToClipboard();
        void CutToClipboard();
        void Clear();
    };
}
