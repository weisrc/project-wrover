import { Canvas, useFrame } from "@react-three/fiber"
import { Line, OrbitControls, PerspectiveCamera } from "@react-three/drei"
import * as THREE from "three";
import { useState } from "react";
import { LineGeometry } from "three/examples/jsm/Addons.js";

type RoverProps = {
    position: THREE.Vector3
    rotation: THREE.Euler
}

export function Rover({ position, rotation }: RoverProps) {
    return <mesh position={position} rotation={rotation}>
        <boxGeometry args={[1, 1, 1]} />
        <meshStandardMaterial color='orange' />
        <Line
            points={[
                new THREE.Vector3(0, 0, 0),
                new THREE.Vector3(0, 0, -10)
            ]}
            lineWidth={2}
            dashed
        />
        <Line
            points={[
                new THREE.Vector3(0, 0, 0),
                new THREE.Vector3(10, 0, 0)
            ]}
            lineWidth={2}
            dashed
        />
        <Line
            points={[
                new THREE.Vector3(0, 0, 0),
                new THREE.Vector3(-10, 0, 0)
            ]}
            lineWidth={2}
            dashed
        />
    </mesh>
}

export function MapCanvas3D() {

    const [position, setPosition] = useState(new THREE.Vector3(0, 0, 0));
    const [rotation, setRotation] = useState(new THREE.Euler())


    return <>
        <Canvas>
            <ambientLight intensity={Math.PI / 2} />
            <Rover position={position} rotation={rotation} />
            <OrbitControls target={position} />
        </Canvas>
    </>
}