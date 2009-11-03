#ifndef DEBUG_H
#define DEBUG_H
#include <QDebug>
#define Debug qDebug() << __FILE__ << "(" << __LINE__  << ")" 
#endif // DEBUG_H