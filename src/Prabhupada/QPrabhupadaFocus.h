#ifndef QPrabhupadaFocus_H
#define QPrabhupadaFocus_H

#  ifdef CS_MODE

#    if defined( QT_BUILD_PRABHUPADA_LIB )
#      define Q_PRABHUPADA_TEMPLATE_EXPORT    Q_DECL_EXPORT
#      define Q_PRABHUPADA_EXPORT             Q_DECL_EXPORT
#    else
#      define Q_PRABHUPADA_TEMPLATE_EXPORT
#      define Q_PRABHUPADA_EXPORT             Q_DECL_IMPORT
#    endif

#    if defined(QT_BUILD_PRABHUPADA_LIB) || defined(Q_OS_DARWIN)

#      define PRABHUPADA_TEMPLATE_CS_OBJECT(className)                                  CS_OBJECT(className)
#      define PRABHUPADA_TEMPLATE_CS_OBJECT_MULTIPLE(className, parentX)                CS_OBJECT_MULTIPLE(className, parentX)
#      define PRABHUPADA_TEMPLATE_CS_GADGET(className)                                  CS_GADGET(className)

#      define PRABHUPADA_TEMPLATE_CS_SLOT_1(access, ...)                                CS_SLOT_1(access, __VA_ARGS__)
#      define PRABHUPADA_TEMPLATE_CS_SLOT_2(slotName)                                   CS_SLOT_2(slotName)
#      define PRABHUPADA_TEMPLATE_CS_SLOT_OVERLOAD(slotName, argTypes)                  CS_SLOT_OVERLOAD(slotName, argTypes)
#      define PRABHUPADA_TEMPLATE_CS_SLOT_OVERLOAD_BOOL(slotName, argTypes)             CS_SLOT_OVERLOAD_BOOL(slotName, argTypes)

#      define PRABHUPADA_TEMPLATE_CS_SIGNAL_1(access, ...)                              CS_SIGNAL_1(access, __VA_ARGS__)
#      define PRABHUPADA_TEMPLATE_CS_SIGNAL_2(signalName, ...)                          CS_SIGNAL_2(signalName, ## __VA_ARGS__)
#      define PRABHUPADA_TEMPLATE_CS_SIGNAL_OVERLOAD(signalName, argTypes, ...)         CS_SIGNAL_OVERLOAD(signalName, argTypes, ## __VA_ARGS__)

#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_READ(name, method)                        CS_PROPERTY_READ(name, method)
#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_WRITE(name, method)                       CS_PROPERTY_WRITE(name, method)
#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_NOTIFY(name, method)                      CS_PROPERTY_NOTIFY(name, method)
#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_RESET(name, method)                       CS_PROPERTY_RESET(name, method)
#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_REVISION(name, data)                      CS_PROPERTY_REVISION(name, data)
#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_DESIGNABLE(name, data)                    CS_PROPERTY_DESIGNABLE(name, data)
#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_DESIGNABLE_NONSTATIC(name, data)          CS_PROPERTY_DESIGNABLE_NONSTATIC(name, data)
#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_SCRIPTABLE(name, data)                    CS_PROPERTY_SCRIPTABLE(name, data)
#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_SCRIPTABLE_NONSTATIC(name, data)          CS_PROPERTY_SCRIPTABLE_NONSTATIC(name, data)
#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_STORED(name, data)                        CS_PROPERTY_STORED(name, data)
#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_STORED_NONSTATIC(name, data)              CS_PROPERTY_STORED_NONSTATIC(name, data)
#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_USER(name, data)                          CS_PROPERTY_USER(name, data)
#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_USER_NONSTATIC(name, data)                CS_PROPERTY_USER_NONSTATIC(name, data)
#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_CONSTANT(name)                            CS_PROPERTY_CONSTANT(name)
#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_FINAL(name)                               CS_PROPERTY_FINAL(name)

#    else
#      define PRABHUPADA_TEMPLATE_CS_OBJECT(className)                                  CS_OBJECT_OUTSIDE(className)
#      define PRABHUPADA_TEMPLATE_CS_OBJECT_MULTIPLE(className, parentX)                CS_OBJECT_MULTIPLE_OUTSIDE(className, parentX)
#      define PRABHUPADA_TEMPLATE_CS_GADGET(className)                                  CS_GADGET_OUTSIDE(className)

#      define PRABHUPADA_TEMPLATE_CS_SLOT_1(access, ...)                                __VA_ARGS__;
#      define PRABHUPADA_TEMPLATE_CS_SLOT_2(slotName)
#      define PRABHUPADA_TEMPLATE_CS_SLOT_OVERLOAD(slotName, argTypes)
#      define PRABHUPADA_TEMPLATE_CS_SLOT_OVERLOAD_BOOL(slotName, argTypes)

#      define PRABHUPADA_TEMPLATE_CS_SIGNAL_1(access, ...)                              __VA_ARGS__;
#      define PRABHUPADA_TEMPLATE_CS_SIGNAL_2(signalName, ...)
#      define PRABHUPADA_TEMPLATE_CS_SIGNAL_OVERLOAD(signalName, argTypes, ...)

#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_READ(name, method)
#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_WRITE(name, method)
#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_NOTIFY(name, method)
#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_RESET(name, method)
#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_REVISION(name, data)
#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_DESIGNABLE(name, data)
#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_DESIGNABLE_NONSTATIC(name, data)
#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_SCRIPTABLE(name, data)
#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_SCRIPTABLE_NONSTATIC(name, data)
#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_STORED(name, data)
#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_STORED_NONSTATIC(name, data)
#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_USER(name, data)
#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_USER_NONSTATIC(name, data)
#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_CONSTANT(name)
#      define PRABHUPADA_TEMPLATE_CS_PROPERTY_FINAL(name)
#    endif
#  else
#    if defined( NNPRABHUPADA_LIBRARY )
#      define Q_PRABHUPADA_TEMPLATE_EXPORT    Q_DECL_EXPORT
#      define Q_PRABHUPADA_EXPORT             Q_DECL_EXPORT
#    else
#      define Q_PRABHUPADA_TEMPLATE_EXPORT
#      define Q_PRABHUPADA_EXPORT             Q_DECL_IMPORT
#    endif
#  endif

#endif
