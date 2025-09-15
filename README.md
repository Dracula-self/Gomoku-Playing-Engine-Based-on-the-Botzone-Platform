# Gomoku-Playing-Engine-Based-on-the-Botzone-Platform
Gomoku Playing Engine Based on the Botzone Platform

# 五子棋AI引擎

一个基于Minimax算法和Alpha-Beta剪枝的五子棋AI实现。

## 功能特点

- 15x15标准棋盘
- Minimax算法 + Alpha-Beta剪枝
- 支持22种棋型识别
- 最大搜索深度11层
- Zobrist哈希优化

## 核心文件

- `gomoku.cpp` - 完整的AI引擎实现

## 主要函数

```cpp
void initial_type();                    // 初始化棋型
void initialTypescore();               // 初始化分值
int deepSearch(...);                   // 搜索最佳走法
int evaluate(Point now);               // 评估位置价值
int getScore(Point now, int direction, int turn); // 棋型识别
```

## 棋型分值

- 五连：100,000,000分
- 活四：999,999分  
- 冲四：30,000分
- 活三：10,500分
- 活二：1,075分
- 活一：50分

## 算法配置

```cpp
int MaxDepth = 11;                     // 搜索深度
double threshold = 0.95 * CLOCKS_PER_SEC; // 时间限制
const int SIZE = 15;                   // 棋盘大小
```

## 使用说明

这是一个AI引擎库，需要配合界面程序使用。主要接口包括局面评估、走法搜索和棋型识别功能。
