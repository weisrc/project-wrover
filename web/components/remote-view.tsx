import { requestEmitter } from "@/lib/common";
import { MapCanvas } from "./map-canvas";
import { Input } from "./ui/input";
import { CameraView } from "./camera-view";
import { useEffect } from "react";

export function RemoteView() {
  useEffect(() => {

    let lastKeys = "";

    const keyMap: Record<string, boolean> = {}

    function keyDown(event: KeyboardEvent) {
      keyMap[event.key] = true;
      handleChange();
    }

    function keyUp(event: KeyboardEvent) {
      keyMap[event.key] = false;
      handleChange();
    }

    function handleChange() {
      const keys = (keyMap.w ? "w" : "")
        + (keyMap.a ? "a" : "")
        + (keyMap.s ? "s" : "")
        + (keyMap.d ? "d" : "")

      if (lastKeys === keys) {
        return;
      }

      lastKeys = keys;

      const speedMap = {
        "w": [100, 100],
        "a": [50, 51],
        "s": [51, 51],
        "d": [51, 50],
        "wa": [50, 100],
        "wd": [100, 50],
        "as": [51, 101],
        "sd": [101, 51],
        "zero": [0, 0]
      } as const

      const speed = speedMap[keys as keyof typeof speedMap] ?? speedMap.zero

      requestEmitter.emit("motor", {
        m0: speed[0],
        m1: speed[1]
      })

    }

    globalThis.addEventListener("keydown", keyDown)
    globalThis.addEventListener("keyup", keyUp)

    return () => {
      globalThis.removeEventListener("keydown", keyDown)
      globalThis.removeEventListener("keyup", keyUp)
    }
  }, [])

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
