
# ====================== build gemm executables ======================
file(GLOB GEMM_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/gemm/*.cpp")
foreach(src ${GEMM_SOURCES})
    get_filename_component(target_name ${src} NAME_WE)
    add_executable(${target_name} ${src})
    set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/gemm")
    target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${TOP_INCLUDE_DIR})
endforeach()

# ====================== build gemv executables ======================
file(GLOB GEMV_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/gemv/*.cpp")
foreach(src ${GEMV_SOURCES})
    get_filename_component(target_name ${src} NAME_WE)
    add_executable(${target_name} ${src})
    set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/gemv")
    target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${TOP_INCLUDE_DIR})
endforeach()


