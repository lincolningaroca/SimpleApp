#ifndef DBCONECTION_H
#define DBCONECTION_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QVariant>
#include <QSqlQuery>
#include <QtCore>

class DbConection
{
public:
  DbConection();
  bool dbConection();
  QString errorMessage() const;
  QString strConexion(){return _strConexion;};
  void setErrorMessage(const QString &errorMessage);
  bool uniqueValidate(QString field, QVariant value);

private:
  QSqlDatabase db;
  QString _errorMessage="";
  QString _strConexion=qApp->applicationDirPath().append("/db/dbejemplo.db");
};

#endif // DBCONECTION_H
