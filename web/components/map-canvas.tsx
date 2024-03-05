import { LocomotionData } from "@/lib/types"
import { HTMLAttributes, useEffect, useRef } from "react"
import { DrawCanvas, DrawFunction } from "./draw-canvas"
import { Vec2 } from "@/lib/vec2";

export type MapCanvasProps = {
    data: LocomotionData[],
} & HTMLAttributes<HTMLDivElement>

export function MapCanvas({ data, ...rest }: MapCanvasProps) {

    data = [
        {
            hall: "",
            sonar: [123, 123, 123]
        },
        {
            hall: "1111111",
            sonar: [223, 223, 223]
        }
    ]

    const radius = 100;
    const diameter = radius * 2;
    const delta = 0.1;
    const angles = [0, -Math.PI / 2, Math.PI / 2];

    const draw: DrawFunction = (ctx, width, height) => {
        const offset = new Vec2(width / 2, height / 2);
        let left = new Vec2(-radius, 0).add(offset);
        let right = new Vec2(radius, 0).add(offset);

        for (const item of data) {
            for (const char of item.hall) {
                if (char === "0") {
                    const angle = right.subtract(left).direction();
                    right = Vec2.polar(angle + delta, diameter).add(left);
                } else if (char === "1") {
                    const angle = left.subtract(right).direction();
                    left = Vec2.polar(angle - delta, diameter).add(right);
                }
            }

            for (let i = 0; i < angles.length; i++) {
                const distance = item.sonar[i];
                const angle = angles[i] + right.subtract(left).direction()
                const center = left.add(right).scale(1 / 2);
                const ray = Vec2.polar(angle, distance);
                const end = ray.add(center);
                ctx.beginPath()
                ctx.strokeStyle = "white";
                ctx.moveTo(center.x, center.y);
                ctx.lineTo(end.x, end.y);
                ctx.stroke()
            }
        }
    }

    return <DrawCanvas draw={draw} {...rest} background="black" />
}