"use client";

import { requestEmitter, responseEmitter } from "@/lib/common";
import { LocomotionData } from "@/lib/types";
import { OrbitControls, Plane, Sphere } from "@react-three/drei";
import { Canvas, useFrame } from "@react-three/fiber";
import { useEffect, useState } from "react";
import { Euler, Quaternion, Vector3 } from "three";
import {
  LineSegment,
  getLineSegmentsFromPoints,
  mergeAndFilterPoints,
  mergeLineSegments,
} from "./point-utils";
import { SONAR_TO_M, processLocomotionData } from "./process-locomotion-data";
import { Rover } from "./rover";

export function MapScene() {
  const [position, setPosition] = useState(new Vector3(0, 0, 0));
  const [rotation, setRotation] = useState(new Quaternion());
  const [smoothRotation, setSmoothRotation] = useState(new Quaternion());
  const [data, setData] = useState<LocomotionData[]>([]);
  const [lines, setLines] = useState<LineSegment[]>([]);

  const [roverWidth] = useState(0.2);
  const [roverLength] = useState(0.15);
  const [roverOffset] = useState(new Vector3(0, 0, -0.05));
  const [distanceFront, setDistanceFront] = useState(0);
  const [distanceLeft, setDistanceLeft] = useState(0);
  const [distanceRight, setDistanceRight] = useState(0);

  const [pointer, setPointer] = useState<Vector3 | null>(null);

  const frontOffset = roverLength / 2 - roverOffset.z;
  const sideOffset = roverWidth / 2;

  useFrame(() => {
    smoothRotation.slerp(rotation, 0.1);
    setSmoothRotation(smoothRotation.clone());
  });

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
    tempPoints = mergeAndFilterPoints(tempPoints, 0.03);
    let tempLines = getLineSegmentsFromPoints(tempPoints, 0.2);
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
  }, [data]);

  function handleLocomotionData(item: LocomotionData) {
    const { sonar } = item;
    setDistanceFront(sonar[0] * SONAR_TO_M);
    setDistanceRight(sonar[1] * SONAR_TO_M);
    setDistanceLeft(sonar[2] * SONAR_TO_M);
    if (item.hall) {
      data.push(item);
      setData([...data]);
    }
  }

  useEffect(() => {
    function onLocomotion(item: LocomotionData) {
      handleLocomotionData(item);
      requestLocomotion();
    }
    function requestLocomotion() {
      setTimeout(() => {
        requestEmitter.emit("locomotion", {});
      }, 100);
    }
    requestLocomotion();
    responseEmitter.on("locomotion", onLocomotion);
    return () => {
      responseEmitter.off("locomotion", onLocomotion);
    };
  }, []);

  return (
    <>
      <group quaternion={smoothRotation}>
        <group position={position.clone().negate()}>
          {lines.map((p, i) => {
            const center = p.start.clone().add(p.end).multiplyScalar(0.5);
            center.z = center.y;
            center.y = 0.1;
            return (
              <mesh
                key={i}
                position={center}
                rotation={
                  new Euler(
                    0,
                    -Math.atan2(p.end.y - p.start.y, p.end.x - p.start.x),
                    0
                  )
                }
              >
                <boxGeometry args={[p.start.distanceTo(p.end), 0.2, 0.05]} />
                <meshStandardMaterial color="#ccc" />
              </mesh>
            );
          })}
          <gridHelper args={[100, 100, "dimgray", "dimgray"]} />
        </group>
      </group>

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

      <Plane
        args={[100, 100]}
        rotation={new Euler(-Math.PI / 2, 0, 0)}
        onPointerMove={(e) => {
          e.point.x *= -1;
          e.point.y = 1;
          setPointer(e.point);
        }}
        onPointerLeave={() => {
          setPointer(null);
        }}
      >
        <meshStandardMaterial color="gray" />
      </Plane>
      <pointLight position={pointer ?? [0, 1, 0]} intensity={1} />
    </>
  );
}

export function MapCanvas3D() {
  return (
    <div className="w-full h-full">
      <Canvas style={{ background: "black" }} camera={{ position: [0, 2, -2] }}>
        <group scale={[-1, 1, 1]}>
          <MapScene />
          <OrbitControls
            mouseButtons={{
              LEFT: 0,
              MIDDLE: 2,
            }}
          />
          <ambientLight intensity={1} />
        </group>
      </Canvas>
    </div>
  );
}
