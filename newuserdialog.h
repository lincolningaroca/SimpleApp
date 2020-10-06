#ifndef NEWUSERDIALOG_H
#define NEWUSERDIALOG_H

#include <QDialog>
#include <QDialog>
#include <QMessageBox>
#include <QRegExp>
#include <QRegExpValidator>
//#include <QSqlQuery>
//#include <QSqlError>
#include "dbconection.h"

namespace Ui {
  class NewUserDialog;
}

class NewUserDialog : public QDialog
{
  Q_OBJECT

public:
  enum frmMode{insertMode, editMode};
//  enum validateMode{NormalMode, DniMode, EmailMode};
  explicit NewUserDialog(frmMode mode, QWidget *parent = nullptr);
  ~NewUserDialog();

  QString dni()const {return _dni;}
  QString nombres()const{ return _nombres;}
  QString email()const{return _email;}
  uint id() const{return _id;}
  void setData(const QList<QVariant> &data);

private slots:
  void on_pushButton_clicked();

  void on_btnOpction_clicked();

  void on_pushButton_2_clicked();

  void on_btnCancelar_clicked();

private:
  Ui::NewUserDialog *ui;

  DbConection db;
  QRegExp regEmailVal;
  QString _dni;
  QString _nombres;
  QString _email;
  frmMode _mode;
  uint _id;
  QList<QVariant> _data;

  void showMode();
  bool validate();
  void setFieldValidate();
  void assignValues();
};

#endif // NEWUSERDIALOG_H
