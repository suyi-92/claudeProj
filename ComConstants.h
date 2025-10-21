#ifndef EMMPMESH_COMMON_COMCONSTANTS_H_
#define EMMPMESH_COMMON_COMCONSTANTS_H_

#include "ComPoint.h"

#include <string>
#include <string_view>
#include <unordered_set>
#include <unordered_map>
#include <any>
#include <functional>
#include <stdexcept>
#include <memory>
#include <typeindex>
#include <iostream>
#include <map>

#define STRLEN 512
#define ZERO_1 1.0e-1
#define ZERO_2 1.0e-2
#define ZERO_3 1.0e-3
#define ZERO_4 1.0e-4
#define ZERO_6 1.0e-6
#define ZERO_7 1.0e-7
#define ZERO_9 1.0e-9
#define ZERO_10 1.0e-10
#define ZERO_15 1.0e-15
#define ZERO_20 1.0e-20
#define ZERO_30 1.0e-30

#define PI 3.1415926535897932385

enum EmMpMeshCoordinateSystemType
{                             //坐标系类型
    EMMPMESH_RECTANGULA = 0,  //直角坐标系
    EMMPMESH_CYLINDRICAL = 1, //柱坐标系
    EMMPMESH_SPHERICAL = 2,   //球坐标系
};

enum EmMpMeshUnitAngle
{ //角度单位枚举
    EMMPMESH_RAD,
    EMMPMESH_DEG,
    EMMPMESH_DEGSEC,
    EMMPMESH_DEGMIN,
};

enum EmMpMeshUnitDistance
{ //距离单位枚举
    EMMPMESH_KM,
    EMMPMESH_M,
    EMMPMESH_DM,
    EMMPMESH_CM,
    EMMPMESH_MM,
    EMMPMESH_UM,
    EMMPMESH_NM,
    EMMPMESH_INCH,
    EMMPMESH_UINCH,
    MEMMPMESH_MIL,
};

enum EmMpMeshLanguageType
{
    EMMPMESH_CHINESE = 0,
    EMMPMESH_ENGLISH = 1,
};

enum EmMpMeshEGEntityType
{
    EMMPMESH_EG_UNKNOW = 0,
    EMMPMESH_EG_REGION = 1,
    EMMPMESH_EG_FACE = 2,
    EMMPMESH_EG_EDGE = 3,
    EMMPMESH_EG_VERTER = 4,
    EMMPMESH_EG_LOOP = 5,
    EMMPMESH_EG_COEDGE = 6,
};

enum EmMpMeshEGFaceType
{
    EMMPMESH_EG_FACE_UNKNOWN = 0,
    EMMPMESH_EG_FACE_INNER = 1,    //face with two cfaces, and they are belonging to same cshell
    EMMPMESH_EG_FACE_BOUNDARY = 2, //face with only one cface
    EMMPMESH_EG_FACE_INTERNAL = 3, //face with two cfaces, but they are belonging to two cshell
};

enum EmMpMeshEGPeriodType
{
    EMMPMESH_EG_PERIOD_NON = 0, //u，v都是非周期
    EMMPMESH_EG_PERIOD_U = 1,   //u周期，v非周期
    EMMPMESH_EG_PERIOD_V = 2,   //u非周期，v周期
    EMMPMESH_EG_PERIOD_UV = 3,  //u周期，v周期
};

enum EmMpMeshEGNoClosedType
{
    EMMPMESH_EG_NOCLOSED_NON = 0, //都是封闭的
    EMMPMESH_EG_NOCLOSED_E = 1,   //外环上非封闭线存在,且不是周期线
    EMMPMESH_EG_NOCLOSED_I = 2,   // 内环上的非封闭线
    EMMPMESH_EG_NOCLOSED_EI = 3,  //都存在

};

// 不允许不初始化使用
// 临时变量，初始化案例：ComboFourInt a{};
// 类成员变量，在构造函数初始化  region_ = {};

struct ComboToInt
{
    int value_[2];
    ComboToInt()
    {
        value_[0] = -1;
        value_[1] = -1;
    }
    ComboToInt(const int &value1, const int &value2)
    {
        value_[0] = value1;
        value_[1] = value2;
    }
};

struct ComboThreeInt
{
    int value_[3];
    ComboThreeInt()
    {
        value_[0] = -1;
        value_[1] = -1;
        value_[2] = -1;
    }
    ComboThreeInt(const int &value1, const int &value2, const int &value3)
    {
        value_[0] = value1;
        value_[1] = value2;
        value_[2] = value3;
    }
};

struct ComboFourInt
{
    int value_[4];
    ComboFourInt()
    {
        value_[0] = -1;
        value_[1] = -1;
        value_[2] = -1;
        value_[3] = -1;
    }
    ComboFourInt(const int &value1, const int &value2, const int &value3, const int &value4)
    {
        value_[0] = value1;
        value_[1] = value2;
        value_[2] = value3;
        value_[3] = value4;
    }
};

struct ComboSixInt
{
    int value_[6];
    ComboSixInt()
    {
        value_[0] = -1;
        value_[1] = -1;
        value_[2] = -1;
        value_[3] = -1;
        value_[4] = -1;
        value_[5] = -1;
    }
    ComboSixInt(const int &value1, const int &value2, const int &value3, const int &value4, const int &value5, const int &value6)
    {
        value_[0] = value1;
        value_[1] = value2;
        value_[2] = value3;
        value_[3] = value4;
        value_[4] = value5;
        value_[5] = value6;
    }
};

// 线段结构体
struct ComLine
{
    ComPoint point_[2];
};

// 三角形结构体
struct ComTriangle
{
    ComPoint point_[3];
};

struct ComTetrahedron
{
    ComPoint point_[4];
};

// 线段结构体
struct ComToLine
{
    ComToPoint point_[2];
};

// 三角形结构体
struct ComToTriangle
{
    ComToPoint point_[3];
};

struct ComToTetrahedron
{
    ComToPoint point_[4];
};

struct LoopCoedge
{
    int edgeIDFromEG_;
    int isReverse_;         // 0:正向 1:反向
    bool isSingularity_[2]; // COEDGE正向上的起点和终点是否为奇点

    LoopCoedge()
        : edgeIDFromEG_(-1),
          isReverse_(-1),
          isSingularity_{false, false}
    {
    }

    LoopCoedge(const int &edgeIDFromEG, const int &isReverse, const bool &isSingularity1, const bool &isSingularity2)
        : edgeIDFromEG_(edgeIDFromEG),
          isReverse_(isReverse),
          isSingularity_{isSingularity1, isSingularity2}
    {
    }
};

// =============================== 模型文件选项 ===============================

// 模型文件格式
enum class ModelFileFormat
{
    SAT = 0,  // ACIS SAT 文件格式
    STEP = 1, // STEP 文件格式
};

// ACIS SAT 文件输出主、次版本号
struct AcisSatFileVersion
{
    int majorVersion_ = 27; // 主版本号
    int minorVersion_ = 0;  // 次版本号
};

// 模型文件相关选项
struct ModelFileOptions
{
    ModelFileFormat fileFormat_;    // 模型文件格式
    std::string filePath_;          // 模型文件路径
    std::string fileName_;          // 模型文件名（不含路径和扩展名）
    AcisSatFileVersion satVersion_; // ACIS SAT 文件输出主、次版本号
};

// =============================== 网格生成选项 ===============================

// 自适应网格选项
struct AdaptiveMeshOptions
{
    double sampleSize_ = 0.1;          // 采样尺寸
    double curvatureAngle_ = 20;       // 曲率角度（度）
    double minSize_ = 1.;              // 最小网格尺寸
    double maxSize_ = 10.;             // 最大网格尺寸
    double refinementFactor_ = 0.5;    // 细化因子
    double ratioFactor_ = 1.35;        // 比率因子
    int selfAdaption_ = 0;             // 自适应标志：0-禁用，1-启用
    double highCurvatureSampling_ = 0; // 高曲率采样密度
};

// 线网格生成选项
struct EdgeMeshGenerationOptions
{
};

// 面网格生成选项
struct SurfMeshGenerationOptions
{
    bool selfAdaption_ = true;     // 自适应曲面网格生成
    bool meshOptimization_ = true; // 网格质量优化（每生成一个模型曲面网格后进行优化）
    bool efficiency_ = true;       // 高效率生成
};

// 体网格生成选项
struct VolumMeshGenerationOptions
{
    bool meshOptimization_ = true; // 网格质量优化（每生成一个模型体网格后进行优化）
};

// 网格生成选项
struct MeshGenerationOptions
{
    AdaptiveMeshOptions adaptiveMeshOptions_;               // 自适应网格选项
    EdgeMeshGenerationOptions edgeMeshGenerationOptions_;   // 线网格生成选项
    SurfMeshGenerationOptions surfMeshGenerationOptions_;   // 面网格生成选项
    VolumMeshGenerationOptions volumMeshGenerationOptions_; // 体网格生成选项
};

// =============================== 网格优化选项 ===============================

// 网格元素类型：顶点、边、面、体
enum class MeshElementType
{
    Vertex = 0, // 顶点
    Edge = 1,   // 边
    Face = 2,   // 面
    Region = 3, // 体
};
inline const int MeshElementTypeNum = 4;

// 网格质量度量准则
enum class MeshQualityMetric
{
    EDGE_RATIO,                // 边长比
    ASPECT_DELTA,              // 纵横比（Delta）
    ASPECT_FROBENIUS,          // 纵横比（Frobenius范数）
    ASPECT_GAMMA,              // 纵横比（gamma）
    ASPECT_RATIO,              // 纵横比（Aspect Ratio）
    COLLAPSE_RATIO,            // 折叠比率
    WEIGHTED_CONDITION_NUMBER, // 加权条件数
    DISTORTION,                // 变形度量
    JACOBIAN,                  // 雅可比矩阵
    MIN_ANGLE,                 // 最小二面角
    AVERAGE_ANGLE,             // 平均二面角
    RADIUS_RATIO,              // 半径比率（Radius Ratio）
    RELATIVE_SIZE_SQUARED,     // 相对大小（Relative Size Squared）
    SCALED_JACOBIAN,           // 缩放雅可比矩阵
    SHAPE,                     // 网格形状度量（Shape Metric）
    SHAPE_AND_SIZE,            // 网格形状和相对大小度量（Shape and Size）
    VOLUME,                    // 体积（Volume）
    SKEWNESS,                  // 网格偏斜度，以体积定义。基于均方根边长
    SKEWNESS_TRI_ANGLE,        // 网格偏斜度，以四面体面的三角形角度定义
    SKEWNESS_DIHE_ANGLE,       // 网格偏斜度，以四面体的二面角定义
    EQUIVOLUME_SKEWNESS,       // 网格偏斜度，以体积定义。基于外接球半径
    EQUIANGLE_SKEWNESS,        // 网格偏斜度，以体积定义。基于以四面体面的三角形角度+四面体的二面角定义
    VOL_LEN_RATIO,             // 体积-长度比
};

// 网格质量函数
enum class MeshQualityFunc
{
    EDGE_RATIO_FUNC,                // 边长比函数
    ASPECT_DELTA_FUNC,              // 纵横比（Delta）函数
    ASPECT_FROBENIUS_FUNC,          // 纵横比（Frobenius范数）函数
    ASPECT_GAMMA_FUNC,              // 纵横比（gamma）函数
    ASPECT_RATIO_FUNC,              // 纵横比（Aspect Ratio）函数
    COLLAPSE_RATIO_FUNC,            // 【未实现】折叠比率函数
    WEIGHTED_CONDITION_NUMBER_FUNC, // 加权条件数函数
    DISTORTION_FUNC,                // 【未实现】变形度量函数
    JACOBIAN_FUNC,                  // 雅可比矩阵函数。不建议单独使用
    MIN_ANGLE_FUNC,                 // 最小二面角函数
    AVERAGE_ANGLE_FUNC,             // 【未实现】平均二面角函数
    RADIUS_RATIO_FUNC,              // 半径比率（Radius Ratio）函数
    RELATIVE_SIZE_SQUARED_FUNC,     // 【未实现】相对大小（Relative Size Squared）函数
    SCALED_JACOBIAN_FUNC,           // 缩放雅可比矩阵函数
    SHAPE_FUNC,                     // 网格形状度量（Shape Metric）函数
    SHAPE_AND_SIZE_FUNC,            // 【未实现】网格形状和相对大小度量（Shape and Size）函数
    VOLUME_FUNC,                    // 体积（Volume）函数。不建议单独使用
    SKEWNESS_FUNC,                  // 网格偏斜度函数，以体积定义。基于均方根边长
    SKEWNESS_TRI_ANGLE_FUNC,        // 【未实现】网格偏斜度函数，以四面体面的三角形角度定义
    SKEWNESS_DIHE_ANGLE_FUNC,       // 【未实现】网格偏斜度函数，以四面体的二面角定义
    EQUIVOLUME_SKEWNESS_FUNC,       // 网格偏斜度函数，以体积定义。基于外接球半径
    EQUIANGLE_SKEWNESS_FUNC,        // 【未实现】网格偏斜度函数，以体积定义。基于以四面体面的三角形角度+四面体的二面角定义
    VOL_LEN_RATIO_FUNC,             // 体积-长度比函数
};

// 目标函数计算方式
enum class CalculateWay
{
    SUM,          // 空腔所有网格质量函数加和
    MAX,          // 空腔所有网格质量函数最大值
    P_NORM,       // 空腔所有网格质量函数向量的 p 范数，默认为归一化形式，常取 p=2
    P_NORM_LIMIT, // 空腔所有网格质量函数向量的 p 范数极限，用于拟合 MAX，常取 p=13
};

// 网格优化光滑化方法
enum class MeshSmoothType
{
    CONJUGATE_GRADIENT, // 共轭梯度法
    LBFGS,              // L-BFGS法
    MinNLC,             // 带非线性等/不等式+边界+线性约束的优化

    GD, // 梯度下降法
    CG, // 共轭梯度法
    QN, // 拟牛顿法
};

// 网格优化光滑化方式
enum class MeshSmoothWay
{
    SIGLE_VERT,   // 一次优化单个顶点
    SIGLE_REGION, // 一次优化单个网格单元的所有顶点
    PATCH_REGION, // 一次优化多个网格单元的顶点
    ALL_REGION,   // 一次优化所有网格单元的顶点
};

enum class LineSearchWay // 线搜索方式
{
    STRONG_WOLFE, // 使用强 Wolfe 条件的线搜索（Numerical Optimization Second Edition）的标准实现
    MORE_THUENTE, // Line search by Moré and Thuente (1994) 的标准实现，同样使用强 Wolfe 条件
};

// 线搜索相关参数
struct LineSearchOptions
{
    LineSearchWay lineSearchWay_ = LineSearchWay::MORE_THUENTE; // 线搜索方式，默认使用 Moré and Thuente 的标准实现

    int maxLineSearchIter_ = 10;  // 线搜索的最大迭代次数，设置为 0 则不限制迭代次数
    double strongWolfeC1_ = 1e-4; // 强 Wolfe 条件中的第一个参数，通常取值为 1e-4
    double strongWolfeC2_ = 0.9;  // 强 Wolfe 条件中的第二个参数，通常取值为 0.9
    double alphaMin_ = 1e-20;     // 最小步长，防止步长过小
    double alphaMax_ = 1e+20;     // 最大步长，防止步长过大
};

// 梯度下降法相关参数
struct GradientDescentOptions
{
    double epsG_ = 1e-8; // 梯度范数阈值，满足 ||∇F(k)||<=epsG_ 则停止迭代
    double epsF_ = 0.;   // 函数值更新阈值，满足 |F(k+1)-F(k)|<=epsF_*max{|F(k)|,|F(k+1)|,1} 则停止迭代
    double epsX_ = 1e-8; // 变量更新阈值，满足 ||X(k+1)-X(k)||<=epsX_ 则停止迭代
    int maxIter_ = 7;    // 最大迭代次数，设置为 0 则不限制迭代次数
    int varNum_ = 0;     // 变量个数

    LineSearchOptions lineSearchOpti_{}; // 线搜索相关参数
};

// 共轭梯度法相关参数
struct ConjugateGradientOptions
{
    int cgType_ = 2; // 共轭梯度法类型，0：Fletcher-Reeves；1：Poli-Ribiere；2：Poli-Ribiere+

    double epsG_ = 1e-8; // 梯度范数阈值，满足 ||∇F(k)||<=epsG_ 则停止迭代
    double epsF_ = 0.;   // 函数值更新阈值，满足 |F(k+1)-F(k)|<=epsF_*max{|F(k)|,|F(k+1)|,1} 则停止迭代
    double epsX_ = 1e-8; // 变量更新阈值，满足 ||X(k+1)-X(k)||<=epsX_ 则停止迭代
    int maxIter_ = 7;    // 最大迭代次数，设置为 0 则不限制迭代次数
    int varNum_ = 0;     // 变量个数

    LineSearchOptions lineSearchOpti_{}; // 线搜索相关参数
};

// 拟牛顿法相关参数
struct QuasiNewtonOptions
{
    int qnType_ = 1;     // 拟牛顿法类型，0：BFGS；1：L-BFGS；2：SR1；3：DFP
    bool Bbased_ = true; // 是否使用 B 作为近似海森矩阵，默认使用 B 作为近似海森矩阵（需要求逆矩阵，但理论更好，变量数较小时可以使用），false 则使用 H 作为近似海森矩阵
    double eps_ = 1e-10; // 一些数值稳定性相关的阈值

    double epsG_ = 1e-8; // 梯度范数阈值，满足 ||∇F(k)||<=epsG_ 则停止迭代
    double epsF_ = 0.;   // 函数值更新阈值，满足 |F(k+1)-F(k)|<=epsF_*max{|F(k)|,|F(k+1)|,1} 则停止迭代
    double epsX_ = 1e-8; // 变量更新阈值，满足 ||X(k+1)-X(k)||<=epsX_ 则停止迭代
    int maxIter_ = 7;    // 最大迭代次数，设置为 0 则不限制迭代次数
    int varNum_ = 0;     // 变量个数

    int m_ = 5; // L-BFGS 中，历史信息的存储个数，默认为 varNum_ > 5 ? 5 : varNum_

    LineSearchOptions lineSearchOpti_{}; // 线搜索相关参数
};

// 网格质量优化算法相关参数
struct MeshOptiAlgorithmOptions
{
    bool useGD_ = true;                    // 是否使用梯度下降法，默认使用
    GradientDescentOptions gdOptions_{};   // 梯度下降法相关参数
    bool useCG_ = false;                   // 是否使用共轭梯度法，默认不使用
    ConjugateGradientOptions cgOptions_{}; // 共轭梯度法相关参数
    bool useQN_ = false;                   // 是否使用拟牛顿法，默认不使用
    QuasiNewtonOptions qnOptions_{};       // 拟牛顿法相关参数
};

// 网格质量优化相关参数
struct MeshOptiOptions
{
    // EMeshOptiQualityMetrics.h
    // 网格质量度量准则
    std::unordered_map<MeshElementType, MeshQualityMetric> meshQualityMetric_ = {
        // 对 Vertex 类型的网格单元，不进行网格质量评判
        {MeshElementType::Vertex, {}},
        // 对 Edge 类型的网格单元，不进行网格质量评判
        {MeshElementType::Edge, {}},
        // 对 Face 类型的网格单元，默认使用纵横比作为网格质量度量准则
        {MeshElementType::Face, MeshQualityMetric::ASPECT_RATIO},
        // 对 Face 类型的网格单元，默认使用纵横比作为网格质量度量准则
        {MeshElementType::Region, MeshQualityMetric::ASPECT_RATIO}};
    // 网格质量函数
    std::unordered_map<MeshElementType, std::vector<std::pair<double, MeshQualityFunc>>> meshQualityFunc_ = {
        // 对 Vertex 类型的网格单元，不进行网格质量评判
        {MeshElementType::Vertex, {}},
        // 对 Edge 类型的网格单元，不进行网格质量评判
        {MeshElementType::Edge, {}},
        // 对 Face 类型的网格单元，默认使用单个纵横比进行优化
        {MeshElementType::Face, {{1.0, MeshQualityFunc::ASPECT_RATIO_FUNC}}},
        // 对 Face 类型的网格单元，默认使用单个纵横比进行优化
        {MeshElementType::Region, {{1.0, MeshQualityFunc::ASPECT_RATIO_FUNC}}}};
    CalculateWay calculateWay_ = CalculateWay::P_NORM; // 目标函数计算方式，默认使用加权后网格质量函数向量的 p 范数
    double p_ = 2.0;                                   // p 范数的 p 值，默认使用 p=2
    bool meshLegality_ = false;                        // 是否优化时，检查网格合法性，默认不检查。可以在【非网格解缠】环境中开启，避免由于某些网格质量函数导致的网格不合法问题；使用 CalculateWay::P_NORM 时建议开启
    bool useLogBarrier_ = false;                       // 是否使用对数屏障函数，默认不使用
    double logBarrier_ = 0.2;                          // 对数屏障函数的系数，默认值为0.2

    // EMeshOptiSmooth.h
    MeshSmoothType smoothType_ = MeshSmoothType::CONJUGATE_GRADIENT; // 网格优化光滑化方法，默认使用共轭梯度法
    MeshSmoothWay smoothWay_ = MeshSmoothWay::SIGLE_VERT;            // 网格优化光滑化方式，默认一次优化单个顶点
    double badRegionQuality_ = 0.3;                                  // 劣质网格单元质量阈值，默认值为0.3，角度相关则是40°
    int ConsecuIterNum_ = 7;                                         // 连续迭代次数，默认值为7

    MeshOptiAlgorithmOptions meshOptiAlgorithmOptions_{}; // 网格质量优化算法相关参数
};

// -----------------------------------------------------------------------------
// 类型擦除基类：支持常用容器操作接口
// -----------------------------------------------------------------------------
struct VectorHolder
{
    using value_type = void;

    virtual ~VectorHolder() = default;

    // 容量管理
    virtual void reserve(size_t n) = 0;
    virtual void shrink_to_fit() = 0;

    // 元素访问
    virtual size_t size() const = 0;
    virtual bool empty() const = 0;
    virtual void clear() = 0;

    // 列表式赋值
    virtual void assign(std::initializer_list<std::any> init) = 0;

    virtual std::any operator[](size_t idx) const = 0;
    virtual std::any at(size_t idx) const = 0;

    // 插入 & 删除
    virtual void push_back(const std::any &value) = 0;
    virtual void pop_back() = 0;

    // 迭代器访问（只读）
    virtual const void *data() const = 0;
    virtual void *data() = 0;

    // 标签式克隆，用于复制容器
    virtual std::unique_ptr<VectorHolder> clone() const = 0;

    // 在位置 pos 插入 data 指向的 count 个元素（按派生的 T 大小）
    virtual void insert(std::size_t pos, void const *data, std::size_t count) = 0;

    // 在位置 pos 插入 count 个 value
    virtual void insert(std::size_t pos, std::size_t count, const std::any &value) = 0;
};

// -----------------------------------------------------------------------------
// 模板派生类：针对 T 提供全部实现
// -----------------------------------------------------------------------------
template <typename T>
struct TypedVectorHolder : VectorHolder
{
    using value_type = T;
    // 默认构造
    TypedVectorHolder() = default;
    explicit TypedVectorHolder(size_t n) { elements.reserve(n); }

    // 列表初始化
    TypedVectorHolder(std::initializer_list<T> init)
        : elements(init)
    {
    }

    // 容量管理
    void reserve(size_t n) override { elements.reserve(n); }
    void shrink_to_fit() override { elements.shrink_to_fit(); }

    // 元素访问
    size_t size() const override { return elements.size(); }
    bool empty() const override { return elements.empty(); }
    void clear() override { elements.clear(); }

    // 列表式赋值
    void assign(std::initializer_list<std::any> init) override
    {
        elements.clear();
        for (auto &val : init)
            elements.push_back(std::any_cast<T>(val));
    }

    std::any operator[](size_t idx) const override { return elements[idx]; }
    std::any at(size_t idx) const override
    {
        if (idx >= elements.size())
            throw std::out_of_range("TypedVectorHolder::at() 越界");
        return elements[idx];
    }

    // 插入 & 删除
    void push_back(const std::any &value) override { elements.push_back(std::any_cast<T>(value)); }
    void pop_back() override { elements.pop_back(); }

    // 迭代器访问
    T *data_typed() { return elements.data(); }
    const T *data_typed() const { return elements.data(); }

    const void *data() const override { return static_cast<const void *>(elements.data()); }
    void *data() override { return static_cast<void *>(elements.data()); }

    // 克隆一份副本
    std::unique_ptr<VectorHolder> clone() const override
    {
        auto copy = std::make_unique<TypedVectorHolder<T>>();
        copy->elements = elements;
        return copy;
    }

    void insert(std::size_t pos, void const *data, std::size_t count) override
    {
        auto ptr = static_cast<T const *>(data);
        elements.insert(elements.begin() + pos, ptr, ptr + count); // 插入 [ptr, ptr+count) 到 elements.begin()+pos
    }

    void insert(std::size_t pos, std::size_t count, const std::any &value) override
    {
        T v = std::any_cast<T>(value);
        elements.insert(elements.begin() + pos, count, v);
    }

    // 范围访问
    auto begin() { return elements.begin(); }
    auto end() { return elements.end(); }
    auto begin() const { return elements.begin(); }
    auto end() const { return elements.end(); }

    // 支持 erasing
    void erase(size_t idx)
    {
        if (idx >= elements.size())
            throw std::out_of_range("erase index");
        elements.erase(elements.begin() + idx);
    }

    const std::vector<T> &data_vector() const { return elements; }

private:
    std::vector<T> elements;
};

// 网格元素数据大小
inline const std::unordered_map<MeshElementType, int> MeshElementSize = {
    {MeshElementType::Vertex, 3}, // 一个顶点，3 个 double
    {MeshElementType::Edge, 2},   // 一条边，2 个顶点 ID
    {MeshElementType::Face, 3},   // 一个面，3 个顶点 ID（假设三角形面）
    {MeshElementType::Region, 4}  // 一个体，4 个顶点 ID（假设四面体体）
};

// 网格属性数据类型
enum class MeshAttrDataType
{
    Int, // 整型属性
    //UChar, // 无符号字符型属性
    Dbl // 双精度浮点型属性
};

// 属性维度枚举
enum class MeshAttrDim
{
    SCALAR, // 标量属性
    VECTOR, // 向量属性
};

inline const std::unordered_map<MeshAttrDataType, std::any> MeshAttrDataDefault = {
    {MeshAttrDataType::Int, 0},
    //{MeshAttrDataType::UChar, 0},
    {MeshAttrDataType::Dbl, 0.},
};

// 统一属性条目：名称、数据类型、作用域（网格元素类型）
using MeshAttrEntry = std::tuple<std::string, MeshAttrDataType, MeshAttrDim, MeshElementType>;
using MeshAttrList = std::vector<MeshAttrEntry>;

inline const std::string VertexAttrNames[] = {
    "vertSize", // 【标量】double 顶点密度信息
    "vertType", // 【标量】int 顶点类型属性: 0: 模型点对应网格点，1: 模型边对应网格点，2: 模型面对应网格点，3: 模型体对应网格点
    "tParam",   // 【标量】double 顶点在对应模型边上的参数坐标
    "regionID", // 【标量】int 当前三角化中包含该顶点的任意单元的编号值
    "normal",   // 【向量】double 顶点法向量
};

inline const std::string EdgeAttrNames[] = {
    "noType", // 占位符
};

inline const std::string FaceAttrNames[] = {
    "triQuality", // 【标量】double 三角形网格质量
    "normal",     // 【向量】double 三角形网格法向量
};

inline const std::string RegionAttrNames[] = {
    "tetQuality",   // 【标量】double 四面体网格质量
    "overFlag",     // 【标量】int 超三角形顶点相关网格单元
    "neigRegionID", // 【向量】int 体网格单元的相邻体网格单元 ID
};

// “工厂”映射：每个网格元素类型该构造哪个 TypedVectorHolder<T>
inline std::unordered_map<MeshElementType, std::function<std::unique_ptr<VectorHolder>()>>
    MeshElementFactory = {
        {MeshElementType::Vertex, [] { return std::make_unique<TypedVectorHolder<double>>(); }},
        {MeshElementType::Edge, [] { return std::make_unique<TypedVectorHolder<int>>(); }},
        {MeshElementType::Face, [] { return std::make_unique<TypedVectorHolder<int>>(); }},
        {MeshElementType::Region, [] { return std::make_unique<TypedVectorHolder<int>>(); }},
};

// “工厂”映射：每个网格属性数据类型该构造哪个 TypedVectorHolder<T>
inline std::unordered_map<MeshAttrDataType, std::function<std::unique_ptr<VectorHolder>()>>
    MeshAttrDataFactory = {
        {MeshAttrDataType::Int, [] { return std::make_unique<TypedVectorHolder<int>>(); }},
        //{MeshAttrDataType::UChar, [] { return std::make_unique<TypedVectorHolder<unsigned char>>(); }},
        {MeshAttrDataType::Dbl, [] { return std::make_unique<TypedVectorHolder<double>>(); }},
};

// 完整网格
using Mesh = std::unordered_map<MeshElementType, std::unique_ptr<VectorHolder>>;

// 完整属性
using MeshAttr = std::unordered_map<MeshElementType, std::unordered_map<std::string, std::pair<std::unique_ptr<VectorHolder>, MeshAttrDataType>>>;

// 网格 Tetview 格式输入输出相关参数
struct MeshTetViewIOOptions
{
    bool outEdge_ = false; // 是否输出边文件
    bool outFace_ = false; // 是否输出面文件
    bool outReg_ = true;   // 是否输出体文件
};

// 网格 VTK 格式输入输出相关参数
struct MeshVTKIOOptions
{
    bool xml_ = true;    // VTK 文件类型：true 表示使用 XML 格式（.vtu）；false 表示使用 Legacy 格式（.vtk）
    bool binary_ = true; // VTK 文件格式：true 表示使用二进制格式；false 表示使用 ASCII 格式

    // 网格输入

    // 网格输出
    std::vector<MeshElementType> meshEleOut_; // 输出网格元素类型列表
    MeshAttrList meshAttrOut_;                // 输出属性列表
};

// 网格 OBJ 格式输入输出相关参数
struct MeshOBJIOOptions
{
    bool vertNormal_ = true; // 是否输出顶点法向量
};

// 网格 STL 格式输入输出相关参数
struct MeshSTLIOOptions
{
    // bool faceNormal_ = true; // 是否输出面法向量，实际上 STL 格式必须输出面法向量
    bool binary_ = true; // STL 文件格式：true 表示使用二进制格式；false 表示使用 ASCII 格式
};

// 网格 PLY 格式输入输出相关参数
struct MeshPLYIOOptions
{
    // 只处理 binary_little_endian
    // 顶点属性：只会是 float 或 double（统一存成 double）
    // 面：只会是三角面，索引类型只可能是 int 或 uint（统一存成 int）
    // 遇到非三角直接跳过

    bool vertNormal_ = true; // 是否输出顶点法向量
    bool binary_ = true;     // PLY 文件格式：true 表示使用二进制格式；false 表示使用 ASCII 格式
};

// 网格 OFF 格式输入输出相关参数
struct MeshOFFIOOptions
{
    bool vertNormal_ = false; // 是否输出顶点法向量
};

// 网格 MSH 格式输入输出相关参数
struct MeshMSHIOOptions
{
    int version_ = 2; // MSH 文件版本号，支持 2(2.2) 和 4(4.1)
    int eleType_ = 4; // MSH 文件中单元类型，4 表示四面体，2 表示三角形
    //bool binary_ = true; // MSH 文件格式：true 表示使用二进制格式；false 表示使用 ASCII 格式。默认直接使用 ASCII 格式
};

// 网格全格式输入输出相关参数
struct MeshIOOptions
{
    std::string filePath_; // 网格文件夹路径
    std::string fileName_; // 网格文件名

    bool outTetview_ = false;             // 是否输出 Tetview 格式文件
    MeshTetViewIOOptions tetviewOptions_; // Tetview 格式输入输出参数

    bool outVTK_ = true;          // 是否输出 VTK 格式文件
    MeshVTKIOOptions vtkOptions_; // VTK 格式输入输出参数

    bool outOBJ_ = false;         // 是否输出 OBJ 格式文件
    MeshOBJIOOptions objOptions_; // OBJ 格式输入输出参数

    bool outSTL_ = false;         // 是否输出 STL 格式文件
    MeshSTLIOOptions stlOptions_; // STL 格式输入输出参数

    bool outPLY_ = false;         // 是否输出 PLY 格式文件
    MeshPLYIOOptions plyOptions_; // PLY 格式输入输出参数

    bool outOFF_ = false;         // 是否输出 OFF 格式文件
    MeshOFFIOOptions offOptions_; // OFF 格式输入输出参数

    bool outMSH_ = false;         // 是否输出 MSH 格式文件
    MeshMSHIOOptions mshOptions_; // MSH 格式输入输出参数
};

// =============================== 日志文件选项 ===============================

// 日志消息级别
enum class LogLevel
{
    Trace, // 细粒度调试信息
    Debug, // 调试信息
    Info,  // 一般信息
    Warn,  // 警告信息
    Error, // 错误信息
    Fatal, // 致命错误
    Perf,  // 性能信息
    Stat,  // 统计信息
};

// 日志文件格式
enum class LogFileFormat
{
    Text,   // 纯文本格式（每行纯值，行内无 key=）
    Json,   // JSON 格式（每行一个 JSON 对象）
    Logfmt, // logfmt 格式（严格 key=value 串）
};

// 日志文件格式对应扩展名
inline const char *LogFileFormatExt(LogFileFormat f)
{
    switch (f)
    {
    case LogFileFormat::Text:
        return "txt";
    case LogFileFormat::Json:
        return "json";
    case LogFileFormat::Logfmt:
        return "logfmt";
    default:
        return "txt";
    }
}

// 日志文件语言
enum class LogFileLanguage
{
    English, // 英文
    Chinese, // 中文
};

// 日志文件选项
struct LogFileOptions
{
    LogFileFormat format_ = LogFileFormat::Text; // 日志文件格式
    std::string filePath_;                       // 日志文件“基路径”，后缀将按 format 自动替换为 .txt/.json/.logfmt
    std::string fileName_;                       // 日志文件名（不含路径与扩展名）
    bool overwrite_ = true;                      // 是否覆盖已有文件（true=截断；false=追加）
    bool flushEveryWrite_ = true;                // 每次写入后立即刷新文件缓冲区

    // 哪些字段将被包含在日志输出中
    bool withTimeStamp_ = true;   // 时间戳
    bool withLevel_ = true;       // 日志级别
    bool withPid_ = false;        // 进程 ID
    bool withThreadId_ = false;   // 线程 ID
    bool withThreadName_ = false; // 线程名称（通过 SetThreadName 设置）
    bool withAppName_ = false;    // 应用名
    bool withCategory_ = true;    // 模块/类别
    bool withFunction_ = true;    // 函数名
    bool withMessage_ = true;     // 日志消息
    bool withExtras_ = true;      // 额外字段（TXT：行末以 "k=v ..." 合并为一列；JSON：放到 extra 对象；logfmt：追加为若干 k=v）

    // 时间格式
    bool useUtc_ = false;                           // 使用 UTC 时间，否则使用本地时间
    bool withMillis_ = true;                        // 时间戳包含毫秒
    std::string timePattern_ = "%Y-%m-%d %H:%M:%S"; // 时间戳格式（strftime）

    // TXT 分隔符（仅 Text 格式生效）
    char textFieldSep_ = '\t';

    // 应用名
    std::string appName_ = "EmMpMesh";

    // 日志级别阈值（含）：低于此级别的日志将被丢弃
    LogLevel minLevel_ = LogLevel::Trace;

    // 国际化（影响级别文本与 TXT 表头列名）
    LogFileLanguage language_ = LogFileLanguage::Chinese;

    // 控制台镜像选项
    bool mirror2Console_ = false;      // 是否镜像到控制台
    bool consoleErrors2Stderr_ = true; // WARN/ERROR/FATAL 是否输出到 stderr（否则 stdout）
};

#endif //EMMPMESH_COMMON_COMCONSTANTS_H_
