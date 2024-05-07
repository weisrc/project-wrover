type N = number

type Data = [N, N, N]

type DistanceFn<T = N> = (x: N, y: N, x0: N, y0: N) => T
type RssiFn<T = N> = (x: N, y: N, x0: N, y0: N, n: N, p: N) => T
type LossFn<T = N> = (x: N, y: N, z: N, x0: N, y0: N, n: N, p: N) => T

export const diffSquared = (x: N, x0: N) => {
    return (x - x0) ** 2
}

export const diffSquared_dx0 = (x: N, x0: N) => {
    return (2 * (x - x0)) * x0
}

export const distanceSum: DistanceFn = (x, y, x0, y0) => {
    return diffSquared(x, x0) + diffSquared(y, y0)
}

export const distanceSum_dx0: DistanceFn = (x, y, x0, y0) => {
    return diffSquared_dx0(x, x0)
}

export const distanceSum_dy0: DistanceFn = (x, y, x0, y0) => {
    return diffSquared_dx0(y, y0)
}

export const distance: DistanceFn = (x, y, x0, y0) => {
    return (distanceSum(x, y, x0, y0)) ** 0.5
}

export const distance_dx0: DistanceFn = (x, y, x0, y0) => {
    const outer = (0.5 * (distanceSum(x, y, x0, y0)) ** -1.5)
    return outer * distanceSum_dx0(x, y, x0, y0)
}

export const distance_dy0: DistanceFn = (x, y, x0, y0) => {
    const outer = (0.5 * (distanceSum(x, y, x0, y0)) ** -1.5)
    return outer * distanceSum_dy0(x, y, x0, y0)
}

export const abs = (x: N) => Math.abs(x)
export const abs_dx = (x: N) => Math.sign(x)

export const log = (x: N) => Math.log(x)
export const log_dx = (x: N) => 1 / x

export const rssi: RssiFn = (x, y, x0, y0, n, p) => {
    return p - 10 * abs(n) * log(distance(x, y, x0, y0));
}

export const rssi_dn: RssiFn = (x, y, x0, y0, n, p) => {
    return p - 10 * abs_dx(n) * log(distance(x, y, x0, y0));
}

export const rssi_dp: RssiFn = (_x, _y, _dx0, _dy0, _n, p) => {
    return p
}

export const rssi_dx0: RssiFn = (x, y, x0, y0, n, _p) => {
    const outer = 10 * abs(n) * log_dx(distance(x, y, x0, y0))
    return outer * distance_dx0(x, y, x0, y0)
}

export const rssi_dy0: RssiFn = (x, y, x0, y0, n, _p) => {
    const outer = 10 * abs(n) * log_dx(distance(x, y, x0, y0))
    return outer * distance_dy0(x, y, x0, y0)
}

export const loss: LossFn = (x, y, z, x0, y0, n, p) => {
    return (rssi(x, y, x0, y0, n, p) - z) ** 2
}

export const loss_drssi_factory = (fn: RssiFn): LossFn => {
    return (x, y, z, x0, y0, n, p) => {
        const outer = 2 * (rssi(x, y, x0, y0, n, p) - z)
        return outer * fn(x, y, x0, y0, n, p)
    }
}

export const loss_dp = loss_drssi_factory(rssi_dp)
export const loss_dn = loss_drssi_factory(rssi_dn)
export const loss_dx0 = loss_drssi_factory(rssi_dx0)
export const loss_dy0 = loss_drssi_factory(rssi_dy0)

export function losses(dataset: Data[], x0: N, y0: N, n: N, p: N) {
    return dataset.map(([x, y, z]) => {
        return
    })
}

