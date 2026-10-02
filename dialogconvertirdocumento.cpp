/**
 * @file dialogconvertirdocumento.cpp
 * @brief Implementación del diálogo para convertir facturas en albaranes y albaranes en facturas.
 */

#include "dialogconvertirdocumento.h"
#include "ui_dialogconvertirdocumento.h"
#include <QMessageBox>
#include <QPushButton>

DialogConvertirDocumento::DialogConvertirDocumento(ModoConversion modo,
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
                                                   QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogConvertirDocumento)
    , m_modo(modo)
    , m_nDocOriginal(nDoc)
    , m_nombreProveedor(nombreProveedor)
    , m_idProveedor(idProveedor)
    , m_pagada(pagada)
{
    ui->setupUi(this);

    // Configuración de textos y campos según la dirección de conversión
    if (m_modo == FacturaAAlbaran) {
        setWindowTitle(tr("Convertir Factura en Albarán"));
        ui->lblTipoDoc->setText(tr("Factura de Compra"));
        ui->lblNuevoNDoc->setText(tr("Nuevo Nº Albarán:"));
        ui->groupBoxNuevo->setTitle(tr("Datos del Nuevo Albarán"));
        ui->lblAdvertencia->setText(tr("Nota: La factura se eliminará y pasará a registrarse como un albarán independiente no facturado."));

        // En albaranes no aplican vencimiento ni estado de pagada
        ui->lblVencimiento->hide();
        ui->dateEditVencimiento->hide();
        ui->checkBoxPagada->hide();

        if (QPushButton *btnOk = ui->buttonBox->button(QDialogButtonBox::Ok)) {
            btnOk->setText(tr("Convertir a Albarán"));
        }
    } else {
        setWindowTitle(tr("Convertir Albarán en Factura"));
        ui->lblTipoDoc->setText(tr("Albarán de Compra"));
        ui->lblNuevoNDoc->setText(tr("Nuevo Nº Factura:"));
        ui->groupBoxNuevo->setTitle(tr("Datos de la Nueva Factura"));
        ui->lblAdvertencia->setText(tr("Nota: El albarán se eliminará y pasará a registrarse directamente como una factura de compra."));

        ui->lblVencimiento->show();
        ui->dateEditVencimiento->show();
        ui->checkBoxPagada->show();

        if (QPushButton *btnOk = ui->buttonBox->button(QDialogButtonBox::Ok)) {
            btnOk->setText(tr("Convertir a Factura"));
        }
    }

    // Llenar información del documento original
    ui->lblProveedor->setText(m_nombreProveedor.isEmpty() ? tr("(Sin proveedor asignado)") : m_nombreProveedor);
    ui->lblNDocOriginal->setText(m_nDocOriginal);
    ui->lblTotal->setText(QString("%1 €  (Base: %2 €, IVA: %3 €, RE: %4 €)")
                              .arg(QString::number(total, 'f', 2),
                                   QString::number(totalBase, 'f', 2),
                                   QString::number(totalIva, 'f', 2),
                                   QString::number(totalRe, 'f', 2)));

    // Precargar campos editables
    ui->lineEditNuevoNDoc->setText(m_nDocOriginal);

    // Fechas en formato yyyy-MM-dd
    QDate fechaDoc = QDate::fromString(fecha, "yyyy-MM-dd");
    if (!fechaDoc.isValid()) {
        fechaDoc = QDate::currentDate();
    }
    ui->dateEditFecha->setDate(fechaDoc);

    if (m_modo == AlbaranAFactura) {
        QDate fechaVenc = QDate::fromString(vencimiento, "yyyy-MM-dd");
        if (!fechaVenc.isValid()) {
            // Por defecto 30 días a partir de la fecha del documento
            fechaVenc = fechaDoc.addDays(30);
        }
        ui->dateEditVencimiento->setDate(fechaVenc);
        ui->checkBoxPagada->setChecked(pagada == 1);
    }

    ui->lineEditNotas->setText(notas);
}

DialogConvertirDocumento::~DialogConvertirDocumento()
{
    delete ui;
}

void DialogConvertirDocumento::on_buttonBox_accepted()
{
    QString nuevoNumero = ui->lineEditNuevoNDoc->text().trimmed();
    if (nuevoNumero.isEmpty()) {
        QMessageBox::warning(this, tr("Dato obligatorio"),
                             tr("Debe especificar un número para el nuevo documento."));
        ui->lineEditNuevoNDoc->setFocus();
        return;
    }

    // Asignar variables finales
    m_nuevoNDoc = nuevoNumero;
    m_nuevaFecha = ui->dateEditFecha->date().toString("yyyy-MM-dd");
    m_nuevoVencimiento = ui->dateEditVencimiento->date().toString("yyyy-MM-dd");
    m_pagada = ui->checkBoxPagada->isChecked() ? 1 : 0;
    m_nuevasNotas = ui->lineEditNotas->text().trimmed();

    accept();
}

QString DialogConvertirDocumento::getNuevoNDoc() const
{
    return m_nuevoNDoc;
}

QString DialogConvertirDocumento::getNuevaFecha() const
{
    return m_nuevaFecha;
}

QString DialogConvertirDocumento::getNuevoVencimiento() const
{
    return m_nuevoVencimiento;
}

int DialogConvertirDocumento::getPagada() const
{
    return m_pagada;
}

QString DialogConvertirDocumento::getNuevasNotas() const
{
    return m_nuevasNotas;
}
