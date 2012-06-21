project(qmwestatistic-binary)

option(WITH_OPENGL "BUILD OPENGL" ON)

find_package(Qt4 REQUIRED)
if(WITH_OPENGL)
    find_package(OpenGL REQUIRED)
endif(WITH_OPENGL)

include(${QT_USE_FILE})

ADD_DEFINITIONS(-D FANN_NO_DLL)

set(qmwestatistic_SRCS
    main.cpp
    CMainWindow.cpp
    CFannNetwork.cpp
    CPlotter.cpp
)

if (WIN32)
    ADD_DEFINITIONS(-D WIN32)
    SET(qmwestatistic_SRCS ${qmwestatistic_SRCS} qmwestatistic.rc)
endif (WIN32)

set(qmwestatistic_HDRS
    CMainWindow.h
    CFannNetwork.h
    CPlotter.h
    version.h
)


SET(qmwestatistic_UIS
	    CMainWindow.ui
)

SET(qmwestatistic_RCS
    resources.qrc
)


SET (QL_OPENGL_INCLUDE_DIR "")
SET (QL_OPENGL_LIBRARIES "")

add_definitions(-DDB_VERSION=8)
SET(qmwestatistic_HDRS ${qmwestatistic_HDRS}  )
SET(qmwestatistic_SRCS ${qmwestatistic_SRCS}  )
SET(qmwestatistic_UIS ${qmwestatistic_UIS} )

qt4_wrap_cpp(qmwestatistic_MOC_SRCS ${qmwestatistic_HDRS})
qt4_wrap_ui(qmwestatistic_UI_HDRS ${qmwestatistic_UIS})
qt4_add_resources(qmwestatistic_RC_SRCS ${qmwestatistic_RCS})

SET (ENABLE_TRANSLATION ON CACHE BOOL "enable translations")
#-D ENABLE_TRANSLATION:bool=OFF
IF(ENABLE_TRANSLATION)
    add_definitions(-DENABLE_TRANSLATION)

    file (GLOB TRANSLATIONS_FILES locale/*.ts)

    option (UPDATE_TRANSLATIONS "Update source translation translations/*.ts
    files (WARNING: make clean will delete the source .ts files! Danger!)")
    if (UPDATE_TRANSLATIONS)
       qt4_create_translation(qmwestatistic_QM_FILES
                              ${qmwestatistic_SRCS}
                              ${qmwestatistic_HDRS}
                              ${qmwestatistic_UIS}
                              ${TRANSLATIONS_FILES}
                          )
    else (UPDATE_TRANSLATIONS)
       qt4_add_translation(qmwestatistic_QM_FILES ${TRANSLATIONS_FILES})
    endif (UPDATE_TRANSLATIONS)

    if (UNIX AND NOT WIN32 AND NOT APPLE)
     install(
             FILES
                   ${qmwestatistic_QM_FILES}
             DESTINATION
                   ${DATA_INSTALL_PREFIX}/qmwestatistic/translations)
    endif (UNIX AND NOT WIN32 AND NOT APPLE)
    if (APPLE)
      add_custom_target(locversion.plist ALL
        COMMAND
            ${CMAKE_SOURCE_DIR}/MacOSX/mklocversion.sh ${MAC_BUNDLE_PATH} ${qmwestatistic_QM_FILES}
      )
    endif (APPLE)
ENDIF(ENABLE_TRANSLATION)

if(UNIX)
    add_definitions(-D_TTY_POSIX_ -Wall )
endif(UNIX)

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${QT_QTXML_INCLUDE_DIR}
  ${QT_QTSVG_INCLUDE_DIR}
  ${QT_QTSQL_INCLUDE_DIR}
  ${QT_QTOPENGL_INCLUDE_DIR}
  ${QT_QTNETWORK_INCLUDE_DIR}
  ${QT_QTDBUS_INCLUDE_DIR}
  ${QT_QTWEBKIT_INCLUDE_DIR}
  ${QT_QTSCRIPT_INCLUDE_DIR}
  ${QL_OPENGL_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/3rdparty/FANN/src/include
  ${CMAKE_SOURCE_DIR}/3rdparty/getopt
  ${CMAKE_SOURCE_DIR}/3rdparty/qwt
)

set(qmwestatistic_ALLINP
    ${qmwestatistic_SRCS}
    ${qmwestatistic_HDRS}
    ${qmwestatistic_UI_HDRS}
    ${qmwestatistic_RC_SRCS}
    ${qmwestatistic_MOC_SRCS}
    ${qmwestatistic_QM_FILES}
)

if (APPLE)
  set(MAC_EXTRA_RES
      ${CMAKE_SOURCE_DIR}/MacOSX/${MAC_ICON_FILE}
      ${CMAKE_SOURCE_DIR}/MacOSX/qmwestatistic-gdb.icns
      ${CMAKE_SOURCE_DIR}/MacOSX/qmwestatistic-gpx.icns
      ${CMAKE_SOURCE_DIR}/MacOSX/qmwestatistic-kml.icns
      ${CMAKE_SOURCE_DIR}/MacOSX/qmwestatistic-loc.icns
      ${CMAKE_SOURCE_DIR}/MacOSX/qmwestatistic-plt.icns
      ${CMAKE_SOURCE_DIR}/MacOSX/qmwestatistic-qlb.icns
      ${CMAKE_SOURCE_DIR}/MacOSX/qmwestatistic-rte.icns
      ${CMAKE_SOURCE_DIR}/MacOSX/qmwestatistic-tcx.icns
      ${CMAKE_SOURCE_DIR}/MacOSX/qmwestatistic-wpt.icns
  )
  set_source_files_properties(
    ${MAC_EXTRA_RES} ${qmwestatistic_QM_FILES}
  PROPERTIES
    MACOSX_PACKAGE_LOCATION Resources
  )
  set(MACOSX_BUNDLE_COPYRIGHT
    "Copyright © 2007-2012 Fabrice Crohas (fcrohas@gmail.com)"
  )
  add_executable(${PROGNAME} MACOSX_BUNDLE ${qmwestatistic_ALLINP} ${MAC_EXTRA_RES})
  set_target_properties(${PROGNAME} PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/MacOSX/Info.plist.in
        LINK_FLAGS "-framework CoreFoundation"
  )

  add_custom_target(mac-deploy macdeployqt "${MAC_BUNDLE_PATH}"
    COMMAND ${CMAKE_SOURCE_DIR}/MacOSX/map2gcm-libs.sh "${MAC_BUNDLE_PATH}")
else(APPLE)
  add_executable(${PROGNAME} WIN32 ${qmwestatistic_ALLINP})
endif(APPLE)

if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE )
endif(MSVC)
target_link_libraries(
    ${PROGNAME}
      ${QT_LIBRARIES}
      ${QT_QTNETWORK_LIBRARIES}
      ${QT_QTXML_LIBRARIES}
      ${QT_QTSQL_LIBRARIES}
      ${QT_QTDBUS_LIBRARIES}
      ${QT_QTMAIN_LIBRARIES}
      ${QT_QTWEBKIT_LIBRARIES}
      ${QT_PHONON_LIBRARIES}
      ${QT_QTSCRIPT_LIBRARIES}
      ${QT_QTXMLPATTERNS_LIBRARIES}
      ${QL_OPENGL_LIBRARIES}
      qwt
      fann
      getopt
  )

install(
    TARGETS
      ${PROGNAME}
    DESTINATION
      ${BIN_INSTALL_DIR}
)
