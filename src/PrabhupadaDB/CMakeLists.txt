add_library(NNPrabhupadaDB SHARED "")
add_library(CopperSpice::NNPrabhupadaDB ALIAS NNPrabhupadaDB)

set_target_properties(NNPrabhupadaDB PROPERTIES OUTPUT_NAME NNPrabhupadaDB${BUILD_ABI})

add_definitions(
   -DQT_BUILD_PRABHUPADADB_LIB
)

set(EXTRA_PRABHUPADADB_CXXFLAGS)
set(EXTRA_PRABHUPADADB_LDFLAGS)

target_include_directories(NNPrabhupadaDB
   PUBLIC
   $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include/QtPrabhupadaDB>
   $<INSTALL_INTERFACE:include/QtPrabhupadaDB>
)

include_directories(
   ${CMAKE_BINARY_DIR}/privateinclude/QtCore/private
   ${CMAKE_BINARY_DIR}/privateinclude/QtGui/private
   ${CMAKE_BINARY_DIR}/privateinclude/QtPrabhupadaDB/private

   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_SOURCE_DIR}/src/3rdparty/harfbuzz/src
)

set(PRABHUPADADB_PUBLIC_INCLUDES
   QtPrabhupadaDB
   QPrabhupadaDB
)

list(APPEND PRABHUPADADB_INCLUDES
   ${CMAKE_CURRENT_SOURCE_DIR}/QPrabhupadaDB.h
   ${CMAKE_CURRENT_SOURCE_DIR}/QtPrabhupadaDB.h
   ${CMAKE_CURRENT_SOURCE_DIR}/QPrabhupadaDBFocus.h
)

#list(APPEND PRABHUPADADB_PRIVATE_INCLUDES
#   ${CMAKE_CURRENT_SOURCE_DIR}/q_svggraphics_p.h
#   ${CMAKE_CURRENT_SOURCE_DIR}/q_svghandler_p.h
#)

target_sources(NNPrabhupadaDB
   PRIVATE
   ${CMAKE_CURRENT_SOURCE_DIR}/QPrabhupadaDB.cpp
)

macro_generate_public("${PRABHUPADADB_PUBLIC_INCLUDES}"   QtPrabhupadaDB)
macro_generate_private("${PRABHUPADADB_PRIVATE_INCLUDES}" QtPrabhupadaDB)
macro_generate_misc("${PRABHUPADADB_INCLUDES}" QtPrabhupadaDB)

#target_link_libraries(NNPrabhupadaDB
#   libpqxx::pqxx
#)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
   target_link_libraries(NNPrabhupadaDB
      PRIVATE
      ws2_32
      #libpqxx::pqxx
   )
endif()

function_variable_fixup("${EXTRA_PRABHUPADADB_CXXFLAGS}" EXTRA_PRABHUPADADB_CXXFLAGS)
function_variable_fixup("${EXTRA_PRABHUPADADB_LDFLAGS}"  EXTRA_PRABHUPADADB_LDFLAGS)

target_sources(NNPrabhupadaDB
   PRIVATE
   ${PRABHUPADADB_INCLUDES}
)

target_link_libraries(NNPrabhupadaDB
   PRIVATE
   CsCore
   CsGui
   CsSql
   NNPrabhupada
)

target_compile_options(NNPrabhupadaDB
   PRIVATE
   $<$<COMPILE_LANGUAGE:CXX>:SHELL:${EXTRA_PRABHUPADADB_CXXFLAGS}>
)

set_target_properties(NNPrabhupadaDB
   PROPERTIES
   LINK_FLAGS ${EXTRA_PRABHUPADADB_LDFLAGS}
)

function_generate_resources(NNPrabhupadaDB)

macro_generate_package(
   NNPrabhupadaDB
   QtPrabhupadaDB
   "${EXTRA_PRABHUPADADB_CXXFLAGS}"
   "CsCore CsGui ScSql NNPrabhupada"
)

if(CMAKE_SYSTEM_NAME MATCHES "(Linux|OpenBSD|FreeBSD|NetBSD|DragonFly)")
   add_definitions(
      -DHAVE_POSIX_MEMALIGN
   )
endif()

install(
   TARGETS NNPrabhupadaDB
   EXPORT CopperSpiceLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
   RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
   DIRECTORY ${CMAKE_BINARY_DIR}/include/QtPrabhupadaDB
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
   COMPONENT Devel
)
