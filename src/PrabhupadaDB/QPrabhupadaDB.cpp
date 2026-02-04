#include <QPrabhupadaDB.h>
#include <QApplication>

QStorageDB::QStorageDB()
  : inherited()
{
}

QStorageDB::QStorageDB( const QStorageDB& A )
  : inherited( A )
  , m_Query   ( A.m_Query )
  , m_SQL     ( A.m_SQL )
  , m_Database( A.m_Database )
  , m_Schema  ( A.m_Schema )
{
}

QStorageDB::QStorageDB( QStorageDB&& A )
  : inherited( std::move( A ) )
  , m_Query   ( std::move( A.m_Query ) )
  , m_SQL     ( std::move( A.m_SQL ) )
  , m_Database( std::move( A.m_Database ) )
  , m_Schema  ( std::move( A.m_Schema ) )
{
}

QStorageDB& QStorageDB::operator = ( const QStorageDB& A )
{
  inherited::operator = ( A );

  m_Query     = A.m_Query;
  m_SQL       = A.m_SQL;
  m_Database  = A.m_Database;
  m_Schema    = A.m_Schema;

  return *this;
}

QStorageDB& QStorageDB::operator = ( QStorageDB&& A )
{
  inherited::operator = ( std::move( A ) );

  m_Query     = std::move( A.m_Query );
  m_SQL       = std::move( A.m_SQL );
  m_Database  = std::move( A.m_Database );
  m_Schema    = std::move( A.m_Schema );

  return *this;
}

QStorageDB::~QStorageDB()
{
}

void QStorageDB::SetDatabase( QSqlDatabase *Value )
{
  if ( m_Database != Value ) {
    m_Database = Value;
    if ( m_Query != nullptr ) {
      delete m_Query;
    }
    if ( Value == nullptr ) {
      m_Query = nullptr;
    } else {
      m_Query = new QSqlQuery( *m_Database );
    }
  }
}

QString QStorageDB::PrefixKeyStorage( bool AUseUserName )
{
  QString APrefix, AUserName;

  APrefix = qApp->objectName();

  if ( m_Database != nullptr && AUseUserName ) {
    AUserName = m_Database->userName();
    if ( !AUserName.isEmpty() ) {
      APrefix += "-" + AUserName;
    }
  }

  APrefix += "-";

  return APrefix;
}

void QStorageDB::ResetSettings()
{
  m_Query->clear();
# ifdef CS_MODE
    m_Query->prepare( QString( "delete from %1\"UserReg\" where \"UserRegKey\" like :APrefix || '%';" ).formatArg( m_Schema ) );
# else
    m_Query->prepare( QString( "delete from %1\"UserReg\" where \"UserRegKey\" like :APrefix || '%';" ).arg( m_Schema ) );
# endif
  m_Query->bindValue( ":APrefix", PrefixKeyStorage( true ) );
  m_Query->exec();
}

bool QStorageDB::BeginLoadDB()
{
  if ( m_Database != nullptr && m_Database->isOpen() && m_Query != nullptr ) {
    m_Query->prepare( QString( "select\n"
                              "  a.\"UserRegKey\"\n"
                              ", a.\"UserData\"\n"
                              "from\n"
                              "    %1\"UserReg\" a\n"
#                             ifdef CS_MODE
                                "where a.\"UserRegKey\" = :UserRegKey;" ).formatArg( m_Schema ) );
#                             else
                                "where a.\"UserRegKey\" = :UserRegKey;" ).arg( m_Schema ) );
#                             endif
    m_Query->bindValue( ":UserRegKey", m_FileName );
    m_Query->exec();
    if ( m_Query->next() ) {
      m_Stream = new QDataStream( m_Query->value( 1 ).toByteArray() );
      return true;
    } else {
      return false;
    }
  }
  return false;
}

void QStorageDB::EndLoadDB()
{
  delete m_Stream;
  m_Query->clear();
}

void QStorageDB::BeginSaveDB()
{
  if ( m_Database != nullptr && m_Database->isOpen() && m_Query != nullptr ) {
    m_Query->prepare( QString( "select\n"
                               "  a.\"UserRegKey\"\n"
                               "from\n"
                               "    %1\"UserReg\" a\n"
#                              ifdef CS_MODE
                                 "where a.\"UserRegKey\" = :UserRegKey;" ).formatArg( m_Schema ) );
#                              else
                                 "where a.\"UserRegKey\" = :UserRegKey;" ).arg( m_Schema ) );
#                              endif
    m_Query->bindValue( ":UserRegKey", m_FileName );
    m_Query->exec();
    m_ByteArray = new QByteArray();
    QBuffer *BU;
    BU = new QBuffer( m_ByteArray );
    if ( BU->open( QIODevice::ReadWrite ) ) {
      m_Stream = new QDataStream( BU );
      if ( m_Query->next() ) {
        m_SQL = QString( "update %1\"UserReg\"\n"
                         "set\n"
                         "  \"UserData\" = :UserData\n"
                         "where\n"
#                        ifdef CS_MODE
                           "    \"UserRegKey\" = :UserRegKey;" ).formatArg( m_Schema );
#                        else
                           "    \"UserRegKey\" = :UserRegKey;" ).arg( m_Schema );
#                        endif
      } else {
        m_SQL = QString( "insert into %1\"UserReg\"\n"
                         "( \"UserRegKey\", \"UserData\" )\n"
                         "values\n"
#                        ifdef CS_MODE
                           "( :UserRegKey, :UserData );" ).formatArg( m_Schema );
#                        else
                           "( :UserRegKey, :UserData );" ).arg( m_Schema );
#                        endif
      }
    }
  }
}

void QStorageDB::EndSaveDB()
{
  m_Query->clear();
  m_Query->prepare( m_SQL );
  m_Query->bindValue( ":UserRegKey", m_FileName );
  m_Query->bindValue( ":UserData", *m_ByteArray, QSql::In | QSql::Binary );
  m_Query->exec();

  delete m_Stream;
}
