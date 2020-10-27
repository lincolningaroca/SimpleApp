#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QFile>
#include <QStyleFactory>
#include <LimeReport>
#include "newuserdialog.h"
#include "dbconection.h"
#include "listauserdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
  Q_OBJECT

public:
  Widget(QWidget *parent = nullptr);
  ~Widget();
  enum optReport{
    RPT_PREVIEW,
    RPT_PRINT
  };

private slots:
  void on_btnNuevo_clicked();
  void on_pushButton_clicked();
  void on_btnEliminar_clicked();
  void on_btnPorLotes_clicked();
  void on_lineEdit_textChanged(const QString &arg1);
  void on_comboBox_currentIndexChanged(int index);
  void on_btnTheme_clicked();
  void on_pushButton_2_clicked();
  void on_pushButton_3_clicked();
  //funciones personalizadas
  void loadReport(QString rptName, optReport opt);

private:
  Ui::Widget *ui;
  DbConection db;
  QSqlTableModel *model;
  QModelIndex index;
  QSortFilterProxyModel *proxyModel;
  LimeReport::ReportEngine *mReport;

  void loadData();



};
#endif // WIDGET_H
