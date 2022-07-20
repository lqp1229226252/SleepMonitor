#ifndef MAGNIFYWINDOW_H
#define MAGNIFYWINDOW_H

#include <QWidget>
#include <qcustomplot.h>
#include <qttreepropertybrowser.h>
#include <qtvariantproperty.h>
#include "myenumpropertymanager.h"
#include "helpers.h"
#include "mytracer.h"
#include "QToolBar"
#include "qpropertyanimation.h"

namespace Ui {
class magnifyWindow;
}

class magnifyWindow : public QWidget
{
    Q_OBJECT

public:
    explicit magnifyWindow(QWidget *parent = nullptr);
    magnifyWindow(QCustomPlot* customPlot,
                  QMap<QString, T>& styleFileVariables,
                  QMap<QtVariantProperty*, AxisPropertyIndex> axisVarProperties,
                  QVector<double>& vec,
                  QVector<QVector<QCPGraphData>>& data,
                  const QVector<QString> names,
                  const QVector<QVector<double>>& properties,
                  QWidget *parent = nullptr);
    ~magnifyWindow();

    void setToolBar();
    void drawing(QCustomPlot* customPlot);
    void updateAxisProperties(const AXIS_EPROPERTIES_NAME propertyName);
    void addLineGraph(QCustomPlot* customPlot, QCPGraph* graph, const QVector<QCPGraphData>& vector, const int index);
    void setProperties();
    void setLineProperties(const int index);

private slots:
    void showTracer(QMouseEvent *event);

    void on_action_hide_triggered();

    void on_action_unfold_triggered();

    void animationFinished();

private:
    Ui::magnifyWindow *ui;

    QPointer<QCustomPlot> otherCustomPlot;
    QPointer<QCustomPlot> customPlot;
    QVector<QString> linesName;
    QVector<QVector<double>> curveProperties;
    QPointer<QtTreePropertyBrowser> propertyBrowser;

    QPointer<MyTracer> m_TracerX;
    QVector<QPointer<MyTracer>> m_TracersY;

    bool side_widget_flag;
    QPropertyAnimation* tableAnimation;
    QPropertyAnimation* plotAnimation;

    QToolBar* toolBar;
    bool buttonRelesed;
    QHBoxLayout *vLayoutToolBar;
    QWidget *sideToolBar;
};

#endif // MAGNIFYWINDOW_H
