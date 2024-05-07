"use client";

import { Data, Params, rssi, step } from "@/lib/ap-predict";
import { Box } from "@react-three/drei";
import { useEffect, useState } from "react";
import { Color } from "three";

const array = new Array(50).fill(0).map((_, i) => (i - 25) * 2)

export function APPredict(props: {
  dataset: Data[],
}) {

  const [params, setParams] = useState<Params>([0, 0, 1, 1])


  useEffect(() => {

    let scopeParams = params

    const interval = setInterval(() => {
      const out = step(props.dataset, scopeParams, 0.01)
      setParams(out)
      scopeParams = out
      console.log(out)
    }, 20)
    return () => clearInterval(interval)
  }, [])

  return <>
    <APGroup params={params} dataset={props.dataset}
      rssiOffset={90} rssiScale={0.3}
    />
  </>
}

export function APGroup(props: {
  dataset: Data[],
  params: Params,
  rssiOffset: number,
  rssiScale: number
}) {

  const scale = 0.05

  return (
    <group>
      {props.dataset.map(([x, y, z], i) => {
        const h = (z + props.rssiOffset) * props.rssiScale
        return <Box key={i} position={[x, h / 2, y]} scale={[scale, h, scale]}>
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
            return <Box key={y * 1000 + x} position={[x, h, y]} scale={0.5}>
              <meshStandardMaterial color={color} />
            </Box>
          })
        })
      }
    </group>
  );
}
