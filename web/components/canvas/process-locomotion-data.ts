import { DualOdometer } from "@/lib/dual-odometer";
import { LocomotionData } from "@/lib/types";
import { Vector2 } from "three";

export const SONAR_TO_M = 1 / 58e2

export type ProcessedLocomotionData = {
  points: Vector2[];
  path: Vector2[];
  rotations: number[];
};

export function processLocomotionData(
  data: LocomotionData[],
  frontOffset: number,
  leftOffset: number,
  rightOffset: number
): ProcessedLocomotionData {

  const radius = 0.15;
  const delta = 0.035;

  const path: Vector2[] = [];
  const rotations: number[] = [];

  const points: Vector2[] = [];
  const meter = new DualOdometer(radius, delta);
  for (const item of data) {
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
    const position = new Vector2(x, y);
    const rotation = meter.getDirection();
    path.push(position);
    rotations.push(rotation);

    points.push(
      new Vector2(item.sonar[0] * SONAR_TO_M + frontOffset, 0)
        .rotateAround(new Vector2(0, 0), rotation + Math.PI / 2)
        .add(position)
    );

    points.push(
      new Vector2(item.sonar[1] * SONAR_TO_M + leftOffset, 0)
        .rotateAround(new Vector2(0, 0), rotation - Math.PI)
        .add(position)
    );

    points.push(
      new Vector2(item.sonar[2] * SONAR_TO_M + rightOffset, 0)
        .rotateAround(new Vector2(0, 0), rotation)
        .add(position)
    );
  }

  return { points, path, rotations };
}
