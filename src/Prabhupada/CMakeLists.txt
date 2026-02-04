add_library(NNPrabhupada SHARED "")
add_library(CopperSpice::NNPrabhupada ALIAS NNPrabhupada)

set_target_properties(NNPrabhupada PROPERTIES OUTPUT_NAME NNPrabhupada${BUILD_ABI})

add_definitions(
   -DQT_BUILD_PRABHUPADA_LIB
   -DCS_MODE
)

set(EXTRA_PRABHUPADA_CXXFLAGS)
set(EXTRA_PRABHUPADA_LDFLAGS)

target_include_directories(NNPrabhupada
   PUBLIC
   $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include/QtPrabhupada>
   $<INSTALL_INTERFACE:include/QtPrabhupada>
)

include_directories(
   ${CMAKE_BINARY_DIR}/privateinclude/QtCore/private
   ${CMAKE_BINARY_DIR}/privateinclude/QtGui/private
   ${CMAKE_BINARY_DIR}/privateinclude/QtPrabhupada/private

   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_SOURCE_DIR}/src/3rdparty/harfbuzz/src
)

set(PRABHUPADA_PUBLIC_INCLUDES
   QPrabhupada
   QtPrabhupada
)

list(APPEND PRABHUPADA_INCLUDES
   ${CMAKE_CURRENT_SOURCE_DIR}/QPrabhupada.h
   ${CMAKE_CURRENT_SOURCE_DIR}/QtPrabhupada.h
   ${CMAKE_CURRENT_SOURCE_DIR}/QPrabhupadaFocus.h
)

#list(APPEND PRABHUPADA_PRIVATE_INCLUDES
#   ${CMAKE_CURRENT_SOURCE_DIR}/QPrabhupada.h
#   ${CMAKE_CURRENT_SOURCE_DIR}/QtPrabhupada.h
#)

target_sources(NNPrabhupada
   PRIVATE
   ${CMAKE_CURRENT_SOURCE_DIR}/QPrabhupada.cpp
)

macro_generate_public("${PRABHUPADA_PUBLIC_INCLUDES}"   QtPrabhupada)
macro_generate_private("${PRABHUPADA_PRIVATE_INCLUDES}" QtPrabhupada)
macro_generate_misc("${PRABHUPADA_INCLUDES}" QtPrabhupada)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
   target_link_libraries(NNPrabhupada
      PRIVATE
      ws2_32
   )
endif()

function_variable_fixup("${EXTRA_PRABHUPADA_CXXFLAGS}" EXTRA_PRABHUPADA_CXXFLAGS)
function_variable_fixup("${EXTRA_PRABHUPADA_LDFLAGS}"  EXTRA_PRABHUPADA_LDFLAGS)

target_sources(NNPrabhupada
   PRIVATE
   ${PRABHUPADA_INCLUDES}
)

target_link_libraries(NNPrabhupada
   PRIVATE
   CsCore
   CsGui
)

target_compile_options(NNPrabhupada
   PRIVATE
   $<$<COMPILE_LANGUAGE:CXX>:SHELL:${EXTRA_PRABHUPADA_CXXFLAGS}>
)

set_target_properties(NNPrabhupada
   PROPERTIES
   LINK_FLAGS ${EXTRA_PRABHUPADA_LDFLAGS}
)

function_generate_resources(NNPrabhupada)

macro_generate_package(
   NNPrabhupada
   QtPrabhupada
  "${EXTRA_PRABHUPADA_CXXFLAGS}"
   "CsCore CsGui"
)

if(CMAKE_SYSTEM_NAME MATCHES "(Linux|OpenBSD|FreeBSD|NetBSD|DragonFly)")
   add_definitions(
      -DHAVE_POSIX_MEMALIGN
   )
endif()

install(
   TARGETS NNPrabhupada
   EXPORT CopperSpiceLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
   RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
   DIRECTORY ${CMAKE_BINARY_DIR}/include/QtPrabhupada
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
   COMPONENT Devel
)
