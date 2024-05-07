"use client";

import { Data, Params, rssi } from "@/lib/ap-predict";
import { Box, Sphere } from "@react-three/drei";

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
            const h = (rssi(x, y, ...props.params) + props.rssiOffset) * props.rssiScale
            return <Box position={[x, h, y]} scale={0.1}>
              <meshStandardMaterial color={[0,1,1]} />
            </Box>
          })
        }).flat()
      }
    </group>
  );
}
