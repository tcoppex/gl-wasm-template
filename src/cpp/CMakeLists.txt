# -----------------------------------------------------------------------------
# Sources.
# -----------------------------------------------------------------------------

# Framework (library).
list(APPEND FrameworkSources
  framework/base_app.cc
)

# Sample (executable).
list(APPEND SampleSources
  sample/main.cc
  sample/sample_app.cc
)

# Example of API usage for emscripten.
if(EMSCRIPTEN)
  list(APPEND SampleSources
    sample/api/lib.cc

    # [fixme?]
    # Compile the framework's API along with sample's ones to make it availables in the JS module.
    framework/api/lib.cc
  )
endif()

# -----------------------------------------------------------------------------
# Target properties and build parameters.
# -----------------------------------------------------------------------------

# Framework Target (library).
set(FrameworkTarget "wasmFramework")
add_library(${FrameworkTarget} ${PROJECT_LIB_TYPE} ${FrameworkSources})

# Sample Target (executable).
set(AppTarget "wasmApp")
add_executable(${AppTarget} ${SampleSources})
target_link_libraries(${AppTarget} ${FrameworkTarget})

# -----------------------------------------------------------------------------

list(APPEND Targets ${FrameworkTarget} ${AppTarget})

# Set generic target properties.
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

  # [prefers LINK_OPTION with a list on Cmake 3.13+]
  set_target_properties(${target} PROPERTIES LINK_FLAGS ${LinkFlagsString})

  # Force Debug / Release output in the same directory (for MSVC).
  helpers_setTargetOutputDirectory(${target} ${PROJECT_BINARY_DIR})
endforeach()

# -----------------------------------------------------------------------------

# Set specific target properties.
if(EMSCRIPTEN)
  # Force the wasm app to be installed in the public distributables directory.
  helpers_setTargetOutputDirectory(${AppTarget} ${PROJECT_PUBLIC_DIR})

  # Create the public distributables directory to be served by the HTTP server.
  add_custom_command(
    TARGET
      ${AppTarget} 
    PRE_BUILD
    COMMAND 
      ${CMAKE_COMMAND} -E make_directory "${PROJECT_PUBLIC_DIR}"
    COMMENT
      "Create directory : \"${PROJECT_PUBLIC_DIR}\"."
  )

  # [ Potentially dangerous ] 
  # Mark the public binary directory for removal using 'clean' (Ninja & Make only).
  set_directory_properties(PROPERTIES ADDITIONAL_CLEAN_FILES ${PROJECT_PUBLIC_DIR})

  # Copy static web data to the public directory.
  add_custom_command(
    TARGET
      ${AppTarget} 
    PRE_BUILD
    COMMAND 
      ${CMAKE_COMMAND} -E copy_directory "${PROJECT_WEB_STATIC_DIR}" "${PROJECT_PUBLIC_DIR}"
    COMMENT
      "Copying static web files content to bin/public/."
  )

  # [TMP] Copy the index.js file to the public directory.
  set(IndexJSFile "${PROJECT_SOURCE_DIR}/js/index.js") #
  add_custom_target(
    CopyIndexJSFile  ALL
    COMMAND 
      ${CMAKE_COMMAND} -E copy "${IndexJSFile}" "${PROJECT_PUBLIC_DIR}/"
    SOURCES
      ${IndexJSFile}
    COMMENT
      "Copying index.js to bin/public/."
  )
endif()

# -----------------------------------------------------------------------------
