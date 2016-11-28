#包含的源文件目录
INCLUDE_DIRECTORIES(
  ${PROJECT_SOURCE_DIR}
  
  /usr/local/include
  /usr/include
)

#包含的库文件目录
LINK_DIRECTORIES(
  ${PROJECT_BINARY_DIR}/lib

  /usr/local/lib
  /usr/lib
)

IF(WIN32)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
ENDIF()

#测试
ADD_EXECUTABLE(UnitTest unittest/main.cpp)

TARGET_LINK_LIBRARIES(UnitTest SimpleSocket Common)