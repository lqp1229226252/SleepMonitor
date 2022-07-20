#ifndef DYNAMICPLOT_H
#define DYNAMICPLOT_H

#include <QWidget>
#include <QMouseEvent>
#include "qcustomplot.h"
#include <QMetaType>
#include <qttreepropertybrowser.h>
#include <qtvariantproperty.h>
#include <qteditorfactory.h>
#include "myenumpropertymanager.h"
#include "helpers.h"
#include "QToolBar"
#include <QVariant>
#include "iostream"
#include "mytracer.h"
#include "qpropertyanimation.h"


namespace Ui {
class DynamicPlot;
}

class DynamicPlot : public QWidget
{
    Q_OBJECT

public:

    enum LINE_STYLE
    {
        lsNone        ///< data points are not connected with any lines (e.g. data only represented
        ///< with symbols according to the scatter style, see \ref setScatterStyle)
        ,lsLine       ///< data points are connected by a straight line
        ,lsStepLeft   ///< line is drawn as steps where the step height is the value of the left data point
        ,lsStepRight  ///< line is drawn as steps where the step height is the value of the right data point
        ,lsStepCenter ///< line is drawn as steps where the step is in between two data points
        ,lsImpulse    ///< each data point is represented by a line parallel to the value axis, which reaches from the data point to the zero-value-line

    };
    Q_ENUM(LINE_STYLE)

    enum SCATTER_SHAPE { ssNone       ///< no scatter symbols are drawn (e.g. in QCPGraph, data only represented with lines)
                         ,ssDot       ///< \enumimage{ssDot.png} a single pixel (use \ref ssDisc or \ref ssCircle if you want a round shape with a certain radius)
                         ,ssCross     ///< \enumimage{ssCross.png} a cross
                         ,ssPlus      ///< \enumimage{ssPlus.png} a plus
                         ,ssCircle    ///< \enumimage{ssCircle.png} a circle
                         ,ssDisc      ///< \enumimage{ssDisc.png} a circle which is filled with the pen's color (not the brush as with ssCircle)
                         ,ssSquare    ///< \enumimage{ssSquare.png} a square
                         ,ssDiamond   ///< \enumimage{ssDiamond.png} a diamond
                         ,ssStar      ///< \enumimage{ssStar.png} a star with eight arms, i.e. a combination of cross and plus
                         ,ssTriangle  ///< \enumimage{ssTriangle.png} an equilateral triangle, standing on baseline
                         ,ssTriangleInverted ///< \enumimage{ssTriangleInverted.png} an equilateral triangle, standing on corner
                         ,ssCrossSquare      ///< \enumimage{ssCrossSquare.png} a square with a cross inside
                         ,ssPlusSquare       ///< \enumimage{ssPlusSquare.png} a square with a plus inside
                         ,ssCrossCircle      ///< \enumimage{ssCrossCircle.png} a circle with a cross inside
                         ,ssPlusCircle       ///< \enumimage{ssPlusCircle.png} a circle with a plus inside
                         ,ssPeace     ///< \enumimage{ssPeace.png} a circle, with one vertical and two downward diagonal lines
                         ,ssPixmap    ///< a custom pixmap specified by \ref setPixmap, centered on the data point coordinates
                         ,ssCustom    ///< custom painter operations are performed per scatter (As QPainterPath, see \ref setCustomPath)
                       };
    Q_ENUM(SCATTER_SHAPE)

    explicit DynamicPlot(QWidget *parent = nullptr, const QString fileName = "");
    ~DynamicPlot() override;

    void setToolBar();
    void readData();
    void init();
    void readPropertiesFile(const QString fName);
    void writePropertiesFile(QString fName);
    void calculatesPropertyValue(const QVector<QVector<QCPGraphData>>& vector, QVector<QVector<double>>& properties);
    void getXYLowerAndUpper(const QVector<QVector<QCPGraphData>>& vector);
    QString colorToString(const QColor color);
    QColor stringToColor(const QString str);

    //自己加的Initialize()接口
    void Initialize(void);
    void drawing_Initialize(QCustomPlot* customPlot);
    void SetValues(double val);
    void AddDatum(double val);
    //上层调用这个函数设置曲线名称
    void setLineName(QString name);
    void setXYvisbile(bool f);

    void initDrawing();
    void addLineGraph(QCustomPlot* customPlot,  const int index);
    void setAxisProperties();
    void setLineProperties(const int index);
    void setCurveProperties(const QVector<QVector<double>>& vec);
    void axisValueChanged(QtProperty* property, const QVariant& value);
    void lineValueChanged(QtProperty* property, const QVariant& value);
    void enumValueChanged (QtProperty *property, int val);
    void setBackgrand();
    void updateAxisProperties(const AXIS_EPROPERTIES_NAME propertyName);
    void updateLineProperties(const LINE_EPROPERTIES_NAME propertyName);
    void updateEnumProperties(const ENUM_EPROPERTIES_NAME propertyName);
    void updateRange(const double xLower, const double xUpper, const double yLower, const double yUpper);
    void magnify(QVector<double> vec);
    void recover();
    void cutArrs(const QVector<QVector<QCPGraphData>>& vec, QVector<QVector<QCPGraphData>>& result, const double xLower, const double xUpper, const double yLower, const double yUpper);
    int binaryLookup(const QVector<QCPGraphData>& vec, const double target, const bool isClosest);
    void setStyleFileVariables();

    QString getFileName() { return fileName;    }

    //自己加入
    void timerEvent(QTimerEvent *event);
    bool flag_setTableVisible=false;



signals:
    void fileNameSignal(const QString fileName);
    void sendMagnifyInf(QCustomPlot* customPlot,
                        QMap<QString, T>&,
                        QMap<QtVariantProperty*, AxisPropertyIndex>,
                        QVector<double>&,
                        QVector<QVector<QCPGraphData>>&,
                        QVector<QString>&,
                        QVector<QVector<double>>&);

protected:
    void closeEvent(QCloseEvent *event) override;

public:
    QPointer<QCustomPlot> myCustomPlot;	    //传入实例化的QcustomPlot
    QVector<QCPGraph*> myGraph;


protected:
    //这里是存传入的绘图图层
    bool isMagnify;                                  //是否已经放大
    QVector<QString> lsName;                            //放大时存储曲线名字
    QVector<QVector<QCPGraphData>> data;             //传入的数据，
    QVector<QVector<QCPGraphData>> dataForMagnify;   //存放放大时的数据
    QVector<QVector<QCPGraphData>> dataTemp;   //存放放大时的中间数据
    QVector<QVector<double>> curvePropertiesFM;         //存放放大时的属性值
    QVector<QVector<double>> curvePropertiesTemp;         //存放放大时的属性值

    QPointer<MyTracer> m_TracerX;                     // 跟踪的点x
    QVector<QPointer<MyTracer>> m_TracersY;

    QToolBar* toolBar;
    QHBoxLayout *vLayoutToolBar;
    QWidget *sideToolBar;

    QMap<QString, T> styleFileVariables;
    QVector<QMap<QString, T>> styleFileVarForLines;

    bool side_widget_flag;
    bool buttonRelesed;
    QPropertyAnimation* tableAnimation;
    QPropertyAnimation* plotAnimation;
public slots:
    //自己加的响应是否打开属性窗口槽函数
    void setTableVisible();


private slots:
    void showTracer(QMouseEvent *event);

    void on_action_open_triggered();

    void on_action_save_triggered();

    void on_action_save_as_triggered();

    void on_action_hide_triggered();

    void on_action_unfold_triggered();

    void animationFinished();

    void on_action_magnify_triggered();

    void on_action_new_win_magnify_triggered();

    void on_action_recover_triggered();



private:
    Ui::DynamicPlot *ui;

    bool magnifyForNewWindow;

    QString fileName;
    QString styleFileName;

    //x轴最大值和最小值
    double xAxisUpper;
    double xAxisLower;
    double yAxisUpper;
    double yAxisLower;
    //背景颜色
    QColor background0;
    QColor background1;
    //坐标轴颜色
    QColor xGridPen;                    //网格线
    QColor yGridPen;
    QColor xSubGridPen;                 //子网格线
    QColor ySubGridPen;
    Qt::PenStyle gridPenStyle;          //网格线样式
    Qt::PenStyle subGridPenStyle;       //子网格线样式
    bool xSubGridVisible;               //子网格线是否可见
    bool ySubGridVisible;
    QColor zeroLinePen;                 //0网格线颜色
    QColor xBasePen;                    //坐标轴颜色
    QColor yBasePen;
    int basePenWidth;                //坐标轴宽度
    QColor tickPen;                     //刻度线颜色
    QColor labelPen;                    //字体颜色
    int labelSize;                  //字体大小

    //坐标轴名字
    QString xAxisName;
    QString yAxisName;
    QString xAxisUnit;
    QString yAxisUnit;
    // Lines
    int linesNum=1;
    int curLineIndex;
    QVector<QString> linesName;
    QVector<QPen> linesPen;
    QVector<QColor> linesColor;
    QVector<Qt::PenStyle> linePenStyle;
    QVector<QCPGraph::LineStyle> lineStyle;
    QVector<QCPScatterStyle::ScatterShape> lineScatterStyle;
    int scatterSize;
    QVector<bool> lineVisible;
    QVector<int> linesWidth;

    QVector<QVector<double>> curveProperties;   //存储曲线属性值：maximum、minimum、average、PV、RMS

    QPointer<QtTreePropertyBrowser> propertyBrowser;
    //    QVector<QtVariantPropertyManager*> pVarManagers;
    // 枚举管理器和工厂的声明
    QVector<myEnumPropertyManager*> enumManagerLines;
    QtEnumEditorFactory *enumFactoryLine;
    myEnumPropertyManager *enumManagerAxis;
    QtEnumEditorFactory *enumFactoryAxis;

    QMap<QtProperty*, AxisPropertyIndex> axisProperties;
    QMap<QtVariantProperty*, AxisPropertyIndex> axisVarProperties;
    QMap<QtProperty*, EnumPropertyIndex> enumProperties;
    QMap<QtProperty*, LinePropertyIndex> linesProperties;
    QMap<QtVariantProperty*, LinePropertyIndex> linesVarProperties;
    QVector<QMap<EPROPERTY_VALUE, QtVariantProperty*>> idToProperty;

    //自己加的
    quint64 sampleNumber = 0;

    bool flag_Initialize = false;

    //曲线名称
    QString line_name;



};

#endif // LINEARPLOT_H
