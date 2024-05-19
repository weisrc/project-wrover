"use client";

import { OrbitControls } from "@react-three/drei";
import { Canvas } from "@react-three/fiber";
import { useRef, useState } from "react";
import { MOUSE, Vector3 } from "three";
import {
  ContextMenu,
  ContextMenuContent,
  ContextMenuItem,
  ContextMenuTrigger,
} from "../ui/context-menu";
import { MapScene } from "./map-scene";
import { NavigationProvider, useNavigation } from "./navigation-ctx";
import { PlanePointerProvider, usePlanePointer } from "./plane-pointer";

export function MapCanvas3D() {
  return (
    <NavigationProvider>
      <PlanePointerProvider>
        <MapCanvasInner />
      </PlanePointerProvider>
    </NavigationProvider>
  );
}

function NavigateMenuItem({ menuAt }: { menuAt?: Vector3 }) {
  const menuAtX = menuAt?.x ?? 0;
  const menuAtY = menuAt?.z ?? 0;

  const [_target, setTarget] = useNavigation();

  return (
    <ContextMenuItem
      onClick={() => {
        setTarget(menuAtX, menuAtY);
      }}
    >
      Navigate to ({menuAtX.toFixed(2)}, {menuAtY.toFixed(2)})
    </ContextMenuItem>
  );
}

function MapCanvasInner() {
  const triggerRef = useRef<HTMLElement>(null);
  const { pointer } = usePlanePointer();
  const [menuAt, setMenuAt] = useState<Vector3>();

  return (
    <ContextMenu>
      <ContextMenuTrigger ref={triggerRef}>
        <Canvas
          style={{ background: "black" }}
          camera={{ position: [0, 2, -2] }}
          onContextMenu={(e) => {
            setMenuAt(pointer ?? menuAt);
            triggerRef.current?.dispatchEvent(
              new MouseEvent("contextmenu", {
                bubbles: true,
                cancelable: true,
                clientX: e.clientX,
                clientY: e.clientY,
              })
            );
          }}
        >
          <group scale={[-1, 1, 1]}>
            <MapScene />
            <OrbitControls
              mouseButtons={{
                LEFT: MOUSE.ROTATE,
                MIDDLE: MOUSE.PAN,
              }}
            />
            <ambientLight intensity={1} />
          </group>
        </Canvas>
      </ContextMenuTrigger>
      <ContextMenuContent>
        <NavigateMenuItem menuAt={menuAt} />
      </ContextMenuContent>
    </ContextMenu>
  );
}
