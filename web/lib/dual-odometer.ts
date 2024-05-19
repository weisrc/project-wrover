/**
 * @author Wei
 * DualOdometer class
 * Similar to the one for the ESP32
 */

import { Vec2 } from "./vec2";

export class DualOdometer {
  public left: Vec2;
  public right: Vec2;

  constructor(public radius: number, public delta: number) {
    this.left = new Vec2(-radius, 0);
    this.right = new Vec2(radius, 0);
  }

  public moveLeft(backwards = false) {
    const sign = backwards ? -1 : 1;
    const angle = this.left.subtract(this.right).direction();
    this.left = Vec2.polar(angle - this.delta * sign, this.radius * 2).add(
      this.right
    );
  }

  public moveRight(backwards = false) {
    const sign = backwards ? -1 : 1;
    const angle = this.right.subtract(this.left).direction();
    this.right = Vec2.polar(angle + this.delta * sign, this.radius * 2).add(
      this.left
    );
  }

  public getDirection() {
    return this.right.subtract(this.left).direction();
  }

  public getCenter() {
    return this.left.between(this.right);
  }
}
