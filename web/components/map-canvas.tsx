import { requestEmitter, responseEmitter } from "@/lib/common";
import { LocomotionData, SonarData } from "@/lib/types";
import { Vec2 } from "@/lib/vec2";
import { HTMLAttributes, useEffect } from "react";
import { DrawCanvas, DrawFunction } from "./draw-canvas";

export type MapCanvasProps = HTMLAttributes<HTMLDivElement>;

const angles = [-Math.PI / 2, 0, -Math.PI];

function drawSonarRays(ctx: CanvasRenderingContext2D, center: Vec2, angle: number, sonar: SonarData, last: boolean) {
    const circleRadius = 3;
    
    for (let i = 0; i < angles.length; i++) {
        const distance = sonar[i] / 58;
        const ray = Vec2.polar(angles[i] + angle, distance);
        const end = ray.add(center)

        if (last) {
            ctx.beginPath()
            ctx.strokeStyle = "pink";
            ctx.moveTo(center.x, center.y);
            ctx.lineTo(end.x, end.y);
            ctx.stroke()

            ctx.fillStyle = "white"
            ctx.fillText(distance.toFixed(1) + "cm", end.x + 20, end.y)
        }

        ctx.beginPath()
        ctx.ellipse(end.x, end.y, circleRadius, circleRadius, 0, 0, 2 * Math.PI * 2);
        ctx.fillStyle = last ? "red" : "white"
        ctx.fill()
    }
}

const data: LocomotionData[] = []

export function MapCanvas(props: MapCanvasProps) {

    useEffect(() => {
        function onLocomotion(item: LocomotionData) {
            if (data.length > 10000) {
                data.shift();
            }
            data.push(item);
            requestLocomotion();
        }

        function requestLocomotion() {
            setTimeout(() => {
                requestEmitter.emit("locomotion", {});
            }, 100);
        }

        requestLocomotion()

        responseEmitter.on("locomotion", onLocomotion)

        return () => {
            responseEmitter.off("locomotion", onLocomotion)
        }
    }, [])

    const radius = 15;
    const diameter = radius * 2;
    const delta = -0.032;

    const draw: DrawFunction = (ctx, width, height) => {
        const offset = new Vec2(width / 2, height / 2);
        let left = new Vec2(-radius, 0).add(offset);
        let right = new Vec2(radius, 0).add(offset);

        for (let i = 0; i < data.length; i++) {
            const item = data[i];
            const isLast = i === data.length - 1
            let moved = false;
            for (const char of item.hall) {
                moved = true;

                // ctx.beginPath();
                // ctx.moveTo(left.x, left.y)
                // ctx.lineTo(right.x, right.y)
                // ctx.strokeStyle = "white";
                // ctx.stroke()

                const which = char.toLowerCase();
                const sign = which === char ? 1 : -1

                if (which === "l") {
                    const angle = left.subtract(right).direction();
                    left = Vec2.polar(angle - delta * sign, diameter).add(right);
                } else if (which === "r") {
                    const angle = right.subtract(left).direction();
                    right = Vec2.polar(angle + delta * sign, diameter).add(left);
                }
            }

            if (!moved && !isLast) {
                continue;
            }

            const angle = right.subtract(left).direction();

            drawSonarRays(ctx, left.between(right), angle, item.sonar, isLast)


        }
    }

    return <DrawCanvas draw={draw} {...props} background="black" />
}