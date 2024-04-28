import { requestEmitter, responseEmitter } from "@/lib/common";
import { LocomotionData } from "@/lib/types";
import { useEffect, useState } from "react";
import { SONAR_TO_M } from "./process-locomotion-data";
import testData from "./test-locomotion-data.json";

export function useLocomotionStates() {
  const [distanceFront, setDistanceFront] = useState(0);
  const [distanceLeft, setDistanceLeft] = useState(0);
  const [distanceRight, setDistanceRight] = useState(0);
  const [data, setData] = useState<LocomotionData[]>([]);

  function add(item: LocomotionData) {
    const { sonar } = item;
    setDistanceFront(sonar[0] * SONAR_TO_M);
    setDistanceRight(sonar[1] * SONAR_TO_M);
    setDistanceLeft(sonar[2] * SONAR_TO_M);
    if (item.hall) {
      data.push(item);
      setData([...data]);
    }
  }

  return { distanceFront, distanceLeft, distanceRight, data, add };
}

export function useTestLocomotionData() {
  const { distanceFront, distanceLeft, distanceRight, data, add } =
    useLocomotionStates();

  useEffect(() => { // run once ideally when use hook is called
    const interval = setInterval(() => {
      const item = testData.shift(); // get the first item
      if (item) {
        add(item as LocomotionData);
      }
    }, 100); // every 100ms

    return () => {
      clearInterval(interval); // clear the interval on cleanup
    };
  }, []);

  return { distanceFront, distanceLeft, distanceRight, data };
}

export function useLocomotionData() {
  const { distanceFront, distanceLeft, distanceRight, data, add } =
    useLocomotionStates();

  useEffect(() => {
    function onLocomotion(item: LocomotionData) {
      add(item);
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
    };
  }, []);

  return { distanceFront, distanceLeft, distanceRight, data };
}
