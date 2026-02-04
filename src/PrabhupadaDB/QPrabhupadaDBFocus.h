#ifndef QPrabhupadaDBFocus_H
#define QPrabhupadaDBFocus_H

#  ifdef CS_MODE

#    if defined( QT_BUILD_PRABHUPADADB_LIB )
#      define Q_PRABHUPADADB_TEMPLATE_EXPORT    Q_DECL_EXPORT
#      define Q_PRABHUPADADB_EXPORT             Q_DECL_EXPORT
#    else
#      define Q_PRABHUPADADB_TEMPLATE_EXPORT
#      define Q_PRABHUPADADB_EXPORT             Q_DECL_IMPORT
#    endif

#    if defined(QT_BUILD_PRABHUPADADB_LIB) || defined(Q_OS_DARWIN)

#      define PRABHUPADADB_TEMPLATE_CS_OBJECT(className)                                  CS_OBJECT(className)
#      define PRABHUPADADB_TEMPLATE_CS_OBJECT_MULTIPLE(className, parentX)                CS_OBJECT_MULTIPLE(className, parentX)
#      define PRABHUPADADB_TEMPLATE_CS_GADGET(className)                                  CS_GADGET(className)

#      define PRABHUPADADB_TEMPLATE_CS_SLOT_1(access, ...)                                CS_SLOT_1(access, __VA_ARGS__)
#      define PRABHUPADADB_TEMPLATE_CS_SLOT_2(slotName)                                   CS_SLOT_2(slotName)
#      define PRABHUPADADB_TEMPLATE_CS_SLOT_OVERLOAD(slotName, argTypes)                  CS_SLOT_OVERLOAD(slotName, argTypes)
#      define PRABHUPADADB_TEMPLATE_CS_SLOT_OVERLOAD_BOOL(slotName, argTypes)             CS_SLOT_OVERLOAD_BOOL(slotName, argTypes)

#      define PRABHUPADADB_TEMPLATE_CS_SIGNAL_1(access, ...)                              CS_SIGNAL_1(access, __VA_ARGS__)
#      define PRABHUPADADB_TEMPLATE_CS_SIGNAL_2(signalName, ...)                          CS_SIGNAL_2(signalName, ## __VA_ARGS__)
#      define PRABHUPADADB_TEMPLATE_CS_SIGNAL_OVERLOAD(signalName, argTypes, ...)         CS_SIGNAL_OVERLOAD(signalName, argTypes, ## __VA_ARGS__)

#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_READ(name, method)                        CS_PROPERTY_READ(name, method)
#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_WRITE(name, method)                       CS_PROPERTY_WRITE(name, method)
#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_NOTIFY(name, method)                      CS_PROPERTY_NOTIFY(name, method)
#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_RESET(name, method)                       CS_PROPERTY_RESET(name, method)
#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_REVISION(name, data)                      CS_PROPERTY_REVISION(name, data)
#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_DESIGNABLE(name, data)                    CS_PROPERTY_DESIGNABLE(name, data)
#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_DESIGNABLE_NONSTATIC(name, data)          CS_PROPERTY_DESIGNABLE_NONSTATIC(name, data)
#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_SCRIPTABLE(name, data)                    CS_PROPERTY_SCRIPTABLE(name, data)
#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_SCRIPTABLE_NONSTATIC(name, data)          CS_PROPERTY_SCRIPTABLE_NONSTATIC(name, data)
#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_STORED(name, data)                        CS_PROPERTY_STORED(name, data)
#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_STORED_NONSTATIC(name, data)              CS_PROPERTY_STORED_NONSTATIC(name, data)
#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_USER(name, data)                          CS_PROPERTY_USER(name, data)
#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_USER_NONSTATIC(name, data)                CS_PROPERTY_USER_NONSTATIC(name, data)
#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_CONSTANT(name)                            CS_PROPERTY_CONSTANT(name)
#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_FINAL(name)                               CS_PROPERTY_FINAL(name)

#    else
#      define PRABHUPADADB_TEMPLATE_CS_OBJECT(className)                                  CS_OBJECT_OUTSIDE(className)
#      define PRABHUPADADB_TEMPLATE_CS_OBJECT_MULTIPLE(className, parentX)                CS_OBJECT_MULTIPLE_OUTSIDE(className, parentX)
#      define PRABHUPADADB_TEMPLATE_CS_GADGET(className)                                  CS_GADGET_OUTSIDE(className)

#      define PRABHUPADADB_TEMPLATE_CS_SLOT_1(access, ...)                                __VA_ARGS__;
#      define PRABHUPADADB_TEMPLATE_CS_SLOT_2(slotName)
#      define PRABHUPADADB_TEMPLATE_CS_SLOT_OVERLOAD(slotName, argTypes)
#      define PRABHUPADADB_TEMPLATE_CS_SLOT_OVERLOAD_BOOL(slotName, argTypes)

#      define PRABHUPADADB_TEMPLATE_CS_SIGNAL_1(access, ...)                              __VA_ARGS__;
#      define PRABHUPADADB_TEMPLATE_CS_SIGNAL_2(signalName, ...)
#      define PRABHUPADADB_TEMPLATE_CS_SIGNAL_OVERLOAD(signalName, argTypes, ...)

#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_READ(name, method)
#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_WRITE(name, method)
#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_NOTIFY(name, method)
#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_RESET(name, method)
#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_REVISION(name, data)
#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_DESIGNABLE(name, data)
#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_DESIGNABLE_NONSTATIC(name, data)
#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_SCRIPTABLE(name, data)
#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_SCRIPTABLE_NONSTATIC(name, data)
#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_STORED(name, data)
#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_STORED_NONSTATIC(name, data)
#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_USER(name, data)
#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_USER_NONSTATIC(name, data)
#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_CONSTANT(name)
#      define PRABHUPADADB_TEMPLATE_CS_PROPERTY_FINAL(name)
#    endif
#  else
#    if defined( NNPRABHUPADADB_LIBRARY )
#      define Q_PRABHUPADADB_TEMPLATE_EXPORT    Q_DECL_EXPORT
#      define Q_PRABHUPADADB_EXPORT             Q_DECL_EXPORT
#    else
#      define Q_PRABHUPADADB_TEMPLATE_EXPORT
#      define Q_PRABHUPADADB_EXPORT             Q_DECL_IMPORT
#    endif
#  endif

#endif
