"use client";

import { Box, Html, Line } from "@react-three/drei";
import { Euler, Vector3 } from "three";

export type RoverProps = {
  position?: Vector3;
  rotation?: Euler;
  distanceFront: number;
  distanceLeft: number;
  distanceRight: number;
  width: number;
  height: number;
  length: number;
  offset: Vector3;
};

export function Rover(props: RoverProps) {
  const halfLength = props.length / 2 + props.offset.z;
  const halfWidth = props.width / 2;

  return (
    <group position={props.position} rotation={props.rotation}>
      <mesh position={props.offset}>
        <boxGeometry args={[props.width, props.height, props.length]} />
        <meshNormalMaterial />
      </mesh>
      <RoverRay
        start={new Vector3(0, 0, halfLength)}
        end={new Vector3(0, 0, props.distanceFront - halfLength)}
      />
      <RoverRay
        start={new Vector3(-halfWidth, 0, 0)}
        end={new Vector3(-props.distanceLeft - halfWidth, 0, 0)}
      />
      <RoverRay
        start={new Vector3(halfWidth, 0, 0)}
        end={new Vector3(props.distanceRight + halfWidth, 0, 0)}
      />
    </group>
  );
}

type RoverRayProps = {
  start: Vector3;
  end: Vector3;
};

export function RoverRay(props: RoverRayProps) {
  const distance = props.start.distanceTo(props.end);

  return (
    <>
      <Html
        position={props.start.clone().add(props.end).divideScalar(2)}
        className="text-white select-none text-sm"
      >
        {distance.toFixed(2)}m
      </Html>
      <Box
        args={[distance, 0.005, 0.005]}
        position={props.start.clone().add(props.end).divideScalar(2)}
        rotation={
          new Euler(
            0,
            -Math.atan2(
              props.end.z - props.start.z,
              props.end.x - props.start.x
            ),
            0
          )
        }
      >
        <meshBasicMaterial color={"lime"}/>
      </Box>
    </>
  );
}
