CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(windicator C)

SET(SRCS
	src/windicator.c
        src/windicator_util.c
        src/windicator_moment_bar.c
        src/windicator_moment_view.c
        src/windicator_brightness.c
        src/windicator_volume.c
        src/windicator_dynamic.c
        src/windicator_call_fwd_btn.c
        src/windicator_music_btn.c
        src/windicator_dnd_btn.c
        src/windicator_flight_mode.c
        src/windicator_battery.c
        src/windicator_scs.c
)

SET(VENDOR "tizen")
SET(PACKAGE ${PROJECT_NAME})
SET(PKGNAME "org.${VENDOR}.${PACKAGE}")
SET(PREFIX ${CMAKE_INSTALL_PREFIX})
#SET(INSTALL_DIR_APPS "/usr/apps/${PKGNAME}")
SET(INSTALL_DIR_PRE "/usr")

SET(BINDIR "${PREFIX}/bin")
SET(RESDIR "${PREFIX}/res")
SET(IMAGEDIR "${RESDIR}/image/${PROJECT_NAME}")
SET(IMAGEDIR2 "${RESDIR}/edje/images")
SET(DATADIR "${PREFIX}/data")
SET(LOCALEDIR "${RESDIR}/locale")
SET(ICONDIR "${RESDIR}/icons")
SET(SOUNDDIR "${RESDIR}/sounds")
SET(DESKTOPICONDIR "${ICONDIR}/default/small")
SET(EDJDIR "${RESDIR}/edje")

IF("${ARCH}" STREQUAL "arm")
        ADD_DEFINITIONS("-DARM_TYPE")
ELSE()
        ADD_DEFINITIONS("-DEMULATOR_TYPE")
ENDIF()
MESSAGE("ARCH: ${ARCH}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")

SET(GC_SECTIONS_FLAGS "-fdata-sections -ffunction-sections -Wl,--gc-sections")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GC_SECTIONS_FLAGS}")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GC_SECTIONS_FLAGS}")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

INCLUDE(FindPkgConfig)
pkg_check_modules(pkgs REQUIRED elementary ecore-imf appcore-common appcore-efl
                                dlog glib-2.0 evas eina edje vconf elementary sensor
                                efl-assist ecore ecore-input ecore-evas capi-appfw-application
                                feedback appsvc deviced capi-system-device capi-system-info
				capi-media-sound-manager efl-extension capi-system-system-settings
				capi-ui-efl-util
)

#??
FIND_LIBRARY(LIB_M m)

IF("$ENV{CFLAGS}" MATCHES "-D_TIZEN_3G_ENABLE")
	SET(EDC_3G_ENABLE "1")
ELSE()
	SET(EDC_3G_ENABLE "0")
ENDIF()

ADD_DEFINITIONS(${pkgs_CFLAGS})
ADD_DEFINITIONS("-fpie")

ADD_DEFINITIONS("-DRSSI_ANT_BAR=${RSSI_ANT_BAR}")
MESSAGE("RSSI_ANT_BAR: ${RSSI_ANT_BAR}")

ADD_DEFINITIONS("-DPREFIX=\"${CMAKE_INSTALL_PREFIX}\"")
ADD_DEFINITIONS("-DFACTORYFS=\"$ENV{FACTORYFS}\"")
ADD_DEFINITIONS("-DSLP_DEBUG")
ADD_DEFINITIONS("-DSLP_PROF")

ADD_DEFINITIONS("-DVENDOR=\"${VENDOR}\"")
ADD_DEFINITIONS("-DPACKAGE=\"${PACKAGE}\"")
ADD_DEFINITIONS("-DPACKAGE_NAME=\"${PKGNAME}\"")
ADD_DEFINITIONS("-DPREFIX=\"${PREFIX}\"")

ADD_DEFINITIONS("-DINSTALL_DIR_APPS=\"${INSTALL_DIR_APPS}\"")
ADD_DEFINITIONS("-DBINDIR=\"${BINDIR}\"")
ADD_DEFINITIONS("-DLOCALEDIR=\"${LOCALEDIR}\"")
ADD_DEFINITIONS("-DICON_DIR=\"${ICONDIR}\"")
ADD_DEFINITIONS("-DIMAGEDIR=\"${IMAGEDIR}\"")
ADD_DEFINITIONS("-DSOUND_DIR=\"${SOUNDDIR}\"")
ADD_DEFINITIONS("-DEDJDIR=\"${EDJDIR}\"")
ADD_DEFINITIONS("-DWINDI_EDJ=\"${EDJDIR}/${PROJECT_NAME}.edj\"")
ADD_DEFINITIONS("-DPROJECT=\"${PROJECT_NAME}\"")

ADD_EXECUTABLE(${PROJECT_NAME} ${SRCS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${pkgs_LDFLAGS} "-pie" ${LIB_M})

IF("${RSSI_ANT_BAR}" STREQUAL "5")
	SET(RSSI_IMAGE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data/groups/5level")
ELSEIF("${RSSI_ANT_BAR}" STREQUAL "6")
	SET(RSSI_IMAGE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data/groups/6level")
ELSE()
	SET(RSSI_IMAGE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data/groups/4level")
ENDIF()

MESSAGE("EDC_3G_ENABLE : ${EDC_3G_ENABLE}")

#edj compile
ADD_CUSTOM_TARGET(${PROJECT_NAME}.edj
	COMMAND edje_cc -id
	${CMAKE_CURRENT_SOURCE_DIR}/data/edje/images
	-D_TIZEN_3G_ENABLE=${EDC_3G_ENABLE}
	${CMAKE_SOURCE_DIR}/data/edje/${PROJECT_NAME}.edc ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.edj
	DEPENDS ${CMAKE_SOURCE_DIR}/data/edje/${PROJECT_NAME}.edc
)
ADD_DEPENDENCIES(${PROJECT_NAME} ${PROJECT_NAME}.edj)
ADD_DEPENDENCIES(${PROJECT_NAME} edj_build)

MESSAGE("bindir: ${BINDIR}")

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ${BINDIR})
INSTALL(FILES ${CMAKE_BINARY_DIR}/windicator.edj DESTINATION ${EDJDIR})


# install application HOME directory
INSTALL(DIRECTORY DESTINATION ${DATADIR})

#ADD_SUBDIRECTORY(po)

# install desktop files
set(PREFIX ${CMAKE_INSTALL_PREFIX})
install(FILES ${CMAKE_SOURCE_DIR}/org.tizen.windicator.xml DESTINATION /usr/share/packages)


#image
INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/data/images/ DESTINATION ${IMAGEDIR})
INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/data/edje/images/ DESTINATION ${IMAGEDIR})


#icon
INSTALL(FILES data/icon/org.tizen.windicator.png DESTINATION /usr/share/icons)

ADD_SUBDIRECTORY(po)

