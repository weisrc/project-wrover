import { DualOdometer } from "@/lib/dual-odometer";
import { LocomotionData } from "@/lib/types";
import { Vector2 } from "three";

export type ProcessedLocomotionData = {
  points: Vector2[];
  path: Vector2[];
  rotations: number[];
};

export function processLocomotionData(
  data: LocomotionData[]
): ProcessedLocomotionData {
  const path: Vector2[] = [];
  const rotations: number[] = [];

  const points: Vector2[] = [];
  const meter = new DualOdometer(0.2, 0.1);
  for (const item of data) {
    for (const char of item.hall) {
      const which = char.toLowerCase();
      const backwards = char !== which;

      if (which === "l") {
        // move the odometer accordingly
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
      new Vector2(item.sonar[0] / 58e2, 0)
        .rotateAround(new Vector2(0, 0), rotation)
        .add(position)
    );

    points.push(
      new Vector2(item.sonar[0] / 58e2, 0)
        .rotateAround(new Vector2(0, 0), rotation)
        .add(position)
    );

    points.push(
      new Vector2(item.sonar[0] / 58e2, 0)
        .rotateAround(new Vector2(0, 0), rotation)
        .add(position)
    );
  }

  return { points, path, rotations };
}
