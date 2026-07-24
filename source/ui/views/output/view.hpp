#pragma once

#include <QWidget>
#include <QTextEdit>

namespace ui::views {
    class Output : public QWidget {
        Q_OBJECT

    public:
        explicit Output(QWidget* parent = nullptr);

        void Comment(const QString& text);
        void Info(const QString& text);
        void Warn(const QString& text);
        void Error(const QString& text);

    private:
        void AppendLine(const QString& text, const QString& color);
        QTextEdit* text_edit_;
        void SetZoom(int point_size);
        int zoom_size_ = 11;

    protected:
        bool eventFilter(QObject*, QEvent*) override;
    };
}

extern ui::views::Output* g_output;
