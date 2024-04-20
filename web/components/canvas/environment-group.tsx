"use client";

import { useFrame } from "@react-three/fiber";
import { useState } from "react";
import { Euler, Quaternion, Vector3 } from "three";
import { LineSegment } from "./point-utils";

function useSmoothRotation(rotation: Quaternion) {
  const [smoothRotation, setSmoothRotation] = useState(new Quaternion());

  useFrame(() => {
    smoothRotation.slerp(rotation, 0.1);
    setSmoothRotation(smoothRotation.clone());
  });

  return smoothRotation;
}

export function EnvironmentGroup(props: {
  position: Vector3;
  lines: LineSegment[];
  rotation: Quaternion;
}) {
  const smoothRotation = useSmoothRotation(props.rotation);

  return (
    <group quaternion={smoothRotation}>
      <group position={props.position.clone().negate()}>
        {props.lines.map((p, i) => {
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
  );
}
