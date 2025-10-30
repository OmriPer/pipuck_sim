set(SRC
    loop_functions.cpp
    loop_functions.hpp
    # Add other loop function source files here
)

add_library(my_loop_functions MODULE ${SRC})

target_link_libraries(my_loop_functions
  PUBLIC
    argos3core_${ARGOS_BUILD_FOR}
    argos3plugin_${ARGOS_BUILD_FOR}_pipuck

)

target_include_directories(my_loop_functions
  PUBLIC
    ${ARGOS_INCLUDE_DIRS}
)

set_target_properties(my_loop_functions PROPERTIES
  OUTPUT_NAME pipuck_drive_forward_dynamics2d_loop_functions
)
