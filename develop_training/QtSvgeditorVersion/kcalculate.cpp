#include "kcalculate.h"
#include <QtMath>

double KCalculate::getDistance(const QPoint & p1, const QPoint & p2)
{
	double dx = static_cast<double>(p2.x()) - static_cast<double>(p1.x());
	double dy = static_cast<double>(p2.y()) - static_cast<double>(p1.y());
	return std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
}
