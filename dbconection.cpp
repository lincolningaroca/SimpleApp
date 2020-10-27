#include "dbconection.h"
#include <QtCore>
#include <QObject>

DbConection::DbConection()
{

}

QString DbConection::errorMessage() const{return _errorMessage;}
void DbConection::setErrorMessage(const QString &errorMessage){_errorMessage = errorMessage;}
bool DbConection::dbConection()
{
  db=QSqlDatabase::addDatabase("QSQLITE");
  if(!db.isDriverAvailable("QSQLITE"))
    {
      setErrorMessage(db.lastError().text());
      return false;
    }
  db.setDatabaseName(strConexion());
  if(!db.open())
    {
      setErrorMessage(db.lastError().text());
      return false;
    }
  return true;
}
bool DbConection::uniqueValidate(QString field, QVariant value)
{
  bool res=false;
  if(!dbConection())
    {
      setErrorMessage(db.lastError().text());
      return false;
    }
  QSqlQuery qry;
  qry.prepare(QString("SELECT COUNT(*) FROM usuarios WHERE %1=?").arg(field));
  qry.addBindValue(value,QSql::In);
  if(!qry.exec())
    {
      setErrorMessage(qry.lastError().text());
      return false;
    }
  qry.next();
  if(qry.value(0).toUInt()==1)
    res=true;
  return res;
}// acabo de agragar un comentario.

