idf_component_register(SRCS "MicroSD.c" "UARTE.c" "Editor.c" "main.c"
                    INCLUDE_DIRS "." "include")

#target_compile_options(${COMPONENT_LIB} PRIVATE -Wunused-function))