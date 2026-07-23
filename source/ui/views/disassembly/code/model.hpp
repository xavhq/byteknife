#pragma once
#include <QAbstractTableModel>
#include <QVector>

namespace ui::views {
    struct DisassemblyRow {
        QString address;
        QString bytes;
        QString text;
    };

    class CodeModel : public QAbstractTableModel {
    public:
        enum Column { Address = 0, Bytes = 1, Text = 2, ColumnCount = 3 };

        explicit CodeModel(QObject* parent = nullptr);

        int rowCount(const QModelIndex& = QModelIndex()) const override;
        int columnCount(const QModelIndex& = QModelIndex()) const override;
        QVariant data(const QModelIndex& index, int role) const override;

        void AddRow(const QString& address, const QString& bytes, const QString& text);
        void Clear();

    private:
        QVector<DisassemblyRow> rows_;
    };
}
