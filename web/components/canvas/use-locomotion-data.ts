import { requestEmitter, responseEmitter } from "@/lib/common";
import { LocomotionData } from "@/lib/types";
import { useEffect, useState } from "react";
import { SONAR_TO_M } from "./process-locomotion-data";
import testData from "./test-locomotion-data.json";

export function useLocomotionData() {
  const [distanceFront, setDistanceFront] = useState(0);
  const [distanceLeft, setDistanceLeft] = useState(0);
  const [distanceRight, setDistanceRight] = useState(0);
  const [data, setData] = useState<LocomotionData[]>([]);

  function handleLocomotionData(item: LocomotionData) {
    const { sonar } = item;
    setDistanceFront(sonar[0] * SONAR_TO_M);
    setDistanceRight(sonar[1] * SONAR_TO_M);
    setDistanceLeft(sonar[2] * SONAR_TO_M);
    if (item.hall) {
      data.push(item);
      setData([...data]);
    }
  }

  useEffect(() => {
    const interval = setInterval(() => {
      const item = testData.shift();
      if (item) {
        handleLocomotionData(item as LocomotionData);
      }
    }, 100);

    function onLocomotion(item: LocomotionData) {
    //   handleLocomotionData(item);
      requestLocomotion();
    }
    function requestLocomotion() {
      setTimeout(() => {
        requestEmitter.emit("locomotion", {});
      }, 100);
    }
    requestLocomotion();
    responseEmitter.on("locomotion", onLocomotion);
    return () => {
      responseEmitter.off("locomotion", onLocomotion);
      clearInterval(interval);
    };
  }, []);

  return { distanceFront, distanceLeft, distanceRight, data };
}
