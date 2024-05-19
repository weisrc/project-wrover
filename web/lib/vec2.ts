/**
 * @author Wei
 * Vec2 class
 */

export class Vec2 {
    constructor(public x: number, public y: number) {
    }

    translate(other: Vec2) {
        this.x += other.x;
        this.y += other.y;
        return this;
    }

    scale(scalor: number) {
        this.x *= scalor;
        this.y *= scalor;
        return this;
    }

    normalize() {
        return this.scale(1 / this.length())
    }

    length2() {
        return this.x ** 2 + this.y ** 2;
    }

    length() {
        return Math.sqrt(this.length2());
    }

    clone() {
        return new Vec2(this.x, this.y);
    }

    add(other: Vec2) {
        return new Vec2(this.x + other.x, this.y + other.y);
    }

    subtract(other: Vec2) {
        return new Vec2(this.x - other.x, this.y - other.y);
    }

    multiply(other: Vec2) {
        return new Vec2(this.x * other.y, this.y * other.y);
    }

    between(other: Vec2) {
        const x = this.x + other.x
        const y = this.y + other.y
        return new Vec2(x / 2, y / 2)
    }

    direction() {
        return Math.atan2(this.y, this.x);
    }

    setDirection(rad: number) {
        const length = this.length();
        this.x = Math.cos(rad) * length;
        this.y = Math.sin(rad) * length;
    }

    rotate(rad: number) {
        this.setDirection(this.direction() + rad)
    }

    static zero() {
        return new this(0, 0)
    }

    static polar(rad: number, length: number) {
        return new Vec2(
            Math.cos(rad) * length,
            Math.sin(rad) * length
        );
    }
}