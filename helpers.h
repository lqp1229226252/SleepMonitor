#ifndef HELPERS_H
#define HELPERS_H
#include "qcustomplot.h"



//属性结构体
typedef struct _PLOT_PROPERTY
{

    //背景设置
    QColor background0;                 //背景颜色上
    QColor background1;                 //背景颜色下

    //坐标轴设置
    QColor xBasePen;                    //x坐标轴颜色
    QColor yBasePen;                    //y坐标轴颜色
    int basePenWidth;                   //坐标轴宽度
    QColor tickPen;                     //刻度线颜色
    QColor labelPen;                    //字体颜色
    int labelSize;                      //字体大小
    QColor xGridPen;                    //x轴网格线颜色
    QColor yGridPen;                    //y轴网格线颜色
    bool xSubGridVisible;               //子网格线x可见
    bool ySubGridVisible;               //子网格线y可见
    QColor xSubGridPen;                 //x轴子网格线颜色
    QColor ySubGridPen;                 //y轴子网格线颜色
    Qt::PenStyle gridPenStyle;          //网格线样式
    Qt::PenStyle subGridPenStyle;       //子网格线样式
    QColor zeroLinePen;                 //0网格线颜色
    //坐标轴名字单位
    QString xAxisName;
    QString yAxisName;
    QString xAxisUnit;
    QString yAxisUnit;
    //x y 上下限
    //x轴最大值和最小值
    double xAxisUpper;
    double xAxisLower;
    double yAxisUpper;
    double yAxisLower;


    //曲线设置

    QString linesName;              //曲线名称
    bool lineVisible;               //曲线可见
    QColor linesColor;              //曲线颜色
    int linesWidth;                 //曲线宽度
    Qt::PenStyle linePenStyle;      //曲线样式
    QCPGraph::LineStyle lineStyle;  //曲线连接方式
    QCPScatterStyle::ScatterShape lineScatterStyle;//曲线节点样式
}PLOT_PROPERTY;

enum class AXIS_EPROPERTIES_NAME : char16_t
{
    FILE_NAME,
    BACKGROUND_0,
    BACKGROUND_1,
    X_AXIS_UNIT,
    Y_AXIS_UNIT,
    X_AXIS_NAME,
    Y_AXIS_NAME,
    X_AXIS_UPPER,
    X_AXIS_LOWER,
    Y_AXIS_UPPER,
    Y_AXIS_LOWER,
    X_GRID_PEN,
    Y_GRID_PEN,
    X_SUBGRID_PEN,
    Y_SUBGRID_PEN,
    X_SUBGRID_VISIBLE,
    Y_SUBGRID_VISIBLE,
    ZERO_LINE_PEN,
    X_BASE_PEN,
    Y_BASE_PEN,
    BASE_PEN_WIDTH,
    TICK_PEN,
    LABEL_PEN,
    LABEL_SIZE,
};

struct AxisPropertyIndex
{
    AXIS_EPROPERTIES_NAME first;
    QString variableName;
};

enum class LINE_EPROPERTIES_NAME : char16_t
{
    LINES_NAME,
    LINE_VISIBLE,
    LINES_COLOR,
    LINE_WIDTH,
};

struct LinePropertyIndex
{
    LINE_EPROPERTIES_NAME first;
    int second;
    QString variableName;
};

enum class ENUM_EPROPERTIES_NAME : char16_t
{
    GRID_PEN_STYLE,
    SUBGRID_PEN_STYLE,
    LINE_PEN_STYLE,
    LINE_STYLE,
    LINE_SCATTER_STYLE,
};

struct EnumPropertyIndex
{
    ENUM_EPROPERTIES_NAME first;
    int second;
    QString variableName;
};

enum class VARIABLE_STYLE : char16_t
{
    STYLE_INT,
    STYLE_BOOL,
    STYLE_DOUBLE,
    STYLE_STRING,
    STYLE_COLOR,
    STYLE_PEN_STYLE,
    STYLE_LINE_STYLE,
    STYLE_SCATTER_STYLE
};

struct T
{
    QVariant vPtr;
    QString name;
    VARIABLE_STYLE style;
};

enum class PILLAR_EPROPERTIES_NAME : char16_t
{
    FILE_NAME,
    BACKGROUND_0,
    BACKGROUND_1,
    PILLAR_NAME,
    X_AXIS_UNIT,
    Y_AXIS_UNIT,
    X_AXIS_NAME,
    Y_AXIS_NAME,
    //    X_AXIS_UPPER,
    //    X_AXIS_LOWER,
    IS_SOLID_COLOR,
    PILLAR_COLOR,
    PILLAR_COLOR_0,
    PILLAR_COLOR_1,
    X_GRID_PEN,
    Y_GRID_PEN,
    Y_SUBGRID_PEN,
    GRID_PEN_STYLE,
    SUBGRID_PEN_STYLE,
    Y_SUBGRID_VISIBLE,
    ZERO_LINE_PEN,
    X_BASE_PEN,
    Y_BASE_PEN,
    BASE_PEN_WIDTH,
    TICK_PEN,
    LABEL_PEN,
    LABEL_SIZE,
};

struct PillarPropertyIndex
{
    PILLAR_EPROPERTIES_NAME first;
    QString variableName;
};

enum class EPROPERTY_VALUE : char16_t
{
    MAXIMUM,
    MINIMUM,
    AVERAGE,
    PV,
    RMS
};

bool comQtGraph(const QCPGraphData &a, const QCPGraphData &b);
bool comQtBars(const QCPBarsData &a, const QCPBarsData &b);

class Helpers
{
public:
    Helpers();
};

#endif // HELPERS_H
