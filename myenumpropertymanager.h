#ifndef MYENUMPROPERTYMANAGER_H
#define MYENUMPROPERTYMANAGER_H
#include <qtpropertymanager.h>

class myEnumPropertyManager : public QtEnumPropertyManager
{
    Q_OBJECT
public:
    myEnumPropertyManager(QObject *parent = 0);
    ~myEnumPropertyManager();

    bool hasProperty(const QtProperty* propety) {    return hasValue(propety);   }
};

#endif // MYENUMPROPERTYMANAGER_H
