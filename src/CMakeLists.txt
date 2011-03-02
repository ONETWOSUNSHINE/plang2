cmake_minimum_required(VERSION 2.8)

set(TARGET plang)

set(SOURCES
    ir/base.cpp
    ir/builtins.cpp
    ir/expressions.cpp
    ir/type_derived.cpp
    ir/type_predicate.cpp
    ir/types.cpp
    ir/type_struct.cpp
    ir/type_union.cpp
    ir/visitor.cpp
    backend_c.cpp
    backend_debug.cpp
    collect_constraints.cpp
    lexer.cpp
    llir.cpp
    llir_process.cpp
    numbers.cpp
    options.cpp
    parser_context.cpp
    parser.cpp
    plang.cpp
    pp_flat_tree.cpp
    prettyprinter.cpp
    solve_constraints.cpp
    typecheck.cpp
    utils.cpp
)

add_definitions(
    -fmessage-length=0
)

add_executable(${TARGET} ${SOURCES})

include_directories (
    ../include
)

target_link_libraries(${TARGET}
    gmpxx
    gmp
)

install(TARGETS plang
  RUNTIME DESTINATION bin)
