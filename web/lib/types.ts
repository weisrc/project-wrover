export type ConnectEvent = {
  ssid: string;
} & (
  | {
      auth: "open";
    }
  | {
      auth: "wpa2";
      password: string;
    }
  | {
      auth: "wpa2_enterprise";
      method: ConnectEventMethod;
      identity: string;
      username: string;
      password: string;
    }
);

export type ConnectEventMethod = "peap" | "ttls" | "tls";

export type NetworkItem = [
  ssid: string,
  rssi: number,
  auth: number,
  channel: number,
  bssid: string
];

export type RequestEvents = {
  connect: ConnectEvent;
  scan: {};
  disconnect: {};
  ssid: {};
  ip: {};
  rssi: {};
  status: {};
  begin: {};
  beginCamera: {};
  stopCamera: {};
  setCameraFPS: { fps: number };
};

export type ResponseEvents = {
  scan: { networks: NetworkItem[] };
  ip: { data: string };
  rssi: { data: string };
  status: { data: string };
  ssid: { data: string };
  socketReady: {};
  binaryData: Blob;
};

export const AUTH_MODE_MAP = {
  open: "OPEN",
  WIFI_AUTH_WEP: "WEP",
  WIFI_AUTH_WPA_PSK: "WPA",
  wpa2: "WPA2",
  WIFI_AUTH_WPA_WPA2_PSK: "WPA/WPA2",
  wpa2_enterprise: "WPA2 Enterprise",
  WIFI_AUTH_WPA3_PSK: "WPA3",
  WIFI_AUTH_WPA2_WPA3_PSK: "WPA2/WPA3",
  WIFI_AUTH_WAPI_PSK: "WAPI",
  WIFI_AUTH_MAX: "MAX",
} as const;

export const AUTH_MODE_KEYS = Object.keys(
  AUTH_MODE_MAP
) as (keyof typeof AUTH_MODE_MAP)[];
export const AUTH_MODE_NAMES = Object.values(AUTH_MODE_MAP);

export type AuthMode = "open" | "wpa2" | "wpa2_enterprise";
