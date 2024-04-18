import { Vector2 } from "three";

type LineSegment = {
  start: Vector2;
  end: Vector2;
};

function findNearestPoint(points: Vector2[], point: Vector2): Vector2 {
  let minDistance = Infinity;
  let closestPoint = points[0];

  for (const p of points) {
    const distance = p.distanceTo(point);
    if (distance < minDistance) {
      minDistance = distance;
      closestPoint = p;
    }
  }

  return closestPoint;
}

function getLineSegmentsFromPoints(
  points: Vector2[],
  maxDistance: number
): LineSegment[] {
  const segments: LineSegment[] = [];

  const visited = new Set<Vector2>();

  for (const point of points) {
    if (visited.has(point)) {
      continue;
    }

    const nearest = findNearestPoint(points, point);

    if (point.distanceTo(nearest) > maxDistance) {
      continue;
    }

    visited.add(point);
    visited.add(nearest);

    segments.push({
      start: point,
      end: nearest,
    });
  }

  return segments;
}
