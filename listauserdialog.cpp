#include "listauserdialog.h"
#include "ui_listauserdialog.h"

ListaUserDialog::ListaUserDialog(QWidget *parent) :
  QDialog(parent), ui(new Ui::ListaUserDialog)
{
  ui->setupUi(this);
  setWindowFlags(Qt::Dialog|Qt::MSWindowsFixedSizeDialogHint);
  setupTable();
  setFieldValidate();
  ui->txtDni->addAction(QIcon(":/images/dni1.png"),QLineEdit::LeadingPosition);
  ui->txtNombres->addAction(QIcon(":/images/name.png"),QLineEdit::LeadingPosition);
  ui->txtEmail->addAction(QIcon(":/images/email.png"),QLineEdit::LeadingPosition);
}

ListaUserDialog::~ListaUserDialog()
{
  delete ui;
}

void ListaUserDialog::on_pushButton_clicked()
{
  if(!db.dbConection()){
      QMessageBox::critical(this,qApp->applicationName(),tr("Ocurrio un error.").append(
                              db.errorMessage()));
      return;
    }
  if(ui->txtDni->text().isEmpty()||ui->txtNombres->text().isEmpty()||ui->txtEmail->text().isEmpty())
    {
      QMessageBox::warning(this,qApp->applicationName(),tr("Complete todos los campos."));
      ui->txtDni->setFocus(Qt::OtherFocusReason);
      return;
    }

  if(ui->txtDni->text().count()!=8)
    {
      QMessageBox::warning(this,qApp->applicationName(),tr("Un DNI debe contener 8 dígitos."));
      ui->txtDni->selectAll();
      ui->txtDni->setFocus(Qt::OtherFocusReason);
      return;
    }
  if(db.uniqueValidate("dni",ui->txtDni->text()))
    {
      QMessageBox::warning(this,qApp->applicationName(),tr("Este número ya esta registrado\n"
                                                           "Ingrese uno nuevo."));
      ui->txtDni->selectAll();
      ui->txtDni->setFocus(Qt::OtherFocusReason);
      return;
    }
  if(db.uniqueValidate("email",ui->txtEmail->text()))
    {
      QMessageBox::warning(this,qApp->applicationName(),tr("Este correo ya esta registrado\n"
                                                           "Ingrese uno nuevo."));
      ui->txtEmail->selectAll();
      ui->txtEmail->setFocus(Qt::OtherFocusReason);
      return;
    }
  if(!regEmailVal.exactMatch(ui->txtEmail->text()))
    {
      QMessageBox::warning(this,qApp->applicationName(),tr("Dirección de correo inválido."));
      ui->txtEmail->selectAll();
      ui->txtEmail->setFocus(Qt::OtherFocusReason);
      return;
    }
  uint rowCount=ui->tblLista->rowCount();
  ui->tblLista->insertRow(rowCount);
  ui->tblLista->setItem(rowCount,0,new QTableWidgetItem(ui->txtDni->text()));
  ui->tblLista->setItem(rowCount,1,new QTableWidgetItem(ui->txtNombres->text().toUpper().simplified()));
  ui->tblLista->setItem(rowCount,2,new QTableWidgetItem(ui->txtEmail->text()));

  //limpiar controles
  ui->txtDni->clear();
  ui->txtNombres->clear();
  ui->txtEmail->clear();
  ui->txtDni->setFocus(Qt::OtherFocusReason);
  ui->tblLista->selectRow(ui->tblLista->rowCount()-1);
}

void ListaUserDialog::on_pushButton_2_clicked()
{
  ui->tblLista->removeRow(ui->tblLista->currentRow());
}

void ListaUserDialog::on_btnGuardar_clicked()
{
  if(!db.dbConection())
    {
      QMessageBox::critical(this,qApp->applicationName(),tr("Ocurrio un error.").append(
                              db.errorMessage()));
      return;
    }

  if(ui->tblLista->rowCount()<5)
    {
      QMessageBox::warning(this,qApp->applicationName(),
                           tr("Para poder insertar una lista de usuarios, debería\n"
                              "tener como mínimo cinco registros."));
      ui->txtDni->setFocus(Qt::OtherFocusReason);
      return;
    }
  QVariantList dni;
  QVariantList nombres;
  QVariantList email;
  QSqlQuery qry;
  qry.prepare("INSERT INTO usuarios(dni,nombres,email) VALUES(?,?,?)");

  for(int i=0;i<ui->tblLista->rowCount();i++)
    {
      dni.append(ui->tblLista->item(i,0)->data(Qt::DisplayRole));
      nombres.append(ui->tblLista->item(i,1)->data(Qt::DisplayRole));
      email.append(ui->tblLista->item(i,2)->data(Qt::DisplayRole));
    }
  qry.addBindValue(dni,QSql::In);
  qry.addBindValue(nombres,QSql::In);
  qry.addBindValue(email,QSql::In);

#ifndef QT_NO_CURSOR
  QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
  if(!qry.execBatch())
    {
      QMessageBox::critical(this,qApp->applicationName(),tr("Ocurrio un error.").append(
                              qry.lastError().text()));
      return;
    }
#ifndef QT_NO_CURSOR
  QGuiApplication::restoreOverrideCursor();
#endif
  QMessageBox::information(this,qApp->applicationName(),tr("Lista guardada."));
  accept();

  //  for (int i=0;i<dni.size();++i) {
  //      qDebug()<<dni.at(i).toString();
  //    }
}
void ListaUserDialog::setFieldValidate()
{
  //validar numero de dni
  QRegExp regDniVal("^[0-9]{8}$");
  QRegExpValidator *valDni=new QRegExpValidator(regDniVal);
  ui->txtDni->setValidator(valDni);

  //validar nombres
  QRegExp regNomVal("^[a-zA_Z\\s]+$");
  QRegExpValidator *valNom=new QRegExpValidator(regNomVal);
  ui->txtNombres->setValidator(valNom);

  //validar nombres
  regEmailVal.setPattern("^(?:[\\w.-]+)@(?:\\w+\\.[a-z0-9]{2,3})(?:\\.[a-z]{2,3})?$");
  QRegExpValidator *valEmail=new QRegExpValidator(regEmailVal);
  ui->txtEmail->setValidator(valEmail);

}

void ListaUserDialog::setupTable()
{
  QStringList headers{"Nro. DNI","Nombres","Email"};
  ui->tblLista->setColumnCount(headers.size());
  ui->tblLista->setHorizontalHeaderLabels(headers);
  ui->tblLista->setSelectionBehavior(QAbstractItemView::SelectRows);

  ui->tblLista->setColumnWidth(1,280);
  ui->tblLista->setColumnWidth(2,170);

}

void ListaUserDialog::on_pushButton_9_clicked()
{
    reject();
}
