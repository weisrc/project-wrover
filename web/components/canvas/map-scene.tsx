"use client";

import { useState } from "react";
import { Vector3 } from "three";
import { EnvironmentGroup } from "./environment-group";
import { PlanePointer } from "./plane-pointer";
import { Rover } from "./rover";
import { useLocomotionData } from "./use-locomotion-data";
import { useProcessedData } from "./use-processed-data";

export function MapScene() {
  const [roverWidth] = useState(0.2);
  const [roverLength] = useState(0.15);
  const [roverOffset] = useState(new Vector3(0, 0, -0.05));

  const frontOffset = roverLength / 2 - roverOffset.z;
  const sideOffset = roverWidth / 2;

  const { data, distanceFront, distanceLeft, distanceRight } =
    useLocomotionData();

  const { position, rotation, lines } = useProcessedData(
    data,
    frontOffset,
    sideOffset
  );

  return (
    <>
      <EnvironmentGroup position={position} lines={lines} rotation={rotation} />

      <Rover
        width={roverWidth}
        height={0.05}
        length={roverLength}
        distanceFront={distanceFront}
        distanceLeft={distanceLeft}
        distanceRight={distanceRight}
        position={new Vector3(0, 0.025, 0)}
        offset={roverOffset}
      />

      <PlanePointer />
    </>
  );
}
