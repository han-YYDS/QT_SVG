#ifndef __K_CALCULATE_H_
#define __K_CALCULATE_H_

#include <QPoint>

// 用于进行图形计算, 没有成员变量, 应该作为全局实例

class KCalculate
{
public:
	static double getDistance(const QPoint& p1, const QPoint& p2);
};

#endif
