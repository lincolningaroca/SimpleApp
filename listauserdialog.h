#ifndef LISTAUSERDIALOG_H
#define LISTAUSERDIALOG_H

#include <QDialog>
#include <QDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include "dbconection.h"

namespace Ui {
  class ListaUserDialog;
}

class ListaUserDialog : public QDialog
{
  Q_OBJECT

public:
  explicit ListaUserDialog(QWidget *parent = nullptr);
  ~ListaUserDialog();

private slots:
  void on_pushButton_clicked();

  void on_pushButton_2_clicked();

  void on_btnGuardar_clicked();

  void on_pushButton_9_clicked();

private:
  Ui::ListaUserDialog *ui;
  DbConection db;
  QRegExp regEmailVal;
  void setupTable();
  void setFieldValidate();
};

#endif // LISTAUSERDIALOG_H
