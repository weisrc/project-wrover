import { requestEmitter } from "@/lib/common";
import { MapCanvas } from "./map-canvas";
import { Input } from "./ui/input";
import { CameraView } from "./camera-view";

export function RemoteView() {


  return (
    <div>
      <MapCanvas data={[]} className="inset-0 fixed"/>
      <CameraView className="fixed bottom-0 right-0"/>
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
