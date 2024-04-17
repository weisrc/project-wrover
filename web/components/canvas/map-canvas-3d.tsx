import { Canvas, useFrame } from "@react-three/fiber"
import { PerspectiveCamera } from "@react-three/drei"

export function ThreeRover() {
    return <mesh >
        <boxGeometry args={[1, 1, 1]} />
        <meshStandardMaterial color='orange' />
    </mesh>
}

export function MapCanvas3D() {
    return <>
        <Canvas>
            <ambientLight intensity={Math.PI / 2} />
            <ThreeRover />
            <PerspectiveCamera makeDefault manual />
        </Canvas>
    </>
}