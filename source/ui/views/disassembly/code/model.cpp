#include <ui/views/disassembly/code/model.hpp>

namespace ui::views {
    CodeModel::CodeModel(QObject* parent) : QAbstractTableModel(parent) {}

    int CodeModel::rowCount(const QModelIndex&) const {
        return this->rows_.size();
    }

    int CodeModel::columnCount(const QModelIndex&) const {
        return ColumnCount;
    }

    QVariant CodeModel::data(const QModelIndex& index, int role) const {
        if (!index.isValid() || role != Qt::DisplayRole) return {};
        const auto& row = this->rows_.at(index.row());
        switch (index.column()) {
            case Address: return row.address;
            case Bytes:   return row.bytes;
            case Text:    return row.text;
            default:      return {};
        }
    }

    void CodeModel::AddRow(const QString& address, const QString& bytes, const QString& text) {
        beginInsertRows(QModelIndex(), this->rows_.size(), this->rows_.size());
        this->rows_.push_back({address, bytes, text});
        endInsertRows();
    }

    void CodeModel::Clear() {
        beginResetModel();
        this->rows_.clear();
        endResetModel();
    }
}
