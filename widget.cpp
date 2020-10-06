#include "widget.h"
#include "ui_widget.h"
#include <QFile>

Widget::Widget(QWidget *parent)
  : QWidget(parent), ui(new Ui::Widget)
{
  ui->setupUi(this);
  setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
  loadData();

  ui->btnNuevo->setShortcut(Qt::Key_N);
  ui->pushButton->setShortcut(Qt::Key_E);
  ui->btnEliminar->setShortcut(Qt::Key_M);
  ui->btnPorLotes->setShortcut(Qt::Key_V);
  //  ui->lineEdit->setInputMask("A-99999999");
  ui->btnTheme->setCheckable(true);
//  ui->pushButton_2->setEnabled(false);
}

Widget::~Widget()
{
  delete ui;
}


void Widget::on_btnNuevo_clicked()
{
  NewUserDialog *newUser=new NewUserDialog(NewUserDialog::insertMode,this);
  if(newUser->exec()==QDialog::Rejected)
    return;
  if(!db.dbConection())
    {
      QMessageBox::critical(this,qApp->applicationName(),tr("Ocurrio un error.").append(
                              db.errorMessage()));
      return;
    }
  QSqlQuery qry;
  qry.prepare("INSERT INTO usuarios(dni,nombres,email) VALUES(?,?,?)");
  qry.addBindValue(newUser->dni(),QSql::In);
  qry.addBindValue(newUser->nombres().toUpper(),QSql::In);
  qry.addBindValue(newUser->email(),QSql::In);
  if(!qry.exec())
    {
      QMessageBox::critical(this,qApp->applicationName(),tr("Error de ejecución.\n").append(
                              qry.lastError().text()));
      return;
    }
  model->select();

  ui->tblUsuarios->selectRow(0);
  ui->tblUsuarios->setFocus(Qt::MouseFocusReason);
}

void Widget::on_pushButton_clicked()
{
  NewUserDialog *editUser=new NewUserDialog(NewUserDialog::editMode,this);
  QList<QVariant> data;
  QModelIndex index=ui->tblUsuarios->currentIndex();
  for(int column=0;column<proxyModel->columnCount();++column){
      data.append(proxyModel->index(index.row(),column).data());
    }
  editUser->setData(data);
  //  qDebug()<<data.at(2).toString();
  if(editUser->exec()==QDialog::Rejected)
    return;

  QSqlQuery qry;
  qry.prepare("UPDATE usuarios SET dni=?,nombres=?,email=? WHERE id=?");
  qry.addBindValue(editUser->dni(),QSql::In);
  qry.addBindValue(editUser->nombres().toUpper(),QSql::In);
  qry.addBindValue(editUser->email(),QSql::In);
  qry.addBindValue(editUser->id(),QSql::In);
  if(!qry.exec())
    {
      QMessageBox::critical(this,qApp->applicationName(),tr("Error de ejecución.\n").append(
                              qry.lastError().text()));
      return;
    }
  QMessageBox::information(this,qApp->applicationName(),"Datos actulizados");
  model->select();
  ui->tblUsuarios->selectRow(index.row());
  ui->tblUsuarios->setFocus(Qt::MouseFocusReason);
}

void Widget::on_btnEliminar_clicked()
{
  index=ui->tblUsuarios->currentIndex();

  if(!db.dbConection())
    {
      QMessageBox::critical(this,qApp->applicationName(),tr("Ocurrio un error.").append(
                              db.errorMessage()));
      return;
    }

  QString name=proxyModel->index(index.row(),2).data().toString();
  QMessageBox msgBox;
  msgBox.setIcon(QMessageBox::Question);
  msgBox.setWindowTitle(qApp->applicationName());
  msgBox.setText(tr("Seguro que desea eliminar a: %1").arg("<h4 style=\"color:#C91903\">"+
                                                           name+"</h4>"));
  msgBox.addButton(tr("Eliminar"),QMessageBox::AcceptRole);
  msgBox.addButton(tr("Cancelar"),QMessageBox::RejectRole);
  if(msgBox.exec()==QMessageBox::RejectRole)
    return;

  QSqlQuery qry;
  qry.prepare("DELETE FROM usuarios WHERE id=?");
  int id=proxyModel->index(index.row(),0).data().toInt();
  qry.addBindValue(id,QSql::In);
  if(!qry.exec())
    {
      QMessageBox::critical(this,qApp->applicationName(),tr("Error de ejecución.\n").append(
                              qry.lastError().text()));
      return;
    }
  model->select();
  ui->tblUsuarios->selectRow(proxyModel->rowCount()-1);
  ui->tblUsuarios->setFocus(Qt::MouseFocusReason);
}

void Widget::on_btnPorLotes_clicked()
{
  ListaUserDialog *userList=new ListaUserDialog(this);
  if(userList->exec()==QDialog::Rejected)
    return;
  model->select();
  ui->tblUsuarios->selectRow(proxyModel->rowCount()-1);
  ui->tblUsuarios->setFocus(Qt::MouseFocusReason);
}

void Widget::on_lineEdit_textChanged(const QString &index)
{
  proxyModel->setFilterKeyColumn(ui->comboBox->currentIndex());
  proxyModel->setFilterRegExp(index);
  ui->tblUsuarios->resizeRowsToContents();
}

void Widget::on_comboBox_currentIndexChanged(int index)
{
  ui->lineEdit->clear();
  ui->lineEdit->setFocus();
  switch (index) {
    case 0:
    case 1:
      {
        QRegExp regDniVal("^[0-9]{8}$");
        QRegExpValidator *valDni=new QRegExpValidator(regDniVal);
        ui->lineEdit->setValidator(valDni);
      }
      break;
    case 2:
      {
        QRegExp regNomVal("^[a-zA_Z\\s]+$");
        QRegExpValidator *valNom=new QRegExpValidator(regNomVal);
        ui->lineEdit->setValidator(valNom);
      }
      break;
    case 3:
      {
        QRegExp regEmailVal("^(?:[\\w.-]+)@(?:\\w+\\.[a-z0-9]{2,3})(?:\\.[a-z]{2,3})?$");
        QRegExpValidator *valEmail=new QRegExpValidator(regEmailVal);
        ui->lineEdit->setValidator(valEmail);
      }
      break;
    }
}
void Widget::loadData()
{
  if(!db.dbConection())
    {
      QMessageBox::critical(this,qApp->applicationName(),tr("Ocurrio un error.").append(
                              db.errorMessage()));
      return;
    }
  model=new QSqlTableModel(this);

  model->setTable("usuarios");
  model->select();
  model->sort(2,Qt::AscendingOrder);


  //proxy model

  proxyModel=new QSortFilterProxyModel(this);
  proxyModel->setSourceModel(model);
  proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
  proxyModel->setFilterKeyColumn(2);
  //  connect(ui->lineEdit,&QLineEdit::textChanged,proxyModel,
  //          &QSortFilterProxyModel::setFilterWildcard);



  ui->tblUsuarios->setModel(proxyModel);
  ui->tblUsuarios->selectRow(0);
  ui->tblUsuarios->setFocus(Qt::MouseFocusReason);

  //comportamineto de la seleccion de la tabla
  ui->tblUsuarios->setSelectionMode(QAbstractItemView::SingleSelection);
  ui->tblUsuarios->setSelectionBehavior(QAbstractItemView::SelectRows);

  //asignar nomnres a las cabeceras
  model->setHeaderData(1,Qt::Horizontal,tr("Nro. DNI"));
  model->setHeaderData(2,Qt::Horizontal,tr("Nombres"));
  model->setHeaderData(3,Qt::Horizontal,tr("Email"));

  ui->tblUsuarios->hideColumn(0);
  ui->tblUsuarios->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
  ui->tblUsuarios->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);

  ui->tblUsuarios->resizeColumnsToContents();
  ui->tblUsuarios->resizeRowsToContents();


  ui->tblUsuarios->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->tblUsuarios->setAlternatingRowColors(true);

  //llenar combobox
  for(int i=0;i<model->columnCount();++i){
      ui->comboBox->addItem(QIcon(":/images/pin.png"),
                            model->headerData(i,Qt::Horizontal).toString());
    }
  ui->lineEdit->addAction((QIcon(":/images/search.png")),QLineEdit::LeadingPosition);
}

void Widget::on_btnTheme_clicked()
{
  if(ui->btnTheme->isChecked())
    {
      qApp->setStyle(QStyleFactory::create("windows"));
      QFile fileName(":/qdarkstyle/style.qss");
      if(!fileName.open(QIODevice::ReadOnly|QIODevice::Text))
        {
          QMessageBox::critical(this,qApp->applicationName(),
                                "Ocurrio un error.0\n"+fileName.errorString());
          return;
        }
      qApp->setStyleSheet(fileName.readAll());
      ui->btnTheme->setText("Cambiar a tema claro");
    }else{
      qApp->setStyleSheet(QString());
      qApp->setStyle(QStyleFactory::create("Fusion"));
      ui->btnTheme->setText("Cambiar a tema oscuro");
    }
}

void Widget::on_pushButton_2_clicked()
{
  QFile fileName(":/rpt/Reporteusuarios.lrxml");
  if(!fileName.open(QIODevice::ReadOnly|QIODevice::Text)){
      QMessageBox::critical(this,qApp->applicationName(),
                            QString("Error al abrir el reporte.\n").append(
                              fileName.errorString()));
      return;
    }
  mReport=new LimeReport::ReportEngine(this);
  mReport->dataManager()->setReportVariable("DATABASE",db.strConexion());
  mReport->loadFromFile(fileName.fileName());
  mReport->previewReport();
}
