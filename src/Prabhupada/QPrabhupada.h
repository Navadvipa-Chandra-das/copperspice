#ifndef QPrabhupada_H
#define QPrabhupada_H

#include <map>
#include <memory>
#include <QPrabhupadaFocus.h>

#ifdef CS_MODE

#include <QtCore>
#include <QtGui>

#else

#include <QDialog>
#include <QComboBox>
#include <QSaveFile>
#include <QFile>
#include <QBuffer>
#include <QMainWindow>
#include <QMessageBox>
#include <QDateTime>

#endif

class Q_PRABHUPADA_EXPORT QClassicLog
{
  public:
    QClassicLog() = delete;
    QClassicLog( const QString &AFileLog );
    QClassicLog( QClassicLog&& A ) = delete;
    QClassicLog( const QClassicLog& A ) = delete;
    QClassicLog& operator = ( const QClassicLog& A ) = delete;
    QClassicLog& operator = ( QClassicLog&& A ) = delete;
    ~QClassicLog();
    QSaveFile *m_SaveFile = nullptr;
    QTextStream *m_Stream = nullptr;
    bool StartLog( const QString &AFileLog );
    void FinishLog();
    void Log( const QString &ALogString );
    static QClassicLog* StaticLog;
};

#ifdef PRABHUPADA_LOG
  #define PrabhupadaLog( S )       QClassicLog::StaticLog->Log( S )
#else
  #define PrabhupadaLog( S )
#endif

#ifdef CS_MODE

template < class TValueType >
class Q_PRABHUPADA_TEMPLATE_EXPORT QEmitValue : public QObject
{
  CS_OBJECT( QEmitValue )
  public:
    using inherited = QObject;
    TValueType m_Value;
    // m_NeedMainWork flag can be used so that the slot is triggered only when the m_Value is actually changed,
    // and with additional calls to the EmitМalueChanged( false ) method, the slot might not perform a long-term action.
    // But m_NeedMainWork can only be used in one slot or not at all.
    // If you need similar functionality for multiple slots, then you need to redo the m_NeedMainWork field to the set< int> type, for example.
    bool m_NeedMainWork = true;
    int m_Stop = 0;
    QEmitValue( QObject *parent = nullptr )
        : inherited( parent )
    {
    };
    QEmitValue( const QEmitValue& A )
        : inherited( A.parent() )
        , m_Value       ( A.m_Value )
        , m_NeedMainWork( A.m_NeedMainWork )
        , m_Stop        ( A.m_Stop )
    {
    };
    QEmitValue( QEmitValue&& A )
        : inherited( A.parent() )
        , m_Value       ( std::move( A.m_Value ) )
        , m_NeedMainWork( std::move( A.m_NeedMainWork ) )
        , m_Stop        ( std::move( A.m_Stop ) )
    {
    };
    QEmitValue& operator = ( const QEmitValue& A )
    {
        m_Value        = A.m_Value;
        m_NeedMainWork = A.m_NeedMainWork;
        m_Stop         = A.m_Stop;

        return *this;
    };
    QEmitValue& operator = ( QEmitValue&& A )
    {
        m_Value        = std::move( A.m_Value );
        m_NeedMainWork = std::move( A.m_NeedMainWork );
        m_Stop         = std::move( A.m_Stop );

        return *this;
    };
    bool operator == ( const QEmitValue& A )
    {
        return m_Value        == A.m_Value &&
               m_NeedMainWork == A.m_NeedMainWork &&
               m_Stop         == A.m_Stop;
    };
    bool operator != ( const QEmitValue& A )
    {
        return m_Value        != A.m_Value ||
               m_NeedMainWork != A.m_NeedMainWork ||
               m_Stop         != A.m_Stop;
    };
    QEmitValue( TValueType Value, QObject *parent = nullptr )
        : inherited( parent )
        , m_Value( Value ) {};
    virtual ~QEmitValue() {};

    CS_SIGNAL_1( Public, void SignalValueChanged( const TValueType& Value ) )
    CS_SIGNAL_2( SignalValueChanged, Value )

    void SetValue( TValueType Value )
    {
        if ( m_Stop == 0 && m_Value != Value ) {
            m_Value = Value;
            m_NeedMainWork = true;
            ValueChanged();
            emit SignalValueChanged( m_Value );
        }
    };
    virtual void ValueChanged() {};
    virtual void LoadFromStream( QDataStream &ST )
    {
        TValueType N;
        ST >> N;
        SetValue( N );
    };
    virtual void SaveToStream( QDataStream &ST )
    {
        ST << m_Value;
    };
    inline void EmitValueChanged( bool ANeedResetMainWork = false )
    {
        if ( ANeedResetMainWork ) {
            m_NeedMainWork = true;
        }
        emit SignalValueChanged( m_Value );
        m_NeedMainWork = false;
    };
};

#endif

#ifdef CS_MODE

using QEmitInt    = QEmitValue< int >;
using QEmitBool   = QEmitValue< bool >;
using QEmitString = QEmitValue< QString >;

#else

class Q_PRABHUPADA_EXPORT QEmitInt : public QObject
{
# ifdef CS_MODE
    CS_OBJECT( QEmitValue )
# else
    Q_OBJECT
# endif
  public:
    using inherited = QObject;
    int m_Value;
    bool m_NeedMainWork = true;
    int m_Stop = 0;
    QEmitInt( QObject *parent = nullptr )
        : inherited( parent )
    {
    };
    QEmitInt( const QEmitInt& A )
        : inherited( A.parent() )
        , m_Value       ( A.m_Value )
        , m_NeedMainWork( A.m_NeedMainWork )
        , m_Stop        ( A.m_Stop )
    {
    };
    QEmitInt( QEmitInt&& A )
        : inherited( A.parent() )
        , m_Value       ( std::move( A.m_Value ) )
        , m_NeedMainWork( std::move( A.m_NeedMainWork ) )
        , m_Stop        ( std::move( A.m_Stop ) )
    {
    };
    QEmitInt& operator = ( const QEmitInt& A )
    {
        m_Value        = A.m_Value;
        m_NeedMainWork = A.m_NeedMainWork;
        m_Stop         = A.m_Stop;

        return *this;
    };
    QEmitInt& operator = ( QEmitInt&& A )
    {
        m_Value        = std::move( A.m_Value );
        m_NeedMainWork = std::move( A.m_NeedMainWork );
        m_Stop         = std::move( A.m_Stop );

        return *this;
    };
    bool operator == ( const QEmitInt& A )
    {
        return m_Value        == A.m_Value &&
               m_NeedMainWork == A.m_NeedMainWork &&
               m_Stop         == A.m_Stop;
    };
    bool operator != ( const QEmitInt& A )
    {
        return m_Value        != A.m_Value ||
               m_NeedMainWork != A.m_NeedMainWork ||
               m_Stop         != A.m_Stop;
    };
    QEmitInt( int Value, QObject *parent = nullptr )
        : inherited( parent )
        , m_Value( Value ) {};
    virtual ~QEmitInt() {};

# ifdef CS_MODE
    CS_SIGNAL_1( Public, void SignalValueChanged( const int& Value ) )
    CS_SIGNAL_2( SignalValueChanged, Value )
# else
    signals:
      void SignalValueChanged( const int& Value );
    public:
# endif

    void SetValue( int Value )
    {
        if ( m_Stop == 0 && m_Value != Value ) {
            m_Value = Value;
            m_NeedMainWork = true;
            ValueChanged();
            emit SignalValueChanged( m_Value );
        }
    };
    virtual void ValueChanged() {};
    virtual void LoadFromStream( QDataStream &ST )
    {
        int N;
        ST >> N;
        SetValue( N );
    };
    virtual void SaveToStream( QDataStream &ST )
    {
        ST << m_Value;
    };
    inline void EmitValueChanged( bool ANeedResetMainWork = false )
    {
        if ( ANeedResetMainWork ) {
            m_NeedMainWork = true;
        }
        emit SignalValueChanged( m_Value );
        m_NeedMainWork = false;
    };
};

class Q_PRABHUPADA_EXPORT QEmitBool : public QObject
{
# ifdef CS_MODE
    CS_OBJECT( QEmitValue )
# else
    Q_OBJECT
# endif
  public:
    using inherited = QObject;
    bool m_Value;
    bool m_NeedMainWork = true;
    int m_Stop = 0;
    QEmitBool( QObject *parent = nullptr )
        : inherited( parent )
    {
    };
    QEmitBool( const QEmitBool& A )
        : inherited( A.parent() )
        , m_Value       ( A.m_Value )
        , m_NeedMainWork( A.m_NeedMainWork )
        , m_Stop        ( A.m_Stop )
    {
    };
    QEmitBool( QEmitBool&& A )
        : inherited( A.parent() )
        , m_Value       ( std::move( A.m_Value ) )
        , m_NeedMainWork( std::move( A.m_NeedMainWork ) )
        , m_Stop        ( std::move( A.m_Stop ) )
    {
    };
    QEmitBool& operator = ( const QEmitBool& A )
    {
        m_Value        = A.m_Value;
        m_NeedMainWork = A.m_NeedMainWork;
        m_Stop         = A.m_Stop;

        return *this;
    };
    QEmitBool& operator = ( QEmitBool&& A )
    {
        m_Value        = std::move( A.m_Value );
        m_NeedMainWork = std::move( A.m_NeedMainWork );
        m_Stop         = std::move( A.m_Stop );

        return *this;
    };
    bool operator == ( const QEmitBool& A )
    {
        return m_Value        == A.m_Value &&
               m_NeedMainWork == A.m_NeedMainWork &&
               m_Stop         == A.m_Stop;
    };
    bool operator != ( const QEmitBool& A )
    {
        return m_Value        != A.m_Value ||
               m_NeedMainWork != A.m_NeedMainWork ||
               m_Stop         != A.m_Stop;
    };
    QEmitBool( bool Value, QObject *parent = nullptr )
        : inherited( parent )
        , m_Value( Value ) {};
    virtual ~QEmitBool() {};

# ifdef CS_MODE
    CS_SIGNAL_1( Public, void SignalValueChanged( const bool& Value ) )
    CS_SIGNAL_2( SignalValueChanged, Value )
# else
    signals:
      void SignalValueChanged( const bool& Value );
    public:
# endif
    void SetValue( bool Value )
    {
        if ( m_Stop == 0 && m_Value != Value ) {
            m_Value = Value;
            m_NeedMainWork = true;
            ValueChanged();
            emit SignalValueChanged( m_Value );
        }
    };
    virtual void ValueChanged() {};
    virtual void LoadFromStream( QDataStream &ST )
    {
        bool N;
        ST >> N;
        SetValue( N );
    };
    virtual void SaveToStream( QDataStream &ST )
    {
        ST << m_Value;
    };
    inline void EmitValueChanged( bool ANeedResetMainWork = false )
    {
        if ( ANeedResetMainWork ) {
            m_NeedMainWork = true;
        }
        emit SignalValueChanged( m_Value );
        m_NeedMainWork = false;
    };
};

class Q_PRABHUPADA_TEMPLATE_EXPORT QEmitString : public QObject
{
# ifdef CS_MODE
    CS_OBJECT( QEmitValue )
# else
    Q_OBJECT
# endif
  public:
    using inherited = QObject;
    QString m_Value;
    bool m_NeedMainWork = true;
    int m_Stop = 0;
    QEmitString( QObject *parent = nullptr )
        : inherited( parent )
    {
    };
    QEmitString( const QEmitString& A )
        : inherited( A.parent() )
        , m_Value       ( A.m_Value )
        , m_NeedMainWork( A.m_NeedMainWork )
        , m_Stop        ( A.m_Stop )
    {
    };
    QEmitString( QEmitString&& A )
        : inherited( A.parent() )
        , m_Value       ( std::move( A.m_Value ) )
        , m_NeedMainWork( std::move( A.m_NeedMainWork ) )
        , m_Stop        ( std::move( A.m_Stop ) )
    {
    };
    QEmitString& operator = ( const QEmitString& A )
    {
        m_Value        = A.m_Value;
        m_NeedMainWork = A.m_NeedMainWork;
        m_Stop         = A.m_Stop;

        return *this;
    };
    QEmitString& operator = ( QEmitString&& A )
    {
        m_Value        = std::move( A.m_Value );
        m_NeedMainWork = std::move( A.m_NeedMainWork );
        m_Stop         = std::move( A.m_Stop );

        return *this;
    };
    bool operator == ( const QEmitString& A )
    {
        return m_Value        == A.m_Value &&
               m_NeedMainWork == A.m_NeedMainWork &&
               m_Stop         == A.m_Stop;
    };
    bool operator != ( const QEmitString& A )
    {
        return m_Value        != A.m_Value ||
               m_NeedMainWork != A.m_NeedMainWork ||
               m_Stop         != A.m_Stop;
    };
    QEmitString( QString Value, QObject *parent = nullptr )
        : inherited( parent )
        , m_Value( Value ) {};
    virtual ~QEmitString() {};

# ifdef CS_MODE
    CS_SIGNAL_1( Public, void SignalValueChanged( const QString& Value ) )
    CS_SIGNAL_2( SignalValueChanged, Value )
# else
    signals:
      void SignalValueChanged( const QString& Value );
    public:
# endif
    void SetValue( QString Value )
    {
        if ( m_Stop == 0 && m_Value != Value ) {
            m_Value = Value;
            m_NeedMainWork = true;
            ValueChanged();
            emit SignalValueChanged( m_Value );
        }
    };
    virtual void ValueChanged() {};
    virtual void LoadFromStream( QDataStream &ST )
    {
        QString N;
        ST >> N;
        SetValue( N );
    };
    virtual void SaveToStream( QDataStream &ST )
    {
        ST << m_Value;
    };
    inline void EmitValueChanged( bool ANeedResetMainWork = false )
    {
        if ( ANeedResetMainWork ) {
            m_NeedMainWork = true;
        }
        emit SignalValueChanged( m_Value );
        m_NeedMainWork = false;
    };
};

#endif

template< class TMap, class TKey, class TValue >
void LoadMap( TMap &MP, QDataStream &ST )
{
  // 1
  std::size_t L;
  ST >> L;
  // 2
  TKey AKey;
  TValue AValue;
  for ( std::size_t I = 0; I < L; ++I ) {
    ST >> AKey;
    ST >> AValue;
    MP[ AKey ] = AValue;
  }
}

template< class TMap >
void SaveMap( TMap &MP, QDataStream &ST )
{
  // 1
  ST << MP.size();
  // 2
  for ( typename TMap::iterator I = MP.begin(); I != MP.end(); ++I ) {
    ST << (*I).first;
    ST << (*I).second;
  }
}

template< class TVector, class TValue >
void LoadVector( TVector &VC, QDataStream &ST )
{
  // 1
  std::size_t L;
  ST >> L;
  // 2
  TValue AValue;
  for ( std::size_t I = 0; I < L; ++I ) {
    ST >> AValue;
    VC.push_back( AValue );
  }
}

template< class TVector >
void SaveVector( TVector &VC, QDataStream &ST )
{
  // 1
  ST << VC.size();
  // 2
  for ( typename TVector::iterator I = VC.begin(); I != VC.end(); ++I ) {
    ST << (*I);
  }
}

enum class QStorageKind : quint8 { File, DB, Memory };

class Q_PRABHUPADA_EXPORT QMapMemoryStorage : public std::map< QString, std::shared_ptr< QDataStream > >
{
  private:
    using inherited = std::map< QString, std::shared_ptr< QDataStream > >;
  public:
    QMapMemoryStorage();
    QMapMemoryStorage( QMapMemoryStorage&& A );
    QMapMemoryStorage( const QMapMemoryStorage& A );
    QMapMemoryStorage& operator = ( const QMapMemoryStorage& A );
    QMapMemoryStorage& operator = ( QMapMemoryStorage&& A );
    ~QMapMemoryStorage();
    void LoadFromStream( QDataStream &ST );
    void SaveToStream(   QDataStream &ST );
};

class Q_PRABHUPADA_EXPORT QStorager
{
  public:
    QStorager();
    virtual ~QStorager();
    virtual void LoadFromStream( void *AObject, QDataStream &ST ) = 0;
    virtual void SaveToStream(   void *AObject, QDataStream &ST ) = 0;
};

class Q_PRABHUPADA_EXPORT QStorage
{
  protected:
    QFile *m_File = nullptr;
    QSaveFile *m_SaveFile = nullptr;
    QDataStream *m_Stream = nullptr;
    QString m_FileName;
    QByteArray *m_ByteArray;
    QMapMemoryStorage m_MapMemoryStorage;
    void PrepareFileName( void* O, QStorageKind AStorageKind, const QString& AKeyStorage, bool AUseUserName );
    bool BeginLoad( QStorageKind AStorageKind );
    void EndLoad( QStorageKind AStorageKind );
    void BeginSave( QStorageKind AStorageKind );
    void EndSave( QStorageKind AStorageKind );
    virtual bool BeginLoadDB() { return false; };
    virtual void EndLoadDB() {};
    virtual void BeginSaveDB() {};
    virtual void EndSaveDB() {};
  public:
    QStorage();
    QStorage( QStorage&& A );
    QStorage( const QStorage& A );
    virtual ~QStorage();
    QStorage& operator = ( const QStorage& A );
    QStorage& operator = ( QStorage&& A );

    bool m_Enabled = true;
    qint8 m_Version = 0;
    bool m_IsPostgreSQL = false;

    void setEnabled( bool Value );
    virtual QString PrefixKeyStorage( bool AUseUserName ) { return ""; };
    QString KeyStorage( QObject *O, QStorageKind AStorageKind, bool AUseUserName );
    virtual void ResetSettings() {};

    bool LoadObject( void* O, QStorageKind AStorageKind, QStorager* ST, const QString& AKeyStorage = "", bool AUseUserName = true );
    void SaveObject( void* O, QStorageKind AStorageKind, QStorager* ST, const QString& AKeyStorage = "", bool AUseUserName = true );
    void RemoveMemory( void* O, const QString& AKeyStorage = "" );
    void ClearMemory() { m_MapMemoryStorage.clear(); };

    void LoadFromStream( QDataStream &ST );
    void SaveToStream( QDataStream &ST );
};

template < class TStoragerType >
bool LoadObject( void* O, QStorage* AStorage, QStorageKind AStorageKind, const QString& AKeyStorage = "", bool AUseUserName = true )
{
  TStoragerType* AStorager = new TStoragerType();
  bool B = AStorage->LoadObject( O, AStorageKind, AStorager, AKeyStorage, AUseUserName );
  delete AStorager;
  return B;
}

template < class TStoragerType >
void SaveObject( void* O, QStorage* AStorage, QStorageKind AStorageKind, const QString& AKeyStorage = "", bool AUseUserName = true )
{
  TStoragerType* AStorager = new TStoragerType();
  AStorage->SaveObject( O, AStorageKind, AStorager, AKeyStorage, AUseUserName );
  delete AStorager;
}

template < class TStoragerType >
void StoragerDialogExec( QDialog* ADialog, QStorage* AStorage, QStorageKind AStorageKind, const QString& AKeyStorage = "", bool AUseUserName = true )
{
  TStoragerType* AStorager = new TStoragerType();

  AStorage->LoadObject( ADialog, AStorageKind, AStorager, AKeyStorage, AUseUserName );
  ADialog->exec();
  AStorage->SaveObject( ADialog, AStorageKind, AStorager, AKeyStorage, AUseUserName );

  delete AStorager;
}

extern Q_PRABHUPADA_EXPORT void PrepareHistoryComboBox( QComboBox *CB, int MaxCount = 30 );
extern Q_PRABHUPADA_EXPORT void LoadFromStreamComboBox( QComboBox *CB, QDataStream &ST );
extern Q_PRABHUPADA_EXPORT void SaveToStreamComboBox( QComboBox *CB, QDataStream &ST );
extern Q_PRABHUPADA_EXPORT void SaveToStreamPrepareHistory( QComboBox *CB, QDataStream &ST, int HistoryCount );

class Q_PRABHUPADA_EXPORT QStoragerMainWindow : public QStorager
{
  public:
    QStoragerMainWindow();
    virtual ~QStoragerMainWindow();
  private:
    using inherited = QStorager;
  public:
    virtual void LoadFromStream( void *AObject, QDataStream &ST );
    virtual void SaveToStream(   void *AObject, QDataStream &ST );
};

class Q_PRABHUPADA_EXPORT QStoragerDialog : public QStorager
{
  public:
    QStoragerDialog();
    virtual ~QStoragerDialog();
  private:
    using inherited = QStorager;
  public:
    virtual void LoadFromStream( void *AObject, QDataStream &ST );
    virtual void SaveToStream(   void *AObject, QDataStream &ST );
};

class Q_PRABHUPADA_EXPORT QLanguageInfo
{
  public:
    QLanguageInfo();
    QLanguageInfo( const QLanguageInfo& A );
    QLanguageInfo( QLanguageInfo&& A );
    QLanguageInfo& operator = ( const QLanguageInfo& A );
    QLanguageInfo& operator = ( QLanguageInfo&& A );
    bool operator == ( const QLanguageInfo& A )
    {
      return m_ID == A.m_ID;
    }
    bool operator != ( const QLanguageInfo& A )
    {
      return m_ID != A.m_ID;
    }
    ~QLanguageInfo();

    int m_ID;
    QString m_Language;
    QString m_LanguageSlovo;
    virtual void LoadFromStream( QDataStream& ST );
    virtual void SaveToStream( QDataStream& ST );
};

extern Q_PRABHUPADA_EXPORT inline bool operator == ( const QLanguageInfo& A, const QLanguageInfo& B )
{
  return A.m_ID == B.m_ID;
}

extern Q_PRABHUPADA_EXPORT inline bool operator != ( const QLanguageInfo& A, const QLanguageInfo& B )
{
  return A.m_ID != B.m_ID;
}

class Q_PRABHUPADA_EXPORT QLanguageVector : public std::vector< QLanguageInfo* >
{
  private:
    using inherited = std::vector< QLanguageInfo* >;
  public:
    QLanguageVector();
    QLanguageVector( const QLanguageVector& A );
    QLanguageVector( QLanguageVector&& A );
    QLanguageVector& operator = ( const QLanguageVector& A );
    QLanguageVector& operator = ( QLanguageVector&& A );
    virtual ~QLanguageVector();
    static const int RussianIndex = 4;
    bool m_LoadSuccess = false;
    void Clear();
    bool FindLanguage( const QString &S, std::size_t& AResultIndex );
    virtual void LoadFromStream( QDataStream &ST );
    virtual void SaveToStream( QDataStream &ST );
    virtual QLanguageInfo* NewLanguageInfo() { return new QLanguageInfo(); };
};

class Q_PRABHUPADA_EXPORT QStoragerLanguageVector : public QStorager
{
  public:
    QStoragerLanguageVector();
    virtual ~QStoragerLanguageVector();
  private:
    using inherited = QStorager;
  public:
    virtual void LoadFromStream( void *AObject, QDataStream &ST );
    virtual void SaveToStream(   void *AObject, QDataStream &ST );
};

class Q_PRABHUPADA_EXPORT QLike
{
  public:
    QLike() = default;
    ~QLike() = default;
    QLike( QLike&& A ) = default;
    QLike( const QLike& A ) = default;
    QLike& operator = ( const QLike& A ) = default;
    QLike& operator = ( QLike&& A ) = default;
    static const QChar CharPercent;
    static const QChar CharUnderline;
    static bool Like( QString::iterator s, QString::iterator s_end, QString::iterator t, QString::iterator t_end );
};

enum class QGender : int
{
  Male
, Avg
, Female
};

enum class QNumberWordDiapazon : int
{
  One
, TwoFourth
, Five
};

enum class QCopeykaKind : int
{
  Word
, Number
};

class Q_PRABHUPADA_EXPORT QNumberWords : public QEmitString
{
  private:
    using inherited = QEmitString;
  public:
    QNumberWords();
    virtual ~QNumberWords();
    QNumberWords( QNumberWords&& A );
    QNumberWords( const QNumberWords& A );
    QNumberWords& operator = ( const QNumberWords& A );
    QNumberWords& operator = ( QNumberWords&& A );

    static const int H_STR = 11;
    static QString MILLION_STR[ H_STR ][ 3 ];

    static QString LIMON_STR[ 3 ];
    static QString ONE_GENDER[ 3 ];
    static QString TWO_GENDER[ 3 ];
    static QString SOTKA[ 9 ];
    static QString TEN_STR[ 10 ];
    static QString TWENTY_STR[ 8 ];
    static QString ONE_STR[ 7 ];
    static QString ZERO;
    static QString MINUS;

    static QString Rubl1Init;
    static QString Rubl24Init;
    static QString Rubl5Init;
    static QString Copeyka1Init;
    static QString Copeyka24Init;
    static QString Copeyka5Init;

    QString m_NumberWords;
    QString m_Rubl1     = Rubl1Init;
    QString m_Rubl24    = Rubl24Init;
    QString m_Rubl5     = Rubl5Init;
    QString m_Copeyka1  = Copeyka1Init;
    QString m_Copeyka24 = Copeyka24Init;
    QString m_Copeyka5  = Copeyka5Init;

    QGender m_RublGender    = QGender::Male;
    QGender m_CopeycaGender = QGender::Female;

    int m_LenCopeyka = 2;
    bool m_NeedCopeyka = true;
    QCopeykaKind m_CopyicaKind = QCopeykaKind::Number;
    bool m_NeedFirstUpper = true;

    inline void RefreshNumberWords() { GenerateNumberWords(); };
    void GenerateNumberWords();
    virtual void ValueChanged();
    void ReInitRublCopeyka();
    static void RetranslateStrings();
    static QNumberWordDiapazon NumberWordDiapazon( char16_t C, bool B );
    static QString ThreeNumberToWords( const QString &S
                              , int P
                              , QGender AGender
                              , QNumberWordDiapazon &ADiapazon );
    static QString StringNumberToWords( QString AStringNumber
                               , QGender AGender
                               , QString (&ACounted)[3]
                               , bool FirstUpper );
};

extern Q_PRABHUPADA_EXPORT void PrabhupadaMessage( const QString &msg
                                                 , const QString &title = ""
                                                 , QWidget *parent = nullptr );
extern Q_PRABHUPADA_EXPORT QString GetRetranslateFileName( const QString& ADir
                                                         , const QString& AFileName
                                                         , const QString& ALanguage );
extern Q_PRABHUPADA_EXPORT void SetRetranslateIcon( QIcon& AIcon
                                                  , const QString& ADir
                                                  , const QString& AFileName
                                                  , QAction* AAction
                                                  , bool AIconVisibleInMenu
                                                  , const QString& ALanguage );
extern Q_PRABHUPADA_EXPORT void PrepareLanguageComboBox( QEmitInt& ALanguageIndex, QLanguageVector& ALanguageVector, QComboBox *CB );


#endif
