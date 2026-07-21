#include <ui/views/binary/disasm/dmodel.hpp>

namespace ui::views {
    DisassemblyModel::DisassemblyModel(QObject* parent) : QAbstractTableModel(parent) {}

    int DisassemblyModel::rowCount(const QModelIndex&) const {
        return this->rows_.size();
    }

    int DisassemblyModel::columnCount(const QModelIndex&) const {
        return ColumnCount;
    }

    QVariant DisassemblyModel::data(const QModelIndex& index, int role) const {
        if (!index.isValid() || role != Qt::DisplayRole) return {};
        const auto& row = this->rows_.at(index.row());
        switch (index.column()) {
            case Address: return row.address;
            case Bytes:   return row.bytes;
            case Text:    return row.text;
            default:      return {};
        }
    }

    void DisassemblyModel::AddRow(const QString& address, const QString& bytes, const QString& text) {
        beginInsertRows(QModelIndex(), this->rows_.size(), this->rows_.size());
        this->rows_.push_back({address, bytes, text});
        endInsertRows();
    }

    void DisassemblyModel::Clear() {
        beginResetModel();
        this->rows_.clear();
        endResetModel();
    }
}
