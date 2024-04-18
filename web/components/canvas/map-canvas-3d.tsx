"use client";

import { requestEmitter, responseEmitter } from "@/lib/common";
import { LocomotionData } from "@/lib/types";
import { Box, OrbitControls, Point } from "@react-three/drei";
import { Canvas } from "@react-three/fiber";
import { useEffect, useState } from "react";
import { Euler, Vector2, Vector3 } from "three";
import { processLocomotionData } from "./process-locomotion-data";
import { Rover } from "./rover";
import testData from "./test-locomotion-data.json"

export function MapScene() {
  const [position, setPosition] = useState(new Vector3(0, 0, 0));
  const [rotation, setRotation] = useState(new Euler());
  const [data, setData] = useState<LocomotionData[]>([]);
  const [points, setPoints] = useState<Vector2[]>([]);

  const [roverWidth] = useState(0.2)
  const [roverLength] = useState(0.15)
  const [distanceFront, setDistanceFront] = useState(0)
  const [distanceLeft, setDistanceLeft] = useState(0)
  const [distanceRight, setDistanceRight] = useState(0)

  useEffect(() => {
    const { points, path, rotations } = processLocomotionData(data);
    const lastPosition = path.at(-1);

    console.log("points", points);
    setPoints(points)

    if (lastPosition) {
      setPosition(new Vector3(lastPosition.x, 0, lastPosition.y));
    }
    setRotation(new Euler(0, rotations.at(-1), 0));
  }, [data]);

  useEffect(() => {
    const interval = setInterval(() => {
      const item = testData.shift()
      console.log(item)
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
      <group rotation={rotation}>
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
        offset={new Vector3(0, 0, 0.05)}
      />
    </>
  );
}

export function MapCanvas3D() {
  return (
    <div className="h-screen">
      <Canvas style={{ background: "black" }} camera={{ position: [0, 2, 2] }}>
        <MapScene />
        <OrbitControls />
        <ambientLight intensity={0.7} />
      </Canvas>
    </div>
  );
}
