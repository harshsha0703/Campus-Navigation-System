#pragma once

/// A latitude/longitude pair, in degrees.
struct Coordinates {
  double lat;
  double lon;

  Coordinates() : lat(0.0), lon(0.0) {}
  Coordinates(double lat, double lon) : lat(lat), lon(lon) {}
};

/// Great-circle distance between two coordinates, in miles.
double distBetween2Points(const Coordinates& c1, const Coordinates& c2);

/// Midpoint along the great-circle path between two coordinates.
Coordinates centerBetween2Points(const Coordinates& c1, const Coordinates& c2);
