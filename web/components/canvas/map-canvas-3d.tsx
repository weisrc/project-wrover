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
  const [data, setData] = useState<LocomotionData[]>(testData as LocomotionData[]);
  const [points, setPoints] = useState<Vector2[]>([]);

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
      {points.map((p, i) => {
        return <Box scale={0.05} position={[p.x, 0, p.y]} key={i}/>
      })}
      
      <Rover
        width={0.2}
        height={0.05}
        length={0.2}
        distanceFront={1}
        distanceLeft={1}
        distanceRight={1}
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
