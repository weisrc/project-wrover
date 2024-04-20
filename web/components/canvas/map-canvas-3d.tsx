"use client";

import { OrbitControls } from "@react-three/drei";
import { Canvas } from "@react-three/fiber";
import { useRef } from "react";
import { MOUSE } from "three";
import {
  ContextMenu,
  ContextMenuContent,
  ContextMenuItem,
  ContextMenuTrigger,
} from "../ui/context-menu";
import { MapScene } from "./map-scene";
import { PlanePointerProvider } from "./plane-pointer";

export function MapCanvas3D() {
  return (
    <PlanePointerProvider>
      <MapCanvasInner />
    </PlanePointerProvider>
  );
}

function MapCanvasInner() {
  const triggerRef = useRef<HTMLElement>(null);

  return (
    <ContextMenu>
      <ContextMenuTrigger ref={triggerRef}>
        <Canvas
          style={{ background: "black" }}
          camera={{ position: [0, 2, -2] }}
          onContextMenu={(e) => {
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
        <ContextMenuItem>Move here</ContextMenuItem>
      </ContextMenuContent>
    </ContextMenu>
  );
}
