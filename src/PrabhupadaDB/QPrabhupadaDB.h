#ifndef QPrabhupadaDB_H
#define QPrabhupadaDB_H

#include <QPrabhupadaDBFocus.h>
#include <QPrabhupada.h>

#ifdef CS_MODE

#include <QtSql>

#else

#include <QDialog>
#include <QComboBox>
#include <QSaveFile>
#include <QFile>
#include <QBuffer>
#include <QMainWindow>
#include <QMessageBox>
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>

#endif

//#include <pqxx/pqxx>

class Q_PRABHUPADADB_EXPORT QStorageDB : public QStorage
{
  private:
    using inherited = QStorage;
  public:
    QStorageDB();
    virtual ~QStorageDB();
    QStorageDB( QStorageDB&& A );
    QStorageDB( const QStorageDB& A );
    QStorageDB& operator = ( const QStorageDB& A );
    QStorageDB& operator = ( QStorageDB&& A );

    QSqlDatabase* m_Database = nullptr;
    QString m_Schema;
    //pqxx::connection* m_PostgreSQL = nullptr;
    void SetDatabase( QSqlDatabase* Value );
    virtual QString PrefixKeyStorage( bool AUseUserName ) override;
    virtual void ResetSettings() override;
  protected:
    QSqlQuery* m_Query = nullptr;
    QString m_SQL;
    virtual bool BeginLoadDB() override;
    virtual void EndLoadDB() override;
    virtual void BeginSaveDB() override;
    virtual void EndSaveDB() override;
};

#endif
