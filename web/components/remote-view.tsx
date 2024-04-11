/**
 * @author Wei
 * Remote view component to control the robot remotely.
 */

import { requestEmitter } from "@/lib/common";
import { MapCanvas } from "./map-canvas";
import { CameraView } from "./camera-view";
import { useEffect } from "react";

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
    <div>
      <MapCanvas className="inset-0 fixed" />
      <CameraView className="fixed bottom-0 right-0" />
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
