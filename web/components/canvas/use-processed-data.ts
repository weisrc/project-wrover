import { LocomotionData } from "@/lib/types";
import { useEffect, useState } from "react";
import { Euler, Quaternion, Vector3 } from "three";
import {
  LineSegment,
  getLineSegmentsFromPoints,
  mergeAndFilterPoints,
  mergeLineSegments,
} from "./point-utils";
import { processLocomotionData } from "./process-locomotion-data";

export function useProcessedData(
  data: LocomotionData[],
  frontOffset: number,
  sideOffset: number
) {
  const [position, setPosition] = useState(new Vector3(0, 0, 0));
  const [rotation, setRotation] = useState(new Quaternion());
  const [lines, setLines] = useState<LineSegment[]>([]);

  useEffect(() => {
    const { points, path, rotations } = processLocomotionData(
      data,
      frontOffset,
      sideOffset,
      sideOffset
    );
    console.log(data);
    const lastPosition = path.at(-1);

    let tempPoints = points;
    // tempPoints = mergeAndFilterPoints(tempPoints, 4);
    let tempLines = getLineSegmentsFromPoints(tempPoints, 1);
    for (let i = 0; i < 100; i++) {
      const nextLines = mergeLineSegments(tempLines, 0.2);
      if (nextLines.length === tempLines.length) {
        break;
      }
      tempLines = nextLines;
    }
    setLines(tempLines);

    if (lastPosition) {
      setPosition(new Vector3(lastPosition.x, 0, lastPosition.y));
    }
    const lastRotation = rotations.at(-1);
    if (lastRotation) {
      setRotation(new Quaternion().setFromEuler(new Euler(0, lastRotation, 0)));
    }
  }, [data, frontOffset, sideOffset]);

  return { position, rotation, lines };
}
