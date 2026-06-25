
function(compile_shader SHADER_SOURCE SHADER_OUTPUT TARGET_NAME)    
    get_filename_component(OUTPUT_DIR ${SHADER_OUTPUT} DIRECTORY)
    file(MAKE_DIRECTORY ${OUTPUT_DIR})

    add_custom_command(
        OUTPUT ${SHADER_OUTPUT}
        COMMAND $<TARGET_FILE:ShaderCompiler>
            --input ${SHADER_SOURCE}
            --output ${SHADER_OUTPUT}
            --target ${TARGET_NAME}
            --format h
        DEPENDS
            ${SHADER_SOURCE} 
            ShaderCompiler
        COMMENT "Compiling shader ${SHADER_SOURCE} to ${SHADER_OUTPUT}"
    )

    set(${OUTPUT_SHADER_FILE} ${SHADER_OUTPUT} PARENT_SCOPE)
endfunction()

file(GLOB_RECURSE SHADER_SOURCES 
    "${PROJECT_SOURCE_DIR}/Shaders/*.vert"
    "${PROJECT_SOURCE_DIR}/Shaders/*.frag"
)

set(GENERATED_SHADER_HEADERS "")

foreach(SOURCE_FILE ${SHADER_SOURCES})
    get_filename_component(FILE_NAME ${SOURCE_FILE} NAME)
    
    if(APPLE)
        set(API "metal")
    elseif(WIN32)
        set(API "dxil")
    else()
        set(API "vulkan")
    endif()

    set(OUTPUT_SHADER_FILE "${CMAKE_CURRENT_BINARY_DIR}/generated/Shaders/${FILE_NAME}.h")
    
    compile_shader(${SOURCE_FILE} ${OUTPUT_SHADER_FILE} ${API})
    
    list(APPEND GENERATED_SHADER_HEADERS ${OUTPUT_SHADER_FILE})
endforeach()

add_custom_target(FundamentalShaders DEPENDS ${GENERATED_SHADER_HEADERS})