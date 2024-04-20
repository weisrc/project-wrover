/**
 * @author Wei
 * Remote view component to control the robot remotely.
 */

import {
  ContextMenu,
  ContextMenuContent,
  ContextMenuItem,
  ContextMenuTrigger,
} from "@/components/ui/context-menu";
import { requestEmitter } from "@/lib/common";
import { MouseEventHandler, useEffect, useRef } from "react";
import { CameraView } from "./camera-view";
import { MapCanvas3D } from "./canvas/map-canvas-3d";
import {
  ResizableHandle,
  ResizablePanel,
  ResizablePanelGroup,
} from "./ui/resizable";
import { CAMERA_FRAME_SIZES } from "@/lib/camera-frame-sizes";

export function RemoteView() {
  useEffect(() => {
    let lastKeys = "";

    const keyMap: Record<string, boolean> = {};

    function keyDown(event: KeyboardEvent) {
      keyMap[event.key] = true;
      handleChange();
    }

    function keyUp(event: KeyboardEvent) {
      keyMap[event.key] = false;
      handleChange();
    }

    function handleChange() {
      const keys =
        (keyMap.w ? "w" : "") +
        (keyMap.a ? "a" : "") +
        (keyMap.s ? "s" : "") +
        (keyMap.d ? "d" : "");

      if (lastKeys === keys) {
        return;
      }

      lastKeys = keys;

      const speedMap = {
        w: [50, 50],
        a: [-50, 50],
        s: [-50, -50],
        d: [50, -50],
        wa: [25, 50],
        wd: [50, 25],
        as: [-25, -50],
        sd: [-50, -25],
        zero: [0, 0],
      } as const;

      const speed = speedMap[keys as keyof typeof speedMap] ?? speedMap.zero;

      const multiplier = 1;

      requestEmitter.emit("motor", {
        m0: speed[0] * multiplier,
        m1: speed[1] * multiplier,
      });
    }

    globalThis.addEventListener("keydown", keyDown);
    globalThis.addEventListener("keyup", keyUp);

    return () => {
      globalThis.removeEventListener("keydown", keyDown);
      globalThis.removeEventListener("keyup", keyUp);
    };
  }, []);

  return (
    <div className="h-screen">
      <ResizablePanelGroup direction="vertical">
        <ResizablePanel>
          <MapCanvas3D />
        </ResizablePanel>
        <ResizableHandle className="bg-slate-500" />
        <ResizablePanel className="flex h-full">
          <ContextMenu>
            <ContextMenuTrigger asChild>
              <CameraView className="h-full" />
            </ContextMenuTrigger>
            <ContextMenuContent>
              {CAMERA_FRAME_SIZES.map((item, i) => {
                return (
                  <ContextMenuItem
                    key={i}
                    onClick={() => {
                      requestEmitter.emit("setCameraFrameSize", {
                        size: item.value,
                      });
                    }}
                  >
                    {item.label}
                  </ContextMenuItem>
                );
              })}
            </ContextMenuContent>
          </ContextMenu>
        </ResizablePanel>
      </ResizablePanelGroup>
      {/* <Input
        type="number"
        onChange={(e) => {
          requestEmitter.emit("setCameraFPS", {
            fps: +e.currentTarget.value,
          });
        }}
      /> */}
    </div>
  );
}
