import { DualOdometer } from "@/lib/dual-odometer";
import { LocomotionData } from "@/lib/types";
import { Vector2, Vector3 } from "three";

export const SONAR_TO_M = 1 / 58e2;
export const VECTOR2_ZERO = new Vector2(0, 0);

export type ProcessedLocomotionData = {
  points: Vector3[];
  path: Vector2[];
  rotations: number[];
};

export function processLocomotionData(
  data: LocomotionData[],
  frontOffset: number,
  leftOffset: number,
  rightOffset: number,
  frontCutoffDistance = 0.3,
  sideCutoffDistance = 3
): ProcessedLocomotionData {
  const radius = 0.15;
  const delta = 0.036;

  const path: Vector2[] = [];
  const rotations: number[] = [];

  const points: Vector3[] = []; // x, y, time
  const meter = new DualOdometer(radius, delta);

  let time = 0;

  for (const item of data) {
    time += 0.001;

    if (!item.hall) {
      continue;
    }

    for (const char of item.hall) {
      const which = char.toLowerCase();
      const backwards = char !== which;

      if (which === "l") {
        meter.moveLeft(backwards);
      } else if (which === "r") {
        meter.moveRight(backwards);
      }
    }

    const { x, y } = meter.getCenter();
    console.log(x, y);
    const position = new Vector2(x, y);
    const rotation = meter.getDirection();
    path.push(position);
    rotations.push(rotation);

    const frontDistance = item.sonar[0] * SONAR_TO_M;
    const rightDistance = item.sonar[1] * SONAR_TO_M;
    const leftDistance = item.sonar[2] * SONAR_TO_M;

    if (frontDistance < frontCutoffDistance) {
      const item = new Vector2(frontDistance + frontOffset, 0)
        .rotateAround(VECTOR2_ZERO, rotation + Math.PI / 2)
        .add(position);
      points.push(new Vector3(item.x, item.y, time));
    }

    if (leftDistance < sideCutoffDistance) {
      const item = new Vector2(leftDistance + leftOffset, 0)
        .rotateAround(VECTOR2_ZERO, rotation + Math.PI)
        .add(position);
      points.push(new Vector3(item.x, item.y, time));
    }

    if (rightDistance < sideCutoffDistance) {
      const item = new Vector2(rightDistance + rightOffset, 0)
        .rotateAround(VECTOR2_ZERO, rotation)
        .add(position);
      points.push(new Vector3(item.x, item.y, time));
    }
  }

  return { points, path, rotations };
}
