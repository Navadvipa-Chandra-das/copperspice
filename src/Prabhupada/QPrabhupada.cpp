#include <QPrabhupada.h>

QMapMemoryStorage::QMapMemoryStorage()
  : inherited()
{
}

QMapMemoryStorage::~QMapMemoryStorage()
{
}

void QMapMemoryStorage::LoadFromStream( QDataStream& ST )
{
  clear();
  // 1
  std::size_t L;
  ST >> L;
  // 2
  QString AFileName;
  QByteArray *BA;
  QBuffer    *BU;
  for ( std::size_t I = 0; I < L; ++I ) {
    BA = new QByteArray();
    BU = new QBuffer( BA );
    ST >> AFileName;
    ST >> *BA;
    if ( BU->open( QIODevice::ReadWrite ) ) {
      emplace( std::make_pair( AFileName, std::make_shared< QDataStream >( BU ) ) );
    }
  }
}

void QMapMemoryStorage::SaveToStream( QDataStream& ST )
{
  // 1
  ST << size();
  // 2
  QDataStream *AStream;
  QByteArray *BA;
  QBuffer    *BU;
  for ( iterator I = begin(); I != end(); ++I ) {
    ST << (*I).first;
    AStream = (*I).second.get();
    BU = static_cast< QBuffer* >( AStream->device() );
    BA = static_cast< QByteArray* >( &BU->buffer() );
    ST << *BA;
  }
}

QMapMemoryStorage::QMapMemoryStorage( const QMapMemoryStorage& A )
  : inherited( A )
{
}

QMapMemoryStorage::QMapMemoryStorage( QMapMemoryStorage&& A )
  : inherited( std::move( A ) )
{
}

QMapMemoryStorage& QMapMemoryStorage::operator = ( const QMapMemoryStorage& A )
{
  inherited::operator = ( A );

  return *this;
}

QMapMemoryStorage& QMapMemoryStorage::operator = ( QMapMemoryStorage&& A )
{
  inherited::operator = ( A );

  return *this;
}

QStorage::QStorage()
{
}

QStorage::QStorage( const QStorage& A )
  : m_Enabled         ( A.m_Enabled )
  , m_Version         ( A.m_Version )
  , m_IsPostgreSQL    ( A.m_IsPostgreSQL )
  , m_File            ( A.m_File )
  , m_SaveFile        ( A.m_SaveFile )
  , m_Stream          ( A.m_Stream )
  , m_FileName        ( A.m_FileName )
  , m_ByteArray       ( A.m_ByteArray )
  , m_MapMemoryStorage( A.m_MapMemoryStorage )
{
}

QStorage::QStorage( QStorage&& A )
  : m_Enabled         ( std::move( A.m_Enabled ) )
  , m_Version         ( std::move( A.m_Version ) )
  , m_IsPostgreSQL    ( std::move( A.m_IsPostgreSQL ) )
  , m_File            ( std::move( A.m_File ) )
  , m_SaveFile        ( std::move( A.m_SaveFile ) )
  , m_Stream          ( std::move( A.m_Stream ) )
  , m_FileName        ( std::move( A.m_FileName ) )
  , m_ByteArray       ( std::move( A.m_ByteArray ) )
  , m_MapMemoryStorage( std::move( A.m_MapMemoryStorage ) )
{
}

QStorage::~QStorage()
{
}

QStorage& QStorage::operator = ( const QStorage& A )
{
  m_Enabled      = A.m_Enabled;
  m_Version      = A.m_Version;
  m_IsPostgreSQL = A.m_IsPostgreSQL;
  m_File         = A.m_File;
  m_SaveFile     = A.m_SaveFile;
  m_Stream       = A.m_Stream;
  m_FileName     = A.m_FileName;
  m_ByteArray    = A.m_ByteArray;

  return *this;
}

QStorage& QStorage::operator = ( QStorage&& A )
{
  m_Enabled      = std::move( A.m_Enabled );
  m_Version      = std::move( A.m_Version );
  m_IsPostgreSQL = std::move( A.m_IsPostgreSQL );
  m_File         = std::move( A.m_File );
  m_SaveFile     = std::move( A.m_SaveFile );
  m_Stream       = std::move( A.m_Stream );
  m_FileName     = std::move( A.m_FileName );
  m_ByteArray    = std::move( A.m_ByteArray );

  return *this;
}

void QStorage::PrepareFileName( void* O, QStorageKind AStorageKind, const QString& AKeyStorage, bool AUseUserName )
{
  if ( AKeyStorage.isEmpty() ) {
    m_FileName = KeyStorage( static_cast< QObject* >( O ), AStorageKind, AUseUserName );
  } else {
    m_FileName = AKeyStorage;
  }
}

bool QStorage::BeginLoad( QStorageKind AStorageKind )
{
  switch ( AStorageKind ) {
    case QStorageKind::File :
      m_File     = new QFile( m_FileName );
      m_Stream   = new QDataStream( m_File );
      return m_File->open( QIODevice::ReadOnly );
    case QStorageKind::DB :
      return BeginLoadDB();
    case QStorageKind::Memory :
      QMapMemoryStorage::iterator I = m_MapMemoryStorage.find( m_FileName );
      if ( I != m_MapMemoryStorage.end() ) {
        m_Stream = m_MapMemoryStorage[ m_FileName ].get();
        m_Stream->device()->seek( 0 );
        return true;
      } else {
        return false;
      }
  }
  return false;
}

void QStorage::EndLoad( QStorageKind AStorageKind )
{
  switch ( AStorageKind ) {
    case QStorageKind::File :
      m_File->close();
      delete m_Stream;
      delete m_File;
      m_File   = nullptr;
      break;
    case QStorageKind::DB :
      EndLoadDB();
      break;
    case QStorageKind::Memory :
      break;
  }
  m_Stream = nullptr;
}

void QStorage::BeginSave( QStorageKind AStorageKind )
{
  switch ( AStorageKind ) {
    case QStorageKind::File :
      m_SaveFile = new QSaveFile( m_FileName );
      if ( m_SaveFile->open( QIODevice::WriteOnly ) ) {
        m_Stream = new QDataStream( m_SaveFile );
      }
      break;
    case QStorageKind::DB :
      BeginSaveDB();
      break;
    case QStorageKind::Memory :
      QMapMemoryStorage::iterator I = m_MapMemoryStorage.find( m_FileName );
      QByteArray *BA;
      QBuffer *BU;
      if ( I == m_MapMemoryStorage.end() ) {
        BA = new QByteArray();
        BU = new QBuffer( BA );
        if ( BU->open( QIODevice::ReadWrite ) ) {
          auto J = m_MapMemoryStorage.emplace( std::make_pair( m_FileName, std::make_shared< QDataStream >( BU ) ) );
          m_Stream = (*J.first).second.get();
        }
      } else {
        m_Stream = m_MapMemoryStorage[ m_FileName ].get();
        BU = static_cast< QBuffer* >( m_Stream->device() );
        BA = static_cast< QByteArray* >( &BU->buffer() );
        BU->seek( 0 );
        BA->truncate( 0 );
      }
      break;
  }
}

void QStorage::EndSave( QStorageKind AStorageKind )
{
  switch ( AStorageKind ) {
    case QStorageKind::File :
      m_SaveFile->commit();
      delete m_Stream;
      delete m_SaveFile;
      m_SaveFile = nullptr;
      break;
    case QStorageKind::DB :
      EndSaveDB();
      break;
    case QStorageKind::Memory :
      break;
  }
  m_Stream = nullptr;
}

bool QStorage::LoadObject( void* O, QStorageKind AStorageKind, QStorager* ST, const QString& AKeyStorage, bool AUseUserName )
{
  PrepareFileName( O, AStorageKind, AKeyStorage, AUseUserName );
  bool LoadSuccess = false;
  if ( m_Enabled ) {
    if ( BeginLoad( AStorageKind ) ) {
      if ( m_Stream != nullptr ) {
        qint8 V;
        *m_Stream >> V;
        if ( V == m_Version ) {
          if ( ST != nullptr ) {
            ST->LoadFromStream( O, *m_Stream );
            *m_Stream >> V;
          }
          LoadSuccess = true;
        }
      }
    }
    EndLoad( AStorageKind );
  }
  return LoadSuccess;
}

void QStorage::SaveObject( void* O, QStorageKind AStorageKind, QStorager* ST, const QString& AKeyStorage, bool AUseUserName )
{
  PrepareFileName( O, AStorageKind, AKeyStorage, AUseUserName );
  if ( m_Enabled ) {
    BeginSave( AStorageKind );
    if ( m_Stream != nullptr ) {
      *m_Stream << m_Version;
      if ( ST != nullptr ) {
        ST->SaveToStream( O, *m_Stream );
        *m_Stream << m_Version;
      }
      EndSave( AStorageKind );
    }
  }
}

void QStorage::RemoveMemory( void* O, const QString& AKeyStorage )
{
  if ( AKeyStorage.isEmpty() ) {
    m_MapMemoryStorage.erase( KeyStorage( static_cast< QObject* >( O ), QStorageKind::Memory, true ) );
  } else {
    m_MapMemoryStorage.erase( AKeyStorage );
  }
}

QString QStorage::KeyStorage( QObject *O, QStorageKind AStorageKind, bool AUseUserName )
{
  QString R, S, APrefix;

  if ( AStorageKind == QStorageKind::DB ) {
    APrefix = PrefixKeyStorage( AUseUserName );
  }

  while ( O != nullptr ) {
    S = O->objectName();
    if ( !S.isEmpty() ) {
      if ( R.isEmpty() )
        R = S;
      else
        R = S + "." + R;
    }

    O = O->parent();
  }

  if ( !R.isEmpty() )
    R += ".ini";

  if ( !APrefix.isEmpty() )
    R = APrefix + R;

  return R;
}

void QStorage::LoadFromStream( QDataStream &ST )
{
  m_MapMemoryStorage.LoadFromStream( ST );
}

void QStorage::SaveToStream( QDataStream &ST )
{
  m_MapMemoryStorage.SaveToStream( ST );
}

void PrepareHistoryComboBox( QComboBox *CB, int MaxCount )
{
  if ( CB->isEditable() ) {
    QString S = CB->currentText();
    if ( !S.isEmpty() ) {
      int I = CB->findText( S );
      if ( I != -1 ) {
        QString SA = CB->itemText( I );
        CB->setItemText( I, CB->itemText( 0 ) );
        CB->setItemText( 0, SA );
      } else {
        CB->insertItem( 0, S );
      }
      for ( I = CB->count() - 1; I >= MaxCount; --I ) {
        CB->removeItem( I );
      }
      CB->setCurrentIndex( 0 );
    }
  }
}

void LoadFromStreamComboBox( QComboBox *CB, QDataStream &ST )
{
  // 1
  QStringList SL;
  ST >> SL;
  CB->addItems( SL );
  // 2
  QString S;
  ST >> S;
  CB->setCurrentText( S );
}

void SaveToStreamComboBox( QComboBox *CB, QDataStream &ST )
{
  // 1, 2
  QStringList SL;
  int L = CB->count();
  for ( int i = 0; i < L; ++i ) {
    SL << CB->itemText( i );
  }
  ST << SL << CB->currentText();
}

void SaveToStreamPrepareHistory( QComboBox *CB, QDataStream &ST, int HistoryCount )
{
  PrepareHistoryComboBox( CB, HistoryCount );
  SaveToStreamComboBox( CB, ST );
}

QStorager::QStorager()
{
}

QStorager::~QStorager()
{
}

QStoragerMainWindow::QStoragerMainWindow()
  : inherited()
{
}

QStoragerMainWindow::~QStoragerMainWindow()
{
}

void QStoragerMainWindow::LoadFromStream( void *AObject, QDataStream &ST )
{
  QMainWindow *O = static_cast< QMainWindow* >( AObject );
  QByteArray BA;
  // 1
  ST >> BA;
  O->restoreGeometry( BA );
  // 2
  ST >> BA;
  O->restoreState( BA );
}

void QStoragerMainWindow::SaveToStream( void *AObject, QDataStream &ST )
{
  QMainWindow *O = static_cast< QMainWindow* >( AObject );
  // 1, 2
  ST << O->saveGeometry() << O->saveState();
}

QStoragerDialog::QStoragerDialog()
: inherited()
{
}

QStoragerDialog::~QStoragerDialog()
{
}

void QStoragerDialog::LoadFromStream( void *AObject, QDataStream &ST )
{
  QDialog *O = static_cast< QDialog* >( AObject );
  QByteArray BA;
  // 1
  ST >> BA;
  O->restoreGeometry( BA );
}

void QStoragerDialog::SaveToStream( void *AObject, QDataStream &ST )
{
  QDialog *O = static_cast< QDialog* >( AObject );
  // 1
  ST << O->saveGeometry();
}

QLanguageInfo::QLanguageInfo()
{
}

QLanguageInfo::QLanguageInfo( const QLanguageInfo &A )
  : m_ID                   ( A.m_ID )
  , m_Language             ( A.m_Language )
  , m_LanguageSlovo        ( A.m_LanguageSlovo )
{
}

QLanguageInfo::QLanguageInfo( QLanguageInfo&& A )
  : m_ID                   ( std::move( A.m_ID ) )
  , m_Language             ( std::move( A.m_Language ) )
  , m_LanguageSlovo        ( std::move( A.m_LanguageSlovo ) )
{
}

QLanguageInfo& QLanguageInfo::operator = ( const QLanguageInfo& A )
{
  m_ID                    = A.m_ID;
  m_Language              = A.m_Language;
  m_LanguageSlovo         = A.m_LanguageSlovo;

  return *this;
}

QLanguageInfo& QLanguageInfo::operator = ( QLanguageInfo&& A )
{
  m_ID                    = std::move( A.m_ID );
  m_Language              = std::move( A.m_Language );
  m_LanguageSlovo         = std::move( A.m_LanguageSlovo );

  return *this;
}

QLanguageInfo::~QLanguageInfo()
{
}

void QLanguageInfo::LoadFromStream( QDataStream& ST )
{
  ST >> m_ID;
  ST >> m_Language;
  ST >> m_LanguageSlovo;
}

void QLanguageInfo::SaveToStream( QDataStream& ST )
{
  ST << m_ID;
  ST << m_Language;
  ST << m_LanguageSlovo;
}

QLanguageVector::QLanguageVector()
  : inherited()
{
}

QLanguageVector::QLanguageVector( const QLanguageVector& A )
  : inherited( A )
  , m_LoadSuccess( A.m_LoadSuccess )
{
}

QLanguageVector::QLanguageVector( QLanguageVector&& A )
  : inherited( std::move( A ) )
  , m_LoadSuccess( std::move( A.m_LoadSuccess ) )
{
}

QLanguageVector& QLanguageVector::operator = ( const QLanguageVector& A )
{
  inherited::operator = ( A );
  m_LoadSuccess = A.m_LoadSuccess;

  return *this;
}

QLanguageVector& QLanguageVector::operator = ( QLanguageVector&& A )
{
  inherited::operator = ( A );
  m_LoadSuccess = std::move( A.m_LoadSuccess );

  return *this;
}

QLanguageVector::~QLanguageVector()
{
  for ( iterator I = begin(); I != end(); ++I ) {
    delete *I;
  }
}

void QLanguageVector::Clear()
{
  for ( iterator I = begin(); I != end(); ++I ) {
    delete *I;
  }
  clear();
  m_LoadSuccess = false;
}

bool QLanguageVector::FindLanguage( const QString &S, std::size_t& AResultIndex )
{
  std::size_t L = size(), I = 0;
  for( ; I < L; ++I ) {
    if ( operator[]( I )->m_Language == S ) {
      AResultIndex = I;
      return true;
    }
  }
  return false;
}

void QLanguageVector::LoadFromStream( QDataStream &ST )
{
  // 1
  clear();
  std::size_t L;
  ST >> L;
  QLanguageInfo* YI;
  for ( std::size_t I = 0; I < L; ++I ) {
    YI = NewLanguageInfo();
    YI->LoadFromStream( ST );
    push_back( YI );
  }
  m_LoadSuccess = L > 0;
}

void QLanguageVector::SaveToStream( QDataStream &ST )
{
  // 1
  std::size_t L = size();
  ST << L;
  for ( iterator I = begin(); I != end(); ++I ) {
    (*I)->SaveToStream( ST );
  }
}

QStoragerLanguageVector::QStoragerLanguageVector()
  : inherited()
{
}

QStoragerLanguageVector::~QStoragerLanguageVector()
{
}

void QStoragerLanguageVector::LoadFromStream( void *AObject, QDataStream &ST )
{
  QLanguageVector *O = static_cast< QLanguageVector* >( AObject );
  // 1
  O->clear();
  std::size_t L;
  ST >> L;
  QLanguageInfo* YI;
  for ( std::size_t I = 0; I < L; ++I ) {
    YI = O->NewLanguageInfo();

    YI->LoadFromStream( ST );
    O->push_back( YI );
  }
  O->m_LoadSuccess = L > 0;
}

void QStoragerLanguageVector::SaveToStream( void *AObject, QDataStream &ST )
{
  QLanguageVector *O = static_cast< QLanguageVector* >( AObject );
  // 1
  std::size_t L = O->size();
  ST << L;
  for ( QLanguageVector::iterator I = O->begin(); I != O->end(); ++I ) {
    (*I)->SaveToStream( ST );
  }
}

const QChar QLike::CharPercent   = '%';
const QChar QLike::CharUnderline = '_';

bool QLike::Like( QString::iterator s, QString::iterator s_end, QString::iterator t, QString::iterator t_end )
{
  if ( t == t_end ) {
    return true;
  }
  QString::iterator tt, ss;
  while ( t != t_end && s != s_end ) {
    if ( ( *t ) == QLike::CharPercent ) {
      tt = ++t;
      ss = s;
      while ( ( ss != s_end ) ) {
        if ( Like( ss++, s_end, tt, t_end ) ) {
          return true;
        }
      }
      return false;
    }
    if ( ( ( *t ) != ( *s ) ) && ( ( *t ) != QLike::CharUnderline ) ) {
      return false;
    }
    t++;
    s++;
  }
  if ( ( s != s_end ) ) {
    return false;
  }
  if ( t == t_end ) {
    return true;
  }
  do {
    if ( ( *t ) != CharPercent ) {
      return false;
    }
  } while ( ++t != t_end );
  return true;
}

QNumberWords::QNumberWords()
  : inherited()
{
}

QNumberWords::~QNumberWords()
{
}

QNumberWords::QNumberWords( const QNumberWords& A )
  : inherited( A )
  , m_NumberWords(    A.m_NumberWords )
  , m_Rubl1(          A.m_Rubl1 )
  , m_Rubl24(         A.m_Rubl24 )
  , m_Rubl5(          A.m_Rubl5 )
  , m_Copeyka1(       A.m_Copeyka1 )
  , m_Copeyka24(      A.m_Copeyka24 )
  , m_Copeyka5(       A.m_Copeyka5 )
  , m_RublGender(     A.m_RublGender )
  , m_CopeycaGender(  A.m_CopeycaGender )
  , m_LenCopeyka(     A.m_LenCopeyka )
  , m_NeedCopeyka(    A.m_NeedCopeyka )
  , m_CopyicaKind(    A.m_CopyicaKind )
  , m_NeedFirstUpper( A.m_NeedFirstUpper )
{
}

QNumberWords::QNumberWords( QNumberWords&& A )
  : inherited( std::move( A ) )
  , m_NumberWords(    std::move( A.m_NumberWords ) )
  , m_Rubl1(          std::move( A.m_Rubl1 ) )
  , m_Rubl24(         std::move( A.m_Rubl24 ) )
  , m_Rubl5(          std::move( A.m_Rubl5 ) )
  , m_Copeyka1(       std::move( A.m_Copeyka1 ) )
  , m_Copeyka24(      std::move( A.m_Copeyka24 ) )
  , m_Copeyka5(       std::move( A.m_Copeyka5 ) )
  , m_RublGender(     std::move( A.m_RublGender ) )
  , m_CopeycaGender(  std::move( A.m_CopeycaGender ) )
  , m_LenCopeyka(     std::move( A.m_LenCopeyka ) )
  , m_NeedCopeyka(    std::move( A.m_NeedCopeyka ) )
  , m_CopyicaKind(    std::move( A.m_CopyicaKind ) )
  , m_NeedFirstUpper( std::move( A.m_NeedFirstUpper ) )
{
}

QNumberWords& QNumberWords::operator = ( const QNumberWords& A )
{
  inherited::operator = ( A );
  m_NumberWords    = A.m_NumberWords;
  m_Rubl1          = A.m_Rubl1;
  m_Rubl24         = A.m_Rubl24;
  m_Rubl5          = A.m_Rubl5;
  m_Copeyka1       = A.m_Copeyka1;
  m_Copeyka24      = A.m_Copeyka24;
  m_Copeyka5       = A.m_Copeyka5;
  m_RublGender     = A.m_RublGender;
  m_CopeycaGender  = A.m_CopeycaGender;
  m_LenCopeyka     = A.m_LenCopeyka;
  m_NeedCopeyka    = A.m_NeedCopeyka;
  m_CopyicaKind    = A.m_CopyicaKind;
  m_NeedFirstUpper = A.m_NeedFirstUpper;

  return *this;
}

QNumberWords& QNumberWords::operator = ( QNumberWords&& A )
{
  inherited::operator = ( A );
  m_NumberWords    = std::move( A.m_NumberWords );
  m_Rubl1          = std::move( A.m_Rubl1 );
  m_Rubl24         = std::move( A.m_Rubl24 );
  m_Rubl5          = std::move( A.m_Rubl5 );
  m_Copeyka1       = std::move( A.m_Copeyka1 );
  m_Copeyka24      = std::move( A.m_Copeyka24 );
  m_Copeyka5       = std::move( A.m_Copeyka5 );
  m_RublGender     = std::move( A.m_RublGender );
  m_CopeycaGender  = std::move( A.m_CopeycaGender );
  m_LenCopeyka     = std::move( A.m_LenCopeyka );
  m_NeedCopeyka    = std::move( A.m_NeedCopeyka );
  m_CopyicaKind    = std::move( A.m_CopyicaKind );
  m_NeedFirstUpper = std::move( A.m_NeedFirstUpper );

  return *this;
}

void QNumberWords::GenerateNumberWords()
{
  QString A[3];
  A[ 0 ] = m_Rubl1;
  A[ 1 ] = m_Rubl24;
  A[ 2 ] = m_Rubl5;
  m_NumberWords = StringNumberToWords( m_Value, m_RublGender, A, m_NeedFirstUpper );
}

void QNumberWords::ReInitRublCopeyka()
{
  m_Rubl1     = Rubl1Init;
  m_Rubl24    = Rubl24Init;
  m_Rubl5     = Rubl5Init;
  m_Copeyka1  = Copeyka1Init;
  m_Copeyka24 = Copeyka24Init;
  m_Copeyka5  = Copeyka5Init;
}

void QNumberWords::ValueChanged()
{
  GenerateNumberWords();
}

QString QNumberWords::MILLION_STR[ QNumberWords::H_STR ][ 3 ];
QString QNumberWords::LIMON_STR[ 3 ];
QString QNumberWords::ONE_GENDER[ 3 ];
QString QNumberWords::TWO_GENDER[ 3 ];
QString QNumberWords::SOTKA[ 9 ];
QString QNumberWords::TEN_STR[ 10 ];
QString QNumberWords::TWENTY_STR[ 8 ];
QString QNumberWords::ONE_STR[ 7 ];
QString QNumberWords::ZERO;
QString QNumberWords::MINUS;

QString QNumberWords::Rubl1Init;
QString QNumberWords::Rubl24Init;
QString QNumberWords::Rubl5Init;
QString QNumberWords::Copeyka1Init;
QString QNumberWords::Copeyka24Init;
QString QNumberWords::Copeyka5Init;

void QNumberWords::RetranslateStrings()
{
  MILLION_STR[ 0 ][ 0 ] = QObject::tr( "тысяча" );
  MILLION_STR[ 0 ][ 1 ] = QObject::tr( "тысячи" );
  MILLION_STR[ 0 ][ 2 ] = QObject::tr( "тысяч" );

  MILLION_STR[ 1 ][ 0 ] = QObject::tr( "миллион" );
  MILLION_STR[ 1 ][ 1 ] = QObject::tr( "миллиона" );
  MILLION_STR[ 1 ][ 2 ] = QObject::tr( "миллионов" );

  MILLION_STR[ 2 ][ 0 ] = QObject::tr( "миллиард" );
  MILLION_STR[ 2 ][ 1 ] = QObject::tr( "миллиарда" );
  MILLION_STR[ 2 ][ 2 ] = QObject::tr( "миллиардов" );

  MILLION_STR[ 3 ][ 0 ] = QObject::tr( "триллион" );
  MILLION_STR[ 3 ][ 1 ] = QObject::tr( "триллиона" );
  MILLION_STR[ 3 ][ 2 ] = QObject::tr( "триллионов" );

  MILLION_STR[ 4 ][ 0 ] = QObject::tr( "квадриллион" );
  MILLION_STR[ 4 ][ 1 ] = QObject::tr( "квадриллиона" );
  MILLION_STR[ 4 ][ 2 ] = QObject::tr( "квадриллионов" );

  MILLION_STR[ 5 ][ 0 ] = QObject::tr( "квинтиллион" );
  MILLION_STR[ 5 ][ 1 ] = QObject::tr( "квинтиллиона" );
  MILLION_STR[ 5 ][ 2 ] = QObject::tr( "квинтиллионов" );

  MILLION_STR[ 6 ][ 0 ] = QObject::tr( "секстиллион" );
  MILLION_STR[ 6 ][ 1 ] = QObject::tr( "секстиллиона" );
  MILLION_STR[ 6 ][ 2 ] = QObject::tr( "секстиллионов" );

  MILLION_STR[ 7 ][ 0 ] = QObject::tr( "септиллион" );
  MILLION_STR[ 7 ][ 1 ] = QObject::tr( "септиллиона" );
  MILLION_STR[ 7 ][ 2 ] = QObject::tr( "септиллионов" );

  MILLION_STR[ 8 ][ 0 ] = QObject::tr( "окталлион" );
  MILLION_STR[ 8 ][ 1 ] = QObject::tr( "окталлиона" );
  MILLION_STR[ 8 ][ 2 ] = QObject::tr( "окталлионов" );

  MILLION_STR[ 9 ][ 0 ] = QObject::tr( "ноналлион" );
  MILLION_STR[ 9 ][ 1 ] = QObject::tr( "ноналлиона" );
  MILLION_STR[ 9 ][ 2 ] = QObject::tr( "ноналлионов" );

  MILLION_STR[ 10 ][ 0 ] = QObject::tr( "дециллион" );
  MILLION_STR[ 10 ][ 1 ] = QObject::tr( "дециллиона" );
  MILLION_STR[ 10 ][ 2 ] = QObject::tr( "дециллионов" );

  LIMON_STR[ 0 ] = QObject::tr( "-ллион' " );
  LIMON_STR[ 1 ] = QObject::tr( "-ллиона' " );
  LIMON_STR[ 2 ] = QObject::tr( "-ллионов' " );

  ONE_GENDER[ 0 ] = QObject::tr( "один " );
  ONE_GENDER[ 1 ] = QObject::tr( "одно " );
  ONE_GENDER[ 2 ] = QObject::tr( "одна " );

  TWO_GENDER[ 0 ] = QObject::tr( "два " );
  TWO_GENDER[ 1 ] = QObject::tr( "два " );
  TWO_GENDER[ 2 ] = QObject::tr( "две " );

  SOTKA[ 0 ] = QObject::tr( "сто " );
  SOTKA[ 1 ] = QObject::tr( "двести " );
  SOTKA[ 2 ] = QObject::tr( "триста " );
  SOTKA[ 3 ] = QObject::tr( "четыреста " );
  SOTKA[ 4 ] = QObject::tr( "пятьсот " );
  SOTKA[ 5 ] = QObject::tr( "шестьсот " );
  SOTKA[ 6 ] = QObject::tr( "семьсот " );
  SOTKA[ 7 ] = QObject::tr( "восемьсот " );
  SOTKA[ 8 ] = QObject::tr( "девятьсот " );

  TEN_STR[ 0 ] = QObject::tr( "десять " );
  TEN_STR[ 1 ] = QObject::tr( "одиннадцать " );
  TEN_STR[ 2 ] = QObject::tr( "двенадцать " );
  TEN_STR[ 3 ] = QObject::tr( "тринадцать " );
  TEN_STR[ 4 ] = QObject::tr( "четырнадцать " );
  TEN_STR[ 5 ] = QObject::tr( "пятнадцать " );
  TEN_STR[ 6 ] = QObject::tr( "шестнадцать " );
  TEN_STR[ 7 ] = QObject::tr( "семнадцать " );
  TEN_STR[ 8 ] = QObject::tr( "восемнадцать " );
  TEN_STR[ 9 ] = QObject::tr( "девятнадцать " );

  TWENTY_STR[ 0 ] = QObject::tr( "двадцать " );
  TWENTY_STR[ 1 ] = QObject::tr( "тридцать " );
  TWENTY_STR[ 2 ] = QObject::tr( "сорок " );
  TWENTY_STR[ 3 ] = QObject::tr( "пятьдесят " );
  TWENTY_STR[ 4 ] = QObject::tr( "шестьдесят " );
  TWENTY_STR[ 5 ] = QObject::tr( "семьдесят " );
  TWENTY_STR[ 6 ] = QObject::tr( "восемьдесят " );
  TWENTY_STR[ 7 ] = QObject::tr( "девяносто " );

  ONE_STR[ 0 ] = QObject::tr( "три " );
  ONE_STR[ 1 ] = QObject::tr( "четыре " );
  ONE_STR[ 2 ] = QObject::tr( "пять " );
  ONE_STR[ 3 ] = QObject::tr( "шесть " );
  ONE_STR[ 4 ] = QObject::tr( "семь " );
  ONE_STR[ 5 ] = QObject::tr( "восемь " );
  ONE_STR[ 6 ] = QObject::tr( "девять " );

  ZERO  = QObject::tr( "ноль "  );
  MINUS = QObject::tr( "минус " );

  Rubl1Init     = QObject::tr( "рубль" );
  Rubl24Init    = QObject::tr( "рубля" );
  Rubl5Init     = QObject::tr( "рублей" );
  Copeyka1Init  = QObject::tr( "копейка" );
  Copeyka24Init = QObject::tr( "копейки" );
  Copeyka5Init  = QObject::tr( "копеек" );
}

QNumberWordDiapazon QNumberWords::NumberWordDiapazon( char16_t C, bool B )
{
  if ( B ) {
    return QNumberWordDiapazon::Five;
  } else {
    switch ( C ) {
      case u'1':
        return QNumberWordDiapazon::One;
      case u'2':
      case u'3':
      case u'4':
        return QNumberWordDiapazon::TwoFourth;
      default:
        return QNumberWordDiapazon::Five;
    }
  }
}

QString QNumberWords::ThreeNumberToWords( const QString &S
                                        , int P
                                        , QGender AGender
                                        , QNumberWordDiapazon &ADiapazon )
{
  bool B = false;
  QString Result;
  Result = "";

  char16_t S0 = S[ 0 ].unicode()
         , S1 = S[ 1 ].unicode()
         , S2 = S[ 2 ].unicode();

  switch ( S0 ) {
    case u'0' :
      break; // ничего не делаем
    case u'1' :
    case u'2' :
    case u'3' :
    case u'4' :
    case u'5' :
    case u'6' :
    case u'7' :
    case u'8' :
    case u'9' :
      Result = Result + SOTKA[ S0 - 49 ];
      break;
    default :
      return QObject::tr( "Ошибочное число " ) + S;
  }

  switch ( S1 ) {
    case u'0' :
      break; // ничего не делаем
    case u'1' :
      if ( S2 >= u'0' && S2 <= u'9' ) {
        Result = Result + TEN_STR[ S2 - 48 ];
      } else {
        return QObject::tr( "Ошибочное число " ) + S;
      }
      B = true;
      break;
    case u'2' :
    case u'3' :
    case u'4' :
    case u'5' :
    case u'6' :
    case u'7' :
    case u'8' :
    case u'9' :
      Result = Result + TWENTY_STR[ S1 - 50 ];
      break;
    default :
      return QObject::tr( "Ошибочное число " ) + S;
  }

  if ( ! B ) {
    switch ( S2 ) {
      case u'0' :
        break; // ничего не делаем
      case u'1' :
        switch ( P ) {
          case 0 :
            Result = Result + ONE_GENDER[ (int)AGender ];
            break;
          case 1 :
            Result = Result + ONE_GENDER[ (int)QGender::Female ];
            break;
          default :
            Result = Result + ONE_GENDER[ (int)QGender::Male ];
        }
        break;
          case u'2' :
            switch ( P ) {
              case 0 :
                Result = Result + TWO_GENDER[ (int)AGender ];
                break;
              case 1 :
                Result = Result + TWO_GENDER[ (int)QGender::Female ];
                break;
              default :
                Result = Result + TWO_GENDER[ (int)QGender::Male ];
            }
            break;
              case u'3' :
              case u'4' :
              case u'5' :
              case u'6' :
              case u'7' :
              case u'8' :
              case u'9' :
                Result = Result + ONE_STR[ S2 - 51 ];
                break;
              default :
                return QObject::tr( "Ошибочное число " ) + S;
    }
  }

  if ( Result == "" )
    return Result;

  if ( P == 0 )
    ADiapazon  = NumberWordDiapazon( S2, B );
  else if ( ( P >= 1 ) && ( P <= H_STR ) )
    Result = Result + MILLION_STR[ P - 1 ][ (int)NumberWordDiapazon( S2, B ) ] + " ";
  else {
    QString T[3] = { "", "", "" };
    QString Lion = StringNumberToWords( QString::number( P - 1 )
    , QGender::Male
    , T
    , false );
    Lion.remove( Lion.size() - 1, 1 );
    Result = Result + "'" + Lion + LIMON_STR[ (int)NumberWordDiapazon( S2, B ) ];
  }
  return Result;
}

QString QNumberWords::StringNumberToWords( QString AStringNumber
                                         , QGender AGender
                                         , QString (&ACounted)[ 3 ]
                                         , bool FirstUpper )
{
  int I, N;
  QNumberWordDiapazon ADiapazon ;
  QString Result, Three;
  bool IsMinus = false;

  if ( !AStringNumber.isEmpty() && AStringNumber[ 0 ] == '-' ) {
    AStringNumber.remove( 0, 1 );
    IsMinus = true;
  }

  ADiapazon = QNumberWordDiapazon::Five;
  N = AStringNumber.size();

  if ( ( N % 3 ) != 0 ) {
    AStringNumber = AStringNumber.rightJustified( ( ( N / 3 ) + 1 ) * 3, '0' );
    N = AStringNumber.size();
  }

  for ( I = ( N / 3 ); I > 0; I-- ) {
    Three  = AStringNumber.mid( N - I * 3, 3 );
    Result = Result + ThreeNumberToWords( Three, I - 1, AGender, ADiapazon );
  }


  if ( Result == "" ) {
    Result = ZERO;
  }

  Result = Result + ACounted[ (int)ADiapazon ];

  if ( FirstUpper ) {
    Result.replace( 0, 1, Result[ 0 ].toUpper() );
  }

  if ( IsMinus ) {
    Result = MINUS + Result;
  }

  return Result;
}

void PrabhupadaMessage( const QString &msg
                      , const QString &title
                      , QWidget *parent )
{
  QMessageBox msgB( parent );

  if ( parent != nullptr )
    msgB.setWindowModality( Qt::WindowModal );

  if ( !title.isEmpty() ) {
    msgB.setWindowTitle( title );
  }
  msgB.setIcon( QMessageBox::Information );

  msgB.setText( msg );
  msgB.exec();
}

QString GetRetranslateFileName( const QString& ADir
                              , const QString& AFileName
                              , const QString& ALanguage )
{
  QString AFileNameTest;
  QFile F;
  AFileNameTest = ADir + ALanguage + "/" + AFileName;
  F.setFileName( AFileNameTest );
  if ( !F.exists() ) {
    AFileNameTest = ADir + AFileName;
  }
  return AFileNameTest;
}

void SetRetranslateIcon( QIcon& AIcon
                       , const QString& ADir
                       , const QString& AFileName
                       , QAction* AAction
                       , bool AIconVisibleInMenu
                       , const QString& ALanguage )
{
  QString AFileNameTest = GetRetranslateFileName( ADir, AFileName, ALanguage );
  AIcon.addFile( AFileNameTest, QSize(), QIcon::Normal, QIcon::Off );
  if ( AAction != nullptr ) {
    AAction->setIcon( AIcon );
    AAction->setIconVisibleInMenu( true );
  }
}

QClassicLog* QClassicLog::StaticLog = nullptr;

QClassicLog::QClassicLog( const QString &AFileLog )
{
  StartLog( AFileLog );
}

QClassicLog::~QClassicLog()
{
  FinishLog();
}

bool QClassicLog::StartLog( const QString &AFileLog )
{
  if ( m_SaveFile != nullptr ) {
    FinishLog();
  }
  m_SaveFile = new QSaveFile( AFileLog );
  if ( m_SaveFile->open( QIODevice::WriteOnly | QIODevice::Text ) ) {
    m_Stream = new QTextStream( m_SaveFile );
    return true;
  } else {
    return false;
  }
}

void QClassicLog::FinishLog()
{
  if ( m_SaveFile != nullptr ) {
    m_SaveFile->commit();
    delete m_Stream;
    delete m_SaveFile;
    m_Stream = nullptr;
    m_SaveFile = nullptr;
  }
}

void QClassicLog::Log( const QString &ALogString )
{
  QString S = QDateTime::currentDateTime().toString( "dd/MM/yyyy hh:mm:ss:zzz " ) + ALogString + "\n";
  *m_Stream << S;
}

void PrepareLanguageComboBox( QEmitInt& ALanguageIndex, QLanguageVector& ALanguageVector, QComboBox *CB )
{
  if ( CB ) {
    ++ALanguageIndex.m_Stop;
    CB->clear();

    for ( QLanguageVector::iterator i = ALanguageVector.begin(); i != ALanguageVector.end(); ++i ) {
      CB->addItem( ( *i )->m_LanguageSlovo );
    }
    CB->setCurrentIndex( ALanguageIndex.m_Value );
    QObject::connect( CB
#                     ifdef CS_MODE
                        , cs_mp_cast< int >( &QComboBox::currentIndexChanged )
#                     else
                        , (void (QComboBox::*)(int))(&QComboBox::currentIndexChanged)
#                     endif
                    , &ALanguageIndex
                    , &QEmitInt::SetValue );
    --ALanguageIndex.m_Stop;
  }
}
