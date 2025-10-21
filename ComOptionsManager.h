// Copyright (c) 2024, 电子科技大学电子科学与工程学院，计算机仿真技术实验室
// All rights reserved.
// 文件名称：ComOptionsManager.h
// 摘    要：配置选项管理类，用于读取、设置、配置和输出 ComConstants.h 中的所有最上层 Options
// 当前版本：1.0
// 作    者：邓龙威
// 完成日期：2025年10月20日

#ifndef EMMPMESH_COMMON_COMOPTIONSMANAGER_H_
#define EMMPMESH_COMMON_COMOPTIONSMANAGER_H_

#include "ComConstants.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

// 网格生成质量级别枚举
enum class MeshQualityLevel
{
    VeryFast,       // 极快生成，质量较低
    Fast,           // 快速生成，质量一般
    Balanced,       // 平衡模式，速度和质量兼顾
    HighQuality,    // 高质量，速度较慢
    VeryHighQuality // 极高质量，速度很慢
};

// 配置选项管理类
class ComOptionsManager
{
public:
    /************************************************************************
    * 功能描述：默认构造函数，初始化所有选项为默认值
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    ComOptionsManager();

    /************************************************************************
    * 功能描述：析构函数
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    ~ComOptionsManager();

    /************************************************************************
    * 功能描述：禁止拷贝和移动
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    ComOptionsManager(const ComOptionsManager &) = delete;
    ComOptionsManager &operator=(const ComOptionsManager &) = delete;
    ComOptionsManager(ComOptionsManager &&) = delete;
    ComOptionsManager &operator=(ComOptionsManager &&) = delete;

    // =============================== 快速设置预设级别 ===============================

    /************************************************************************
    * 功能描述：根据预设的网格质量级别设置所有相关选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void setQualityLevel(MeshQualityLevel level);

    /************************************************************************
    * 功能描述：获取当前的质量级别（如果是自定义设置则返回 Balanced）
    * 返回值：MeshQualityLevel
    * 作者：邓龙威
    /************************************************************************/
    MeshQualityLevel getQualityLevel() const { return currentLevel_; }

    /************************************************************************
    * 功能描述：将质量级别转换为字符串描述
    * 返回值：std::string
    * 作者：邓龙威
    /************************************************************************/
    std::string qualityLevelToString(MeshQualityLevel level) const;

    // =============================== 设置选项函数 ===============================

    /************************************************************************
    * 功能描述：设置线搜索选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void setLineSearchOptions(const LineSearchOptions &lsOpts) { lineSearchOptions_ = lsOpts; }

    /************************************************************************
    * 功能描述：设置梯度下降法选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void setGradientDescentOptions(const GradientDescentOptions &gdOpts) { gradientDescentOptions_ = gdOpts; }

    /************************************************************************
    * 功能描述：设置共轭梯度法选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void setConjugateGradientOptions(const ConjugateGradientOptions &cgOpts) { conjugateGradientOptions_ = cgOpts; }

    /************************************************************************
    * 功能描述：设置拟牛顿法选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void setQuasiNewtonOptions(const QuasiNewtonOptions &qnOpts) { quasiNewtonOptions_ = qnOpts; }

    /************************************************************************
    * 功能描述：设置网格优化算法选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void setMeshOptiAlgorithmOptions(const MeshOptiAlgorithmOptions &moaOpts) { meshOptiAlgorithmOptions_ = moaOpts; }

    /************************************************************************
    * 功能描述：设置网格优化选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void setMeshOptiOptions(const MeshOptiOptions &moOpts) { meshOptiOptions_ = moOpts; }

    /************************************************************************
    * 功能描述：设置 TetView IO 选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void setMeshTetViewIOOptions(const MeshTetViewIOOptions &tvOpts) { meshTetViewIOOptions_ = tvOpts; }

    /************************************************************************
    * 功能描述：设置 VTK IO 选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void setMeshVTKIOOptions(const MeshVTKIOOptions &vtkOpts) { meshVTKIOOptions_ = vtkOpts; }

    /************************************************************************
    * 功能描述：设置 OBJ IO 选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void setMeshOBJIOOptions(const MeshOBJIOOptions &objOpts) { meshOBJIOOptions_ = objOpts; }

    /************************************************************************
    * 功能描述：设置 STL IO 选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void setMeshSTLIOOptions(const MeshSTLIOOptions &stlOpts) { meshSTLIOOptions_ = stlOpts; }

    /************************************************************************
    * 功能描述：设置 PLY IO 选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void setMeshPLYIOOptions(const MeshPLYIOOptions &plyOpts) { meshPLYIOOptions_ = plyOpts; }

    /************************************************************************
    * 功能描述：设置 OFF IO 选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void setMeshOFFIOOptions(const MeshOFFIOOptions &offOpts) { meshOFFIOOptions_ = offOpts; }

    /************************************************************************
    * 功能描述：设置 MSH IO 选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void setMeshMSHIOOptions(const MeshMSHIOOptions &mshOpts) { meshMSHIOOptions_ = mshOpts; }

    /************************************************************************
    * 功能描述：设置网格 IO 选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void setMeshIOOptions(const MeshIOOptions &ioOpts) { meshIOOptions_ = ioOpts; }

    /************************************************************************
    * 功能描述：设置日志文件选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void setLogFileOptions(const LogFileOptions &logOpts) { logFileOptions_ = logOpts; }

    // =============================== 获取选项函数 ===============================

    /************************************************************************
    * 功能描述：获取线搜索选项
    * 返回值：const LineSearchOptions& - 线搜索选项的常量引用
    * 作者：邓龙威
    /************************************************************************/
    const LineSearchOptions &getLineSearchOptions() const { return lineSearchOptions_; }

    /************************************************************************
    * 功能描述：获取梯度下降法选项
    * 返回值：const GradientDescentOptions& - 梯度下降法选项的常量引用
    * 作者：邓龙威
    /************************************************************************/
    const GradientDescentOptions &getGradientDescentOptions() const { return gradientDescentOptions_; }

    /************************************************************************
    * 功能描述：获取共轭梯度法选项
    * 返回值：const ConjugateGradientOptions& - 共轭梯度法选项的常量引用
    * 作者：邓龙威
    /************************************************************************/
    const ConjugateGradientOptions &getConjugateGradientOptions() const { return conjugateGradientOptions_; }

    /************************************************************************
    * 功能描述：获取拟牛顿法选项
    * 返回值：const QuasiNewtonOptions& - 拟牛顿法选项的常量引用
    * 作者：邓龙威
    /************************************************************************/
    const QuasiNewtonOptions &getQuasiNewtonOptions() const { return quasiNewtonOptions_; }

    /************************************************************************
    * 功能描述：获取网格优化算法选项
    * 返回值：const MeshOptiAlgorithmOptions& - 网格优化算法选项的常量引用
    * 作者：邓龙威
    /************************************************************************/
    const MeshOptiAlgorithmOptions &getMeshOptiAlgorithmOptions() const { return meshOptiAlgorithmOptions_; }

    /************************************************************************
    * 功能描述：获取网格优化选项
    * 返回值：const MeshOptiOptions& - 网格优化选项的常量引用
    * 作者：邓龙威
    /************************************************************************/
    const MeshOptiOptions &getMeshOptiOptions() const { return meshOptiOptions_; }

    /************************************************************************
    * 功能描述：获取 TetView IO 选项
    * 返回值：const MeshTetViewIOOptions& - TetView IO 选项的常量引用
    * 作者：邓龙威
    /************************************************************************/
    const MeshTetViewIOOptions &getMeshTetViewIOOptions() const { return meshTetViewIOOptions_; }

    /************************************************************************
    * 功能描述：获取 VTK IO 选项
    * 返回值：const MeshVTKIOOptions& - VTK IO 选项的常量引用
    * 作者：邓龙威
    /************************************************************************/
    const MeshVTKIOOptions &getMeshVTKIOOptions() const { return meshVTKIOOptions_; }

    /************************************************************************
    * 功能描述：获取 OBJ IO 选项
    * 返回值：const MeshOBJIOOptions& - OBJ IO 选项的常量引用
    * 作者：邓龙威
    /************************************************************************/
    const MeshOBJIOOptions &getMeshOBJIOOptions() const { return meshOBJIOOptions_; }

    /************************************************************************
    * 功能描述：获取 STL IO 选项
    * 返回值：const MeshSTLIOOptions& - STL IO 选项的常量引用
    * 作者：邓龙威
    /************************************************************************/
    const MeshSTLIOOptions &getMeshSTLIOOptions() const { return meshSTLIOOptions_; }

    /************************************************************************
    * 功能描述：获取 PLY IO 选项
    * 返回值：const MeshPLYIOOptions& - PLY IO 选项的常量引用
    * 作者：邓龙威
    /************************************************************************/
    const MeshPLYIOOptions &getMeshPLYIOOptions() const { return meshPLYIOOptions_; }

    /************************************************************************
    * 功能描述：获取 OFF IO 选项
    * 返回值：const MeshOFFIOOptions& - OFF IO 选项的常量引用
    * 作者：邓龙威
    /************************************************************************/
    const MeshOFFIOOptions &getMeshOFFIOOptions() const { return meshOFFIOOptions_; }

    /************************************************************************
    * 功能描述：获取 MSH IO 选项
    * 返回值：const MeshMSHIOOptions& - MSH IO 选项的常量引用
    * 作者：邓龙威
    /************************************************************************/
    const MeshMSHIOOptions &getMeshMSHIOOptions() const { return meshMSHIOOptions_; }

    /************************************************************************
    * 功能描述：获取网格 IO 选项
    * 返回值：const MeshIOOptions& - 网格 IO 选项的常量引用
    * 作者：邓龙威
    /************************************************************************/
    const MeshIOOptions &getMeshIOOptions() const { return meshIOOptions_; }

    /************************************************************************
    * 功能描述：获取日志文件选项
    * 返回值：const LogFileOptions& - 日志文件选项的常量引用
    * 作者：邓龙威
    /************************************************************************/
    const LogFileOptions &getLogFileOptions() const { return logFileOptions_; }

    // =============================== 文件读写函数 ===============================

    /************************************************************************
    * 功能描述：从配置文件读取所有选项
    * 返回值：0 表示成功，非 0 表示失败
    * 作者：邓龙威
    /************************************************************************/
    int loadFromFile(const std::string &filePath);

    /************************************************************************
    * 功能描述：保存所有选项到配置文件
    * 返回值：0 表示成功，非 0 表示失败
    * 作者：邓龙威
    /************************************************************************/
    int saveToFile(const std::string &filePath) const;

    // =============================== 输出函数 ===============================

    /************************************************************************
    * 功能描述：打印所有选项到控制台
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void printAllOptions() const;

    /************************************************************************
    * 功能描述：打印线搜索选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void printLineSearchOptions() const;

    /************************************************************************
    * 功能描述：打印梯度下降法选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void printGradientDescentOptions() const;

    /************************************************************************
    * 功能描述：打印共轭梯度法选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void printConjugateGradientOptions() const;

    /************************************************************************
    * 功能描述：打印拟牛顿法选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void printQuasiNewtonOptions() const;

    /************************************************************************
    * 功能描述：打印网格优化算法选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void printMeshOptiAlgorithmOptions() const;

    /************************************************************************
    * 功能描述：打印网格优化选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void printMeshOptiOptions() const;

    /************************************************************************
    * 功能描述：打印 TetView IO 选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void printMeshTetViewIOOptions() const;

    /************************************************************************
    * 功能描述：打印 VTK IO 选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void printMeshVTKIOOptions() const;

    /************************************************************************
    * 功能描述：打印 OBJ IO 选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void printMeshOBJIOOptions() const;

    /************************************************************************
    * 功能描述：打印 STL IO 选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void printMeshSTLIOOptions() const;

    /************************************************************************
    * 功能描述：打印 PLY IO 选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void printMeshPLYIOOptions() const;

    /************************************************************************
    * 功能描述：打印 OFF IO 选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void printMeshOFFIOOptions() const;

    /************************************************************************
    * 功能描述：打印 MSH IO 选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void printMeshMSHIOOptions() const;

    /************************************************************************
    * 功能描述：打印网格 IO 选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void printMeshIOOptions() const;

    /************************************************************************
    * 功能描述：打印日志文件选项
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void printLogFileOptions() const;

    // =============================== 重置函数 ===============================

    /************************************************************************
    * 功能描述：重置所有选项为默认值
    * 返回值：无
    * 作者：邓龙威
    /************************************************************************/
    void resetToDefaults();

private:
    /************************************************************************
    * 功能描述：将枚举转换为字符串
    * 返回值：std::string
    * 作者：邓龙威
    /************************************************************************/
    std::string lineSearchWayToString(LineSearchWay way) const;
    std::string calculateWayToString(CalculateWay way) const;
    std::string meshSmoothTypeToString(MeshSmoothType type) const;
    std::string meshSmoothWayToString(MeshSmoothWay way) const;
    std::string meshQualityMetricToString(MeshQualityMetric metric) const;
    std::string meshQualityFuncToString(MeshQualityFunc func) const;
    std::string meshElementTypeToString(MeshElementType type) const;
    std::string logLevelToString(LogLevel level) const;
    std::string logFileFormatToString(LogFileFormat format) const;
    std::string logFileLanguageToString(LogFileLanguage lang) const;

    /************************************************************************
    * 功能描述：将字符串转换为枚举
    * 返回值：对应的枚举类型
    * 作者：邓龙威
    /************************************************************************/
    LineSearchWay stringToLineSearchWay(const std::string &str) const;
    CalculateWay stringToCalculateWay(const std::string &str) const;
    MeshSmoothType stringToMeshSmoothType(const std::string &str) const;
    MeshSmoothWay stringToMeshSmoothWay(const std::string &str) const;
    MeshQualityMetric stringToMeshQualityMetric(const std::string &str) const;
    MeshQualityFunc stringToMeshQualityFunc(const std::string &str) const;
    MeshElementType stringToMeshElementType(const std::string &str) const;
    LogLevel stringToLogLevel(const std::string &str) const;
    LogFileFormat stringToLogFileFormat(const std::string &str) const;
    LogFileLanguage stringToLogFileLanguage(const std::string &str) const;

    /************************************************************************
    * 功能描述：字符串处理辅助函数
    * 返回值：处理后的字符串或布尔值
    * 作者：邓龙威
    /************************************************************************/
    std::string trim(const std::string &str) const;
    bool stringToBool(const std::string &str) const;

private:
    // 存储各种选项的成员变量
    LineSearchOptions lineSearchOptions_;
    GradientDescentOptions gradientDescentOptions_;
    ConjugateGradientOptions conjugateGradientOptions_;
    QuasiNewtonOptions quasiNewtonOptions_;
    MeshOptiAlgorithmOptions meshOptiAlgorithmOptions_;
    MeshOptiOptions meshOptiOptions_;
    MeshTetViewIOOptions meshTetViewIOOptions_;
    MeshVTKIOOptions meshVTKIOOptions_;
    MeshOBJIOOptions meshOBJIOOptions_;
    MeshSTLIOOptions meshSTLIOOptions_;
    MeshPLYIOOptions meshPLYIOOptions_;
    MeshOFFIOOptions meshOFFIOOptions_;
    MeshMSHIOOptions meshMSHIOOptions_;
    MeshIOOptions meshIOOptions_;
    LogFileOptions logFileOptions_;

    // 当前质量级别
    MeshQualityLevel currentLevel_;
};

#endif // EMMPMESH_COMMON_COMOPTIONSMANAGER_H_
