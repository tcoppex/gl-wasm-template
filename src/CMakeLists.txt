# -----------------------------------------------------------------------------
# Sources.
# -----------------------------------------------------------------------------

set(TARGET_NAME "${CMAKE_PROJECT_NAME}")

list(APPEND Sources
  cpp/main.cc
  cpp/app.cc
)

list(APPEND Headers
  cpp/app.h
)

# Example of API usage for emscripten.
if (EMSCRIPTEN)
list(APPEND Sources
  cpp/api_sample.cc
)
list(APPEND Headers
  cpp/api_sample.h
)
endif()

# -----------------------------------------------------------------------------
# Target properties and build parameters.
# -----------------------------------------------------------------------------

# All dependencies files.
list(APPEND Dependencies 
  ${Sources}
  ${Headers}
)

# Transform link flags from a list to a string.
set(LinkFlags "")
foreach(flag ${CustomLinkFlags})
  set(LinkFlags "${LinkFlags} ${flag}")
endforeach()

add_executable(${TARGET_NAME} ${Dependencies})

# Set targets properties.
list(APPEND Targets ${TARGET_NAME})

foreach(target ${Targets})
  target_compile_options(
    ${target}
    PRIVATE
      "${CXX_FLAGS}"
      "$<$<CONFIG:Debug>:${CXX_FLAGS_DEBUG}>"
      "$<$<CONFIG:Release>:${CXX_FLAGS_RELEASE}>"
      "$<$<CONFIG:DebugWithRelInfo>:${CXX_FLAGS_RELWITHDEBINFO}>"
  )
  target_compile_definitions(${target} PRIVATE ${CustomDefinitions})
  target_include_directories(${target} PRIVATE ${CustomIncludeDirs}
#    INTERFACE
#      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
#      $<INSTALL_INTERFACE:include>
  )
  target_link_libraries(${target} ${CustomLibs})
  set_target_properties(${target} PROPERTIES LINK_FLAGS ${LinkFlags})

  # Force Debug / Release output in the same directory (for MSVC).
  helpers_setTargetOutputDirectory(${target} ${PROJECT_BINARY_DIR})
endforeach()

# (prototype)
if (EMSCRIPTEN)
  add_custom_command(
    TARGET
      ${TARGET_NAME} 
    POST_BUILD
    COMMAND 
      ${CMAKE_COMMAND} -E copy_directory "${PROJECT_BINARY_DIR}/" "${PROJECT_PUBLIC_DIR}/"
    COMMENT
      "[tmp] Copying web wasm binaries to \"${PROJECT_PUBLIC_DIR}\""
  )  
endif()

# -----------------------------------------------------------------------------
