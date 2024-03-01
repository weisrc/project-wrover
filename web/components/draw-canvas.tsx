import { HTMLAttributes, useEffect, useRef } from "react"

export type DrawCanvasProps = {
    draw: (ctx: CanvasRenderingContext2D) => void
} & HTMLAttributes<HTMLCanvasElement>

export function DrawCanvas({draw, ...rest}: DrawCanvasProps) {

    const canvasRef = useRef<HTMLCanvasElement>(null)

    useEffect(() => {
        let handle = -1;
        const ctx = canvasRef.current!.getContext("2d")!
    
        function render() {
            draw(ctx)
            handle = requestAnimationFrame(render)
        }

        return () => {
            cancelAnimationFrame(handle)
        }
    }, [draw])

    return <canvas ref={canvasRef} {...rest}/>
}