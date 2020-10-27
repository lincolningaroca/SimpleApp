#include "newuserdialog.h"
#include "ui_newuserdialog.h"
#include <QInputDialog>

NewUserDialog::NewUserDialog(frmMode mode, QWidget *parent) :
  QDialog(parent), ui(new Ui::NewUserDialog), _mode(mode)
{
  ui->setupUi(this);
  setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
  showMode();
  setFieldValidate();
  ui->pushButton->setShortcut(Qt::CTRL+Qt::Key_D);
  ui->pushButton_2->setShortcut(Qt::CTRL+Qt::Key_E);
  ui->leDni->addAction(QIcon(":/images/dni1.png"),QLineEdit::LeadingPosition);
  ui->leEmail->addAction(QIcon(":/images/email.png"),QLineEdit::LeadingPosition);
  ui->leNombres->addAction(QIcon(":/images/name.png"),QLineEdit::LeadingPosition);
}

NewUserDialog::~NewUserDialog()
{
  delete ui;
}

void NewUserDialog::showMode()
{
  if(_mode==insertMode)
    {
      setWindowTitle("Nuevo usuario");
      ui->btnOpction->setText("Guardar datos");
      ui->btnOpction->setDefault(true);
      ui->pushButton->setVisible(false);
      ui->pushButton_2->setVisible(false);
    }
  else{
      setWindowTitle("Editar datos..");
      ui->btnOpction->setText("Actualizar datos");
      ui->btnOpction->setDefault(true);
      ui->leDni->setReadOnly(true);
      ui->leEmail->setReadOnly(true);
      ui->pushButton->setToolTip("<img src=\":/images/dni1.png\" />"
                                 "<p style=\"color:'#32547f'\"><h4>Cambiar DNI (Ctrl+D)</h4></p>");
      ui->pushButton_2->setToolTip("<p style=\"color:'#32547f'\">"
                                   "<img src=\":/images/email.png\" />"
                                   "<h4>Cambiar email (Ctrl+E)</h4></p>");
    }
}
bool NewUserDialog::validate()
{
  //validar que no dejen los campos  vacios
  if(ui->leDni->text().isEmpty())
    {
      QMessageBox::warning(this,qApp->applicationName(),tr("Este campo es requerido."));
      ui->leDni->setFocus(Qt::OtherFocusReason);
      return false;
    }
  if(ui->leNombres->text().isEmpty())
    {
      QMessageBox::warning(this,qApp->applicationName(),tr("Este campo es requerido."));
      ui->leNombres->setFocus(Qt::OtherFocusReason);
      return false;
    }
  if(ui->leEmail->text().isEmpty())
    {
      QMessageBox::warning(this,qApp->applicationName(),tr("Este campo es requerido."));
      ui->leEmail->setFocus(Qt::OtherFocusReason);
      return false;
    }
  //validar que la informacion ingresada es correcta
  if(ui->leDni->text().count()!=8)
    {
      QMessageBox::warning(this,qApp->applicationName(),tr("Un Nro. de DNI consta de 8 dígitos."));
      ui->leDni->selectAll();
      ui->leDni->setFocus(Qt::OtherFocusReason);
      return false;
    }
  if(!regEmailVal.exactMatch(ui->leEmail->text()))
    {
      QMessageBox::warning(this,qApp->applicationName(),tr("La dirección de correo no es válida."));
      ui->leEmail->selectAll();
      ui->leEmail->setFocus(Qt::OtherFocusReason);
      return false;
    }
  if(db.uniqueValidate("dni",ui->leDni->text()))
    {
      QMessageBox::warning(this,qApp->applicationName(),tr("Este número ya esta registrado\n"
                                                           "Ingrese uno nuevo."));
      ui->leDni->selectAll();
      ui->leDni->setFocus(Qt::OtherFocusReason);
      return false;
    }
  if(db.uniqueValidate("email",ui->leEmail->text()))
    {
      QMessageBox::warning(this,qApp->applicationName(),tr("Este correo ya esta registrado\n"
                                                           "Ingrese uno nuevo."));
      ui->leEmail->selectAll();
      ui->leEmail->setFocus(Qt::OtherFocusReason);
      return false;
    }

  return true;
}
void NewUserDialog::setFieldValidate()
{
  //validar numero de dni
  QRegExp regDniVal("^\\d{8}$");
  QRegExpValidator *valDni=new QRegExpValidator(regDniVal);
  ui->leDni->setValidator(valDni);

  //validar nombres
  QRegExp regNomVal("^([a-zA-ZñÑ]\\s?)+\\S$");
  QRegExpValidator *valNom=new QRegExpValidator(regNomVal);
  ui->leNombres->setValidator(valNom);

  //validar nombres
  regEmailVal.setPattern("^(?:[\\w.-]+)@(?:\\w+\\.[a-z0-9]{2,3})(?:\\.[a-z]{2,3})?$");
  QRegExpValidator *valEmail=new QRegExpValidator(regEmailVal);
  ui->leEmail->setValidator(valEmail);

}
void NewUserDialog::assignValues()
{
  _dni=ui->leDni->text();
  _nombres=ui->leNombres->text().simplified();
  _email=ui->leEmail->text();
}
void NewUserDialog::setData(const QList<QVariant> &data)
{
  //  for(auto d:data){
  //      _data.append(data.at(d));
  //    }
  _data=data;
  _id=_data.at(0).toUInt();
  ui->leDni->setText(_data.at(1).toString());
  ui->leNombres->setText(_data.at(2).toString());
  ui->leEmail->setText(_data.at(3).toString());
  //  qDebug()<<_data.at(0).toUInt();
}
void NewUserDialog::on_pushButton_clicked()
{
  uint dni=QInputDialog::getInt(this,"Cambio de número de DNI",
                                "Ingrese un número, para ser verificado.\t  ",
                                ui->leDni->text().toUInt());
  if(QString::number(dni).count()!=8)
    {
      QMessageBox::warning(this,qApp->applicationName(),tr("Un DNI debe contener 8 dígitos."));
      return;
    }
  if(dni==ui->leDni->text().toUInt()){
      return;
    }
  if(db.uniqueValidate("dni",dni))
    {
      QMessageBox::warning(this,qApp->applicationName(),tr("Este número ya esta registrado\n"
                                                           "Ingrese uno nuevo."));
      return;
    }
  ui->leDni->setText(QString::number(dni));
}

void NewUserDialog::on_btnOpction_clicked()
{
  if(_mode==insertMode)
    {
      if(!validate()){
          return;
        }
      assignValues();
      accept();
    }
  else{
      assignValues();
      accept();
    }
}

void NewUserDialog::on_pushButton_2_clicked()
{
  QString email=QInputDialog::getText(this,"Cambio de email",
                                      "Ingrese una dirección de correo, para ser verificado.\t  ");
  if(email.isEmpty())
    return;

  if(email.compare(ui->leEmail->text())==0){
      return;
    }
  if(!regEmailVal.exactMatch(email))
    {
      QMessageBox::warning(this,qApp->applicationName(),tr("Dirección de correo inválido."));
      return;
    }
  if(db.uniqueValidate("email",email))
    {
      QMessageBox::warning(this,qApp->applicationName(),tr("Este correo ya esta registrado\n"
                                                           "Ingrese uno nuevo."));
      return;
    }
  ui->leEmail->setText(email);
}

void NewUserDialog::on_btnCancelar_clicked()
{
    reject();
}
