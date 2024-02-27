import { requestEmitter, responseEmitter } from "@/lib/common";
import { useEffect, useState } from "react";
import { Input } from "./ui/input";
import { SonarData } from "@/lib/types";

export function RemoteView() {
  const [captureUrl, setCaptureUrl] = useState("");

  const [sonar, setSonar] = useState<SonarData>();

  useEffect(() => {
    requestEmitter.emit("beginCamera", {});
    requestEmitter.emit("sonar", {});

    const onBinaryData = (data: Blob) => {
      const url = URL.createObjectURL(data);
      setCaptureUrl(url);
    }

    const onSonar = ({data}: {data: SonarData}) => {
      setSonar(data);
      setTimeout(() => {
        requestEmitter.emit("sonar", {});
      }, 500);
    }

    responseEmitter.on("binaryData", onBinaryData);
    responseEmitter.on("sonar", onSonar);

    return () => {
      responseEmitter.off("binaryData", onBinaryData);
      responseEmitter.off("sonar", onSonar);
    }
  }, []);

  return (
    <div>
      Camera view
      <img src={captureUrl} />
      <Input
        type="number"
        onChange={(e) => {
          requestEmitter.emit("setCameraFPS", {
            fps: +e.currentTarget.value,
          });
        }}
      />
      <pre>
        {JSON.stringify(sonar)}
      </pre>
    </div>
  );
}
