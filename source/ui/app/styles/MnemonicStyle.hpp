#include <QProxyStyle>

namespace ui::Theme {
    class MnemonicStyle : public QProxyStyle {
    public:
        using QProxyStyle::QProxyStyle;

        int styleHint(
            StyleHint hint,
            const QStyleOption* option = nullptr,
            const QWidget* widget = nullptr,
            QStyleHintReturn* returnData = nullptr
        ) const override {
            if (hint == QStyle::SH_UnderlineShortcut) {
                if (qobject_cast<const QToolButton*>(widget))
                    return 0; /* no underline on toolbar buttons */
                return 1;
            }
            if (hint == QStyle::SH_DialogButtonBox_ButtonsHaveIcons)
                return 0; /* no icons on Yes/No/OK/Cancel */
            return QProxyStyle::styleHint(hint, option, widget, returnData);
        }

        QIcon standardIcon(
            StandardPixmap standardIcon,
            const QStyleOption* option = nullptr,
            const QWidget* widget = nullptr
        ) const override {
            switch (standardIcon) {
                case QStyle::SP_MessageBoxQuestion:
                case QStyle::SP_MessageBoxInformation:
                case QStyle::SP_MessageBoxWarning:
                case QStyle::SP_MessageBoxCritical:
                    return QIcon();
                default:
                    return QProxyStyle::standardIcon(standardIcon, option, widget);
            }
        }
    };
}
