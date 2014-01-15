# Install script for directory: /home/fred/jrtplib-3.9.1/src

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/jrtplib3" TYPE FILE FILES
    "/home/fred/jrtplib-3.9.1/src/rtcpapppacket.h"
    "/home/fred/jrtplib-3.9.1/src/rtcpbyepacket.h"
    "/home/fred/jrtplib-3.9.1/src/rtcpcompoundpacket.h"
    "/home/fred/jrtplib-3.9.1/src/rtcpcompoundpacketbuilder.h"
    "/home/fred/jrtplib-3.9.1/src/rtcppacket.h"
    "/home/fred/jrtplib-3.9.1/src/rtcppacketbuilder.h"
    "/home/fred/jrtplib-3.9.1/src/rtcprrpacket.h"
    "/home/fred/jrtplib-3.9.1/src/rtcpscheduler.h"
    "/home/fred/jrtplib-3.9.1/src/rtcpsdesinfo.h"
    "/home/fred/jrtplib-3.9.1/src/rtcpsdespacket.h"
    "/home/fred/jrtplib-3.9.1/src/rtcpsrpacket.h"
    "/home/fred/jrtplib-3.9.1/src/rtcpunknownpacket.h"
    "/home/fred/jrtplib-3.9.1/src/rtpaddress.h"
    "/home/fred/jrtplib-3.9.1/src/rtpcollisionlist.h"
    "/home/fred/jrtplib-3.9.1/src/rtpconfig.h"
    "/home/fred/jrtplib-3.9.1/src/rtpdebug.h"
    "/home/fred/jrtplib-3.9.1/src/rtpdefines.h"
    "/home/fred/jrtplib-3.9.1/src/rtperrors.h"
    "/home/fred/jrtplib-3.9.1/src/rtphashtable.h"
    "/home/fred/jrtplib-3.9.1/src/rtpinternalsourcedata.h"
    "/home/fred/jrtplib-3.9.1/src/rtpipv4address.h"
    "/home/fred/jrtplib-3.9.1/src/rtpipv4destination.h"
    "/home/fred/jrtplib-3.9.1/src/rtpipv6address.h"
    "/home/fred/jrtplib-3.9.1/src/rtpipv6destination.h"
    "/home/fred/jrtplib-3.9.1/src/rtpkeyhashtable.h"
    "/home/fred/jrtplib-3.9.1/src/rtplibraryversion.h"
    "/home/fred/jrtplib-3.9.1/src/rtpmemorymanager.h"
    "/home/fred/jrtplib-3.9.1/src/rtpmemoryobject.h"
    "/home/fred/jrtplib-3.9.1/src/rtppacket.h"
    "/home/fred/jrtplib-3.9.1/src/rtppacketbuilder.h"
    "/home/fred/jrtplib-3.9.1/src/rtppollthread.h"
    "/home/fred/jrtplib-3.9.1/src/rtprandom.h"
    "/home/fred/jrtplib-3.9.1/src/rtprandomrand48.h"
    "/home/fred/jrtplib-3.9.1/src/rtprandomrands.h"
    "/home/fred/jrtplib-3.9.1/src/rtprandomurandom.h"
    "/home/fred/jrtplib-3.9.1/src/rtprawpacket.h"
    "/home/fred/jrtplib-3.9.1/src/rtpsession.h"
    "/home/fred/jrtplib-3.9.1/src/rtpsessionparams.h"
    "/home/fred/jrtplib-3.9.1/src/rtpsessionsources.h"
    "/home/fred/jrtplib-3.9.1/src/rtpsourcedata.h"
    "/home/fred/jrtplib-3.9.1/src/rtpsources.h"
    "/home/fred/jrtplib-3.9.1/src/rtpstructs.h"
    "/home/fred/jrtplib-3.9.1/src/rtptimeutilities.h"
    "/home/fred/jrtplib-3.9.1/src/rtptransmitter.h"
    "/home/fred/jrtplib-3.9.1/src/rtptypes_win.h"
    "/home/fred/jrtplib-3.9.1/src/rtptypes.h"
    "/home/fred/jrtplib-3.9.1/src/rtpudpv4transmitter.h"
    "/home/fred/jrtplib-3.9.1/src/rtpudpv6transmitter.h"
    "/home/fred/jrtplib-3.9.1/src/rtpbyteaddress.h"
    "/home/fred/jrtplib-3.9.1/src/rtpexternaltransmitter.h"
    "/home/fred/jrtplib-3.9.1/src/extratransmitters/rtpfaketransmitter.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/libjrtp.a")
  IF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
  IF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
FILE(INSTALL DESTINATION "/usr/local/lib" TYPE STATIC_LIBRARY FILES "/home/fred/jrtplib-3.9.1/src/libjrtp.a")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FOREACH(file
      "$ENV{DESTDIR}/usr/local/lib/libjrtp.so.3.9.1"
      "$ENV{DESTDIR}/usr/local/lib/libjrtp.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    ENDIF()
  ENDFOREACH()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/libjrtp.so.3.9.1;/usr/local/lib/libjrtp.so")
  IF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
  IF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
FILE(INSTALL DESTINATION "/usr/local/lib" TYPE SHARED_LIBRARY FILES
    "/home/fred/jrtplib-3.9.1/src/libjrtp.so.3.9.1"
    "/home/fred/jrtplib-3.9.1/src/libjrtp.so"
    )
  FOREACH(file
      "$ENV{DESTDIR}/usr/local/lib/libjrtp.so.3.9.1"
      "$ENV{DESTDIR}/usr/local/lib/libjrtp.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_REMOVE
           FILE "${file}")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/home/fred/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-strip" "${file}")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF()
  ENDFOREACH()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

