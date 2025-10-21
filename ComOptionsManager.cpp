#include "pch.h"

#include "ComOptionsManager.h"

#include <iostream>

ComOptionsManager::ComOptionsManager()
    : currentLevel_(MeshQualityLevel::Balanced)
{
    resetToDefaults();
}

ComOptionsManager::~ComOptionsManager()
{
}

void ComOptionsManager::setQualityLevel(MeshQualityLevel level)
{
    currentLevel_ = level;
    resetToDefaults();

    switch (level)
    {
    case MeshQualityLevel::VeryFast:
        {
            // 极快生成模式：最少的迭代次数，简单的优化算法
            gradientDescentOptions_.maxIter_ = 3;
            meshOptiOptions_.ConsecuIterNum_ = 3;
            meshOptiOptions_.badRegionQuality_ = 0.2;
            meshOptiOptions_.smoothType_ = MeshSmoothType::GD;
            meshOptiOptions_.smoothWay_ = MeshSmoothWay::SIGLE_VERT;
            meshOptiAlgorithmOptions_.useGD_ = true;
            meshOptiAlgorithmOptions_.useCG_ = false;
            meshOptiAlgorithmOptions_.useQN_ = false;
            break;
        }
    case MeshQualityLevel::Fast:
        {
            // 快速生成模式：较少的迭代次数，使用梯度下降
            gradientDescentOptions_.maxIter_ = 5;
            meshOptiOptions_.ConsecuIterNum_ = 5;
            meshOptiOptions_.badRegionQuality_ = 0.25;
            meshOptiOptions_.smoothType_ = MeshSmoothType::GD;
            meshOptiOptions_.smoothWay_ = MeshSmoothWay::SIGLE_VERT;
            meshOptiAlgorithmOptions_.useGD_ = true;
            meshOptiAlgorithmOptions_.useCG_ = false;
            meshOptiAlgorithmOptions_.useQN_ = false;
            break;
        }
    case MeshQualityLevel::Balanced:
        {
            // 平衡模式：默认设置（在 resetToDefaults 中已设置）
            break;
        }
    case MeshQualityLevel::HighQuality:
        {
            // 高质量模式：较多的迭代次数，使用 L-BFGS
            gradientDescentOptions_.maxIter_ = 10;
            conjugateGradientOptions_.maxIter_ = 10;
            quasiNewtonOptions_.maxIter_ = 10;
            meshOptiOptions_.ConsecuIterNum_ = 10;
            meshOptiOptions_.badRegionQuality_ = 0.4;
            meshOptiOptions_.smoothType_ = MeshSmoothType::LBFGS;
            meshOptiOptions_.smoothWay_ = MeshSmoothWay::SIGLE_VERT;
            meshOptiOptions_.meshLegality_ = true;
            meshOptiAlgorithmOptions_.useGD_ = true;
            meshOptiAlgorithmOptions_.useCG_ = true;
            meshOptiAlgorithmOptions_.useQN_ = false;
            break;
        }
    case MeshQualityLevel::VeryHighQuality:
        {
            // 极高质量模式：最多的迭代次数，使用多种优化算法组合
            gradientDescentOptions_.maxIter_ = 15;
            conjugateGradientOptions_.maxIter_ = 15;
            quasiNewtonOptions_.maxIter_ = 15;
            meshOptiOptions_.ConsecuIterNum_ = 15;
            meshOptiOptions_.badRegionQuality_ = 0.5;
            meshOptiOptions_.smoothType_ = MeshSmoothType::LBFGS;
            meshOptiOptions_.smoothWay_ = MeshSmoothWay::PATCH_REGION;
            meshOptiOptions_.meshLegality_ = true;
            meshOptiOptions_.useLogBarrier_ = true;
            meshOptiAlgorithmOptions_.useGD_ = true;
            meshOptiAlgorithmOptions_.useCG_ = true;
            meshOptiAlgorithmOptions_.useQN_ = true;
            break;
        }
    }
}

std::string ComOptionsManager::qualityLevelToString(MeshQualityLevel level) const
{
    switch (level)
    {
    case MeshQualityLevel::VeryFast:
        return "极快生成";
    case MeshQualityLevel::Fast:
        return "快速生成";
    case MeshQualityLevel::Balanced:
        return "平衡模式";
    case MeshQualityLevel::HighQuality:
        return "高质量";
    case MeshQualityLevel::VeryHighQuality:
        return "极高质量";
    default:
        return "未知";
    }
}

int ComOptionsManager::loadFromFile(const std::string &filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "无法打开配置文件: " << filePath << std::endl;
        return 1;
    }

    std::string line;
    std::string currentSection;

    while (std::getline(file, line))
    {
        line = trim(line);

        // 跳过空行和注释
        if (line.empty() || line[0] == '#' || line[0] == ';')
            continue;

        // 识别节（section）格式：=============================== SectionName ===============================
        if (line.find("===") != std::string::npos)
        {
            // 移除所有等号
            std::string temp = line;
            temp.erase(std::remove(temp.begin(), temp.end(), '='), temp.end());

            // 去除首尾空格
            size_t start = temp.find_first_not_of(' ');
            size_t end = temp.find_last_not_of(' ');

            if (start != std::string::npos && end != std::string::npos)
            {
                currentSection = temp.substr(start, end - start + 1);
                continue;
            }
        }

        // 解析键值对
        size_t pos = line.find('=');
        if (pos == std::string::npos)
            continue;

        std::string key = trim(line.substr(0, pos));
        std::string value = trim(line.substr(pos + 1));

        // 根据当前节设置相应的选项
        if (currentSection == "LineSearchOptions")
        {
            if (key == "lineSearchWay")
                lineSearchOptions_.lineSearchWay_ = stringToLineSearchWay(value);
            else if (key == "maxLineSearchIter")
                lineSearchOptions_.maxLineSearchIter_ = std::stoi(value);
            else if (key == "strongWolfeC1")
                lineSearchOptions_.strongWolfeC1_ = std::stod(value);
            else if (key == "strongWolfeC2")
                lineSearchOptions_.strongWolfeC2_ = std::stod(value);
            else if (key == "alphaMin")
                lineSearchOptions_.alphaMin_ = std::stod(value);
            else if (key == "alphaMax")
                lineSearchOptions_.alphaMax_ = std::stod(value);
        }
        else if (currentSection == "GradientDescentOptions")
        {
            if (key == "epsG")
                gradientDescentOptions_.epsG_ = std::stod(value);
            else if (key == "epsF")
                gradientDescentOptions_.epsF_ = std::stod(value);
            else if (key == "epsX")
                gradientDescentOptions_.epsX_ = std::stod(value);
            else if (key == "maxIter")
                gradientDescentOptions_.maxIter_ = std::stoi(value);
            else if (key == "varNum")
                gradientDescentOptions_.varNum_ = std::stoi(value);
        }
        else if (currentSection == "ConjugateGradientOptions")
        {
            if (key == "cgType")
                conjugateGradientOptions_.cgType_ = std::stoi(value);
            else if (key == "epsG")
                conjugateGradientOptions_.epsG_ = std::stod(value);
            else if (key == "epsF")
                conjugateGradientOptions_.epsF_ = std::stod(value);
            else if (key == "epsX")
                conjugateGradientOptions_.epsX_ = std::stod(value);
            else if (key == "maxIter")
                conjugateGradientOptions_.maxIter_ = std::stoi(value);
            else if (key == "varNum")
                conjugateGradientOptions_.varNum_ = std::stoi(value);
        }
        else if (currentSection == "QuasiNewtonOptions")
        {
            if (key == "qnType")
                quasiNewtonOptions_.qnType_ = std::stoi(value);
            else if (key == "Bbased")
                quasiNewtonOptions_.Bbased_ = stringToBool(value);
            else if (key == "eps")
                quasiNewtonOptions_.eps_ = std::stod(value);
            else if (key == "epsG")
                quasiNewtonOptions_.epsG_ = std::stod(value);
            else if (key == "epsF")
                quasiNewtonOptions_.epsF_ = std::stod(value);
            else if (key == "epsX")
                quasiNewtonOptions_.epsX_ = std::stod(value);
            else if (key == "maxIter")
                quasiNewtonOptions_.maxIter_ = std::stoi(value);
            else if (key == "varNum")
                quasiNewtonOptions_.varNum_ = std::stoi(value);
            else if (key == "m")
                quasiNewtonOptions_.m_ = std::stoi(value);
        }
        else if (currentSection == "MeshOptiAlgorithmOptions")
        {
            if (key == "useGD")
                meshOptiAlgorithmOptions_.useGD_ = stringToBool(value);
            else if (key == "useCG")
                meshOptiAlgorithmOptions_.useCG_ = stringToBool(value);
            else if (key == "useQN")
                meshOptiAlgorithmOptions_.useQN_ = stringToBool(value);
        }
        else if (currentSection == "MeshOptiOptions")
        {
            if (key == "calculateWay")
                meshOptiOptions_.calculateWay_ = stringToCalculateWay(value);
            else if (key == "p")
                meshOptiOptions_.p_ = std::stod(value);
            else if (key == "meshLegality")
                meshOptiOptions_.meshLegality_ = stringToBool(value);
            else if (key == "useLogBarrier")
                meshOptiOptions_.useLogBarrier_ = stringToBool(value);
            else if (key == "logBarrier")
                meshOptiOptions_.logBarrier_ = std::stod(value);
            else if (key == "smoothType")
                meshOptiOptions_.smoothType_ = stringToMeshSmoothType(value);
            else if (key == "smoothWay")
                meshOptiOptions_.smoothWay_ = stringToMeshSmoothWay(value);
            else if (key == "badRegionQuality")
                meshOptiOptions_.badRegionQuality_ = std::stod(value);
            else if (key == "ConsecuIterNum")
                meshOptiOptions_.ConsecuIterNum_ = std::stoi(value);
        }
        else if (currentSection == "MeshTetViewIOOptions")
        {
            if (key == "outEdge")
                meshTetViewIOOptions_.outEdge_ = stringToBool(value);
            else if (key == "outFace")
                meshTetViewIOOptions_.outFace_ = stringToBool(value);
            else if (key == "outReg")
                meshTetViewIOOptions_.outReg_ = stringToBool(value);
        }
        else if (currentSection == "MeshVTKIOOptions")
        {
            if (key == "xml")
                meshVTKIOOptions_.xml_ = stringToBool(value);
            else if (key == "binary")
                meshVTKIOOptions_.binary_ = stringToBool(value);
        }
        else if (currentSection == "MeshOBJIOOptions")
        {
            if (key == "vertNormal")
                meshOBJIOOptions_.vertNormal_ = stringToBool(value);
        }
        else if (currentSection == "MeshSTLIOOptions")
        {
            if (key == "binary")
                meshSTLIOOptions_.binary_ = stringToBool(value);
        }
        else if (currentSection == "MeshPLYIOOptions")
        {
            if (key == "vertNormal")
                meshPLYIOOptions_.vertNormal_ = stringToBool(value);
            else if (key == "binary")
                meshPLYIOOptions_.binary_ = stringToBool(value);
        }
        else if (currentSection == "MeshOFFIOOptions")
        {
            if (key == "vertNormal")
                meshOFFIOOptions_.vertNormal_ = stringToBool(value);
        }
        else if (currentSection == "MeshMSHIOOptions")
        {
            if (key == "version")
                meshMSHIOOptions_.version_ = std::stoi(value);
            else if (key == "eleType")
                meshMSHIOOptions_.eleType_ = std::stoi(value);
        }
        else if (currentSection == "MeshIOOptions")
        {
            if (key == "filePath")
                meshIOOptions_.filePath_ = value;
            else if (key == "fileName")
                meshIOOptions_.fileName_ = value;
            else if (key == "outTetview")
                meshIOOptions_.outTetview_ = stringToBool(value);
            else if (key == "outVTK")
                meshIOOptions_.outVTK_ = stringToBool(value);
            else if (key == "outOBJ")
                meshIOOptions_.outOBJ_ = stringToBool(value);
            else if (key == "outSTL")
                meshIOOptions_.outSTL_ = stringToBool(value);
            else if (key == "outPLY")
                meshIOOptions_.outPLY_ = stringToBool(value);
            else if (key == "outOFF")
                meshIOOptions_.outOFF_ = stringToBool(value);
            else if (key == "outMSH")
                meshIOOptions_.outMSH_ = stringToBool(value);
        }
        else if (currentSection == "LogFileOptions")
        {
            if (key == "format")
                logFileOptions_.format_ = stringToLogFileFormat(value);
            else if (key == "filePath")
                logFileOptions_.filePath_ = value;
            else if (key == "fileName")
                logFileOptions_.fileName_ = value;
            else if (key == "overwrite")
                logFileOptions_.overwrite_ = stringToBool(value);
            else if (key == "flushEveryWrite")
                logFileOptions_.flushEveryWrite_ = stringToBool(value);
            else if (key == "withTimeStamp")
                logFileOptions_.withTimeStamp_ = stringToBool(value);
            else if (key == "withLevel")
                logFileOptions_.withLevel_ = stringToBool(value);
            else if (key == "withPid")
                logFileOptions_.withPid_ = stringToBool(value);
            else if (key == "withThreadId")
                logFileOptions_.withThreadId_ = stringToBool(value);
            else if (key == "withThreadName")
                logFileOptions_.withThreadName_ = stringToBool(value);
            else if (key == "withAppName")
                logFileOptions_.withAppName_ = stringToBool(value);
            else if (key == "withCategory")
                logFileOptions_.withCategory_ = stringToBool(value);
            else if (key == "withFunction")
                logFileOptions_.withFunction_ = stringToBool(value);
            else if (key == "withMessage")
                logFileOptions_.withMessage_ = stringToBool(value);
            else if (key == "withExtras")
                logFileOptions_.withExtras_ = stringToBool(value);
            else if (key == "useUtc")
                logFileOptions_.useUtc_ = stringToBool(value);
            else if (key == "withMillis")
                logFileOptions_.withMillis_ = stringToBool(value);
            else if (key == "timePattern")
                logFileOptions_.timePattern_ = value;
            else if (key == "textFieldSep")
                logFileOptions_.textFieldSep_ = value.empty() ? '\t' : value[0];
            else if (key == "appName")
                logFileOptions_.appName_ = value;
            else if (key == "minLevel")
                logFileOptions_.minLevel_ = stringToLogLevel(value);
            else if (key == "language")
                logFileOptions_.language_ = stringToLogFileLanguage(value);
            else if (key == "mirror2Console")
                logFileOptions_.mirror2Console_ = stringToBool(value);
            else if (key == "consoleErrors2Stderr")
                logFileOptions_.consoleErrors2Stderr_ = stringToBool(value);
        }
    }

    file.close();
    return 0;
}

int ComOptionsManager::saveToFile(const std::string &filePath) const
{
    std::ofstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "无法创建配置文件: " << filePath << std::endl;
        return 1;
    }

    file << "=======================================================================\n";
    file << "                        网格划分 v2025 运行配置文件                        \n";
    file << "            电子科技大学 计算机仿真技术研究院 版权所有 2022-2025              \n";
    file << "=======================================================================\n";

    file << "\n\n";
    file << "=============================== 工程参数 ===============================\n";
    file << "# 自动生成\n";
    file << "# 当前质量级别: " << qualityLevelToString(currentLevel_) << "\n";

    // LineSearchOptions
    file << "\n\n";
    file << "=============================== LineSearchOptions ===============================\n";
    file << "lineSearchWay = " << lineSearchWayToString(lineSearchOptions_.lineSearchWay_) << "\n";
    file << "maxLineSearchIter = " << lineSearchOptions_.maxLineSearchIter_ << "\n";
    file << "strongWolfeC1 = " << lineSearchOptions_.strongWolfeC1_ << "\n";
    file << "strongWolfeC2 = " << lineSearchOptions_.strongWolfeC2_ << "\n";
    file << "alphaMin = " << lineSearchOptions_.alphaMin_ << "\n";
    file << "alphaMax = " << lineSearchOptions_.alphaMax_ << "\n";

    // GradientDescentOptions
    file << "\n\n";
    file << "=============================== GradientDescentOptions ===============================\n";
    file << "epsG = " << gradientDescentOptions_.epsG_ << "\n";
    file << "epsF = " << gradientDescentOptions_.epsF_ << "\n";
    file << "epsX = " << gradientDescentOptions_.epsX_ << "\n";
    file << "maxIter = " << gradientDescentOptions_.maxIter_ << "\n";
    file << "varNum = " << gradientDescentOptions_.varNum_ << "\n";

    // ConjugateGradientOptions
    file << "\n\n";
    file << "=============================== ConjugateGradientOptions ===============================\n";
    file << "cgType = " << conjugateGradientOptions_.cgType_ << "\n";
    file << "epsG = " << conjugateGradientOptions_.epsG_ << "\n";
    file << "epsF = " << conjugateGradientOptions_.epsF_ << "\n";
    file << "epsX = " << conjugateGradientOptions_.epsX_ << "\n";
    file << "maxIter = " << conjugateGradientOptions_.maxIter_ << "\n";
    file << "varNum = " << conjugateGradientOptions_.varNum_ << "\n";

    // QuasiNewtonOptions
    file << "\n\n";
    file << "=============================== QuasiNewtonOptions ===============================\n";
    file << "qnType = " << quasiNewtonOptions_.qnType_ << "\n";
    file << "Bbased = " << (quasiNewtonOptions_.Bbased_ ? "true" : "false") << "\n";
    file << "eps = " << quasiNewtonOptions_.eps_ << "\n";
    file << "epsG = " << quasiNewtonOptions_.epsG_ << "\n";
    file << "epsF = " << quasiNewtonOptions_.epsF_ << "\n";
    file << "epsX = " << quasiNewtonOptions_.epsX_ << "\n";
    file << "maxIter = " << quasiNewtonOptions_.maxIter_ << "\n";
    file << "varNum = " << quasiNewtonOptions_.varNum_ << "\n";
    file << "m = " << quasiNewtonOptions_.m_ << "\n";

    // MeshOptiAlgorithmOptions
    file << "\n\n";
    file << "=============================== MeshOptiAlgorithmOptions ===============================\n";
    file << "useGD = " << (meshOptiAlgorithmOptions_.useGD_ ? "true" : "false") << "\n";
    file << "useCG = " << (meshOptiAlgorithmOptions_.useCG_ ? "true" : "false") << "\n";
    file << "useQN = " << (meshOptiAlgorithmOptions_.useQN_ ? "true" : "false") << "\n";

    // MeshOptiOptions
    file << "\n\n";
    file << "=============================== MeshOptiOptions ===============================\n";
    file << "calculateWay = " << calculateWayToString(meshOptiOptions_.calculateWay_) << "\n";
    file << "p = " << meshOptiOptions_.p_ << "\n";
    file << "meshLegality = " << (meshOptiOptions_.meshLegality_ ? "true" : "false") << "\n";
    file << "useLogBarrier = " << (meshOptiOptions_.useLogBarrier_ ? "true" : "false") << "\n";
    file << "logBarrier = " << meshOptiOptions_.logBarrier_ << "\n";
    file << "smoothType = " << meshSmoothTypeToString(meshOptiOptions_.smoothType_) << "\n";
    file << "smoothWay = " << meshSmoothWayToString(meshOptiOptions_.smoothWay_) << "\n";
    file << "badRegionQuality = " << meshOptiOptions_.badRegionQuality_ << "\n";
    file << "ConsecuIterNum = " << meshOptiOptions_.ConsecuIterNum_ << "\n";

    // MeshTetViewIOOptions
    file << "\n\n";
    file << "=============================== MeshTetViewIOOptions ===============================\n";
    file << "outEdge = " << (meshTetViewIOOptions_.outEdge_ ? "true" : "false") << "\n";
    file << "outFace = " << (meshTetViewIOOptions_.outFace_ ? "true" : "false") << "\n";
    file << "outReg = " << (meshTetViewIOOptions_.outReg_ ? "true" : "false") << "\n";

    // MeshVTKIOOptions
    file << "\n\n";
    file << "=============================== MeshVTKIOOptions ===============================\n";
    file << "xml = " << (meshVTKIOOptions_.xml_ ? "true" : "false") << "\n";
    file << "binary = " << (meshVTKIOOptions_.binary_ ? "true" : "false") << "\n";

    // MeshOBJIOOptions
    file << "\n\n";
    file << "=============================== MeshOBJIOOptions ===============================\n";
    file << "vertNormal = " << (meshOBJIOOptions_.vertNormal_ ? "true" : "false") << "\n";

    // MeshSTLIOOptions
    file << "\n\n";
    file << "=============================== MeshSTLIOOptions ===============================\n";
    file << "binary = " << (meshSTLIOOptions_.binary_ ? "true" : "false") << "\n";

    // MeshPLYIOOptions
    file << "\n\n";
    file << "=============================== MeshPLYIOOptions ===============================\n";
    file << "vertNormal = " << (meshPLYIOOptions_.vertNormal_ ? "true" : "false") << "\n";
    file << "binary = " << (meshPLYIOOptions_.binary_ ? "true" : "false") << "\n";

    // MeshOFFIOOptions
    file << "\n\n";
    file << "=============================== MeshOFFIOOptions ===============================\n";
    file << "vertNormal = " << (meshOFFIOOptions_.vertNormal_ ? "true" : "false") << "\n";

    // MeshMSHIOOptions
    file << "\n\n";
    file << "=============================== MeshMSHIOOptions ===============================\n";
    file << "version = " << meshMSHIOOptions_.version_ << "\n";
    file << "eleType = " << meshMSHIOOptions_.eleType_ << "\n";

    // MeshIOOptions
    file << "\n\n";
    file << "=============================== MeshIOOptions ===============================\n";
    file << "filePath = " << meshIOOptions_.filePath_ << "\n";
    file << "fileName = " << meshIOOptions_.fileName_ << "\n";
    file << "outTetview = " << (meshIOOptions_.outTetview_ ? "true" : "false") << "\n";
    file << "outVTK = " << (meshIOOptions_.outVTK_ ? "true" : "false") << "\n";
    file << "outOBJ = " << (meshIOOptions_.outOBJ_ ? "true" : "false") << "\n";
    file << "outSTL = " << (meshIOOptions_.outSTL_ ? "true" : "false") << "\n";
    file << "outPLY = " << (meshIOOptions_.outPLY_ ? "true" : "false") << "\n";
    file << "outOFF = " << (meshIOOptions_.outOFF_ ? "true" : "false") << "\n";
    file << "outMSH = " << (meshIOOptions_.outMSH_ ? "true" : "false") << "\n";

    // LogFileOptions
    file << "\n\n";
    file << "=============================== LogFileOptions ===============================\n";
    file << "format = " << logFileFormatToString(logFileOptions_.format_) << "\n";
    file << "filePath = " << logFileOptions_.filePath_ << "\n";
    file << "fileName = " << logFileOptions_.fileName_ << "\n";
    file << "overwrite = " << (logFileOptions_.overwrite_ ? "true" : "false") << "\n";
    file << "flushEveryWrite = " << (logFileOptions_.flushEveryWrite_ ? "true" : "false") << "\n";
    file << "withTimeStamp = " << (logFileOptions_.withTimeStamp_ ? "true" : "false") << "\n";
    file << "withLevel = " << (logFileOptions_.withLevel_ ? "true" : "false") << "\n";
    file << "withPid = " << (logFileOptions_.withPid_ ? "true" : "false") << "\n";
    file << "withThreadId = " << (logFileOptions_.withThreadId_ ? "true" : "false") << "\n";
    file << "withThreadName = " << (logFileOptions_.withThreadName_ ? "true" : "false") << "\n";
    file << "withAppName = " << (logFileOptions_.withAppName_ ? "true" : "false") << "\n";
    file << "withCategory = " << (logFileOptions_.withCategory_ ? "true" : "false") << "\n";
    file << "withFunction = " << (logFileOptions_.withFunction_ ? "true" : "false") << "\n";
    file << "withMessage = " << (logFileOptions_.withMessage_ ? "true" : "false") << "\n";
    file << "withExtras = " << (logFileOptions_.withExtras_ ? "true" : "false") << "\n";
    file << "useUtc = " << (logFileOptions_.useUtc_ ? "true" : "false") << "\n";
    file << "withMillis = " << (logFileOptions_.withMillis_ ? "true" : "false") << "\n";
    file << "timePattern = " << logFileOptions_.timePattern_ << "\n";
    file << "textFieldSep = " << logFileOptions_.textFieldSep_ << "\n";
    file << "appName = " << logFileOptions_.appName_ << "\n";
    file << "minLevel = " << logLevelToString(logFileOptions_.minLevel_) << "\n";
    file << "language = " << logFileLanguageToString(logFileOptions_.language_) << "\n";
    file << "mirror2Console = " << (logFileOptions_.mirror2Console_ ? "true" : "false") << "\n";
    file << "consoleErrors2Stderr = " << (logFileOptions_.consoleErrors2Stderr_ ? "true" : "false") << "\n";

    file.close();
    return 0;
}

void ComOptionsManager::printAllOptions() const
{
    std::cout << "=======================================================================\n";
    std::cout << "                        网格划分 v2025 运行配置文件                        \n";
    std::cout << "            电子科技大学 计算机仿真技术研究院 版权所有 2022-2025              \n";
    std::cout << "=======================================================================\n";

    std::cout << "\n\n";
    std::cout << "=============================== 工程参数 ===============================\n";

    std::cout << "\n\n";
    std::cout << "# 自动生成\n";
    std::cout << "# 当前质量级别: " << qualityLevelToString(currentLevel_) << "\n";

    printLineSearchOptions();
    printGradientDescentOptions();
    printConjugateGradientOptions();
    printQuasiNewtonOptions();
    printMeshOptiAlgorithmOptions();
    printMeshOptiOptions();
    printMeshTetViewIOOptions();
    printMeshVTKIOOptions();
    printMeshOBJIOOptions();
    printMeshSTLIOOptions();
    printMeshPLYIOOptions();
    printMeshOFFIOOptions();
    printMeshMSHIOOptions();
    printMeshIOOptions();
    printLogFileOptions();
}

void ComOptionsManager::printLineSearchOptions() const
{
    std::cout << "=============================== LineSearchOptions ===============================\n";
    std::cout << "lineSearchWay = " << lineSearchWayToString(lineSearchOptions_.lineSearchWay_) << "\n";
    std::cout << "maxLineSearchIter = " << lineSearchOptions_.maxLineSearchIter_ << "\n";
    std::cout << "strongWolfeC1 = " << lineSearchOptions_.strongWolfeC1_ << "\n";
    std::cout << "strongWolfeC2 = " << lineSearchOptions_.strongWolfeC2_ << "\n";
    std::cout << "alphaMin = " << lineSearchOptions_.alphaMin_ << "\n";
    std::cout << "alphaMax = " << lineSearchOptions_.alphaMax_ << "\n\n";
}

void ComOptionsManager::printGradientDescentOptions() const
{
    std::cout << "=============================== GradientDescentOptions ===============================\n";
    std::cout << "epsG = " << gradientDescentOptions_.epsG_ << "\n";
    std::cout << "epsF = " << gradientDescentOptions_.epsF_ << "\n";
    std::cout << "epsX = " << gradientDescentOptions_.epsX_ << "\n";
    std::cout << "maxIter = " << gradientDescentOptions_.maxIter_ << "\n";
    std::cout << "varNum = " << gradientDescentOptions_.varNum_ << "\n\n";
}

void ComOptionsManager::printConjugateGradientOptions() const
{
    std::cout << "=============================== ConjugateGradientOptions ===============================\n";
    std::cout << "cgType = " << conjugateGradientOptions_.cgType_ << "\n";
    std::cout << "epsG = " << conjugateGradientOptions_.epsG_ << "\n";
    std::cout << "epsF = " << conjugateGradientOptions_.epsF_ << "\n";
    std::cout << "epsX = " << conjugateGradientOptions_.epsX_ << "\n";
    std::cout << "maxIter = " << conjugateGradientOptions_.maxIter_ << "\n";
    std::cout << "varNum = " << conjugateGradientOptions_.varNum_ << "\n\n";
}

void ComOptionsManager::printQuasiNewtonOptions() const
{
    std::cout << "=============================== QuasiNewtonOptions ===============================\n";
    std::cout << "qnType = " << quasiNewtonOptions_.qnType_ << "\n";
    std::cout << "Bbased = " << (quasiNewtonOptions_.Bbased_ ? "true" : "false") << "\n";
    std::cout << "eps = " << quasiNewtonOptions_.eps_ << "\n";
    std::cout << "epsG = " << quasiNewtonOptions_.epsG_ << "\n";
    std::cout << "epsF = " << quasiNewtonOptions_.epsF_ << "\n";
    std::cout << "epsX = " << quasiNewtonOptions_.epsX_ << "\n";
    std::cout << "maxIter = " << quasiNewtonOptions_.maxIter_ << "\n";
    std::cout << "varNum = " << quasiNewtonOptions_.varNum_ << "\n";
    std::cout << "m = " << quasiNewtonOptions_.m_ << "\n\n";
}

void ComOptionsManager::printMeshOptiAlgorithmOptions() const
{
    std::cout << "=============================== MeshOptiAlgorithmOptions ===============================\n";
    std::cout << "useGD = " << (meshOptiAlgorithmOptions_.useGD_ ? "true" : "false") << "\n";
    std::cout << "useCG = " << (meshOptiAlgorithmOptions_.useCG_ ? "true" : "false") << "\n";
    std::cout << "useQN = " << (meshOptiAlgorithmOptions_.useQN_ ? "true" : "false") << "\n\n";
}

void ComOptionsManager::printMeshOptiOptions() const
{
    std::cout << "=============================== MeshOptiOptions ===============================\n";
    std::cout << "calculateWay = " << calculateWayToString(meshOptiOptions_.calculateWay_) << "\n";
    std::cout << "p = " << meshOptiOptions_.p_ << "\n";
    std::cout << "meshLegality = " << (meshOptiOptions_.meshLegality_ ? "true" : "false") << "\n";
    std::cout << "useLogBarrier = " << (meshOptiOptions_.useLogBarrier_ ? "true" : "false") << "\n";
    std::cout << "logBarrier = " << meshOptiOptions_.logBarrier_ << "\n";
    std::cout << "smoothType = " << meshSmoothTypeToString(meshOptiOptions_.smoothType_) << "\n";
    std::cout << "smoothWay = " << meshSmoothWayToString(meshOptiOptions_.smoothWay_) << "\n";
    std::cout << "badRegionQuality = " << meshOptiOptions_.badRegionQuality_ << "\n";
    std::cout << "ConsecuIterNum = " << meshOptiOptions_.ConsecuIterNum_ << "\n\n";
}

void ComOptionsManager::printMeshTetViewIOOptions() const
{
    std::cout << "=============================== MeshTetViewIOOptions ===============================\n";
    std::cout << "outEdge = " << (meshTetViewIOOptions_.outEdge_ ? "true" : "false") << "\n";
    std::cout << "outFace = " << (meshTetViewIOOptions_.outFace_ ? "true" : "false") << "\n";
    std::cout << "outReg = " << (meshTetViewIOOptions_.outReg_ ? "true" : "false") << "\n\n";
}

void ComOptionsManager::printMeshVTKIOOptions() const
{
    std::cout << "=============================== MeshVTKIOOptions ===============================\n";
    std::cout << "xml = " << (meshVTKIOOptions_.xml_ ? "true" : "false") << "\n";
    std::cout << "binary = " << (meshVTKIOOptions_.binary_ ? "true" : "false") << "\n\n";
}

void ComOptionsManager::printMeshOBJIOOptions() const
{
    std::cout << "=============================== MeshOBJIOOptions ===============================\n";
    std::cout << "vertNormal = " << (meshOBJIOOptions_.vertNormal_ ? "true" : "false") << "\n\n";
}

void ComOptionsManager::printMeshSTLIOOptions() const
{
    std::cout << "=============================== MeshSTLIOOptions ===============================\n";
    std::cout << "binary = " << (meshSTLIOOptions_.binary_ ? "true" : "false") << "\n\n";
}

void ComOptionsManager::printMeshPLYIOOptions() const
{
    std::cout << "=============================== MeshPLYIOOptions ===============================\n";
    std::cout << "vertNormal = " << (meshPLYIOOptions_.vertNormal_ ? "true" : "false") << "\n";
    std::cout << "binary = " << (meshPLYIOOptions_.binary_ ? "true" : "false") << "\n\n";
}

void ComOptionsManager::printMeshOFFIOOptions() const
{
    std::cout << "=============================== MeshOFFIOOptions ===============================\n";
    std::cout << "vertNormal = " << (meshOFFIOOptions_.vertNormal_ ? "true" : "false") << "\n\n";
}

void ComOptionsManager::printMeshMSHIOOptions() const
{
    std::cout << "=============================== MeshMSHIOOptions ===============================\n";
    std::cout << "version = " << meshMSHIOOptions_.version_ << "\n";
    std::cout << "eleType = " << meshMSHIOOptions_.eleType_ << "\n\n";
}

void ComOptionsManager::printMeshIOOptions() const
{
    std::cout << "=============================== MeshIOOptions ===============================\n";
    std::cout << "filePath = " << meshIOOptions_.filePath_ << "\n";
    std::cout << "fileName = " << meshIOOptions_.fileName_ << "\n";
    std::cout << "outTetview = " << (meshIOOptions_.outTetview_ ? "true" : "false") << "\n";
    std::cout << "outVTK = " << (meshIOOptions_.outVTK_ ? "true" : "false") << "\n";
    std::cout << "outOBJ = " << (meshIOOptions_.outOBJ_ ? "true" : "false") << "\n";
    std::cout << "outSTL = " << (meshIOOptions_.outSTL_ ? "true" : "false") << "\n";
    std::cout << "outPLY = " << (meshIOOptions_.outPLY_ ? "true" : "false") << "\n";
    std::cout << "outOFF = " << (meshIOOptions_.outOFF_ ? "true" : "false") << "\n";
    std::cout << "outMSH = " << (meshIOOptions_.outMSH_ ? "true" : "false") << "\n\n";
}

void ComOptionsManager::printLogFileOptions() const
{
    std::cout << "=============================== LogFileOptions ===============================\n";
    std::cout << "format = " << logFileFormatToString(logFileOptions_.format_) << "\n";
    std::cout << "filePath = " << logFileOptions_.filePath_ << "\n";
    std::cout << "fileName = " << logFileOptions_.fileName_ << "\n";
    std::cout << "overwrite = " << (logFileOptions_.overwrite_ ? "true" : "false") << "\n";
    std::cout << "flushEveryWrite = " << (logFileOptions_.flushEveryWrite_ ? "true" : "false") << "\n";
    std::cout << "withTimeStamp = " << (logFileOptions_.withTimeStamp_ ? "true" : "false") << "\n";
    std::cout << "withLevel = " << (logFileOptions_.withLevel_ ? "true" : "false") << "\n";
    std::cout << "withPid = " << (logFileOptions_.withPid_ ? "true" : "false") << "\n";
    std::cout << "withThreadId = " << (logFileOptions_.withThreadId_ ? "true" : "false") << "\n";
    std::cout << "withThreadName = " << (logFileOptions_.withThreadName_ ? "true" : "false") << "\n";
    std::cout << "withAppName = " << (logFileOptions_.withAppName_ ? "true" : "false") << "\n";
    std::cout << "withCategory = " << (logFileOptions_.withCategory_ ? "true" : "false") << "\n";
    std::cout << "withFunction = " << (logFileOptions_.withFunction_ ? "true" : "false") << "\n";
    std::cout << "withMessage = " << (logFileOptions_.withMessage_ ? "true" : "false") << "\n";
    std::cout << "withExtras = " << (logFileOptions_.withExtras_ ? "true" : "false") << "\n";
    std::cout << "useUtc = " << (logFileOptions_.useUtc_ ? "true" : "false") << "\n";
    std::cout << "withMillis = " << (logFileOptions_.withMillis_ ? "true" : "false") << "\n";
    std::cout << "timePattern = " << logFileOptions_.timePattern_ << "\n";
    std::cout << "textFieldSep = " << logFileOptions_.textFieldSep_ << "\n";
    std::cout << "appName = " << logFileOptions_.appName_ << "\n";
    std::cout << "minLevel = " << logLevelToString(logFileOptions_.minLevel_) << "\n";
    std::cout << "language = " << logFileLanguageToString(logFileOptions_.language_) << "\n";
    std::cout << "mirror2Console = " << (logFileOptions_.mirror2Console_ ? "true" : "false") << "\n";
    std::cout << "consoleErrors2Stderr = " << (logFileOptions_.consoleErrors2Stderr_ ? "true" : "false") << "\n\n";
}

void ComOptionsManager::resetToDefaults()
{
    lineSearchOptions_ = LineSearchOptions{};
    gradientDescentOptions_ = GradientDescentOptions{};
    conjugateGradientOptions_ = ConjugateGradientOptions{};
    quasiNewtonOptions_ = QuasiNewtonOptions{};
    meshOptiAlgorithmOptions_ = MeshOptiAlgorithmOptions{};
    meshOptiOptions_ = MeshOptiOptions{};
    meshTetViewIOOptions_ = MeshTetViewIOOptions{};
    meshVTKIOOptions_ = MeshVTKIOOptions{};
    meshOBJIOOptions_ = MeshOBJIOOptions{};
    meshSTLIOOptions_ = MeshSTLIOOptions{};
    meshPLYIOOptions_ = MeshPLYIOOptions{};
    meshOFFIOOptions_ = MeshOFFIOOptions{};
    meshMSHIOOptions_ = MeshMSHIOOptions{};
    meshIOOptions_ = MeshIOOptions{};
    logFileOptions_ = LogFileOptions{};
}

std::string ComOptionsManager::lineSearchWayToString(LineSearchWay way) const
{
    switch (way)
    {
    case LineSearchWay::STRONG_WOLFE:
        return "STRONG_WOLFE";
    case LineSearchWay::MORE_THUENTE:
        return "MORE_THUENTE";
    default:
        return "UNKNOWN";
    }
}

std::string ComOptionsManager::calculateWayToString(CalculateWay way) const
{
    switch (way)
    {
    case CalculateWay::SUM:
        return "SUM";
    case CalculateWay::MAX:
        return "MAX";
    case CalculateWay::P_NORM:
        return "P_NORM";
    case CalculateWay::P_NORM_LIMIT:
        return "P_NORM_LIMIT";
    default:
        return "UNKNOWN";
    }
}

std::string ComOptionsManager::meshSmoothTypeToString(MeshSmoothType type) const
{
    switch (type)
    {
    case MeshSmoothType::CONJUGATE_GRADIENT:
        return "CONJUGATE_GRADIENT";
    case MeshSmoothType::LBFGS:
        return "LBFGS";
    case MeshSmoothType::MinNLC:
        return "MinNLC";
    case MeshSmoothType::GD:
        return "GD";
    case MeshSmoothType::CG:
        return "CG";
    case MeshSmoothType::QN:
        return "QN";
    default:
        return "UNKNOWN";
    }
}

std::string ComOptionsManager::meshSmoothWayToString(MeshSmoothWay way) const
{
    switch (way)
    {
    case MeshSmoothWay::SIGLE_VERT:
        return "SIGLE_VERT";
    case MeshSmoothWay::SIGLE_REGION:
        return "SIGLE_REGION";
    case MeshSmoothWay::PATCH_REGION:
        return "PATCH_REGION";
    case MeshSmoothWay::ALL_REGION:
        return "ALL_REGION";
    default:
        return "UNKNOWN";
    }
}

std::string ComOptionsManager::meshQualityMetricToString(MeshQualityMetric metric) const
{
    switch (metric)
    {
    case MeshQualityMetric::EDGE_RATIO:
        return "EDGE_RATIO";
    case MeshQualityMetric::ASPECT_DELTA:
        return "ASPECT_DELTA";
    case MeshQualityMetric::ASPECT_FROBENIUS:
        return "ASPECT_FROBENIUS";
    case MeshQualityMetric::ASPECT_GAMMA:
        return "ASPECT_GAMMA";
    case MeshQualityMetric::ASPECT_RATIO:
        return "ASPECT_RATIO";
    case MeshQualityMetric::COLLAPSE_RATIO:
        return "COLLAPSE_RATIO";
    case MeshQualityMetric::WEIGHTED_CONDITION_NUMBER:
        return "WEIGHTED_CONDITION_NUMBER";
    case MeshQualityMetric::DISTORTION:
        return "DISTORTION";
    case MeshQualityMetric::JACOBIAN:
        return "JACOBIAN";
    case MeshQualityMetric::MIN_ANGLE:
        return "MIN_ANGLE";
    case MeshQualityMetric::AVERAGE_ANGLE:
        return "AVERAGE_ANGLE";
    case MeshQualityMetric::RADIUS_RATIO:
        return "RADIUS_RATIO";
    case MeshQualityMetric::RELATIVE_SIZE_SQUARED:
        return "RELATIVE_SIZE_SQUARED";
    case MeshQualityMetric::SCALED_JACOBIAN:
        return "SCALED_JACOBIAN";
    case MeshQualityMetric::SHAPE:
        return "SHAPE";
    case MeshQualityMetric::SHAPE_AND_SIZE:
        return "SHAPE_AND_SIZE";
    case MeshQualityMetric::VOLUME:
        return "VOLUME";
    case MeshQualityMetric::SKEWNESS:
        return "SKEWNESS";
    case MeshQualityMetric::SKEWNESS_TRI_ANGLE:
        return "SKEWNESS_TRI_ANGLE";
    case MeshQualityMetric::SKEWNESS_DIHE_ANGLE:
        return "SKEWNESS_DIHE_ANGLE";
    case MeshQualityMetric::EQUIVOLUME_SKEWNESS:
        return "EQUIVOLUME_SKEWNESS";
    case MeshQualityMetric::EQUIANGLE_SKEWNESS:
        return "EQUIANGLE_SKEWNESS";
    case MeshQualityMetric::VOL_LEN_RATIO:
        return "VOL_LEN_RATIO";
    default:
        return "UNKNOWN";
    }
}

std::string ComOptionsManager::meshQualityFuncToString(MeshQualityFunc func) const
{
    switch (func)
    {
    case MeshQualityFunc::EDGE_RATIO_FUNC:
        return "EDGE_RATIO_FUNC";
    case MeshQualityFunc::ASPECT_DELTA_FUNC:
        return "ASPECT_DELTA_FUNC";
    case MeshQualityFunc::ASPECT_FROBENIUS_FUNC:
        return "ASPECT_FROBENIUS_FUNC";
    case MeshQualityFunc::ASPECT_GAMMA_FUNC:
        return "ASPECT_GAMMA_FUNC";
    case MeshQualityFunc::ASPECT_RATIO_FUNC:
        return "ASPECT_RATIO_FUNC";
    case MeshQualityFunc::COLLAPSE_RATIO_FUNC:
        return "COLLAPSE_RATIO_FUNC";
    case MeshQualityFunc::WEIGHTED_CONDITION_NUMBER_FUNC:
        return "WEIGHTED_CONDITION_NUMBER_FUNC";
    case MeshQualityFunc::DISTORTION_FUNC:
        return "DISTORTION_FUNC";
    case MeshQualityFunc::JACOBIAN_FUNC:
        return "JACOBIAN_FUNC";
    case MeshQualityFunc::MIN_ANGLE_FUNC:
        return "MIN_ANGLE_FUNC";
    case MeshQualityFunc::AVERAGE_ANGLE_FUNC:
        return "AVERAGE_ANGLE_FUNC";
    case MeshQualityFunc::RADIUS_RATIO_FUNC:
        return "RADIUS_RATIO_FUNC";
    case MeshQualityFunc::RELATIVE_SIZE_SQUARED_FUNC:
        return "RELATIVE_SIZE_SQUARED_FUNC";
    case MeshQualityFunc::SCALED_JACOBIAN_FUNC:
        return "SCALED_JACOBIAN_FUNC";
    case MeshQualityFunc::SHAPE_FUNC:
        return "SHAPE_FUNC";
    case MeshQualityFunc::SHAPE_AND_SIZE_FUNC:
        return "SHAPE_AND_SIZE_FUNC";
    case MeshQualityFunc::VOLUME_FUNC:
        return "VOLUME_FUNC";
    case MeshQualityFunc::SKEWNESS_FUNC:
        return "SKEWNESS_FUNC";
    case MeshQualityFunc::SKEWNESS_TRI_ANGLE_FUNC:
        return "SKEWNESS_TRI_ANGLE_FUNC";
    case MeshQualityFunc::SKEWNESS_DIHE_ANGLE_FUNC:
        return "SKEWNESS_DIHE_ANGLE_FUNC";
    case MeshQualityFunc::EQUIVOLUME_SKEWNESS_FUNC:
        return "EQUIVOLUME_SKEWNESS_FUNC";
    case MeshQualityFunc::EQUIANGLE_SKEWNESS_FUNC:
        return "EQUIANGLE_SKEWNESS_FUNC";
    case MeshQualityFunc::VOL_LEN_RATIO_FUNC:
        return "VOL_LEN_RATIO_FUNC";
    default:
        return "UNKNOWN";
    }
}

std::string ComOptionsManager::meshElementTypeToString(MeshElementType type) const
{
    switch (type)
    {
    case MeshElementType::Vertex:
        return "Vertex";
    case MeshElementType::Edge:
        return "Edge";
    case MeshElementType::Face:
        return "Face";
    case MeshElementType::Region:
        return "Region";
    default:
        return "UNKNOWN";
    }
}

std::string ComOptionsManager::logLevelToString(LogLevel level) const
{
    switch (level)
    {
    case LogLevel::Trace:
        return "Trace";
    case LogLevel::Debug:
        return "Debug";
    case LogLevel::Info:
        return "Info";
    case LogLevel::Warn:
        return "Warn";
    case LogLevel::Error:
        return "Error";
    case LogLevel::Fatal:
        return "Fatal";
    case LogLevel::Perf:
        return "Perf";
    case LogLevel::Stat:
        return "Stat";
    default:
        return "UNKNOWN";
    }
}

std::string ComOptionsManager::logFileFormatToString(LogFileFormat format) const
{
    switch (format)
    {
    case LogFileFormat::Text:
        return "Text";
    case LogFileFormat::Json:
        return "Json";
    case LogFileFormat::Logfmt:
        return "Logfmt";
    default:
        return "UNKNOWN";
    }
}

std::string ComOptionsManager::logFileLanguageToString(LogFileLanguage lang) const
{
    switch (lang)
    {
    case LogFileLanguage::English:
        return "English";
    case LogFileLanguage::Chinese:
        return "Chinese";
    default:
        return "UNKNOWN";
    }
}

LineSearchWay ComOptionsManager::stringToLineSearchWay(const std::string &str) const
{
    if (str == "STRONG_WOLFE")
        return LineSearchWay::STRONG_WOLFE;
    if (str == "MORE_THUENTE")
        return LineSearchWay::MORE_THUENTE;
    return LineSearchWay::MORE_THUENTE;
}

CalculateWay ComOptionsManager::stringToCalculateWay(const std::string &str) const
{
    if (str == "SUM")
        return CalculateWay::SUM;
    if (str == "MAX")
        return CalculateWay::MAX;
    if (str == "P_NORM")
        return CalculateWay::P_NORM;
    if (str == "P_NORM_LIMIT")
        return CalculateWay::P_NORM_LIMIT;
    return CalculateWay::P_NORM;
}

MeshSmoothType ComOptionsManager::stringToMeshSmoothType(const std::string &str) const
{
    if (str == "CONJUGATE_GRADIENT")
        return MeshSmoothType::CONJUGATE_GRADIENT;
    if (str == "LBFGS")
        return MeshSmoothType::LBFGS;
    if (str == "MinNLC")
        return MeshSmoothType::MinNLC;
    if (str == "GD")
        return MeshSmoothType::GD;
    if (str == "CG")
        return MeshSmoothType::CG;
    if (str == "QN")
        return MeshSmoothType::QN;
    return MeshSmoothType::CONJUGATE_GRADIENT;
}

MeshSmoothWay ComOptionsManager::stringToMeshSmoothWay(const std::string &str) const
{
    if (str == "SIGLE_VERT")
        return MeshSmoothWay::SIGLE_VERT;
    if (str == "SIGLE_REGION")
        return MeshSmoothWay::SIGLE_REGION;
    if (str == "PATCH_REGION")
        return MeshSmoothWay::PATCH_REGION;
    if (str == "ALL_REGION")
        return MeshSmoothWay::ALL_REGION;
    return MeshSmoothWay::SIGLE_VERT;
}

MeshQualityMetric ComOptionsManager::stringToMeshQualityMetric(const std::string &str) const
{
    if (str == "EDGE_RATIO")
        return MeshQualityMetric::EDGE_RATIO;
    if (str == "ASPECT_DELTA")
        return MeshQualityMetric::ASPECT_DELTA;
    if (str == "ASPECT_FROBENIUS")
        return MeshQualityMetric::ASPECT_FROBENIUS;
    if (str == "ASPECT_GAMMA")
        return MeshQualityMetric::ASPECT_GAMMA;
    if (str == "ASPECT_RATIO")
        return MeshQualityMetric::ASPECT_RATIO;
    if (str == "COLLAPSE_RATIO")
        return MeshQualityMetric::COLLAPSE_RATIO;
    if (str == "WEIGHTED_CONDITION_NUMBER")
        return MeshQualityMetric::WEIGHTED_CONDITION_NUMBER;
    if (str == "DISTORTION")
        return MeshQualityMetric::DISTORTION;
    if (str == "JACOBIAN")
        return MeshQualityMetric::JACOBIAN;
    if (str == "MIN_ANGLE")
        return MeshQualityMetric::MIN_ANGLE;
    if (str == "AVERAGE_ANGLE")
        return MeshQualityMetric::AVERAGE_ANGLE;
    if (str == "RADIUS_RATIO")
        return MeshQualityMetric::RADIUS_RATIO;
    if (str == "RELATIVE_SIZE_SQUARED")
        return MeshQualityMetric::RELATIVE_SIZE_SQUARED;
    if (str == "SCALED_JACOBIAN")
        return MeshQualityMetric::SCALED_JACOBIAN;
    if (str == "SHAPE")
        return MeshQualityMetric::SHAPE;
    if (str == "SHAPE_AND_SIZE")
        return MeshQualityMetric::SHAPE_AND_SIZE;
    if (str == "VOLUME")
        return MeshQualityMetric::VOLUME;
    if (str == "SKEWNESS")
        return MeshQualityMetric::SKEWNESS;
    if (str == "SKEWNESS_TRI_ANGLE")
        return MeshQualityMetric::SKEWNESS_TRI_ANGLE;
    if (str == "SKEWNESS_DIHE_ANGLE")
        return MeshQualityMetric::SKEWNESS_DIHE_ANGLE;
    if (str == "EQUIVOLUME_SKEWNESS")
        return MeshQualityMetric::EQUIVOLUME_SKEWNESS;
    if (str == "EQUIANGLE_SKEWNESS")
        return MeshQualityMetric::EQUIANGLE_SKEWNESS;
    if (str == "VOL_LEN_RATIO")
        return MeshQualityMetric::VOL_LEN_RATIO;
    return MeshQualityMetric::ASPECT_RATIO;
}

MeshQualityFunc ComOptionsManager::stringToMeshQualityFunc(const std::string &str) const
{
    if (str == "EDGE_RATIO_FUNC")
        return MeshQualityFunc::EDGE_RATIO_FUNC;
    if (str == "ASPECT_DELTA_FUNC")
        return MeshQualityFunc::ASPECT_DELTA_FUNC;
    if (str == "ASPECT_FROBENIUS_FUNC")
        return MeshQualityFunc::ASPECT_FROBENIUS_FUNC;
    if (str == "ASPECT_GAMMA_FUNC")
        return MeshQualityFunc::ASPECT_GAMMA_FUNC;
    if (str == "ASPECT_RATIO_FUNC")
        return MeshQualityFunc::ASPECT_RATIO_FUNC;
    if (str == "COLLAPSE_RATIO_FUNC")
        return MeshQualityFunc::COLLAPSE_RATIO_FUNC;
    if (str == "WEIGHTED_CONDITION_NUMBER_FUNC")
        return MeshQualityFunc::WEIGHTED_CONDITION_NUMBER_FUNC;
    if (str == "DISTORTION_FUNC")
        return MeshQualityFunc::DISTORTION_FUNC;
    if (str == "JACOBIAN_FUNC")
        return MeshQualityFunc::JACOBIAN_FUNC;
    if (str == "MIN_ANGLE_FUNC")
        return MeshQualityFunc::MIN_ANGLE_FUNC;
    if (str == "AVERAGE_ANGLE_FUNC")
        return MeshQualityFunc::AVERAGE_ANGLE_FUNC;
    if (str == "RADIUS_RATIO_FUNC")
        return MeshQualityFunc::RADIUS_RATIO_FUNC;
    if (str == "RELATIVE_SIZE_SQUARED_FUNC")
        return MeshQualityFunc::RELATIVE_SIZE_SQUARED_FUNC;
    if (str == "SCALED_JACOBIAN_FUNC")
        return MeshQualityFunc::SCALED_JACOBIAN_FUNC;
    if (str == "SHAPE_FUNC")
        return MeshQualityFunc::SHAPE_FUNC;
    if (str == "SHAPE_AND_SIZE_FUNC")
        return MeshQualityFunc::SHAPE_AND_SIZE_FUNC;
    if (str == "VOLUME_FUNC")
        return MeshQualityFunc::VOLUME_FUNC;
    if (str == "SKEWNESS_FUNC")
        return MeshQualityFunc::SKEWNESS_FUNC;
    if (str == "SKEWNESS_TRI_ANGLE_FUNC")
        return MeshQualityFunc::SKEWNESS_TRI_ANGLE_FUNC;
    if (str == "SKEWNESS_DIHE_ANGLE_FUNC")
        return MeshQualityFunc::SKEWNESS_DIHE_ANGLE_FUNC;
    if (str == "EQUIVOLUME_SKEWNESS_FUNC")
        return MeshQualityFunc::EQUIVOLUME_SKEWNESS_FUNC;
    if (str == "EQUIANGLE_SKEWNESS_FUNC")
        return MeshQualityFunc::EQUIANGLE_SKEWNESS_FUNC;
    if (str == "VOL_LEN_RATIO_FUNC")
        return MeshQualityFunc::VOL_LEN_RATIO_FUNC;
    return MeshQualityFunc::ASPECT_RATIO_FUNC;
}

MeshElementType ComOptionsManager::stringToMeshElementType(const std::string &str) const
{
    if (str == "Vertex")
        return MeshElementType::Vertex;
    if (str == "Edge")
        return MeshElementType::Edge;
    if (str == "Face")
        return MeshElementType::Face;
    if (str == "Region")
        return MeshElementType::Region;
    return MeshElementType::Vertex;
}

LogLevel ComOptionsManager::stringToLogLevel(const std::string &str) const
{
    if (str == "Trace")
        return LogLevel::Trace;
    if (str == "Debug")
        return LogLevel::Debug;
    if (str == "Info")
        return LogLevel::Info;
    if (str == "Warn")
        return LogLevel::Warn;
    if (str == "Error")
        return LogLevel::Error;
    if (str == "Fatal")
        return LogLevel::Fatal;
    if (str == "Perf")
        return LogLevel::Perf;
    if (str == "Stat")
        return LogLevel::Stat;
    return LogLevel::Trace;
}

LogFileFormat ComOptionsManager::stringToLogFileFormat(const std::string &str) const
{
    if (str == "Text")
        return LogFileFormat::Text;
    if (str == "Json")
        return LogFileFormat::Json;
    if (str == "Logfmt")
        return LogFileFormat::Logfmt;
    return LogFileFormat::Text;
}

LogFileLanguage ComOptionsManager::stringToLogFileLanguage(const std::string &str) const
{
    if (str == "English")
        return LogFileLanguage::English;
    if (str == "Chinese")
        return LogFileLanguage::Chinese;
    return LogFileLanguage::Chinese;
}

std::string ComOptionsManager::trim(const std::string &str) const
{
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

bool ComOptionsManager::stringToBool(const std::string &str) const
{
    std::string lowerStr = str;
    for (auto &c : lowerStr)
        c = std::tolower(c);
    return (lowerStr == "true" || lowerStr == "1" || lowerStr == "yes" || lowerStr == "on");
}
