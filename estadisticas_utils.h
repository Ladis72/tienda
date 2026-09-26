#ifndef ESTADISTICAS_UTILS_H
#define ESTADISTICAS_UTILS_H

#include <QStandardItem>
#include <QString>
#include <QVariant>

/**
 * @brief Elemento estándar para QStandardItemModel que implementa ordenación numérica
 * estricta mediante el valor almacenado en Qt::UserRole, manteniendo el formato visual
 * con sufijo (€, %, unidades, días, etc.).
 */
class NumericStandardItem : public QStandardItem {
public:
  NumericStandardItem(double valor, int decimales = 0, const QString &sufijo = "") {
    setData(valor, Qt::UserRole);
    if (decimales == 0) {
      setText(QString::number(valor, 'f', 0) + (sufijo.isEmpty() ? "" : " " + sufijo));
    } else {
      setText(QString::number(valor, 'f', decimales) + (sufijo.isEmpty() ? "" : " " + sufijo));
    }
    setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
  }

  bool operator<(const QStandardItem &other) const override {
    QVariant v1 = data(Qt::UserRole);
    QVariant v2 = other.data(Qt::UserRole);
    if (v1.isValid() && v2.isValid()) {
      return v1.toDouble() < v2.toDouble();
    }
    return QStandardItem::operator<(other);
  }
};

#endif // ESTADISTICAS_UTILS_H
