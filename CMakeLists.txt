idf_component_register(SRCS "MicroSD.c" "Editor.c" "UARTE.c" "main.c"
                    INCLUDE_DIRS "." "include")

#target_compile_options(${COMPONENT_LIB} PRIVATE -Wunused-function)