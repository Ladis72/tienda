#include "notaswidget.h"
#include "configuracion.h"
#include "gestorencargosdialog.h"
#include <QAbstractItemDelegate>
#include <QApplication>
#include <QCheckBox>
#include <QPainter>
#include <QStyledItemDelegate>

extern Configuracion *conf;

// ─────────────────────────────────────────────────────────────
// Delegate para colorear filas según prioridad y estado
// ─────────────────────────────────────────────────────────────
class NotasColorDelegate : public QStyledItemDelegate {
public:
  explicit NotasColorDelegate(QObject *parent = nullptr)
      : QStyledItemDelegate(parent) {}

  void paint(QPainter *painter, const QStyleOptionViewItem &option,
             const QModelIndex &index) const override {
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    // Columna de prioridad = 1, estado = 6 (en el modelo fuente)
    // Obtenemos fila del modelo fuente a través del proxy
    const QAbstractItemModel *m = index.model();
    QString prioridad = m->data(m->index(index.row(), 1)).toString();
    QString estado = m->data(m->index(index.row(), 6)).toString();

    QColor bg;
    QColor text = QColor(33, 33, 33);
    if (estado == "Completada") {
      bg = QColor(232, 245, 233); // Verde muy suave (Material Green 50)
      text = QColor(56, 142, 60); // Verde oscuro para texto
    } else if (prioridad == "Alta") {
      bg = QColor(255, 235, 238); // Rojo muy suave (Material Red 50)
      text = QColor(198, 40, 40); // Rojo oscuro para texto
    } else if (prioridad == "Normal") {
      bg = QColor(241, 248, 233); // Verde muy claro (App Theme Light)
      text = QColor(85, 139, 47); // Verde oscuro de la aplicación
    } else {
      bg = QColor(250, 250, 250);   // Gris muy suave
      text = QColor(117, 117, 117); // Gris para texto
    }

    if (opt.state & QStyle::State_Selected) {
      bg = bg.darker(108);
    }

    painter->fillRect(opt.rect, bg);
    painter->setPen(text);

    // Dibujamos el texto manualmente para controlar el color dinámico si no
    // está seleccionado
    if (!(opt.state & QStyle::State_Selected)) {
      opt.palette.setColor(QPalette::Text, text);
      opt.palette.setColor(QPalette::WindowText, text);
    }

    opt.backgroundBrush = Qt::NoBrush;
    QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter);

    // Línea divisoria sutil en la parte inferior de la fila
    painter->setPen(QColor(0, 0, 0, 20));
    painter->drawLine(opt.rect.bottomLeft(), opt.rect.bottomRight());
  }
};

// ─────────────────────────────────────────────────────────────
// NotasWidget
// ─────────────────────────────────────────────────────────────
NotasWidget::NotasWidget(QWidget *parent)
    : QWidget(parent), tablaView(nullptr), modelo(nullptr), proxyModel(nullptr),
      btnNueva(nullptr), btnEncargos(nullptr), cboFiltro(nullptr),
      lblContador(nullptr), delegate(new NotasColorDelegate(this)) {
  conexionDB = conf->getConexionLocal();
  setupUi();
  refrescar();
}

void NotasWidget::setConexion(const QString &conn) {
  conexionDB = conn;
  refrescar();
}

void NotasWidget::setupUi() {
  // ── Barra superior ──────────────────────────────────────
  btnNueva = new QPushButton(tr("➕ Nueva nota"), this);
  btnNueva->setObjectName("btnNuevaNota");
  btnNueva->setMinimumHeight(40);

  btnEncargos = new QPushButton(tr("📦 Encargos"), this);
  btnEncargos->setObjectName("btnEncargos");
  btnEncargos->setMinimumHeight(40);
  connect(btnEncargos, &QPushButton::clicked, this, [this]() {
    GestorEncargosDialog dial("", this);
    dial.exec();
  });

  cboFiltro = new QComboBox(this);
  cboFiltro->addItems({tr("Todas"), tr("Pendientes"), tr("Completadas")});
  cboFiltro->setFixedWidth(120);
  cboFiltro->setCurrentText("Pendientes");

  lblContador = new QLabel(this);
  lblContador->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  lblContador->setStyleSheet("color: #C62828; font-weight: bold;");

  QHBoxLayout *topBar = new QHBoxLayout;
  topBar->addWidget(btnNueva);
  topBar->addWidget(btnEncargos);
  topBar->addSpacing(10);
  topBar->addWidget(new QLabel(tr("Filtrar:"), this));
  topBar->addWidget(cboFiltro);
  topBar->addStretch();
  topBar->addWidget(lblContador);

  // ── Tabla ───────────────────────────────────────────────
  modelo = new QSqlQueryModel(this);
  proxyModel = new QSortFilterProxyModel(this);
  proxyModel->setSourceModel(modelo);

  tablaView = new QTableView(this);
  tablaView->setModel(proxyModel);
  tablaView->setItemDelegate(delegate);
  tablaView->setSelectionBehavior(QAbstractItemView::SelectRows);
  tablaView->setSelectionMode(QAbstractItemView::SingleSelection);
  tablaView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  tablaView->setContextMenuPolicy(Qt::CustomContextMenu);
  tablaView->verticalHeader()->setDefaultSectionSize(32);

  // Permitir la ordenación de la tabla al hacer clic en las cabeceras de columna
  tablaView->setSortingEnabled(true);

  // Dejamos que el estilo global style.qss maneje la tabla
  // Solo ajustamos que no muestre la rejilla para un look más moderno
  tablaView->setShowGrid(false);

  tablaView->horizontalHeader()->setStretchLastSection(false);
  tablaView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
  tablaView->setAlternatingRowColors(false);
  tablaView->setColumnHidden(0, true); // id
  tablaView->setColumnHidden(7, true); // descripcion
  tablaView->setToolTip(
      tr("Doble clic para ver/editar. Clic derecho para más opciones."));

  // ── Layout principal ─────────────────────────────────────
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setContentsMargins(6, 6, 6, 6);
  mainLayout->setSpacing(4);

  // Título de sección con estilo premium ajustado al tema
  QLabel *titulo = new QLabel(tr("📋  NOTAS"), this);
  titulo->setStyleSheet("font-size: 11pt; font-weight: bold; color: #558b2f; "
                        "letter-spacing: 1px;");

  QPushButton *btnOcultar = new QPushButton("✖", this);
  btnOcultar->setToolTip(tr("Ocultar panel de notas"));
  btnOcultar->setFixedSize(24, 24);
  btnOcultar->setStyleSheet("QPushButton { border: none; background: "
                            "transparent; color: #9E9E9E; font-weight: bold; }"
                            "QPushButton:hover { color: #ef5350; }");

  QHBoxLayout *headerTitle = new QHBoxLayout;
  headerTitle->addWidget(titulo);
  headerTitle->addStretch();
  headerTitle->addWidget(btnOcultar);

  QFrame *linea = new QFrame(this);
  linea->setFrameShape(QFrame::HLine);
  linea->setFrameShadow(QFrame::Plain);
  linea->setStyleSheet("color: #dcedc8;"); // color de borde suave del tema

  mainLayout->addLayout(headerTitle);
  mainLayout->addWidget(linea);
  mainLayout->addLayout(topBar);
  mainLayout->addWidget(tablaView);

  this->setObjectName("NotasWidget");
  this->setStyleSheet("QWidget#NotasWidget { background-color: #ffffff; "
                      "border: 1px solid #e0e0e0; border-radius: 8px; }");
  setLayout(mainLayout);

  // ── Conexiones ───────────────────────────────────────────
  connect(btnNueva, &QPushButton::clicked, this, &NotasWidget::onNuevaNota);
  connect(btnOcultar, &QPushButton::clicked, this, &NotasWidget::hideRequested);
  connect(cboFiltro, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &NotasWidget::onFiltroChanged);
  connect(tablaView, &QTableView::customContextMenuRequested, this,
          &NotasWidget::onContextMenu);
  connect(tablaView, &QTableView::doubleClicked, this,
          &NotasWidget::onDoubleClicked);
}

// ─────────────────────────────────────────────────────────────
// Refrescar datos
// ─────────────────────────────────────────────────────────────
void NotasWidget::refrescar() {
  QString filtro;
  if (cboFiltro) {
    int idx = cboFiltro->currentIndex();
    if (idx == 1)
      filtro = "Pendiente";
    else if (idx == 2)
      filtro = "Completada";
  }

  QSqlQueryModel *nuevoModelo = base.consultarNotas(conexionDB, filtro);
  proxyModel->setSourceModel(nuevoModelo);
  if (modelo) {
    modelo->deleteLater();
  }
  modelo = nuevoModelo;

  // Ajustar anchos de columna
  tablaView->setColumnHidden(0, true);                  // id
  tablaView->setColumnHidden(7, true);                  // descripcion
  tablaView->horizontalHeader()->resizeSection(1, 75);  // Prioridad
  tablaView->horizontalHeader()->resizeSection(2, 200); // Título
  tablaView->horizontalHeader()->resizeSection(3, 100); // Usuario
  tablaView->horizontalHeader()->resizeSection(4, 120); // Creada
  tablaView->horizontalHeader()->resizeSection(5, 90);  // Límite
  tablaView->horizontalHeader()->resizeSection(6, 90);  // Estado

  actualizarContador();
}

void NotasWidget::actualizarContador() {
  int pendientes = base.contarNotasPendientes(conexionDB);
  emit pendingCountChanged(pendientes);

  if (pendientes > 0)
    lblContador->setText(QString(tr("⚠ %1 pendiente(s)")).arg(pendientes));
  else
    lblContador->setText(tr("✓ Sin pendientes"));
  lblContador->setStyleSheet(pendientes > 0
                                 ? "color: #C62828; font-weight: bold;"
                                 : "color: #2E7D32; font-weight: bold;");
}

// ─────────────────────────────────────────────────────────────
// Obtener ID de la nota seleccionada
// ─────────────────────────────────────────────────────────────
int NotasWidget::idNotaSeleccionada() {
  QModelIndex idx = tablaView->currentIndex();
  if (!idx.isValid())
    return -1;
  // Columna 0 = id (hidden pero presente en el modelo)
  QModelIndex idxSource = proxyModel->mapToSource(idx);
  return modelo->data(modelo->index(idxSource.row(), 0)).toInt();
}

// ─────────────────────────────────────────────────────────────
// Diálogo para crear / editar nota
// ─────────────────────────────────────────────────────────────
bool NotasWidget::dialogoNota(QString &titulo, QString &descripcion,
                              QString &fechaLimite, QString &prioridad,
                              int idEdicion) {
  QDialog dlg(this);
  dlg.setWindowTitle(idEdicion < 0 ? tr("Nueva nota") : tr("Editar nota"));
  dlg.setMinimumWidth(420);

  QFormLayout *form = new QFormLayout;

  QLineEdit *edTitulo = new QLineEdit(&dlg);
  edTitulo->setMaxLength(200);
  edTitulo->setPlaceholderText(tr("Título de la nota..."));
  edTitulo->setText(titulo);
  form->addRow(tr("Título *:"), edTitulo);

  QTextEdit *edDesc = new QTextEdit(&dlg);
  edDesc->setPlaceholderText(tr("Descripción opcional..."));
  edDesc->setPlainText(descripcion);
  // edDesc->setMaximumHeight(100);
  form->addRow(tr("Descripción:"), edDesc);

  QComboBox *cboPrio = new QComboBox(&dlg);
  cboPrio->addItems({tr("Alta"), tr("Normal"), tr("Baja")});
  cboPrio->setCurrentText(prioridad.isEmpty() ? tr("Normal") : prioridad);
  form->addRow(tr("Prioridad:"), cboPrio);

  QDateEdit *dateLimite = new QDateEdit(&dlg);
  dateLimite->setCalendarPopup(true);
  dateLimite->setDisplayFormat("dd/MM/yyyy");
  dateLimite->setSpecialValueText(tr("Sin fecha límite"));
  dateLimite->setDate(QDate::currentDate());
  // Si ya tiene fecha, la mostramos
  if (!fechaLimite.isEmpty()) {
    QDate d = QDate::fromString(fechaLimite, "dd/MM/yyyy");
    if (!d.isValid())
      d = QDate::fromString(fechaLimite, "yyyy-MM-dd");
    if (d.isValid())
      dateLimite->setDate(d);
  }

  QCheckBox *chkSinFecha = new QCheckBox(tr("Sin fecha límite"), &dlg);
  chkSinFecha->setChecked(fechaLimite.isEmpty());
  connect(chkSinFecha, &QCheckBox::toggled, dateLimite,
          &QDateEdit::setDisabled);
  dateLimite->setDisabled(chkSinFecha->isChecked());

  QHBoxLayout *fechaBox = new QHBoxLayout;
  fechaBox->addWidget(dateLimite);
  fechaBox->addWidget(chkSinFecha);
  form->addRow(tr("Fecha límite:"), fechaBox);

  QDialogButtonBox *buttons = new QDialogButtonBox(
      QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
  buttons->button(QDialogButtonBox::Ok)->setText(tr("Guardar"));
  buttons->button(QDialogButtonBox::Cancel)->setText(tr("Cancelar"));

  QVBoxLayout *vlay = new QVBoxLayout(&dlg);
  vlay->addLayout(form);
  vlay->addWidget(buttons);

  connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
  connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

  if (dlg.exec() != QDialog::Accepted)
    return false;

  if (edTitulo->text().trimmed().isEmpty()) {
    QMessageBox::warning(this, tr("Campo requerido"),
                         tr("El título es obligatorio."));
    return false;
  }

  titulo = edTitulo->text().trimmed();
  descripcion = edDesc->toPlainText().trimmed();
  prioridad = cboPrio->currentText();
  fechaLimite =
      chkSinFecha->isChecked() ? "" : dateLimite->date().toString("yyyy-MM-dd");
  return true;
}

// ─────────────────────────────────────────────────────────────
// Slots
// ─────────────────────────────────────────────────────────────
void NotasWidget::onNuevaNota() {
  QString titulo, descripcion, fechaLimite, prioridad = "Normal";
  if (!dialogoNota(titulo, descripcion, fechaLimite, prioridad))
    return;

  QString usuario = conf->getUsuario();
  if (!base.crearNota(conexionDB, titulo, descripcion, usuario, fechaLimite,
                      prioridad)) {
    QMessageBox::critical(this, tr("Error"),
                          tr("No se ha podido guardar la nota."));
    return;
  }
  refrescar();
}

void NotasWidget::onFiltroChanged(int /*index*/) { refrescar(); }

void NotasWidget::onDoubleClicked(const QModelIndex &index) {
  Q_UNUSED(index)
  int id = idNotaSeleccionada();
  if (id < 0)
    return;

  // Recuperar datos actuales de la nota seleccionada (del modelo)
  QModelIndex srcIdx = proxyModel->mapToSource(tablaView->currentIndex());
  int row = srcIdx.row();

  QString prioridad = modelo->data(modelo->index(row, 1)).toString();
  QString titulo = modelo->data(modelo->index(row, 2)).toString();
  QString fechaLimite = modelo->data(modelo->index(row, 5)).toString();
  QString descripcion = modelo->data(modelo->index(row, 7)).toString();

  if (!dialogoNota(titulo, descripcion, fechaLimite, prioridad, id))
    return;

  if (!base.editarNota(conexionDB, id, titulo, descripcion, fechaLimite,
                       prioridad)) {
    QMessageBox::critical(this, tr("Error"),
                          tr("No se ha podido actualizar la nota."));
    return;
  }
  refrescar();
}

void NotasWidget::onContextMenu(const QPoint &pos) {
  QModelIndex idx = tablaView->indexAt(pos);
  if (!idx.isValid())
    return;

  QModelIndex srcIdx = proxyModel->mapToSource(idx);
  QString estadoActual =
      modelo->data(modelo->index(srcIdx.row(), 6)).toString();
  QString prioActual = modelo->data(modelo->index(srcIdx.row(), 1)).toString();

  QMenu menu(this);

  // Estado
  if (estadoActual != "Completada") {
    QAction *acComp = menu.addAction(tr("✅  Marcar como completada"));
    connect(acComp, &QAction::triggered, this,
            &NotasWidget::onMarcarCompletada);
  } else {
    QAction *acPend = menu.addAction(tr("🔄  Marcar como pendiente"));
    connect(acPend, &QAction::triggered, this, &NotasWidget::onMarcarPendiente);
  }
  menu.addSeparator();

  // Prioridad
  QMenu *menuPrio = menu.addMenu(tr("🏷  Cambiar prioridad"));
  for (const QString &p : {tr("Alta"), tr("Normal"), tr("Baja")}) {
    QAction *ac = menuPrio->addAction((p == prioActual ? "● " : "○ ") + p);
    QString pFinal = p;
    connect(ac, &QAction::triggered, this,
            [this, pFinal]() { onCambiarPrioridad(pFinal); });
  }
  menu.addSeparator();

  QAction *acEditar = menu.addAction(tr("✏  Editar nota"));
  connect(acEditar, &QAction::triggered, this,
          [this, idx]() { onDoubleClicked(idx); });

  QAction *acBorrar = menu.addAction(tr("🗑  Eliminar nota"));
  connect(acBorrar, &QAction::triggered, this, &NotasWidget::onEliminar);

  menu.exec(tablaView->viewport()->mapToGlobal(pos));
}

void NotasWidget::onMarcarCompletada() {
  int id = idNotaSeleccionada();
  if (id < 0)
    return;
  base.actualizarEstadoNota(conexionDB, id, "Completada");
  refrescar();
}

void NotasWidget::onMarcarPendiente() {
  int id = idNotaSeleccionada();
  if (id < 0)
    return;
  base.actualizarEstadoNota(conexionDB, id, "Pendiente");
  refrescar();
}

void NotasWidget::onEliminar() {
  int id = idNotaSeleccionada();
  if (id < 0)
    return;
  int resp = QMessageBox::question(
      this, tr("Eliminar nota"),
      tr("¿Seguro que quieres eliminar esta nota?\nEsta acción no se puede "
         "deshacer."),
      QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
  if (resp != QMessageBox::Yes)
    return;
  base.eliminarNota(conexionDB, id);
  refrescar();
}

void NotasWidget::onCambiarPrioridad(const QString &prioridad) {
  int id = idNotaSeleccionada();
  if (id < 0)
    return;
  base.actualizarPrioridadNota(conexionDB, id, prioridad);
  refrescar();
}

#include <QStyleOption>
void NotasWidget::paintEvent(QPaintEvent *) {
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
