project(FacelessPerson)
cmake_minimum_required(VERSION 2.8.12)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# OSX only config (might work UNIX too, not tested)
if(APPLE)
    # add c++11 flags
    add_compile_options(-std=c++11)
    # use OpenCV dynamic libraries instead of static: there's an issue compiling OpenCV static libraries (ld: library not found for -lcrt0.o)
    find_package( OpenCV REQUIRED )
endif(APPLE)

include_directories("3dparty/sdm/src/include")
include_directories("3dparty/sdm/src/test")
include_directories("3dparty/sdm/src/test")
aux_source_directory("3dparty/sdm/src/include" SOURCES_FILE)

install(FILES 3dparty/sdm/example/haar_roboman_ff_alt2.xml DESTINATION ${CMAKE_BINARY_DIR} )
install(FILES 3dparty/sdm/example/roboman-landmark-model.bin DESTINATION ${CMAKE_BINARY_DIR} )

ADD_EXECUTABLE(FacelessPerson  src/main.cpp)
TARGET_LINK_LIBRARIES(FacelessPerson ${OpenCV_LIBS})




