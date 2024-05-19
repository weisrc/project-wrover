/**
 * @author Wei
 * DrawCanvas component
 * Resizes the canvas to fit the parent div and calls the draw function to render the canvas
 */

import { HTMLAttributes, useEffect, useRef } from "react"

export type DrawFunction = (ctx: CanvasRenderingContext2D, width: number, height: number) => void

export type DrawCanvasProps = {
    draw: DrawFunction,
    background: string;
} & HTMLAttributes<HTMLDivElement>

export function DrawCanvas({ background, draw, ...rest }: DrawCanvasProps) {

    const canvasRef = useRef<HTMLCanvasElement>(null);
    const divRef = useRef<HTMLDivElement>(null);

    useEffect(() => {

        const canvas = canvasRef.current!;
        const div = divRef.current!;

        const observer = new ResizeObserver(([entry]) => {
            canvas.width = entry.contentRect.width;
            canvas.height = entry.contentRect.height;
        })

        observer.observe(div);

        let handle = -1;

        const ctx = canvas.getContext("2d")!

        function render() {
            ctx.fillStyle = background
            ctx.fillRect(0, 0, canvas.width, canvas.height);
            draw(ctx, canvas.width, canvas.height)
            handle = requestAnimationFrame(render)
        }

        render();

        return () => {
            cancelAnimationFrame(handle)
            observer.disconnect()
        }
    }, [draw, background])

    return <div  {...rest} ref={divRef} style={{
        background
    }}>
        <canvas ref={canvasRef} />
    </div>
}