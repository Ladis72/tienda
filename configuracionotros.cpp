#include "configuracionotros.h"
#include "ui_configuracionotros.h"
#include <QCoreApplication>
#include <QSettings>
#include <QFileDialog>
#include <QDir>
#include <QSqlQuery>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QMessageBox>
#include <QTimer>

ConfiguracionOtros::ConfiguracionOtros(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConfiguracionOtros)
{
    ui->setupUi(this);
    // Leer la configuración global actual de la base de datos
    QMap<QString, QVariant> config = base.leerConfiguracion();
    ui->checkBoxRE->setChecked(config.value("recargoeq").toBool());
    
    if (ui->checkBoxNube) {
        ui->checkBoxNube->setChecked(config.value("precios_locales").toBool());
    }

    // Poblar combos de vendedores desde la base de datos
    QSqlQuery q = base.usuarios(QSqlDatabase::database(conf->getConexionLocal()));
    ui->comboBoxF1->addItem(tr("Ninguno"), QVariant());
    ui->comboBoxF2->addItem(tr("Ninguno"), QVariant());
    ui->comboBoxF3->addItem(tr("Ninguno"), QVariant());
    ui->comboBoxF4->addItem(tr("Ninguno"), QVariant());
    
    while (q.next()) {
        QString id = q.value("id").toString();
        QString nombre = q.value("nombre").toString();
        QString itemText = id + "-" + nombre;
        ui->comboBoxF1->addItem(itemText, id);
        ui->comboBoxF2->addItem(itemText, id);
        ui->comboBoxF3->addItem(itemText, id);
        ui->comboBoxF4->addItem(itemText, id);
    }

    // Cargar valores actuales de vendedores
    ui->comboBoxF1->setCurrentIndex(ui->comboBoxF1->findData(config.value("vendedor_f1").toString()));
    ui->comboBoxF2->setCurrentIndex(ui->comboBoxF2->findData(config.value("vendedor_f2").toString()));
    ui->comboBoxF3->setCurrentIndex(ui->comboBoxF3->findData(config.value("vendedor_f3").toString()));
    ui->comboBoxF4->setCurrentIndex(ui->comboBoxF4->findData(config.value("vendedor_f4").toString()));

    // Cargar configuración de Google Sheets desde tienda.ini
    QString iniPath = QCoreApplication::applicationDirPath() + "/tienda.ini";
    QSettings settings(iniPath, QSettings::IniFormat);
    settings.beginGroup("GoogleSheets");
    ui->checkBoxGoogleSheets->setChecked(settings.value("enviarGoogleSheets", false).toBool());
    ui->lineEditUrlGoogleSheets->setText(settings.value("urlGoogleSheets", "").toString());
    settings.endGroup();

    // Cargar configuración de Ollama desde tienda.ini
    settings.beginGroup("Ollama");
    ui->lineEditUrlOllama->setText(settings.value("url", "http://localhost:11434").toString());
    QString modeloGuardado = settings.value("modelo", "qwen3-vl:8b").toString();
    if (!modeloGuardado.isEmpty()) {
        ui->comboBoxModeloOllama->addItem(modeloGuardado);
        ui->comboBoxModeloOllama->setCurrentText(modeloGuardado);
    }
    settings.endGroup();

    // Consultar automáticamente los modelos disponibles de Ollama en segundo plano
    QTimer::singleShot(50, this, &ConfiguracionOtros::on_pushButtonProbarOllama_clicked);

    // Inicializar fecha de sincronización de histórico a 1 de enero del año actual
    if (ui->dateEditSyncDesde) {
        ui->dateEditSyncDesde->setDate(QDate(QDate::currentDate().year(), 1, 1));
    }
}

ConfiguracionOtros::~ConfiguracionOtros()
{
    delete ui;
}

void ConfiguracionOtros::on_pushButtonAceptar_clicked()
{
    QMap<QString, QVariant> config;
    config["recargoeq"] = ui->checkBoxRE->isChecked() ? 1 : 0;
    
    if (ui->checkBoxNube) {
        config["precios_locales"] = ui->checkBoxNube->isChecked() ? 1 : 0;
    }

    config["vendedor_f1"] = ui->comboBoxF1->currentData();
    config["vendedor_f2"] = ui->comboBoxF2->currentData();
    config["vendedor_f3"] = ui->comboBoxF3->currentData();
    config["vendedor_f4"] = ui->comboBoxF4->currentData();
    
    // Guardar la configuración actualizada en la base de datos
    base.GuardarConfiguracion(config);
    conf->setUsarPreciosLocales(ui->checkBoxNube->isChecked());

    // Guardar configuración de Google Sheets y Ollama en tienda.ini
    QString iniPath = QCoreApplication::applicationDirPath() + "/tienda.ini";
    QSettings settings(iniPath, QSettings::IniFormat);
    settings.beginGroup("GoogleSheets");
    settings.setValue("enviarGoogleSheets", ui->checkBoxGoogleSheets->isChecked());
    settings.setValue("urlGoogleSheets", ui->lineEditUrlGoogleSheets->text().trimmed());
    settings.endGroup();

    settings.beginGroup("Ollama");
    QString urlOllama = ui->lineEditUrlOllama->text().trimmed();
    QString modeloAnterior = settings.value("modelo", "").toString().trimmed();
    QString nuevoModelo = ui->comboBoxModeloOllama->currentText().trimmed();
    settings.setValue("url", urlOllama);
    settings.setValue("modelo", nuevoModelo);
    settings.endGroup();

    // Si el modelo ha cambiado, descargar el modelo anterior de la memoria VRAM
    if (!modeloAnterior.isEmpty() && modeloAnterior != nuevoModelo) {
        if (urlOllama.isEmpty()) urlOllama = "http://localhost:11434";
        if (urlOllama.endsWith("/")) urlOllama.chop(1);

        QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
        QNetworkRequest req((QUrl(urlOllama + "/api/generate")));
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        QJsonObject obj;
        obj["model"] = modeloAnterior;
        obj["keep_alive"] = 0;
        QNetworkReply *reply = mgr->post(req, QJsonDocument(obj).toJson(QJsonDocument::Compact));
        connect(reply, &QNetworkReply::finished, [reply, mgr]() {
            reply->deleteLater();
            mgr->deleteLater();
        });
    }

    emit accept();
}

void ConfiguracionOtros::on_pushButtonProbarOllama_clicked()
{
    QString urlStr = ui->lineEditUrlOllama->text().trimmed();
    if (urlStr.isEmpty()) urlStr = "http://localhost:11434";
    if (urlStr.endsWith("/")) urlStr.chop(1);

    ui->labelEstadoOllama->setText("Comprobando conexión con " + urlStr + "...");
    ui->pushButtonProbarOllama->setEnabled(false);

    QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
    QUrl url(urlStr + "/api/tags");
    QNetworkRequest req(url);

    QNetworkReply *reply = mgr->get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply, mgr]() {
        reply->deleteLater();
        mgr->deleteLater();
        ui->pushButtonProbarOllama->setEnabled(true);

        if (reply->error() != QNetworkReply::NoError) {
            ui->labelEstadoOllama->setText("❌ Error conectando: " + reply->errorString());
            return;
        }

        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (!doc.isObject()) {
            ui->labelEstadoOllama->setText("❌ Respuesta no válida del servidor Ollama.");
            return;
        }

        QJsonArray modelsArray = doc.object().value("models").toArray();
        QStringList modelos;
        for (const QJsonValue &v : modelsArray) {
            QString name = v.toObject().value("name").toString();
            if (!name.isEmpty()) modelos.append(name);
        }

        if (modelos.isEmpty()) {
            ui->labelEstadoOllama->setText("⚠️ Conectado a Ollama, pero no hay modelos descargados.");
            return;
        }

        QString actual = ui->comboBoxModeloOllama->currentText();
        ui->comboBoxModeloOllama->clear();
        for (const QString &m : modelos) {
            ui->comboBoxModeloOllama->addItem(m);
        }

        int idx = ui->comboBoxModeloOllama->findText(actual);
        if (idx >= 0) {
            ui->comboBoxModeloOllama->setCurrentIndex(idx);
        } else {
            ui->comboBoxModeloOllama->setCurrentIndex(0);
        }

        ui->labelEstadoOllama->setText(QString("✅ Conexión exitosa. %1 modelo(s) detectado(s).").arg(modelos.size()));
    });
}

/**
 * @brief Vuelca el histórico de tickets, arqueos y stock local a la nube (nubeCervantes).
 */
void ConfiguracionOtros::on_pushButtonSincronizarNube_clicked()
{
    ui->pushButtonSincronizarNube->setEnabled(false);
    ui->labelEstadoSyncNube->setText("Sincronizando histórico con la nube... Por favor, espera.");
    qApp->processEvents();

    int idTienda = conf ? conf->getIdTienda() : 1;
    QString fechaDesde = ui->dateEditSyncDesde->date().toString("yyyy-MM-dd");

    QString resultado;
    bool ok = baseDatos::volcarHistoricoLocalANube(idTienda, fechaDesde, resultado);

    ui->pushButtonSincronizarNube->setEnabled(true);
    if (ok) {
        ui->labelEstadoSyncNube->setText("✅ " + resultado.replace("\n", " "));
        QMessageBox::information(this, "Sincronización Nube", resultado);
    } else {
        ui->labelEstadoSyncNube->setText("❌ Error: " + resultado);
        QMessageBox::warning(this, "Error de Sincronización", resultado);
    }
}

