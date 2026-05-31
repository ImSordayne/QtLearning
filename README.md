<!--
 * @Author: Sordayne 455248805@qq.com
 * @FilePath: \QtLearning\README.md
-->
# QtLearning 项目说明
<!-- markdownlint-disable MD033 -->
<div style="display: grid; place-items: center;">
    <img src="logo.svg" alt="logo" style="width: 256px;height: 256px;"/>
    <h1 style="font-size: 60px">QtLearning</h1>
</div>
<!-- markdownlint-enable MD033 -->

## 项目简介

这是一个 Qt 框架的学习项目，作者是一名正在学习 Qt 开发的初中生。

> 📝 **注：** 本项目是学习过程中的练习作品。部分代码由 AI 辅助生成（DeepSeek），另一部分为个人编写，代码风格可能存在不统一的情况，感谢您的理解！

## 学习目标

- 掌握 Qt 框架的基本使用
- 理解面向对象的 GUI 编程思想
- 通过实践巩固 C++ 知识
- 积累实际项目经验

## 项目结构

```text
QtLearning/
├── Widgets/                  # Qt Widgets 项目
|   ├── Base64ConverUI/       # 一个可以将文件用Base64编码转换为文本的软件
|   ├── calculator/           # 一个用Qt实现的计算器
|   ├── Letter/               # 一封特殊的信件
|   └── PerformanceDetector   # 一个查看系统信息的工具(By Deepseek)
├── Quick/                    # Qt Quick 项目
└── docs/                     # 学习笔记(待添加)
```

## 环境要求

- Qt 6.x 或更高版本
- C++11 及以上标准编译器
- CMake 构建工具
- vcpkg (可选)用于安装项目依赖,您也可以通过官方安装器手动安装

## 编译与运行

```bash
# 克隆项目
git clone https://github.com/ImSordayne/QtLearning.git

# 进入项目目录
cd QtLearning

# 生成 CMake 构建缓存
cmake -S . -B build

# 开始构建
cmake --build build

# 也可以进入 Widgets / Quick 目录逐个构建
```

## 学习资源

- [Qt 官方文档](https://doc.qt.io/)
- 还有一些其他的学习资料,可以添加我的个人QQ领取
- B站 Qt 教程视频

## 更新计划

- [ ] 基础控件学习
- [ ] 信号与槽机制
- [ ] 布局管理器实践
- [ ] 简单小项目开发（计算器、记事本等）

## 联系方式

如有建议或问题，欢迎通过以下方式联系：

- GitHub Issues: [提交问题](https://github.com/ImSordayne/QtLearning/issues)
- Email: <455248805@qq.com>
- QQ: 455248805
- 微信: haolin201204

## 许可

本项目仅供学习交流使用。

---

⭐️ 如果这个项目对你有帮助，欢迎点个 Star!
