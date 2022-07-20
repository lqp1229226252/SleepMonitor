#ifndef HELPERS_H
#define HELPERS_H
#include "qcustomplot.h"

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
