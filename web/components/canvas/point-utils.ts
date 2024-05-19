import { Vector3 } from "three";

export type LineSegment = {
  start: Vector3;
  end: Vector3;
};

export function findNearestPoint(
  points: Vector3[],
  point: Vector3
): [Vector3, number] {
  let minDistance = Infinity;
  let closestPoint = points[0];

  for (const p of points) {
    if (p === point) {
      continue;
    }

    const distance = p.distanceTo(point);
    if (distance < minDistance) {
      minDistance = distance;
      closestPoint = p;
    }
  }

  return [closestPoint, minDistance];
}

export function mergeAndFilterPoints(
  points: Vector3[],
  maxDistance: number
): Vector3[] {
  const merged = new Set<Vector3>();
  const visited = new Set<Vector3>();

  for (const point of points) {
    if (visited.has(point)) {
      continue;
    }

    visited.add(point);

    const [nearest, distance] = findNearestPoint(points, point);

    if (!nearest) {
      continue;
    }

    visited.add(nearest);

    if (distance > maxDistance) {
      continue;
    }

    merged.add(
      new Vector3(
        point.x + nearest.x,
        point.y + nearest.y,
        point.z + nearest.z
      ).multiplyScalar(0.5)
    );
  }

  return Array.from(merged);
}

export function getLineSegmentsFromPoints(
  points: Vector3[],
  maxDistance: number
): LineSegment[] {
  const segments: LineSegment[] = [];

  const visited = new Set<Vector3>();

  for (const point of points) {
    if (visited.has(point)) {
      continue;
    }

    const [nearest, distance] = findNearestPoint(points, point);

    if (distance > maxDistance) {
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

export function findNearestLineSegment(
  segments: LineSegment[],
  target: LineSegment
): [LineSegment, number, Vector3] {
  let minDistance = Infinity;
  let nearest = segments[0];

  const segmentDirection = target.start.clone().sub(target.end).normalize();

  for (const segment of segments) {
    if (segment === target) {
      continue;
    }

    const spaceTimeDistance = Math.min(
      target.start.distanceTo(segment.start),
      target.start.distanceTo(segment.end),
      target.end.distanceTo(segment.start),
      target.end.distanceTo(segment.end)
    );

    const segmentNorm = segment.start.clone().sub(segment.end).normalize();

    const similarity = Math.abs(segmentDirection.dot(segmentNorm));

    const distance = (1 - similarity) + spaceTimeDistance;

    if (distance < minDistance) {
      minDistance = distance;
      nearest = segment;
    }
  }

  return [nearest, minDistance, segmentDirection];
}

export function mergeLineSegments(
  segments: LineSegment[],
  maxDistance: number
): LineSegment[] {
  const merged: LineSegment[] = [];
  const visited = new Set<LineSegment>();

  for (const segment of segments) {
    if (visited.has(segment)) {
      continue;
    }

    visited.add(segment);

    const [nearest, distance, direction] = findNearestLineSegment(
      segments,
      segment
    );

    if (!nearest) {
      merged.push(segment);
      continue;
    }

    visited.add(nearest);

    if (distance > maxDistance) {
      merged.push(segment);
      merged.push(nearest);
      continue;
    }

    let startDistance = -Infinity;
    let endDistance = Infinity;
    let start = segment.start;
    let end = segment.end;

    const center = segment.start
      .clone()
      .add(segment.end)
      .add(nearest.start)
      .add(nearest.end)
      .multiplyScalar(0.25);

    for (const p of [segment.start, segment.end, nearest.start, nearest.end]) {
      const similarity = direction.dot(p.clone().sub(center));
      if (similarity > startDistance) {
        startDistance = similarity;
        start = p;
      } else if (similarity < endDistance) {
        endDistance = similarity;
        end = p;
      }
    }

    merged.push({
      start,
      end,
    });
  }

  return merged;
}
