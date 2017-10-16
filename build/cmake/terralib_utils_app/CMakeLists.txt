if(WIN32)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS -DTERRALIBUTILSAPP -D_CRT_SECURE_NO_WARNINGS -DBOOST_LOG_DYN_LINK)
endif()

include_directories(
  ${TERRALIB_UTILS_ABSOLUTE_ROOT_DIR}/src
  ${terralib_INCLUDE_DIRS}
  ${terralib_DIR}
  ${Boost_INCLUDE_DIR}
  ${Boost_INCLUDE_DIR}/qwt
)

if(Qt5_FOUND)
  set(CMAKE_AUTOMOC ON)
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

file(GLOB TERRALIBUTILS_SRC_FILES ${TERRALIB_UTILS_ABSOLUTE_ROOT_DIR}/src/terralib_utils_app/*.cpp)
file(GLOB TERRALIBUTILS_HDR_FILES ${TERRALIB_UTILS_ABSOLUTE_ROOT_DIR}/src/terralib_utils_app/*.h)

source_group("Source Files"  FILES ${TERRALIBUTILS_SRC_FILES})
source_group("Header Files"  FILES ${TERRALIBUTILS_HDR_FILES})


set(TERRALIBUTILS_DEPENDENCIES "terralib_utils")
list(APPEND TERRALIBUTILS_DEPENDENCIES "terralib_mod_core")

if(Qt5_FOUND)
  
  add_executable(terralibUtilsApp ${TERRALIBUTILS_SRC_FILES} ${TERRALIBUTILS_HDR_FILES})
  
  target_link_libraries(terralibUtilsApp ${TERRALIBUTILS_DEPENDENCIES} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
  
  SET (QT_USE_QTMAIN TRUE)
  
  set_target_properties(terralibUtilsApp PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
  
endif()

set_target_properties(terralibUtilsApp
                      PROPERTIES VERSION ${TERRALIB_UTILS_VERSION_MAJOR}.${TERRALIB_UTILS_VERSION_MINOR}
                                 SOVERSION ${TERRALIB_UTILS_VERSION_MAJOR}.${TERRALIB_UTILS_VERSION_MINOR})

install(TARGETS terralibUtilsApp
        EXPORT terralibUtils-targets
		BUNDLE DESTINATION . COMPONENT runtime
        RUNTIME DESTINATION ${TERRALIB_UTILS_DESTINATION_RUNTIME} COMPONENT runtime
        LIBRARY DESTINATION ${TERRALIB_UTILS_DESTINATION_LIBRARY} COMPONENT runtime
        ARCHIVE DESTINATION ${TERRALIB_UTILS_DESTINATION_ARCHIVE} COMPONENT runtime)

install(FILES ${TERRALIBUTILS_HDR_FILES}
        DESTINATION ${TERRALIB_UTILS_DESTINATION_HEADERS}/terralibUtilsApp COMPONENT devel)
		
if(WIN32)
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/qt.conf
       "[Paths]\nBinaries = .\nPlugins = ../qtplugins\nTranslations = ../share/terralib/translations")
         
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qt.conf
          DESTINATION ${TERRALIB_UTILS_DESTINATION_RUNTIME} COMPONENT runtime)
endif()
