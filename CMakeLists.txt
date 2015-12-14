# Tutorial: http://www.cmake.org/cmake/help/cmake_tutorial.html

cmake_minimum_required(VERSION 2.6)
project(concordia-server CXX)
set (CONCORDIA_SERVER_VERSION_MAJOR 1)
set (CONCORDIA_SERVER_VERSION_MINOR 0)
set(BASE_TARGETS concordia-server)

# ================================================
# Functional settings
# ================================================

set (STOP_WORDS_ENABLED "false")


# ================================================
# Config
# ================================================

set (INDEX_DIRECTORY "${concordia-server_SOURCE_DIR}/index")
set (RESOURCES_DIRECTORY "${concordia-server_SOURCE_DIR}/resources")

configure_file (
  "${concordia-server_SOURCE_DIR}/concordia.cfg.in"
  "${concordia-server_SOURCE_DIR}/concordia.cfg"
  )

set(COMPILED_BINARIES_PATH "${concordia-server_SOURCE_DIR}/build/concordia-server")
set(SCRIPTS_PATH "${concordia-server_SOURCE_DIR}/scripts")
configure_file (
  "${concordia-server_SOURCE_DIR}/scripts/cmake_stubs/start.sh.in"
  "${concordia-server_SOURCE_DIR}/scripts/start.sh"
  )
configure_file (
  "${concordia-server_SOURCE_DIR}/scripts/cmake_stubs/stop.sh.in"
  "${concordia-server_SOURCE_DIR}/scripts/stop.sh"
  )
configure_file (
  "${concordia-server_SOURCE_DIR}/scripts/cmake_stubs/restart.sh.in"
  "${concordia-server_SOURCE_DIR}/scripts/restart.sh"
  )
configure_file (
  "${concordia-server_SOURCE_DIR}/scripts/cmake_stubs/watchdog.sh.in"
  "${concordia-server_SOURCE_DIR}/scripts/watchdog.sh"
  )

set(CONFIG_FILE_PATH "${concordia-server_SOURCE_DIR}/concordia.cfg")
set(LOG_FILE_PATH "${concordia-server_SOURCE_DIR}/logs/concordia-server.log")

# --------------
# db settings
# --------------
set (DB_NAME "concordia_server")
set (DB_USER "concordia")
set (DB_PASSWORD "concordia")
set (DB_HOST "localhost")
set (DB_PORT "6543")

configure_file (
  "${concordia-server_SOURCE_DIR}/concordia-server/config.hpp.in"
  "${concordia-server_SOURCE_DIR}/concordia-server/config.hpp"
  )


# ================================================
# Third-party libraries
# ================================================

# ----------------------------------------------------
# Regular expression libraries
# ----------------------------------------------------
option(WITH_RE2 "Using RE2 regular expression library" OFF)
message(STATUS "Using RE2 regular expression library ${WITH_RE2}")

if(WITH_RE2)
    set(HAVE_RE2 1)
endif(WITH_RE2)

option(WITH_PCRE "Using PCRE regular expression library" ON)
message(STATUS "Using PCRE regular expression library ${WITH_PCRE}")

if(WITH_PCRE)
  set(HAVE_PCRE 1)
endif(WITH_PCRE)

# ----------------------------------------------------
# Boost
# ----------------------------------------------------
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS 
      serialization unit_test_framework system filesystem program_options iostreams regex locale REQUIRED)

# ----------------------------------------------------
# libconfig
# ----------------------------------------------------
find_library(LIBCONFIG_LIB NAMES config++ REQUIRED)
find_path(LIBCONFIG_INCLUDE libconfig.h++)

if(EXISTS ${LIBCONFIG_LIB} AND EXISTS ${LIBCONFIG_INCLUDE})
  message(STATUS "Found Libconfig")
  include_directories(${LIBCONFIG_INCLUDE})
  link_directories(${LIBCONFIG_LIB})
endif(EXISTS ${LIBCONFIG_LIB} AND EXISTS ${LIBCONFIG_INCLUDE})


# ----------------------------------------------------
# Logging
# ----------------------------------------------------
find_library(LOG4CPP_LIB NAMES log4cpp REQUIRED)
find_path(LOG4CPP_INCLUDE log4cpp/Appender.hh)

if(EXISTS ${LOG4CPP_LIB} AND EXISTS ${LOG4CPP_INCLUDE})
  message(STATUS "Found Log4cpp")
  include_directories(${LOG4CPP_INCLUDE})
  link_directories(${LOG4CPP_LIB})
endif(EXISTS ${LOG4CPP_LIB} AND EXISTS ${LOG4CPP_INCLUDE})

# ----------------------------------------------------
# FastCGI
# ----------------------------------------------------
find_library(FASTCGI_LIB NAMES fcgi REQUIRED)

if(EXISTS ${FASTCGI_LIB})
  message(STATUS "Found fastcgi")
  link_directories(${FASTCGI_LIB})
endif(EXISTS ${FASTCGI_LIB})

# ----------------------------------------------------
# FastCGI++
# ----------------------------------------------------
find_library(FASTCGIPP_LIB NAMES fcgi++ REQUIRED)

if(EXISTS ${FASTCGIPP_LIB})
  message(STATUS "Found fastcgi++")
  link_directories(${FASTCGIPP_LIB})
endif(EXISTS ${FASTCGIPP_LIB})

# ----------------------------------------------------
# libpq
# ----------------------------------------------------
find_library(PQ_LIB NAMES pq REQUIRED)
find_path(PQ_INCLUDE libpq-fe.h HINTS "/usr/include/postgresql")

if(EXISTS ${PQ_LIB})
  message(STATUS "Found libpq")
  include_directories(${PQ_INCLUDE})
  link_directories(${PQ_LIB})
endif(EXISTS ${PQ_LIB})

# ----------------------------------------------------
# Concordia
# ----------------------------------------------------
find_library(CONCORDIA_LIB NAMES conconrdia REQUIRED)
find_path(CONCORDIA_INCLUDE concordia.hpp)

if(EXISTS ${CONCORDIA_LIB} AND EXISTS ${CONCORDIA_INCLUDE})
  message(STATUS "Found Concordia")
  include_directories(${CONCORDIA_INCLUDE})
  link_directories(${CONCORDIA_LIB})
endif(EXISTS ${CONCORDIA_LIB} AND EXISTS ${CONCORDIA_INCLUDE})

# ----------------------------------------------------
# divsufsort
# ----------------------------------------------------
find_library(DIVSUFSORT_LIB NAMES divsufsort REQUIRED)
find_path(DIVSUFSORT_INCLUDE divsufsort.h)

if(EXISTS ${DIVSUFSORT_LIB} AND EXISTS ${DIVSUFSORT_INCLUDE})
  message(STATUS "Found divsufsort")
  include_directories(${DIVSUFSORT_INCLUDE})
  link_directories(${DIVSUFSORT_LIB})
endif(EXISTS ${DIVSUFSORT_LIB} AND EXISTS ${DIVSUFSORT_INCLUDE})

# ----------------------------------------------------
# utf8case
# ----------------------------------------------------
find_library(UTF8CASE_LIB NAMES utf8case REQUIRED)

if(EXISTS ${UTF8CASE_LIB})
  message(STATUS "Found utf8case")
  link_directories(${UTF8CASE_LIB})
endif(EXISTS ${UTF8CASE_LIB})

add_subdirectory(concordia-server)

  



