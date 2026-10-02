/**
 * @file dialogconvertirdocumento.h
 * @brief Declaración del diálogo para convertir facturas en albaranes y albaranes en facturas.
 */

#ifndef DIALOGCONVERTIRDOCUMENTO_H
#define DIALOGCONVERTIRDOCUMENTO_H

#include <QDialog>
#include <QString>
#include <QDate>

namespace Ui {
class DialogConvertirDocumento;
}

/**
 * @class DialogConvertirDocumento
 * @brief Permite revisar y ajustar número, fechas y notas al reclasificar un documento entre factura y albarán.
 */
class DialogConvertirDocumento : public QDialog
{
    Q_OBJECT

public:
    enum ModoConversion {
        FacturaAAlbaran,
        AlbaranAFactura
    };

    /**
     * @brief Constructor del diálogo de conversión.
     * @param modo Indica si se convierte FacturaAAlbaran o AlbaranAFactura.
     * @param nDoc Número original del documento.
     * @param nombreProveedor Nombre comercial o razón social del proveedor.
     * @param idProveedor Identificador del proveedor.
     * @param totalBase Base imponible del documento.
     * @param totalIva Importe de IVA del documento.
     * @param totalRe Importe de Recargo de Equivalencia del documento.
     * @param total Importe total del documento.
     * @param fecha Fecha del documento en formato yyyy-MM-dd.
     * @param vencimiento Fecha de vencimiento en formato yyyy-MM-dd (aplicable si es factura).
     * @param pagada Estado de pago (0 o 1, aplicable a facturas).
     * @param notas Observaciones registradas en el documento.
     * @param parent Widget padre.
     */
    explicit DialogConvertirDocumento(ModoConversion modo,
                                     const QString &nDoc,
                                     const QString &nombreProveedor,
                                     const QString &idProveedor,
                                     double totalBase,
                                     double totalIva,
                                     double totalRe,
                                     double total,
                                     const QString &fecha,
                                     const QString &vencimiento,
                                     int pagada,
                                     const QString &notas,
                                     QWidget *parent = nullptr);
    ~DialogConvertirDocumento();

    /**
     * @brief Obtiene el nuevo número de documento asignado por el usuario.
     */
    QString getNuevoNDoc() const;

    /**
     * @brief Obtiene la fecha del documento en formato yyyy-MM-dd.
     */
    QString getNuevaFecha() const;

    /**
     * @brief Obtiene la fecha de vencimiento en formato yyyy-MM-dd.
     */
    QString getNuevoVencimiento() const;

    /**
     * @brief Obtiene el estado de pagada (1 si se marcó, 0 en caso contrario).
     */
    int getPagada() const;

    /**
     * @brief Obtiene las notas u observaciones modificadas.
     */
    QString getNuevasNotas() const;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogConvertirDocumento *ui;
    ModoConversion m_modo;
    QString m_nDocOriginal;
    QString m_nombreProveedor;
    QString m_idProveedor;

    QString m_nuevoNDoc;
    QString m_nuevaFecha;
    QString m_nuevoVencimiento;
    int m_pagada;
    QString m_nuevasNotas;
};

#endif // DIALOGCONVERTIRDOCUMENTO_H
