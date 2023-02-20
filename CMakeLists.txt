
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set(PROJECT_NAME GigaInt)
project(${PROJECT_NAME} LANGUAGES CXX)
add_executable(${PROJECT_NAME} "")

# add_compile_options("-Weverything -Werror")
# target_compile_options(${PROJECT_NAME} PRIVATE "-Wall" "-Wextra")
# if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
#     target_compile_options(${PROJECT_NAME} PRIVATE ${GNU_SANITIZE_FLAGS})
#     target_link_options(${PROJECT_NAME} PRIVATE ${GNU_SANITIZE_FLAGS})
# endif()

target_sources(${PROJECT_NAME}
    PRIVATE
        src/main.cpp
        src/Watcher.cpp
        src/Watcher.hpp
        src/Logger.cpp
        src/Logger.hpp
        src/DotLogger.cpp
        src/DotLogger.hpp
        src/Num.hpp
        src/Num.cpp
)


if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -O1 -fPIC -Weverything  -Wno-implicit-int-conversion -Wno-documentation -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-weak-vtables -Wno-sign-conversion -Wno-covered-switch-default -Werror=return-type)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # add_compile_options(-Wall -Wextra -Wno-error=padded -Wno-weak-vtables)
    set (GNU_SANITIZE_FLAGS
        -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
    )
    target_compile_options(${PROJECT_NAME} PRIVATE -O1 -ggdb3 -Wall -Wextra -Wpedantic -Werror=return-type
        -fno-elide-constructors
        #  -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE 
        ${GNU_SANITIZE_FLAGS}
    )
    target_link_options(${PROJECT_NAME} PRIVATE -rdynamic -fno-elide-constructors)
    target_link_options(${PROJECT_NAME} PRIVATE ${GNU_SANITIZE_FLAGS})
endif()

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)