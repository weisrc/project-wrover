"use client";

import { requestEmitter, responseEmitter } from "@/lib/common";
import { LocomotionData } from "@/lib/types";
import { Box, OrbitControls, Point } from "@react-three/drei";
import { Canvas, useFrame } from "@react-three/fiber";
import { useEffect, useState } from "react";
import { Euler, Vector2, Vector3 } from "three";
import { SONAR_TO_M, processLocomotionData } from "./process-locomotion-data";
import { Rover } from "./rover";
import testData from "./test-locomotion-data.json"

export function MapScene() {
  const [position, setPosition] = useState(new Vector3(0, 0, 0));
  const [rotation, setRotation] = useState(0)
  const [smoothRotation, setSmoothRotation] = useState(0)
  const [data, setData] = useState<LocomotionData[]>([]);
  const [points, setPoints] = useState<Vector2[]>([]);

  const [roverWidth] = useState(0.2)
  const [roverLength] = useState(0.15)
  const [roverOffset] = useState(new Vector3(0, 0, -0.05))
  const [distanceFront, setDistanceFront] = useState(0)
  const [distanceLeft, setDistanceLeft] = useState(0)
  const [distanceRight, setDistanceRight] = useState(0)

  const frontOffset = roverLength / 2 - roverOffset.z
  const sideOffset = roverWidth / 2

  useFrame(() => {
    console.log(smoothRotation)
    const adjusted = rotation >= 0 ? rotation : rotation + 2 * Math.PI
    const smoothed = (smoothRotation + adjusted) / 2
    setSmoothRotation(smoothed)
  })

  useEffect(() => {
    const { points, path, rotations } = processLocomotionData(data,
      frontOffset,
      sideOffset,
      sideOffset
    );
    const lastPosition = path.at(-1);

    setPoints(points)

    const sonar = data.at(-1)?.sonar

    if (sonar) {
      setDistanceFront(sonar[0] * SONAR_TO_M)
      setDistanceLeft(sonar[1] * SONAR_TO_M)
      setDistanceRight(sonar[2] * SONAR_TO_M)
    }

    if (lastPosition) {
      setPosition(new Vector3(lastPosition.x, 0, lastPosition.y));
    }
    const lastRotation = rotations.at(-1)
    if (lastRotation) {
      setRotation(lastRotation);
    }
  }, [data]);

  useEffect(() => {
    const interval = setInterval(() => {
      const item = testData.shift()
      if (item) {
        data.push(item as LocomotionData)
        setData(data.slice())
      }
    }, 100)
    return () => clearInterval(interval)
  }, [])

  useEffect(() => {
    function onLocomotion(item: LocomotionData) {
      setData((data) => {
        data.push(item);
        return data;
      });
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
      <group rotation={new Euler(0, smoothRotation, 0)}>
        <group position={position.clone().negate()}>
          {points.map((p, i) => {
            return <Box scale={0.05} position={[p.x, 0, p.y]} key={i} />
          })}
        </group>
      </group>

      <Rover
        width={roverWidth}
        height={0.05}
        length={roverLength}
        distanceFront={distanceFront}
        distanceLeft={distanceLeft}
        distanceRight={distanceRight}
        offset={roverOffset}
      />
    </>
  );
}

export function MapCanvas3D() {
  return (
    <div className="h-screen">
      <Canvas
        style={{ background: "black" }}
        camera={{ position: [0, 2, -2] }}>
        <MapScene />
        <OrbitControls />
        <ambientLight intensity={0.7} />
      </Canvas>
    </div>
  );
}
