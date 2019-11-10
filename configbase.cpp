#include "configbase.h"
#include "ui_configbase.h"
ConfigBase::ConfigBase(QString tabla , QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigBase)
{
    ui->setupUi(this);
    if(tabla == "configMaster"){
        datos = base->datosConexionMaster();
        datos.append("master");
    }else{
        datos = base->datosConexion();
        datos.append("local");
        }
    ui->lineEditDireccion->setText(datos.at(0));
    ui->lineEditPuerto->setText(datos.at(1));
    ui->lineEditUsuario->setText(datos.at(3));
    ui->lineEditClave->setText(datos.at(4));
    ui->lineEditBaseDatos->setText(datos.at(2));
}

ConfigBase::~ConfigBase()
{
    delete ui;
}

void ConfigBase::on_pushButton_clicked()
{
    //QString host,puerto,baseDatos,usuario,clave;
    host = ui->lineEditDireccion->text();
    puerto = ui->lineEditPuerto->text();
    nombreBaseDatos = ui->lineEditBaseDatos->text();
    usuario = ui->lineEditUsuario->text();
    clave = ui->lineEditClave->text();
    if(base->conectar(host,puerto,nombreBaseDatos,usuario,clave)){
        actualizarDatos();
        close();
    }
}

void ConfigBase::actualizarDatos()
{
    if(datos.at(5) == "master"){
        base->guardarDatosConexionMaster(host,puerto,nombreBaseDatos,usuario,clave);
    }else{
    base->guardarDatosConexion(host,puerto,nombreBaseDatos,usuario,clave);
        }
}
