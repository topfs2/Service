# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8.11)
project (Service)

include(FindPkgConfig)

set( Boost_USE_STATIC_LIBS ON ) 
find_package( Boost REQUIRED COMPONENTS thread )

include_directories( ${Boost_INCLUDE_DIRS} )

pkg_search_module(YAJL REQUIRED yajl)
include_directories(${YAJL_INCLUDE_DIRS})

pkg_search_module(LIBLOG4CXX REQUIRED liblog4cxx)
include_directories(${LIBLOG4CXX_INCLUDE_DIRS})

find_package (Threads)

add_executable (test test.cpp test.cpp Variant.cpp Variant.h Time.cpp Time.h Locks.cpp Locks.h Thread.cpp Thread.h IMainloop.h BaseMainloop.h BaseMainloop.cpp ProcessMainloop.h ProcessMainloop.cpp ThreadedMainloop.h ThreadedMainloop.cpp PowerService.cpp PowerService.h ServiceBase.cpp ServiceBase.h Message.h MessagesCore.h MessagesProperties.h MessagesPower.h MessagesUtils.h IMailbox.h MessageRouter.h MessageRouter.cpp ReactorMailbox.h ReactorMailbox.cpp SimpleReactorMailbox.h SimpleReactorMailbox.cpp MonitorMailbox.h MonitorMailbox.cpp)
target_link_libraries(test ${YAJL_LIBRARIES})
target_link_libraries(test ${LIBLOG4CXX_LIBRARIES})
target_link_libraries (test ${CMAKE_THREAD_LIBS_INIT})
