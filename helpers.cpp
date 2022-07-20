#include "helpers.h"

Helpers::Helpers()
{

}

bool comQtGraph(const QCPGraphData &a, const QCPGraphData &b)
{
    if (a.key <= b.key)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool comQtBars(const QCPBarsData &a, const QCPBarsData &b)
{
    if (a.key <= b.key)
    {
        return true;
    }
    else
    {
        return false;
    }
}
