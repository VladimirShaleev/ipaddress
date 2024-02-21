find_package(Doxygen)

if(NOT DOXYGEN_FOUND)
  message(STATUS "No Doxygen found. Documentation won't be built")
else()
  include(FetchContent)
  FetchContent_Declare(
    doxygen-awesome-css
    GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css.git
    GIT_TAG v2.3.1
  )
  FetchContent_MakeAvailable(doxygen-awesome-css)

  file(GLOB IPADDRESS_DOC_SOURCES ${CMAKE_CURRENT_LIST_DIR}/../include/ipaddress/*)
  file(GLOB IPADDRESS_DOC_MARKDOWN ${CMAKE_CURRENT_LIST_DIR}/*.md)
  list(APPEND IPADDRESS_DOC_MARKDOWN ${CMAKE_CURRENT_LIST_DIR}/../README.md)

  set(IPADDRESS_DOC_CSS "${doxygen-awesome-css_SOURCE_DIR}" CACHE STRING "doxygen-awesome")

  CONFIGURE_FILE(Doxyfile.in Doxyfile @ONLY)

  add_custom_command(OUTPUT html
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/html
    DEPENDS Doxyfile ${IPADDRESS_DOC_SOURCES} ${IPADDRESS_DOC_MARKDOWN}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../
  )

  add_custom_target(doc ALL DEPENDS html)
  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
    DESTINATION ${IPADDRESS_DOC_INSTALL_DIR}
    COMPONENT doc)
endif()
