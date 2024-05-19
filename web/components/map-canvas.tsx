/**
 * @author Wei
 * MapCanvas component
 */

import { requestEmitter, responseEmitter } from "@/lib/common";
import { LocomotionData, SonarData } from "@/lib/types";
import { Vec2 } from "@/lib/vec2";
import { HTMLAttributes, useEffect } from "react";
import { DrawCanvas, DrawFunction } from "./draw-canvas";
import { DualOdometer } from "@/lib/dual-odometer";

export type MapCanvasProps = HTMLAttributes<HTMLDivElement>;

const angles = [-Math.PI / 2, 0, -Math.PI];

/**
 * Draw the sonar rays, points and distance text
 * @param ctx Context2D
 * @param rawCenter center as percieved by the odometer
 * @param angle angle of the odometer
 * @param sonar sonar data
 * @param last is it the last data point
 * @param offset offset to center the map
 */
function drawSonarRays(
  ctx: CanvasRenderingContext2D,
  rawCenter: Vec2,
  angle: number,
  sonar: SonarData,
  last: boolean,
  offset: Vec2
) {
  const circleRadius = 3;
  const center = rawCenter.add(offset);

  for (let i = 0; i < angles.length; i++) {
    // there are three sonar sensors at different angles
    const distance = sonar[i] / 58;
    const point = Vec2.polar(angles[i] + angle, distance).add(center);

    if (last) {
      // draw the sonar rays and distances
      ctx.beginPath();
      ctx.strokeStyle = "pink";
      ctx.moveTo(center.x, center.y);
      ctx.lineTo(point.x, point.y);
      ctx.stroke();

      ctx.fillStyle = "white";
      ctx.fillText(distance.toFixed(1) + "cm", point.x + 20, point.y);
    }

    ctx.beginPath(); // draw the sonar points
    ctx.ellipse(
      point.x,
      point.y,
      circleRadius,
      circleRadius,
      0,
      0,
      2 * Math.PI * 2
    );
    ctx.fillStyle = last ? "red" : "white";
    ctx.fill();
  }
}

const data: LocomotionData[] = [];

/**
 * MapCanvas component
 * @param props MapCanvasProps
 * @returns
 */
export function MapCanvas(props: MapCanvasProps) {
  useEffect(() => {
    function onLocomotion(item: LocomotionData) {
      if (data.length > 10000) {
        data.shift();
      }
      data.push(item);
      console.log("data is here", data);
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

  const radius = 15;
  const delta = 0.043;

  let offset: Vec2;

  const draw: DrawFunction = (ctx, width, height) => {
    const meter = new DualOdometer(radius, delta);
    const screenCenter = new Vec2(width / 2, height / 2);
    offset ??= screenCenter;

    for (let i = 0; i < data.length; i++) {
      // draw the samples
      const item = data[i];
      const isLast = i === data.length - 1;
      let moved = false;
      for (const char of item.hall) {
        moved = true;

        // ctx.beginPath();
        // ctx.moveTo(left.x, left.y)
        // ctx.lineTo(right.x, right.y)
        // ctx.strokeStyle = "white";
        // ctx.stroke()

        const which = char.toLowerCase();
        const backwards = char !== which;

        if (which === "l") {
          // move the odometer accordingly
          meter.moveLeft(backwards);
        } else if (which === "r") {
          meter.moveRight(backwards);
        }
      }

      if (!moved && !isLast) {
        // dont draw the samples when robot is not moving unless it is the last sample
        continue;
      }

      const angle = meter.getDirection();

      drawSonarRays(ctx, meter.getCenter(), angle, item.sonar, isLast, offset);
    }

    offset = screenCenter.subtract(meter.getCenter()); // update the offset based on new odometer center
  };

  return <DrawCanvas draw={draw} {...props} background="black" />;
}
