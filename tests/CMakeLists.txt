cmake_minimum_required(VERSION 2.8)

# Tests for statements modification

set(TARGET_ST verification_tests)

set(SOURCES_ST
    ../src/autoptr.cpp
    ../src/utils.cpp
    ../src/ir/base.cpp
    ../src/ir/builtins.cpp
    ../src/ir/expressions.cpp
    ../src/ir/type_derived.cpp
    ../src/ir/type_predicate.cpp
    ../src/ir/type_array.cpp
    ../src/ir/type_subtype.cpp
    ../src/ir/types.cpp
    ../src/ir/type_struct.cpp
    ../src/ir/type_union.cpp
    ../src/ir/visitor.cpp
    ../src/ir/declarations.cpp
    ../src/lexer.cpp
    ../src/numbers.cpp
    ../src/options.cpp
    ../src/parser_context.cpp
    ../src/parser.cpp
    ../src/verification_tests.cpp
    ../src/pp_flat_tree.cpp
    ../src/pp_syntax.cpp
    ../src/prettyprinter.cpp
    ../src/statement_tree.cpp
    ../src/node_analysis.cpp
    ../src/term_rewriting.cpp
    ../src/typecheck.cpp
    ../src/solve_constraints.cpp
    ../src/collect_constraints.cpp
    ../src/type_lattice.cpp
    ../src/generate_semantics.cpp
    ../src/generate_name.cpp
    ../src/verification_context.cpp
    ../src/cvc3_solver.cpp
    ../src/typecheck/compact.cpp
    ../src/typecheck/eval.cpp
    ../src/typecheck/expand.cpp
    ../src/typecheck/explode.cpp
    ../src/typecheck/guess.cpp
    ../src/typecheck/infer.cpp
    ../src/typecheck/lift.cpp
    ../src/typecheck/operations.cpp
    ../src/typecheck/prune.cpp
    ../src/typecheck/refute.cpp
    ../src/typecheck/unify.cpp
    ../src/sort_module.cpp
)

add_executable(${TARGET_ST} ${SOURCES_ST})

set_target_properties(${TARGET_ST} PROPERTIES COMPILE_DEFINITIONS "CONDITIONS_FOR_IF")

target_link_libraries(${TARGET_ST} ${CVC3LIB_LIBRARIES})

include_directories (
    ../include
)

target_link_libraries(${TARGET_ST}
    gmpxx
    gmp
)

set(SOURCES_INDENTING_STREAM
    ../src/test_indenting_stream.cpp
    ../src/utils.cpp
)

add_executable(test_indenting_stream ${SOURCES_INDENTING_STREAM})
target_link_libraries(test_indenting_stream gmpxx gmp)

add_executable(test_indenting_stream_w ${SOURCES_INDENTING_STREAM})
target_link_libraries(test_indenting_stream_w gmpxx gmp)
set_target_properties(test_indenting_stream
    PROPERTIES COMPILE_DEFINITIONS "WIDE")

macro(run_tests FLAGS TESTS)
    foreach(TEST ${TESTS})
        add_test(${TEST}
            ${PROJECT_SOURCE_DIR}/tools/test.sh
            ${PROJECT_SOURCE_DIR}/tools
            ${PROJECT_BINARY_DIR}/src
            ${PROJECT_SOURCE_DIR}/tests/${TEST}
            ${FLAGS})
    endforeach()
endmacro()

macro(run_pp_tests FLAGS TESTS)
    foreach(TEST ${TESTS})
        add_test(${TEST}
            ${PROJECT_SOURCE_DIR}/tools/test_pp.sh
            ${PROJECT_SOURCE_DIR}/tools
            ${PROJECT_BINARY_DIR}/src
            ${PROJECT_SOURCE_DIR}/tests/${TEST}
            ${FLAGS})
    endforeach()
endmacro()

macro(run_cg_tests FLAGS TESTS)
    foreach(TEST ${TESTS})
        add_test(${TEST}
            ${PROJECT_SOURCE_DIR}/tools/test_cg.sh
            ${PROJECT_SOURCE_DIR}/tools
            ${PROJECT_BINARY_DIR}/src
            ${PROJECT_SOURCE_DIR}/tests/${TEST}
            ${FLAGS})
    endforeach()
endmacro()

macro(run_ot_tr_tests FLAGS TESTS)
    foreach(TEST ${TESTS})
        add_test(${TEST}
            ${PROJECT_SOURCE_DIR}/tools/test_ot_tr.sh
            ${PROJECT_SOURCE_DIR}/tools
            ${PROJECT_BINARY_DIR}/src
            ${PROJECT_SOURCE_DIR}/tests/${TEST}
            ${FLAGS})
    endforeach()
endmacro()

macro(run_ot_pi_tests FLAGS TESTS)
    foreach(TEST ${TESTS})
        add_test(${TEST}
            ${PROJECT_SOURCE_DIR}/tools/test_ot_pi.sh
            ${PROJECT_SOURCE_DIR}/tools
            ${PROJECT_BINARY_DIR}/src
            ${PROJECT_SOURCE_DIR}/tests/${TEST}
            ${FLAGS})
    endforeach()
endmacro()

macro(run_st_matching FLAGS TESTS)
    foreach(TEST ${TESTS})
        add_test(${TEST}
            ${PROJECT_SOURCE_DIR}/tools/test_st.sh
            ${PROJECT_SOURCE_DIR}/tools
            ${PROJECT_BINARY_DIR}/tests
            ${PROJECT_SOURCE_DIR}/tests/${TEST}
            ${FLAGS})
    endforeach()
endmacro()

macro(run_verification_tests FLAGS TESTS)
    foreach(TEST ${TESTS})
        add_test(${TEST}
            ${PROJECT_SOURCE_DIR}/tools/test_verification_tests.sh
            ${PROJECT_SOURCE_DIR}/tools
            ${PROJECT_BINARY_DIR}/tests
            ${PROJECT_SOURCE_DIR}/tests/${TEST}
            ${FLAGS})
    endforeach()
endmacro()

macro(run_cond_tests FLAGS TESTS)
    foreach(TEST ${TESTS})
        add_test(${TEST}
            ${PROJECT_SOURCE_DIR}/tools/test_cond.sh
            ${PROJECT_SOURCE_DIR}/tools
            ${PROJECT_BINARY_DIR}/tests
            ${PROJECT_SOURCE_DIR}/tests/${TEST}
            ${FLAGS})
    endforeach()
endmacro()

macro(run_vf_tests FLAGS TESTS)
    foreach(TEST ${TESTS})
        add_test(${TEST}
            ${PROJECT_SOURCE_DIR}/tools/test_vf.sh
            ${PROJECT_SOURCE_DIR}/tools
            ${PROJECT_BINARY_DIR}/src
            ${PROJECT_SOURCE_DIR}/tests/${TEST}
            ${FLAGS})
    endforeach()
endmacro()

macro(run_pvs_tests FLAGS TESTS)
    foreach(TEST ${TESTS})
        add_test(${TEST}
            ${PROJECT_SOURCE_DIR}/tools/test_pvs.sh
            ${PROJECT_SOURCE_DIR}/tools
            ${PROJECT_BINARY_DIR}/src
            ${PROJECT_SOURCE_DIR}/tests/${TEST}
            ${FLAGS})
    endforeach()
endmacro()

macro(run_cvc3_tests FLAGS TESTS)
    foreach(TEST ${TESTS})
        add_test(${TEST}
            ${PROJECT_SOURCE_DIR}/tools/test_cvc3.sh
            ${PROJECT_SOURCE_DIR}/tools
            ${PROJECT_BINARY_DIR}/tests
            ${PROJECT_SOURCE_DIR}/tests/${TEST}
            ${FLAGS})
    endforeach()
endmacro()

macro(run_indenting_stream_tests TESTS)
    foreach(TEST ${TESTS})
        add_test(${TEST}
            ${PROJECT_SOURCE_DIR}/tools/test_indenting_stream.sh
            ${PROJECT_BINARY_DIR}/tests/test_indenting_stream
            ${PROJECT_SOURCE_DIR}/tests/${TEST}
            ${FLAGS})
        add_test(${TEST}_wide
            ${PROJECT_SOURCE_DIR}/tools/test_indenting_stream.sh
            ${PROJECT_BINARY_DIR}/tests/test_indenting_stream
            ${PROJECT_SOURCE_DIR}/tests/${TEST}
            ${FLAGS})
    endforeach()
endmacro()

set(TESTS_PARSING
    parsing/logic_mul.psrc
    parsing/logic_gcd.psrc
    parsing/formula_result.psrc
    parsing/field_expr.psrc
    parsing/submodules.psrc
    parsing/subformula_args.psrc
    parsing/array_subtype.psrc
    parsing/quantifier.psrc
    parsing/replacement.psrc
    parsing/variable_declarations.psrc
    parsing/literal.psrc
    parsing/lemma_status.psrc
    parsing/normalize_binary.psrc
    parsing/import.psrc
    parsing/jumps.psrc
    parsing/predicate_type.psrc
    parsing/ternary_if_call.psrc
    parsing/formula_measure.psrc
    parsing/accessors.psrc
    parsing/recognizers.psrc
    parsing/function_call_without_arguments.psrc
    parsing/label_in_module.psrc
    parsing/redefined_predicates.psrc
    parsing/redefined_formulas.psrc
    parsing/redefined_variables.psrc
    parsing/not_redefined_variables.psrc
    parsing/redefined_type.psrc
    parsing/redefined_modules.psrc
)

set(TESTS_INFERENCE
    sample.psrc
    inference/literals.psrc
    inference/literals_structured.psrc
    inference/literals_array.psrc
    inference/literals_negative.psrc
    inference/param_array.psrc
    inference/procedure_call.psrc
    inference/binder.psrc
    inference/lambda.psrc
    inference/struct_constructor.psrc
    inference/array_part.psrc
    inference/array_constructor.psrc
    inference/array_iterator.psrc
    inference/arithmetic_overflow.psrc
    inference/pragmas.psrc
    inference/predicate_ref.psrc
    inference/union_monotonicity.psrc
    inference/union_bounds.psrc
    inference/named_reference_type_restoring.psrc
    inference/union_type.psrc
    inference/quantified_formula_type.psrc
    inference/ternary_if_type.psrc
    inference/formula_bounded_variables.psrc
    inference/recognizers.psrc
    inference/accessors.psrc
    inference/recursive_types.psrc
    inference/replacement.psrc
)

set(TESTS_REWRITING
    rewriting/named_references.psrc
)

set(TESTS_PRETTYPRINTER
    prettyprinter/simply_formulas.psrc
    prettyprinter/call_branch_label_after_branch_arg.psrc
    prettyprinter/hyperfunction_call.psrc
    prettyprinter/jumps_and_labels.psrc
    prettyprinter/labels_and_simply_operators.psrc
    prettyprinter/modules_and_pred_in_pred.psrc
    prettyprinter/type_and_variable_decls.psrc
    prettyprinter/var_decl_in_call.psrc
    prettyprinter/array_constructor.psrc
    prettyprinter/array_iterator.psrc
    prettyprinter/binder.psrc
    prettyprinter/cast.psrc
    prettyprinter/field_expr.psrc
    prettyprinter/map_constructor.psrc
    prettyprinter/predicate_type.psrc
    prettyprinter/struct_constructor.psrc
    prettyprinter/types.psrc
    prettyprinter/union_constructor.psrc
    prettyprinter/union_element.psrc
    prettyprinter/decls_order.psrc
    prettyprinter/identifier_path.psrc
    prettyprinter/module_ordering.psrc
)

set(TESTS_CALLGRAPH
    callgraph/base_test.psrc
    callgraph/multiple_calls.psrc
    callgraph/lambda.psrc
    callgraph/functioncall.psrc
    callgraph/call_results.psrc
    callgraph/call_arguments.psrc
    callgraph/same_names.psrc
    callgraph/printing_graph_SCC.psrc
)

set(TESTS_SEMANTICS
    semantics/sample1.psrc
    semantics/sample2.psrc
    semantics/switch_real.psrc
    semantics/arCons.psrc
    semantics/arMod.psrc
    semantics/compat.psrc
    semantics/arSum.psrc
    semantics/natSum.psrc
)

set(TESTS_TRASFORMATION_TAIL_RECURSION
    transformation/tail_recursion/tail_recursion.psrc
)

set(TESTS_TRASFORMATION_PREDICATE_INLINING
    transformation/predicate_inlining/hyperfunction.psrc
)

set(TESTS_STATEMENT_TREE_MATCHING
    statement_tree/example.psrc
)

set(TESTS_STATEMENT_TREE
    statement_tree/moving_out_call.psrc
)

set(TESTS_MOVING_OUT
    moving_out/example.psrc
    moving_out/array_constructor.psrc
)

set(TESTS_CONDITIONS
    semantics/example.psrc
)

set(TESTS_VERIFICATION
    verification/if_call.psrc
    verification/logic.psrc
    verification/logic_call.psrc
    verification/split.psrc
    verification/submodules.psrc
    verification/switch.psrc
)

set(TESTS_PVS
    backend_pvs/enum.psrc
    backend_pvs/variable.psrc
    backend_pvs/formula.psrc
    backend_pvs/list.psrc
    backend_pvs/set.sprc
    backend_pvs/array.psrc
)

set(TESTS_CVC3
    cvc3/literals.psrc
    cvc3/types.psrc
    cvc3/expressions.psrc
    cvc3/replacement.psrc
)

set(TESTS_INDENTING_STREAM
    indenting_stream/custom_indentation
    indenting_stream/custom_level
    indenting_stream/indent_underflow
    indenting_stream/indent_unindent
    indenting_stream/inline
    indenting_stream/inline_newline
    indenting_stream/inline_trim
    indenting_stream/midline_indent
    indenting_stream/newline
    indenting_stream/single_line
    indenting_stream/skip_line
    indenting_stream/two_lines
    indenting_stream/verbatim
)

run_tests("-t0" "${TESTS_PARSING}")
run_tests("-t2" "${TESTS_INFERENCE}")
run_tests("-t3" "${TESTS_REWRITING}")
run_tests("-t1" "${TESTS_SEMANTICS}")
run_pp_tests("-t0" "${TESTS_PRETTYPRINTER}")

find_program(SCCMAP sccmap)

if(SCCMAP)
    run_cg_tests("-t2" "${TESTS_CALLGRAPH}")
else()
    message(STATUS "WARNING: 'sccmap' was not found. Not generating some tests.")
endif()

run_st_matching("" "${TESTS_STATEMENT_TREE_MATCHING}")
run_verification_tests("-t" "${TESTS_STATEMENT_TREE}")
run_tests("-m -t1" "${TESTS_MOVING_OUT}")
run_verification_tests("-f" "verification/conjunctive_normal_form.psrc")
run_cond_tests("" "${TESTS_CONDITIONS}")
run_vf_tests("-t1" "${TESTS_VERIFICATION}")
run_pvs_tests("-t1" "${TESTS_PVS}")
run_cvc3_tests("" "${TESTS_CVC3}")
run_ot_tr_tests("-t2" "${TESTS_TRASFORMATION_TAIL_RECURSION}")
run_ot_pi_tests("-t2" "${TESTS_TRASFORMATION_PREDICATE_INLINING}")
run_indenting_stream_tests("${TESTS_INDENTING_STREAM}")
