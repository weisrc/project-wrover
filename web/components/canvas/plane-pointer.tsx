"use client";

import { Plane } from "@react-three/drei";
import {
  Dispatch,
  SetStateAction,
  createContext,
  useContext,
  useState,
} from "react";
import { Euler, Vector3 } from "three";

type PlanePointerContextType = {
  pointer: Vector3 | null;
  setPointer: Dispatch<SetStateAction<Vector3 | null>>;
  locked: boolean;
  setLocked: Dispatch<SetStateAction<boolean>>;
};

const PlanePointerContext = createContext<PlanePointerContextType>({
  pointer: null,
  setPointer: () => {},
  locked: false,
  setLocked: () => {},
});

export function PlanePointerProvider(props: { children: React.ReactNode }) {
  const [pointer, setPointer] = useState<Vector3 | null>(null);
  const [locked, setLocked] = useState(false);

  return (
    <PlanePointerContext.Provider
      value={{ pointer, setPointer, locked, setLocked }}
    >
      {props.children}
    </PlanePointerContext.Provider>
  );
}

export function usePlanePointer() {
  const { pointer, setPointer, locked, setLocked } =
    useContext(PlanePointerContext);

  return {
    pointer,
    locked,
    setLocked,
    setPointer: (e: Vector3 | null) => {
      if (locked) return;
      setPointer(e);
    },
  };
}

export function PlanePointer() {
  const { pointer, setPointer } = usePlanePointer();

  return (
    <>
      <Plane
        args={[100, 100]}
        rotation={new Euler(-Math.PI / 2, 0, 0)}
        onPointerMove={(e) => {
          e.point.x *= -1; // threejs is left-handed
          e.point.y = 1;
          setPointer(e.point);
        }}
        onPointerLeave={() => {
          setPointer(null);
        }}
      >
        <meshStandardMaterial color="#333" />
      </Plane>
      <pointLight position={pointer ?? [0, 1, 0]} intensity={1} />
    </>
  );
}
