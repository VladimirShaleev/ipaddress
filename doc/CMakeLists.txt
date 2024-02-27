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

  FetchContent_Declare(
    doxygen-cards
    GIT_REPOSITORY https://github.com/micro-os-plus/build-helper-xpack.git
    GIT_TAG v2.14.1
  )
  FetchContent_MakeAvailable(doxygen-cards)

  # Generate Get Started from README
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/getstarted.cmake"
    [=[
    file(READ "${SOURCE}" TEXT)
    file(READ "${LICENSE}" LICENSE)
    string(REPLACE "# ipaddress" "@mainpage Get Started\n\n@tableofcontents" TEXT "${TEXT}")
    string(REPLACE "## Documentation" "" TEXT "${TEXT}")
    string(REPLACE "For more details on setup, usage and code examples can be found in the full [documentation](https://vladimirshaleev.github.io/ipaddress/)." "" TEXT "${TEXT}")
    string(CONCAT TEXT "${TEXT}" "\n\n## Licensing\n\n```\n${LICENSE}\n```\n"
    [==[
    
@htmlonly

<div class="cards">

<div class="card">
  <a href="https://github.com/VladimirShaleev/ipaddress">
  <div class="card_container">
    <svg viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg" fill="#000000"><g id="SVGRepo_bgCarrier" stroke-width="0"></g><g id="SVGRepo_tracerCarrier" stroke-linecap="round" stroke-linejoin="round"></g><g id="SVGRepo_iconCarrier"> <title>github</title> <rect width="24" height="24" fill="none"></rect> <path d="M12,2A10,10,0,0,0,8.84,21.5c.5.08.66-.23.66-.5V19.31C6.73,19.91,6.14,18,6.14,18A2.69,2.69,0,0,0,5,16.5c-.91-.62.07-.6.07-.6a2.1,2.1,0,0,1,1.53,1,2.15,2.15,0,0,0,2.91.83,2.16,2.16,0,0,1,.63-1.34C8,16.17,5.62,15.31,5.62,11.5a3.87,3.87,0,0,1,1-2.71,3.58,3.58,0,0,1,.1-2.64s.84-.27,2.75,1a9.63,9.63,0,0,1,5,0c1.91-1.29,2.75-1,2.75-1a3.58,3.58,0,0,1,.1,2.64,3.87,3.87,0,0,1,1,2.71c0,3.82-2.34,4.66-4.57,4.91a2.39,2.39,0,0,1,.69,1.85V21c0,.27.16.59.67.5A10,10,0,0,0,12,2Z"></path> </g></svg>
    <h2>GitHub</h2>
    <p>Go to the project's GitHub repository.</p>
  </div>
  </a>
</div>

<div class="card">
  <a href="tutorial.html">
  <div class="card_container">
    <svg viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg"><g id="SVGRepo_bgCarrier" stroke-width="0"></g><g id="SVGRepo_tracerCarrier" stroke-linecap="round" stroke-linejoin="round"></g><g id="SVGRepo_iconCarrier"> <g id="Arrow / Arrow_Circle_Right"> <path id="Vector" d="M13 15L16 12M16 12L13 9M16 12H8M21 12C21 7.02944 16.9706 3 12 3C7.02944 3 3 7.02944 3 12C3 16.9706 7.02944 21 12 21C16.9706 21 21 16.9706 21 12Z" stroke="#000000" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path> </g> </g></svg>
    <h2>Tutorial</h2>
    <p>Learn the library for applied use, as well as how to configure the library to suit your needs.</p>
  </div>
  </a>
</div>

</div>

@endhtmlonly

    ]==]
    )
    file(WRITE "${TARGET}" "${TEXT}")
    ]=])
  add_custom_command(
      OUTPUT getstarted
      COMMAND "${CMAKE_COMMAND}"
          "-DLICENSE=${CMAKE_CURRENT_LIST_DIR}/../LICENSE"
          "-DSOURCE=${CMAKE_CURRENT_LIST_DIR}/../README.md"
          "-DTARGET=\"${CMAKE_CURRENT_BINARY_DIR}/get-started.md\""
          -P "${CMAKE_CURRENT_BINARY_DIR}/getstarted.cmake"
      DEPENDS ${CMAKE_CURRENT_LIST_DIR}/../README.md
  )

  file(GLOB IPADDRESS_DOC_SOURCES ${CMAKE_CURRENT_LIST_DIR}/../include/ipaddress/*)
  file(GLOB IPADDRESS_DOC_MARKDOWN ${CMAKE_CURRENT_LIST_DIR}/*.md)

  set(IPADDRESS_DOC_CSS "${doxygen-awesome-css_SOURCE_DIR}" CACHE STRING "doxygen-awesome")
  set(IPADDRESS_DOC_CARDS_CSS "${doxygen-cards_SOURCE_DIR}" CACHE STRING "doxygen-cards")
  set(IPADDRESS_DOC_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share/doc/${PROJECT_NAME}" CACHE PATH "Path to the documentation")

  CONFIGURE_FILE(Doxyfile.in Doxyfile @ONLY)

  add_custom_command(OUTPUT html
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/html
    DEPENDS Doxyfile getstarted ${IPADDRESS_DOC_SOURCES} ${IPADDRESS_DOC_MARKDOWN}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../
  )

  # Hide sync button
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/replace.cmake"
    [=[
    file(READ "${SOURCE}" TEXT)
    string(REPLACE "n.html('<img src=\"'+relpath+'sync_off.png\" title=\"'+SYNCOFFMSG+'\"/>');" "" TEXT "${TEXT}")
    string(REPLACE "n.html('<img src=\"'+relpath+'sync_on.png\" title=\"'+SYNCONMSG+'\"/>');" "" TEXT "${TEXT}")
    file(WRITE "${TARGET}" "${TEXT}")
    ]=])
  add_custom_command(
      OUTPUT navtree
      COMMAND "${CMAKE_COMMAND}"
          "-DSOURCE=${CMAKE_CURRENT_BINARY_DIR}/html/navtree.js"
          "-DTARGET=${CMAKE_CURRENT_BINARY_DIR}/html/navtree.js"
          -P "${CMAKE_CURRENT_BINARY_DIR}/replace.cmake"
      DEPENDS html "${CMAKE_CURRENT_BINARY_DIR}/replace.cmake"
  )
  
  add_custom_target(doc ALL DEPENDS html navtree)
  
  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION doc COMPONENT doc)
endif()
