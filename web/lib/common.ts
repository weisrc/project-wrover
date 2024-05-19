/**
 * @author Wei
 * Common constants and singletons.
 */

import { EventEmitter } from "./event-emitter";
import { RequestEvents, ResponseEvents } from "./types";

export const NAME = "WRover";

export const requestEmitter = new EventEmitter<RequestEvents>();
export const responseEmitter = new EventEmitter<ResponseEvents>();
