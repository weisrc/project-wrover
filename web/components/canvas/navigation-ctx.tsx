import { Sphere } from "@react-three/drei";
import { createContext, useContext, useEffect, useState } from "react";
import { Vector3 } from "three";
import { getLastMeter } from "./process-locomotion-data";
import { requestEmitter, responseEmitter } from "@/lib/common";

export const NavigationContext = createContext<
  [Vector3 | undefined, (x: number, y: number) => void]
>([undefined, () => {}]);

export function NavigationProvider(props: { children: React.ReactNode }) {
  const [position, setPosition] = useState<Vector3>();

  useEffect(() => {
    function listener() {
      setPosition(undefined);
    }

    responseEmitter.on("navigation", listener);

    return () => {
      responseEmitter.off("navigation", listener);
    };
  }, []);

  function set(x: number, y: number) {
    const meter = getLastMeter();
    if (!meter) return;

    requestEmitter.emit("configureOdometer", {
      left: {
        x: meter.left.x,
        y: meter.left.y,
      },
      right: {
        x: meter.right.x,
        y: meter.right.y,
      },
      delta: meter.delta,
    });

    requestEmitter.emit("navigate", {
      x,
      y,
    });

    setPosition(new Vector3(x, 0, y));
  }

  return (
    <NavigationContext.Provider value={[position, set]}>
      {props.children}
    </NavigationContext.Provider>
  );
}

export function useNavigation() {
  return useContext(NavigationContext);
}

export function NavigationTarget() {
  const [target] = useNavigation();
  if (!target) return null;
  return <Sphere position={target} scale={0.1}></Sphere>;
}
