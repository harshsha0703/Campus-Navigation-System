#include "dist.h"

#include <cmath>

static const double PI = 3.14159265358979323846;
static const double EARTH_RADIUS_MI = 3963.1;

static double toRadians(double degrees) {
  return degrees * PI / 180.0;
}

static double toDegrees(double radians) {
  return radians * 180.0 / PI;
}

double distBetween2Points(const Coordinates& c1, const Coordinates& c2) {
  double lat1 = toRadians(c1.lat);
  double lon1 = toRadians(c1.lon);
  double lat2 = toRadians(c2.lat);
  double lon2 = toRadians(c2.lon);

  double dLat = lat2 - lat1;
  double dLon = lon2 - lon1;

  double a = std::sin(dLat / 2) * std::sin(dLat / 2) +
             std::cos(lat1) * std::cos(lat2) *
                 std::sin(dLon / 2) * std::sin(dLon / 2);
  double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

  return EARTH_RADIUS_MI * c;
}

Coordinates centerBetween2Points(const Coordinates& c1, const Coordinates& c2) {
  double lat1 = toRadians(c1.lat);
  double lon1 = toRadians(c1.lon);
  double lat2 = toRadians(c2.lat);
  double lon2 = toRadians(c2.lon);

  double dLon = lon2 - lon1;

  double bx = std::cos(lat2) * std::cos(dLon);
  double by = std::cos(lat2) * std::sin(dLon);

  double lat3 = std::atan2(
      std::sin(lat1) + std::sin(lat2),
      std::sqrt((std::cos(lat1) + bx) * (std::cos(lat1) + bx) + by * by));
  double lon3 = lon1 + std::atan2(by, std::cos(lat1) + bx);

  return Coordinates(toDegrees(lat3), toDegrees(lon3));
}
