set(SRC
  controller0.hpp
  controller0.cpp
  # Add other controller source files here
)

add_library(pipuck_controller0 MODULE ${SRC})

target_link_libraries(pipuck_controller0
  PUBLIC
    argos3core_${ARGOS_BUILD_FOR}
    argos3plugin_${ARGOS_BUILD_FOR}_pipuck

)

target_include_directories(pipuck_controller0
  PUBLIC
    ${ARGOS_INCLUDE_DIRS}
)

set_target_properties(pipuck_controller0 PROPERTIES
  OUTPUT_NAME pipuck_controller0
)
