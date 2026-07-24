#include <ui/views/output/view.hpp>

#include <QVBoxLayout>
#include <QDateTime>
#include <QFontDatabase>
#include <QWheelEvent>
#include <QSettings>

ui::views::Output* g_output = nullptr;

namespace ui::views {

    Output::Output(QWidget* parent) : QWidget(parent) {
        g_output = this;

        this->text_edit_ = new QTextEdit(this);
        this->text_edit_->setReadOnly(true);
        this->text_edit_->setFrameShape(QFrame::NoFrame);
        this->text_edit_->document()->setDocumentMargin(10);

        QSettings settings;
        this->zoom_size_ = settings.value("output/zoom", 11).toInt();

        this->text_edit_->setStyleSheet(
            "QTextEdit {"
            "  background-color: palette(base);"
            "  border: none;"
            "}"
        );

        auto* layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(this->text_edit_);

        this->text_edit_->viewport()->installEventFilter(this);
        this->SetZoom(this->zoom_size_);
    }

    bool Output::eventFilter(QObject* watched, QEvent* event) {
        if (watched == this->text_edit_->viewport() && event->type() == QEvent::Wheel) {
            auto* wheel_event = static_cast<QWheelEvent*>(event);
            if (wheel_event->modifiers() & Qt::ControlModifier) {
                int delta = wheel_event->angleDelta().y() > 0 ? 1 : -1;
                this->SetZoom(qBound(7, this->zoom_size_ + delta, 24));
                return true;
            }
        }
        return QWidget::eventFilter(watched, event);
    }

    void Output::SetZoom(int point_size) {
        this->zoom_size_ = point_size;

        QFont font(QFontDatabase::systemFont(QFontDatabase::FixedFont));
        font.setPointSize(this->zoom_size_);
        this->text_edit_->setFont(font);

        QSettings settings;
        settings.setValue("output/zoom", this->zoom_size_);
    }

    void Output::AppendLine(const QString& text, const QString& color) {
        QString timestamp = QDateTime::currentDateTime().toString("HH:mm:ss");

        this->text_edit_->append(
            QString(
                "<div style=\"margin: 3px 0;\">"
                "<span style=\"color: #9a9da5; font-size: %1pt;\">%2</span>"
                "&nbsp;&nbsp;"
                "<span style=\"color: %3;\">%4</span>"
                "</div>"
            ).arg(qMax(9, this->zoom_size_ - 2)).arg(timestamp, color, text.toHtmlEscaped())
        );
    }

    void Output::Comment(const QString& text) { this->AppendLine(text, "#7a7d85"); }
    void Output::Info(const QString& text)    { this->AppendLine(text, "#5aa6ff"); }
    void Output::Warn(const QString& text)    { this->AppendLine(text, "#e0a94a"); }
    void Output::Error(const QString& text)   { this->AppendLine(text, "#f26d6d"); }
}
