# 定义编译器和通用选项
CXX = g++
CXXFLAGS = -Wall -std=c++17 -g
SRC = main.cpp

# --- 自动识别操作系统 ---

# 1. 检测 Windows (Windows_NT 是 Windows 的环境变量)
ifeq ($(OS),Windows_NT)
    # Windows 下的配置
    TARGET = main.exe
    # Windows 需要链接 winmm 库
    LDFLAGS = -lwinmm
    RM = del /Q
else
    # 2. 检测非 Windows (Mac 或 Linux)
    UNAME_S := $(shell uname -s)
    
    ifeq ($(UNAME_S),Darwin)
        # macOS 下的配置
        TARGET = main
        # Mac 不需要 winmm，如果有其他库写在这里
        LDFLAGS = 
        RM = rm -f
    else
        # Linux 下的配置 (预留)
        TARGET = main
        LDFLAGS = 
        RM = rm -f
    endif
endif

# --- 编译规则 ---

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	$(RM) $(TARGET)