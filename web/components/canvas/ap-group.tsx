"use client";

import { Data, Params, rssi } from "@/lib/ap-predict";
import { Box } from "@react-three/drei";
import { Color } from "three";

const array = new Array(50).fill(0).map((_, i) => (i - 25) * 2)

export function APGroup(props: {
  dataset: Data[],
  params: Params,
  rssiOffset: number,
  rssiScale: number
}) {

  const scale = 0.05

  return (
    <group>
      {props.dataset.map(([x, y, z]) => {
        const h = (z + props.rssiOffset) * props.rssiScale
        return <Box position={[x, h / 2, y]} scale={[scale, h, scale]}>
          <meshStandardMaterial color={"red"} />
        </Box>
      })}

      {
        array.map((x) => {
          return array.map((y) => {
            const value = rssi(x, y, ...props.params)
            const h = (value + props.rssiOffset) * props.rssiScale
            const color = new Color()
            color.setHSL(value / 90, 1, 0.5)
            return <Box position={[x, h, y]} scale={0.5}>
              <meshStandardMaterial color={color} />
            </Box>
          })
        }).flat()
      }
    </group>
  );
}
