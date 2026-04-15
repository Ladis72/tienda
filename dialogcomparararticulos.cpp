#include "dialogcomparararticulos.h"
#include "ui_dialogcomparararticulos.h"

DialogCompararArticulos::DialogCompararArticulos(const QSqlRecord &local, const QSqlRecord &nube, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCompararArticulos),
    m_resultado(Cancelar)
{
    ui->setupUi(this);

    // Rellenar datos locales
    ui->lblLocalCod->setText(local.value("cod").toString());
    ui->lblLocalDesc->setText(local.value("descripcion").toString());
    ui->lblLocalPvp->setText(local.value("pvp").toString());
    ui->lblLocalIva->setText(local.value("iva").toString());

    // Rellenar datos nube
    ui->lblNubeCod->setText(nube.value("cod").toString());
    ui->lblNubeDesc->setText(nube.value("descripcion").toString());
    ui->lblNubePvp->setText(nube.value("pvp").toString());
    ui->lblNubeIva->setText(nube.value("iva").toString());

    // Estilo para resaltar diferencias (opcional pero recomendado)
    if (local.value("descripcion").toString() != nube.value("descripcion").toString())
        ui->lblNubeDesc->setStyleSheet("font-weight: bold; color: darkred;");
}

DialogCompararArticulos::~DialogCompararArticulos()
{
    delete ui;
}

void DialogCompararArticulos::on_btnSincronizarLocal_clicked()
{
    m_resultado = UsarNube;
    accept();
}

void DialogCompararArticulos::on_btnSobrescribirNube_clicked()
{
    m_resultado = SobrescribirNube;
    accept();
}

void DialogCompararArticulos::on_btnCancelar_clicked()
{
    m_resultado = Cancelar;
    reject();
}
