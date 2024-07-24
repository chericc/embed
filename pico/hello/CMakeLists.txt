set(BIN hello_world)

aux_source_directory(. src)

add_executable(${BIN}
    ${src})

target_link_libraries(${BIN} pico_stdlib)
pico_add_extra_outputs(${BIN})

